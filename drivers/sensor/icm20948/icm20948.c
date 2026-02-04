#include <zephyr/drivers/i2c.h>
#include <zephyr/init.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/sys/util.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#include "icm20948.h"
#include "icm20948_dmp.h"

#define DT_DRV_COMPAT invensense_icm20948

#if DT_NUM_INST_STATUS_OKAY(DT_DRV_COMPAT) == 0
#warning "Custom ICM20948 driver enabled without any devices"
#endif

LOG_MODULE_REGISTER(icm20948, CONFIG_SENSOR_LOG_LEVEL);

/* see "Accelerometer Measurements" section from register map description */
static void icm20948_convert_accel(struct sensor_value *val, float *float_val, int16_t raw_val, uint8_t shift)
{
	int64_t conv_val = ((int64_t)raw_val * SENSOR_G) >> shift;
	*float_val = (float)conv_val / 1000000.0f;
	sensor_value_from_micro(val, conv_val);
}

#define ICM20948_GYRO_SCALE 2181661 // 1000000 * PI * 125 / 180
/* see "Gyroscope Measurements" section from register map description */
static void icm20948_convert_gyro(struct sensor_value *val, float *float_val, int16_t raw_val, uint8_t shift)
{
	int64_t conv_val = ((int64_t)raw_val * ICM20948_GYRO_SCALE) >> shift;
	*float_val = (float)conv_val / 1000000.0f;
	sensor_value_from_micro(val, conv_val);
}

/* see "Temperature Measurement" section from register map description */
static inline void icm20948_convert_temp(struct sensor_value *val, float *float_val, int16_t raw_val)
{
    *float_val = ((float)raw_val - 21.f) / 333.87f + 21.f;
	sensor_value_from_float(val, *float_val);
}

int icm20948_channel_get(const struct device *dev,
			       enum sensor_channel chan,
			       struct sensor_value *val)
{
	struct icm20948_data *drv_data = dev->data;

    uint8_t accel_shift = 14 - drv_data->bank2.bytes.ACCEL_CONFIG.bits.ACCEL_FS_SEL;
    uint8_t gyro_shift = 14 - drv_data->bank2.bytes.GYRO_CONFIG_1.bits.GYRO_FS_SEL;

	int16_t accel_x, accel_y, accel_z, gyro_x, gyro_y, gyro_z, temp;

	accel_x = sys_be16_to_cpu(*(int16_t *)&drv_data->bank0.bytes.ACCEL_XOUT_H);
	accel_y = sys_be16_to_cpu(*(int16_t *)&drv_data->bank0.bytes.ACCEL_YOUT_H);
	accel_z = sys_be16_to_cpu(*(int16_t *)&drv_data->bank0.bytes.ACCEL_ZOUT_H);
	gyro_x = sys_be16_to_cpu(*(int16_t *)&drv_data->bank0.bytes.GYRO_XOUT_H);
	gyro_y = sys_be16_to_cpu(*(int16_t *)&drv_data->bank0.bytes.GYRO_YOUT_H);
	gyro_z = sys_be16_to_cpu(*(int16_t *)&drv_data->bank0.bytes.GYRO_ZOUT_H);
	temp = sys_be16_to_cpu(*(int16_t *)&drv_data->bank0.bytes.TEMP_OUT_H);
	switch (chan) {
	case SENSOR_CHAN_ACCEL_XYZ:
		icm20948_convert_accel(val, &drv_data->converted.accel_x, accel_x, accel_shift);
		icm20948_convert_accel(val + 1, &drv_data->converted.accel_y, accel_y, accel_shift);
		icm20948_convert_accel(val + 2, &drv_data->converted.accel_z, accel_z, accel_shift);
		break;
	case SENSOR_CHAN_ACCEL_X:
		icm20948_convert_accel(val, &drv_data->converted.accel_x, accel_x, accel_shift);
		break;
	case SENSOR_CHAN_ACCEL_Y:
		icm20948_convert_accel(val, &drv_data->converted.accel_y, accel_y, accel_shift);
		break;
	case SENSOR_CHAN_ACCEL_Z:
		icm20948_convert_accel(val, &drv_data->converted.accel_z, accel_z, accel_shift);
		break;
	case SENSOR_CHAN_GYRO_XYZ:
		icm20948_convert_gyro(val, &drv_data->converted.gyro_x, gyro_x, gyro_shift);
		icm20948_convert_gyro(val + 1, &drv_data->converted.gyro_y, gyro_y, gyro_shift);
		icm20948_convert_gyro(val + 2, &drv_data->converted.gyro_z, gyro_z, gyro_shift);
		break;
	case SENSOR_CHAN_GYRO_X:
		icm20948_convert_gyro(val, &drv_data->converted.gyro_x, gyro_x, gyro_shift);
		break;
	case SENSOR_CHAN_GYRO_Y:
		icm20948_convert_gyro(val, &drv_data->converted.gyro_y, gyro_y, gyro_shift);
		break;
	case SENSOR_CHAN_GYRO_Z:
		icm20948_convert_gyro(val, &drv_data->converted.gyro_z, gyro_z, gyro_shift);
		break;
	case SENSOR_CHAN_DIE_TEMP:
		icm20948_convert_temp(val, &drv_data->converted.temp, temp);
        break;
    default:
        return -ENOTSUP;
    }
	return 0;
}

int icm20948_sample_fetch(const struct device *dev,
				enum sensor_channel chan) {
	struct icm20948_data *drv_data = dev->data;
	int fifo_count;
	switch (chan) {
	case SENSOR_CHAN_ALL:
		icm20948_read(dev, ICM20948_BANK0, ICM20948_BANK0_ACCEL_XOUT_H, &drv_data->bank0.bytes.ACCEL_XOUT_H, 14);
		icm20948_read(dev, ICM20948_BANK2, ICM20948_BANK2_GYRO_CONFIG_1, &drv_data->bank2.bytes.GYRO_CONFIG_1.byte, 2);
		icm20948_read(dev, ICM20948_BANK2, ICM20948_BANK2_ACCEL_CONFIG, &drv_data->bank2.bytes.ACCEL_CONFIG.byte, 2);
		
		icm20948_get_FIFO_count(dev, &fifo_count);
		icm20948_read_FIFO(dev, fifo_count);
		break;
	default:
		return -ENOTSUP;
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
	ret = icm20948_read(dev, ICM20948_BANK0, ICM20948_BANK0_FIFO_RST,&drv_data->bank0.bytes.FIFO_RST.byte, 1);
	drv_data->bank0.bytes.FIFO_RST.bits.FIFO_RESET = 0x1f;
	ret |= icm20948_write(dev, ICM20948_BANK0, ICM20948_BANK0_FIFO_RST, &drv_data->bank0.bytes.FIFO_RST.byte, 1);
	drv_data->bank0.bytes.FIFO_RST.bits.FIFO_RESET = 0x1e;
	ret |= icm20948_write(dev, ICM20948_BANK0, ICM20948_BANK0_FIFO_RST, &drv_data->bank0.bytes.FIFO_RST.byte, 1);
	return ret;
}

int icm20948_get_FIFO_count(const struct device *dev, uint16_t *fifo_count) {
	struct icm20948_data *drv_data = dev->data;
	if (icm20948_read(dev, ICM20948_BANK0, ICM20948_BANK0_FIFO_COUNTH, &drv_data->bank0.bytes.FIFO_COUNTH.byte, 2)!=0) {
		return -EIO;
	}
	drv_data->bank0.bytes.FIFO_COUNTH.byte &= 0x1f;
	*fifo_count = sys_be16_to_cpu(*(uint16_t*)&drv_data->bank0.bytes.FIFO_COUNTH.byte);
	return 0;
}

int icm20948_read_FIFO(const struct device *dev, size_t length) {
	struct icm20948_data *drv_data = dev->data;
	if (length > sizeof(drv_data->fifo)) {
		LOG_ERR("Not enough space in buffer for FIFO data.");
		return -ENOMEM;
	}
	return icm20948_read(dev, ICM20948_BANK0, ICM20948_BANK0_FIFO_R_W, &drv_data->fifo, length);
}

int icm20948_firmware_load(const struct device *dev) {
		if (icm20948_mem_write(dev, DMP_LOAD_START, (const uint8_t*)&icm20948_dmp_firmware, sizeof(icm20948_dmp_firmware), true)!=0) {
			LOG_ERR("Error loading DMP firmware.");
			return -1;
		} else {
			LOG_DBG("DMP firmware loaded successfully");
		}
		return 0;

	}

int icm20948_mem_read(const struct device *dev, uint16_t addr, uint8_t *data, size_t length) {
	struct icm20948_data *drv_data = dev->data;
	unsigned int nread = 0;
	int rc=0;
	size_t chunksize;
	
	/* Use 0xFF as an invalid sentinel to force bank select on first iteration. */
	drv_data->bank0.bytes.MEM_BANK_SEL = 0xff;
	while (nread < length) {
		if (drv_data->bank0.bytes.MEM_BANK_SEL != (uint8_t)(addr >> 8)) {
			drv_data->bank0.bytes.MEM_BANK_SEL = (uint8_t)(addr >> 8);
			rc |= icm20948_write(dev, ICM20948_BANK0, ICM20948_BANK0_MEM_BANK_SEL, &drv_data->bank0.bytes.MEM_BANK_SEL, 1);
		}

		drv_data->bank0.bytes.MEM_ADDR = addr & 0xff;
		rc |= icm20948_write(dev, ICM20948_BANK0, ICM20948_BANK0_MEM_ADDR, &drv_data->bank0.bytes.MEM_ADDR, 1);
		
		chunksize = MIN(ICM20948_MAX_SERIAL_READ, MIN(DMP_MEM_BANK_SIZE - drv_data->bank0.bytes.MEM_ADDR, length));
		rc |= icm20948_read(dev, ICM20948_BANK0, ICM20948_BANK0_MEM_R_W, data+nread, chunksize);

		nread += chunksize;
		addr += chunksize;
		length -= chunksize;		
	}
	return rc;

}

int icm20948_mem_write(const struct device *dev, uint16_t addr, const uint8_t *data, size_t length, bool verify) {
	struct icm20948_data *drv_data = dev->data;
	int rc = 0;
	unsigned int nwritten = 0;
	size_t chunksize;
	uint8_t verify_buf[ICM20948_MAX_SERIAL_WRITE];
	
	/* Use 0xFF as an invalid sentinel to force bank select on first iteration. */
	drv_data->bank0.bytes.MEM_BANK_SEL = 0xff;
	while (nwritten < length) {
		if (drv_data->bank0.bytes.MEM_BANK_SEL != (uint8_t)(addr >> 8)) {
			drv_data->bank0.bytes.MEM_BANK_SEL = (uint8_t)(addr >> 8);
			rc |= icm20948_write(dev, ICM20948_BANK0, ICM20948_BANK0_MEM_BANK_SEL, &drv_data->bank0.bytes.MEM_BANK_SEL, 1);
		}

		drv_data->bank0.bytes.MEM_ADDR = addr & 0xff;
		rc |= icm20948_write(dev, ICM20948_BANK0, ICM20948_BANK0_MEM_ADDR, &drv_data->bank0.bytes.MEM_ADDR, 1);
		
		chunksize = MIN(ICM20948_MAX_SERIAL_WRITE, MIN(DMP_MEM_BANK_SIZE - drv_data->bank0.bytes.MEM_ADDR, length));
		rc |= icm20948_write(dev, ICM20948_BANK0, ICM20948_BANK0_MEM_R_W, data + nwritten, chunksize);

		// check if write was successful by reading back
		if (verify) {
			rc |= icm20948_write(dev, ICM20948_BANK0, ICM20948_BANK0_MEM_ADDR, &drv_data->bank0.bytes.MEM_ADDR, 1);
			rc |= icm20948_read(dev, ICM20948_BANK0, ICM20948_BANK0_MEM_R_W, verify_buf, chunksize);
			if (memcmp(data + nwritten, verify_buf, chunksize) != 0) {
				LOG_ERR("Memory write verification failed at bank %d, address 0x%x.", drv_data->bank0.bytes.MEM_BANK_SEL, drv_data->bank0.bytes.MEM_ADDR);
				return -EIO;
			}
		}

		nwritten += chunksize;
		addr += chunksize;
		length -= chunksize;		
	}
	if (verify) {
		LOG_DBG("Memory write verified successfully.");
	}
	return rc;

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


	while (!device_is_ready(cfg->i2c.bus)) {
		LOG_ERR("ICM20948 I2C bus device not ready");
		device_init(cfg->i2c.bus);
		// return -EIO;
		k_msleep(1000);
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
	icm20948_read(dev, ICM20948_BANK0, ICM20948_BANK0_PWR_MGMT_1, &drv_data->bank0.bytes.PWR_MGMT_1.byte, 1);
    drv_data->bank0.bytes.PWR_MGMT_1.bits.SLEEP = 0;
	icm20948_write(dev, ICM20948_BANK0, ICM20948_BANK0_PWR_MGMT_1, &drv_data->bank0.bytes.PWR_MGMT_1.byte, 1);

	/* set feature enables */
	icm20948_read(dev, ICM20948_BANK0, ICM20948_BANK0_USER_CTRL, &drv_data->bank0.bytes.USER_CTRL.byte, 1);
	drv_data->bank0.bytes.USER_CTRL.bits.DMP_EN = 0;
	drv_data->bank0.bytes.USER_CTRL.bits.FIFO_EN = cfg->fifo_enable;
	icm20948_write(dev, ICM20948_BANK0, ICM20948_BANK0_USER_CTRL, &drv_data->bank0.bytes.USER_CTRL.byte, 1);
	

	/* set accelerometer sample rate */
	icm20948_read(dev, ICM20948_BANK2, ICM20948_BANK2_ACCEL_SMPLRT_DIV_1, &drv_data->bank2.bytes.ACCEL_SMPLRT_DIV_1.byte, 2);
    drv_data->bank2.bytes.ACCEL_SMPLRT_DIV_1.bits.ACCEL_SMPLRT_DIV = cfg->accel_sr_div >> 8;
    drv_data->bank2.bytes.ACCEL_SMPLRT_DIV_2 = cfg->accel_sr_div & 0xFF;
	icm20948_write(dev, ICM20948_BANK2, ICM20948_BANK2_ACCEL_SMPLRT_DIV_1, &drv_data->bank2.bytes.ACCEL_SMPLRT_DIV_1.byte, 2);

    /* set accelerometer config */
	icm20948_read(dev, ICM20948_BANK2, ICM20948_BANK2_ACCEL_CONFIG, &drv_data->bank2.bytes.ACCEL_CONFIG.byte, 1);
    drv_data->bank2.bytes.ACCEL_CONFIG.bits.ACCEL_FCHOICE = cfg->accel_fchoice;
    drv_data->bank2.bytes.ACCEL_CONFIG.bits.ACCEL_FS_SEL = cfg->accel_fs_sel;
    drv_data->bank2.bytes.ACCEL_CONFIG.bits.ACCEL_DLPFCFG = cfg->accel_dlpf;
    icm20948_write(dev, ICM20948_BANK2, ICM20948_BANK2_ACCEL_CONFIG, &drv_data->bank2.bytes.ACCEL_CONFIG.byte, 1);

    /* set gyroscope sample rate */
	icm20948_read(dev, ICM20948_BANK2, ICM20948_BANK2_GYRO_SMPLRT_DIV, &drv_data->bank2.bytes.GYRO_SMPLRT_DIV, 1);
    drv_data->bank2.bytes.GYRO_SMPLRT_DIV = cfg->gyro_sr_div;
    icm20948_write(dev, ICM20948_BANK2, ICM20948_BANK2_GYRO_SMPLRT_DIV, &drv_data->bank2.bytes.GYRO_SMPLRT_DIV, 1);

    /* set gyroscope config */
	icm20948_read(dev, ICM20948_BANK2, ICM20948_BANK2_GYRO_CONFIG_1, &drv_data->bank2.bytes.GYRO_CONFIG_1.byte, 1);
    drv_data->bank2.bytes.GYRO_CONFIG_1.bits.GYRO_FCHOICE = cfg->gyro_fchoice;
    drv_data->bank2.bytes.GYRO_CONFIG_1.bits.GYRO_FS_SEL = cfg->gyro_fs_sel;
    drv_data->bank2.bytes.GYRO_CONFIG_1.bits.GYRO_DLPFCFG = cfg->gyro_dlpf;
    icm20948_write(dev, ICM20948_BANK2, ICM20948_BANK2_GYRO_CONFIG_1, &drv_data->bank2.bytes.GYRO_CONFIG_1.byte, 1);

    /* enable i2c passthrough to communicate with magnetometer */
	icm20948_read(dev, ICM20948_BANK0, ICM20948_BANK0_INT_PIN_CFG, &drv_data->bank0.bytes.INT_PIN_CFG.byte, 1);
    drv_data->bank0.bytes.INT_PIN_CFG.bits.BYPASS_EN = 1;
	icm20948_write(dev, ICM20948_BANK0, ICM20948_BANK0_INT_PIN_CFG, &drv_data->bank0.bytes.INT_PIN_CFG.byte, 1);

	icm20948_firmware_load(dev);

	LOG_INF("ICM20948 initialization complete");

	return 0;
}

#define ICM20948_DEFINE(inst)											\
																		\
	static struct icm20948_data icm20948_data_##inst;					\
																		\
	static const struct icm20948_config icm20948_config_##inst = {		\
		.i2c = I2C_DT_SPEC_INST_GET(inst),								\
		.int_gpio = GPIO_DT_SPEC_INST_GET(inst, irq_gpios),				\
		                                                                \
        .gyro_sr_div = DT_INST_PROP(inst, gyro_sr_div),					\
        .gyro_dlpf = DT_INST_ENUM_IDX(inst, gyro_dlpf),					\
        .gyro_fs_sel = DT_INST_ENUM_IDX(inst, gyro_fs_sel),				\
        .gyro_fchoice = DT_INST_PROP(inst, gyro_fchoice),				\
        .accel_sr_div = DT_INST_PROP(inst, accel_sr_div),				\
        .accel_dlpf = DT_INST_ENUM_IDX(inst, accel_dlpf),				\
        .accel_fs_sel = DT_INST_ENUM_IDX(inst, accel_fs_sel),			\
        .accel_fchoice = DT_INST_PROP(inst, accel_fchoice),				\
		.fifo_enable = DT_INST_PROP(inst, fifo_enable),					\
																		\
	};																	\
																		\
	SENSOR_DEVICE_DT_INST_DEFINE(inst, icm20948_init, NULL,				\
			      &icm20948_data_##inst, &icm20948_config_##inst,		\
			      POST_KERNEL, ICM20948_INIT_PRIORITY,					\
			      &icm20948_driver_api);								

DT_INST_FOREACH_STATUS_OKAY(ICM20948_DEFINE);
 