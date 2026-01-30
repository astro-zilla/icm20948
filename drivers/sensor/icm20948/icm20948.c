#include <zephyr/drivers/i2c.h>
#include <zephyr/init.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/sys/utils.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#include "icm20948.h"
#include "icm20948_dmp_firmware.h"

#define DT_DRV_COMPAT invensense_icm20948

#if DT_NUM_INST_STATUS_OKAY(DT_DRV_COMPAT) == 0
#warning "Custom ICM20948 driver enabled without any devices"
#endif

LOG_MODULE_REGISTER(ICM20948, CONFIG_SENSOR_LOG_LEVEL);

/* see "Accelerometer Measurements" section from register map description */
static void icm20948_convert_accel(struct sensor_value *val, int16_t raw_val, uint8_t shift)
{

	int64_t conv_val = ((int64_t)raw_val * SENSOR_G) >> shift;
	val->val1 = conv_val / 1000000;
	val->val2 = conv_val % 1000000;
}

#define ICM20948_GYRO_SCALE_250DPS SENSOR_PI / 180U / 250U
/* see "Gyroscope Measurements" section from register map description */
static void icm20948_convert_gyro(struct sensor_value *val, int16_t raw_val, uint8_t shift)
{
	int64_t conv_val = ((int64_t)raw_val * ICM20948_GYRO_SCALE_250DPS) >> shift;
	val->val1 = conv_val / 1000000;
	val->val2 = conv_val % 1000000;
}

/* see "Temperature Measurement" section from register map description */
static inline void icm20948_convert_temp(struct sensor_value *val, int16_t raw_val)
{
    int64_t conv_val = ((int64_t)raw_val - 21) / 333.87 + 21;
	val->val1 = raw_val / 340 + 36;
	val->val2 = ((int64_t)(raw_val % 340) * 1000000) / 340 + 530000;

	if (val->val2 < 0) {
		val->val1--;
		val->val2 += 1000000;
	} else if (val->val2 >= 1000000) {
		val->val1++;
		val->val2 -= 1000000;
	}
}

int icm20948_channel_get(const struct device *dev,
			       enum sensor_channel chan,
			       struct sensor_value *val)
{
	struct icm20948_data *drv_data = dev->data;
    uint8_t accel_shift = 1 + drv_data->bank2.bytes.ACCEL_CONFIG.bits.ACCEL_FS_SEL;
    uint8_t gyro_shift = 2500 << drv_data->bank2.bytes.GYRO_CONFIG_1.bits.GYRO_FS_SEL;
	switch (chan) {
	case SENSOR_CHAN_ACCEL_XYZ:
		icm20948_convert_accel(val, drv_data->bank0.bytes.ACCEL_XOUT_H, accel_shift);
		icm20948_convert_accel(val + 1, drv_data->bank0.bytes.ACCEL_YOUT_H, accel_shift);
		icm20948_convert_accel(val + 2, drv_data->bank0.bytes.ACCEL_ZOUT_H, accel_shift);
		break;
	case SENSOR_CHAN_ACCEL_X:
		icm20948_convert_accel(val, drv_data->bank0.bytes.ACCEL_XOUT_H, accel_shift);
		break;
	case SENSOR_CHAN_ACCEL_Y:
		icm20948_convert_accel(val, drv_data->bank0.bytes.ACCEL_YOUT_H, accel_shift);
		break;
	case SENSOR_CHAN_ACCEL_Z:
		icm20948_convert_accel(val, drv_data->bank0.bytes.ACCEL_ZOUT_H, accel_shift);
		break;
	case SENSOR_CHAN_GYRO_XYZ:
		icm20948_convert_gyro(val, drv_data->bank0.bytes.GYRO_XOUT_H, gyro_shift);
		icm20948_convert_gyro(val + 1, drv_data->bank0.bytes.GYRO_YOUT_H, gyro_shift);
		icm20948_convert_gyro(val + 2, drv_data->bank0.bytes.GYRO_ZOUT_H, gyro_shift);
		break;
	case SENSOR_CHAN_GYRO_X:
		icm20948_convert_gyro(val, drv_data->bank0.bytes.GYRO_XOUT_H, gyro_shift);
		break;
	case SENSOR_CHAN_GYRO_Y:
		icm20948_convert_gyro(val, drv_data->bank0.bytes.GYRO_YOUT_H, gyro_shift);
		break;
	case SENSOR_CHAN_GYRO_Z:
		icm20948_convert_gyro(val, drv_data->bank0.bytes.GYRO_ZOUT_H, gyro_shift);
		break;
	case SENSOR_CHAN_DIE_TEMP:
		icm20948_convert_temp(val, drv_data->bank0.bytes.TEMP_OUT_H);
        break;
    default:
        return -ENOTSUP;
    }

	return 0;
}

int icm20948_sample_fetch(const struct device *dev,
				enum sensor_channel chan) {
	struct icm20948_data *drv_data = dev->data;
	if (icm20948_read(dev, ICM20948_BANK0, ICM20948_BANK0_ACCEL_XOUT_H, &drv_data->bank0.bytes.ACCEL_XOUT_H,
			      14) < 0) {
		LOG_ERR("Failed to read data sample.");
		return -EIO;
	}

	return 0;
}

static const struct sensor_driver_api icm20948_driver_api = {
	.sample_fetch = &icm20948_sample_fetch,
	.channel_get = &icm20948_channel_get,
};

int icm20948_reset_FIFO(const struct device* dev) {
	int ret;
	struct icm20948_data *drv_data = dev->data;
	ret |= icm20948_read(dev, ICM20948_BANK0, &drv_data.bank0.bytes.FIFO_RST.byte, 1);
	drv_data.bank0.bytes.FIFO_RST.bits.FIFO_RESET = 0x1f;
	ret |= icm20948_write(dev, ICM20948_BANK0, &drv_data.bank0.bytes.FIFO_RST.byte, 1);
	drv_data.bank0.bytes.FIFO_RST.bits.FIFO_RESET = 0x1e;
	ret |= icm20948_write(dev, ICM20948_BANK0, &drv_data.bank0.bytes.FIFO_RST.byte, 1);
	return ret;
}

int16_t icm20948_get_FIFO_cnt(const struct device *dev) {
	struct icm20948_data *drv_data = dev->data;
	if (icm20948_read(dev, ICM20948_BANK0, &drv_data.bank0.bytes.FIFO_COUNTH.byte, 2)!=0) {
		return -EIO
	}
	drv_data.bank0.bytes.FIFO_COUNTH.bits.RSVD = 0;
	return *(int16_t*)&drv_data.bank0.bytes.FIFO_COUNTH.byte;
}

int icm20948_firmware_load(const struct device *dev) {
		if (icm20948_mem_write(dev, DMP_LOAD_START, &icm20948_firmware, sizeof(icm20948_dmp_firmware))!=0) {
			LOG_ERR("Error loading DMP firmware.");
		} else {
			LOG_INF("DMP firmware loaded successfully");
		}
	}

int icm20948_mem_read(const struct device *dev, uint16_t addr, uint8_t *data, size_t length) {
	struct icm20948_data *drv_data = dev->data;
	unsigned int nread = 0;
	size_t chunksize;
	
	drv_data.bank0.bytes.MEM_BANK_SEL = addr >> 8;
	icm20948_write(dev, ICM20948_BANK0, ICM20948_BANK0_MEM_BANK_SEL, &drv_data.bank0.bytes.MEM_BANK_SEL.byte, 1);

	while (nread < length) {
		drv_data.bank0.bytes.MEM_ADDR = addr & 0xff;
		icm20948_write(dev, ICM20948_BANK0, ICM20948_BANK0_MEM_ADDR, &drv_data.bank0.bytes.MEM_ADDR.byte, 1);
		
		chunksize = min3(ICM20948_MAX_SERIAL_READ, DMP_MEM_BANK_SIZE-drv_data.bank0.bytes.MEM_ADDR, length)
		icm20948_read(dev, ICM20948_BANK0, ICM20948_BANK0_MEM_R_W, data+nread, chunksize);

		nread += chunksize;
		addr += chunksize;
		length -= chunksize;		
	}

}

int icm20948_mem_write(const struct device *dev, uint16_t addr, uint8_t *data, size_t length) {
	struct icm20948_data *drv_data = dev->data;
	unsigned int nwritten = 0;
	size_t chunksize;
	
	drv_data.bank0.bytes.MEM_BANK_SEL = addr >> 8;
	icm20948_write(dev, ICM20948_BANK0, ICM20948_BANK0_MEM_BANK_SEL, &drv_data.bank0.bytes.MEM_BANK_SEL.byte, 1);

	while (nwritten < length) {
		drv_data.bank0.bytes.MEM_ADDR = addr & 0xff;
		icm20948_write(dev, ICM20948_BANK0, ICM20948_BANK0_MEM_ADDR, &drv_data.bank0.bytes.MEM_ADDR.byte, 1);
		
		chunksize = min3(ICM20948_MAX_SERIAL_READ, DMP_MEM_BANK_SIZE-drv_data.bank0.bytes.MEM_ADDR, length)
		icm20948_write(dev, ICM20948_BANK0, ICM20948_BANK0_MEM_R_W, data+nwritten, chunksize);

		nwritten+=chunksize;
		addr += chunksize;
		length -= chunksize;		
	}

}

int icm20948_read(const struct device *dev, icm20948_reg_bank_sel_t bank, uint8_t reg_addr, uint8_t *data, size_t length) {
    const struct icm20948_config *cfg = dev->config;
    struct icm20948_data *drv_data = dev->data;

    if (drv_data->reg_bank_sel != bank) {
        drv_data->reg_bank_sel = bank;
        if(i2c_burst_write_dt(&cfg->i2c, ICM20948_ADDR_REG_BANK_SEL, &drv_data->reg_bank_sel, 1) < 0) {
			LOG_ERR("Failed to set register bank.");
			return -EIO;
		}
    }

    if (i2c_burst_read_dt(&cfg->i2c, reg_addr, data, length) < 0) {
        LOG_ERR("Failed to read from device.");
        return -EIO;
    }

    return 0;
}

int icm20948_write(const struct device *dev, icm20948_reg_bank_sel_t bank, uint8_t reg_addr, const uint8_t *data, size_t length) {
    const struct icm20948_config *cfg = dev->config;
    struct icm20948_data *drv_data = dev->data;

    if (drv_data->reg_bank_sel != bank) {
        drv_data->reg_bank_sel = bank;
                if(i2c_burst_write_dt(&cfg->i2c, ICM20948_ADDR_REG_BANK_SEL, &drv_data->reg_bank_sel, 1) < 0) {
			LOG_ERR("Failed to set register bank.");
			return -EIO;
		}
		LOG_DBG("Switched to bank %d", drv_data->reg_bank_sel);
    }

    if (i2c_burst_write_dt(&cfg->i2c, reg_addr, data, length) < 0) {
        LOG_ERR("Failed to write to device.");
        return -EIO;
    }
    return 0;
}

int icm20948_init(const struct device *dev)
{
	struct icm20948_data *drv_data = dev->data;
	const struct icm20948_config *cfg = dev->config;
	LOG_INF("Initializing ICM20948 sensor");

	if (!device_is_ready(cfg->i2c.bus)) {
		LOG_ERR("Bus device is not ready");
		return -ENODEV;
	}

	k_msleep(100); /* wait for sensor boot time */

    /* check chip ID */
	drv_data->reg_bank_sel = ICM20948_BANK3;
	icm20948_read(dev, ICM20948_BANK0, ICM20948_BANK0_WHO_AM_I, &drv_data->bank0.bytes.WHO_AM_I, 1);

	if (drv_data->bank0.bytes.WHO_AM_I != ICM20948_WHO_AM_I_DEFAULT) {
		LOG_ERR("Invalid chip ID.");
		return -EINVAL;
	}

	/* wake up chip */
    drv_data->bank0.bytes.PWR_MGMT_1.bits.SLEEP = 0;
	icm20948_write(dev, ICM20948_BANK0, ICM20948_BANK0_PWR_MGMT_1, &drv_data->bank0.bytes.PWR_MGMT_1.byte, 1);

	/* set accelerometer sample rate */
    drv_data->bank2.bytes.ACCEL_SMPLRT_DIV_1.bits.ACCEL_SMPLRT_DIV = cfg->accel_sr_div >> 8;
    drv_data->bank2.bytes.ACCEL_SMPLRT_DIV_2 = cfg->accel_sr_div & 0xFF;
	icm20948_write(dev, ICM20948_BANK2, ICM20948_BANK2_ACCEL_SMPLRT_DIV_1, &drv_data->bank2.bytes.ACCEL_SMPLRT_DIV_1.byte, 2);

    /* set accelerometer config */
    drv_data->bank2.bytes.ACCEL_CONFIG.bits.ACCEL_FCHOICE = cfg->accel_fchoice;
    drv_data->bank2.bytes.ACCEL_CONFIG.bits.ACCEL_FS_SEL = cfg->accel_fs_sel;
    drv_data->bank2.bytes.ACCEL_CONFIG.bits.ACCEL_DLPFCFG = cfg->accel_dlpf;
    icm20948_write(dev, ICM20948_BANK2, ICM20948_BANK2_ACCEL_CONFIG, &drv_data->bank2.bytes.ACCEL_CONFIG.byte, 1);

    /* set gyroscope sample rate */
    drv_data->bank2.bytes.GYRO_SMPLRT_DIV = cfg->gyro_sr_div;
    icm20948_write(dev, ICM20948_BANK2, ICM20948_BANK2_GYRO_SMPLRT_DIV, &drv_data->bank2.bytes.GYRO_SMPLRT_DIV, 1);

    /* set gyroscope config */
    drv_data->bank2.bytes.GYRO_CONFIG_1.bits.GYRO_FCHOICE = cfg->gyro_fchoice;
    drv_data->bank2.bytes.GYRO_CONFIG_1.bits.GYRO_FS_SEL = cfg->gyro_fs_sel;
    drv_data->bank2.bytes.GYRO_CONFIG_1.bits.GYRO_DLPFCFG = cfg->gyro_dlpf;
    icm20948_write(dev, ICM20948_BANK2, ICM20948_BANK2_GYRO_CONFIG_1, &drv_data->bank2.bytes.GYRO_CONFIG_1.byte, 1);

    // /* set magnetometer i2c config */
    // drv_data->bank3.bytes.I2C_MST_CTRL.bits = {.MULT_MST_EN = 1, .I2C_MST_CLK = 7};
    // icm20948_write(dev, ICM20948_BANK3, ICM20948_BANK3_I2C_MST_CTRL, &drv_data->bank3.bytes.I2C_MST_CTRL, 1);
    // drv_data->bank3.bytes.I2C_MST_DELAY_CTRL.bits.I2C_SLV0_DELAY_EN = 1;
    // icm20948_write(dev, ICM20948_BANK3, ICM20948_BANK3_I2C_MST_DELAY_CTRL, &drv_data->bank3.bytes.I2C_MST_DELAY_CTRL, 1);
    // drv_data->bank3.bytes.I2C_SLV0_ADDR.bits.I2C_ID_0 = ICM20948_AK09916_I2C_ADDR;
    // drv_data->bank3.bytes.I2C_SLV0_ADDR.bits.I2C_RW_0 = 1; // read

	LOG_INF("ICM20948 initialization complete");

	return 0;
}

#define ICM20948_DEFINE(inst)									\
	static struct icm20948_data icm20948_data_##inst;\
\
	static const struct icm20948_config icm20948_config_##inst = {	\
		.i2c = I2C_DT_SPEC_INST_GET(inst),\
\
        .gyro_sr_div = DT_INST_PROP(inst, gyro_sr_div),\
        .gyro_dlpf = DT_INST_ENUM_IDX(inst, gyro_dlpf),\
        .gyro_fs_sel = DT_INST_ENUM_IDX(inst, gyro_fs_sel),\
        .gyro_fchoice = DT_INST_PROP(inst, gyro_fchoice),\
        .accel_sr_div = DT_INST_PROP(inst, accel_sr_div),\
        .accel_dlpf = DT_INST_ENUM_IDX(inst, accel_dlpf),\
        .accel_fs_sel = DT_INST_ENUM_IDX(inst, accel_fs_sel),\
        .accel_fchoice = DT_INST_PROP(inst, accel_fchoice),\
\
		.int_gpio = GPIO_DT_SPEC_INST_GET(inst, irq_gpios),\
	};\
\
	SENSOR_DEVICE_DT_INST_DEFINE(inst, icm20948_init, NULL,\
			      &icm20948_data_##inst, &icm20948_config_##inst,\
			      POST_KERNEL, CONFIG_SENSOR_INIT_PRIORITY,\
			      &icm20948_driver_api);

DT_INST_FOREACH_STATUS_OKAY(ICM20948_DEFINE)
