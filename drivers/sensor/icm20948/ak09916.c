#include <zephyr/drivers/i2c.h>
#include <zephyr/init.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#include "ak09916.h"


#define DT_DRV_COMPAT asahi_kasei_ak09916

LOG_MODULE_REGISTER(ak09916, CONFIG_SENSOR_LOG_LEVEL);

int ak09916_set_mode(const struct device *dev, ak09916_mode_t mode) {
    ak09916_reg_t registers = {0};
    registers.bytes.CONTROL_2.bits.MODE = mode;
    return ak09916_write(dev, AK09916_ADDR_CONTROL_2, &registers.bytes.CONTROL_2.byte, 1);
}

int ak09916_read(const struct device *dev, uint8_t reg_addr, uint8_t *data, size_t length) {
    const struct ak09916_config *cfg = dev->config;

    if (i2c_burst_read_dt(&cfg->i2c, reg_addr, data, length) < 0) {
        LOG_ERR("Failed to read from AK09916 device.");
        return -EIO;
    }

    return 0;
}

int ak09916_write(const struct device *dev, uint8_t reg_addr, const uint8_t *data, size_t length) {
    const struct ak09916_config *cfg = dev->config;

    if (i2c_burst_write_dt(&cfg->i2c, reg_addr, data, length) < 0) {
        LOG_ERR("Failed to write to AK09916 device.");
        return -EIO;
    }
    return 0;
}

int ak09916_sample_fetch(const struct device *dev, enum sensor_channel chan) {
    struct ak09916_data *data = dev->data;

    ak09916_read(dev, AK09916_ADDR_STATUS_1, &data->registers.bytes.STATUS_1.byte, 9);
    if (!(data->registers.bytes.STATUS_1.bits.DRDY)) {
        // LOG_WRN("AK09916 data not ready.");
        return -EAGAIN;
    }

    ak09916_read(dev, AK09916_ADDR_MAG_XOUT_L, &data->registers.bytes.MAG_XOUT_L, 7);

    return 0;
}

int ak09916_convert_mag(struct sensor_value *val, float *float_val, int16_t raw_val, uint8_t st2) {
    /* Check for overflow */
    if (st2 & 0x08) {
        return -ERANGE;
    }
    /* Sensitivity adjustment values from datasheet */
    *float_val = (float)raw_val * 0.15f; // convert to Gauss
    return sensor_value_from_float(val, *float_val);
}

int ak09916_channel_get(const struct device *dev, enum sensor_channel chan, struct sensor_value *val) {
    struct ak09916_data *data = dev->data;

    int16_t mag_x = sys_le16_to_cpu(*(int16_t *)&data->registers.bytes.MAG_XOUT_L);
    int16_t mag_y = sys_le16_to_cpu(*(int16_t *)&data->registers.bytes.MAG_YOUT_L);
    int16_t mag_z = sys_le16_to_cpu(*(int16_t *)&data->registers.bytes.MAG_ZOUT_L);

    switch (chan) {
        case SENSOR_CHAN_MAGN_X:
            return ak09916_convert_mag(&val[0], &data->converted.mag_x, mag_x, data->registers.bytes.STATUS_2.byte);
        case SENSOR_CHAN_MAGN_Y:
            return ak09916_convert_mag(&val[0], &data->converted.mag_y, mag_y, data->registers.bytes.STATUS_2.byte);
        case SENSOR_CHAN_MAGN_Z:
            return ak09916_convert_mag(&val[0], &data->converted.mag_z, mag_z, data->registers.bytes.STATUS_2.byte);
        case SENSOR_CHAN_MAGN_XYZ:
            ak09916_convert_mag(&val[0], &data->converted.mag_x, mag_x, data->registers.bytes.STATUS_2.byte);
            ak09916_convert_mag(&val[1], &data->converted.mag_y, mag_y, data->registers.bytes.STATUS_2.byte);
            ak09916_convert_mag(&val[2], &data->converted.mag_z, mag_z, data->registers.bytes.STATUS_2.byte);
            return 0;
        default:
            return -ENOTSUP;
    }
}
    

int ak09916_init(const struct device *dev) {
    struct ak09916_data *data = dev->data;
    const struct ak09916_config *cfg = dev->config;
    LOG_INF("Initializing AK09916 magnetometer");
    /* check bus is ready*/
    if (!device_is_ready(cfg->i2c.bus)) {
		LOG_ERR("AK09916 I2C bus device not ready");
		return -EIO;
	}
    k_msleep(100); /* wait for sensor boot time */
    /* get company ID and device ID */
    if (ak09916_read(dev, AK09916_ADDR_COMPANY_ID, &data->registers.bytes.COMPANY_ID, 2) < 0) {
        LOG_ERR("Failed to read AK09916 WIA.");
        return -EIO;
    }
    /* check values */
    if (data->registers.bytes.COMPANY_ID != AK09916_COMPANY_ID || data->registers.bytes.DEVICE_ID != AK09916_DEVICE_ID) {
        LOG_ERR("Invalid AK09916 WIA: COMPANY_ID=0x%02x, DEVICE_ID=0x%02x",
                data->registers.bytes.COMPANY_ID, data->registers.bytes.DEVICE_ID);
        return -EINVAL;
    }
    return ak09916_set_mode(dev, cfg->mode);
 }

struct sensor_driver_api ak09916_driver_api = {
    .sample_fetch = &ak09916_sample_fetch,
    .channel_get = &ak09916_channel_get,
};

static const ak09916_mode_t modes[] = {
    AK09916_MODE_POWERDOWN,
    AK09916_MODE_SINGLEMEASURE,
    AK09916_MODE_CONTINUOUS_10HZ,
    AK09916_MODE_CONTINUOUS_20HZ,
    AK09916_MODE_CONTINUOUS_50HZ,
    AK09916_MODE_CONTINUOUS_100HZ,
    AK09916_MODE_SELFTEST
};

#define AK09916_DEFINE(inst)											\
																		\
	static struct ak09916_data ak09916_data_##inst;					\
																		\
	static const struct ak09916_config ak09916_config_##inst = {		\
		.i2c = I2C_DT_SPEC_INST_GET(inst),								\
        .mode = modes[DT_INST_ENUM_IDX(inst, mode)],                        			\
	};																	\
																		\
	SENSOR_DEVICE_DT_INST_DEFINE(inst, ak09916_init, NULL,				\
			      &ak09916_data_##inst, &ak09916_config_##inst,		\
			      POST_KERNEL, AK09916_INIT_PRIORITY,				\
			      &ak09916_driver_api);								

DT_INST_FOREACH_STATUS_OKAY(AK09916_DEFINE);