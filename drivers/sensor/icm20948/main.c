#include <zephyr/drivers/i2c.h>
#include <zephyr/init.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/sys/util.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#include "Invn/Devices/Drivers/ICM20948/Icm20948Defs.h"

#include "main.h"
#include "sensor.h"

#define DT_DRV_COMPAT invensense_icm20948

#if DT_NUM_INST_STATUS_OKAY(DT_DRV_COMPAT) == 0
#warning "Custom ICM20948 driver enabled without any devices"
#endif

LOG_MODULE_REGISTER(icm20948, CONFIG_SENSOR_LOG_LEVEL);

int icm20948_channel_get(const struct device *dev,
			       enum sensor_channel chan,
			       struct sensor_value *val)
{
	return 0;
}

int icm20948_sample_fetch(const struct device *dev,
				enum sensor_channel chan) {
	return 0;
}

static const struct sensor_driver_api icm20948_driver_api = {
	.sample_fetch = &icm20948_sample_fetch,
	.channel_get = &icm20948_channel_get,
};

int icm20948_init(const struct device *dev)
{
	struct icm20948_data *data = dev->data;
	const struct icm20948_config *cfg = dev->config;
	struct inv_icm20948 *icm_device = &data->icm_device;
	int rc = 0;

	LOG_INF("Initializing ICM20948 on I2C bus %p, address 0x%02x", cfg->i2c.bus, cfg->i2c.addr);

	data->icm_device.serif.context = (void *)&cfg->i2c;
	data->icm_device.serif.read_reg = (int(*)(void*, uint8_t, uint8_t*,uint32_t))i2c_burst_read_dt;
	data->icm_device.serif.write_reg = (int(*)(void*, uint8_t, const uint8_t*,uint32_t))i2c_burst_write_dt;
	data->icm_device.serif.max_read = INV_MAX_SERIAL_READ;
	data->icm_device.serif.max_write = INV_MAX_SERIAL_WRITE;
	data->icm_device.serif.is_spi = 0;

	// inv_icm20948_reset_states(&icm_device, &data->icm_device.serif);

	// inv_icm20948_register_aux_compass(&icm_device, INV_ICM20948_COMPASS_ID_AK09916, AK0991x_DEFAULT_I2C_ADDR);

	rc |= icm20948_sensor_setup(icm_device);
	if (rc != 0) {
		LOG_ERR("Failed to initialize ICM20948: %d", rc);
		return rc;
	}
	return 0;
}
#define ICM20948_DEFINE(inst)\
\
	static struct icm20948_data icm20948_data_##inst;\
\
	static const struct icm20948_config icm20948_config_##inst = {\
		.i2c = I2C_DT_SPEC_INST_GET(inst),\
		.int_gpio = GPIO_DT_SPEC_INST_GET(inst, irq_gpios),\
		.base_driver = {\
			.wake_state = 0,\
			.chip_lp_ln_mode = CHIP_LOW_POWER_ICM20948,\
			.pwr_mgmt_1 = BIT_CLK_PLL,\
			.pwr_mgmt_2 = BIT_PWR_ACCEL_STBY | BIT_PWR_GYRO_STBY | BIT_PWR_PRESSURE_STBY,\
			.user_ctrl = 0,\
			.gyro_div = DT_INST_PROP(inst, gyro_sr_div),\
			.secondary_div = DT_INST_PROP(inst, mag_sr_div),\
			.accel_div = DT_INST_PROP(inst, accel_sr_div),\
			.gyro_averaging = 1,\
			.accel_averaging = 1,\
			.gyro_fullscale = DT_INST_ENUM_IDX(inst, gyro_fs_sel),\
			.accel_fullscale = DT_INST_ENUM_IDX(inst, accel_fs_sel),\
			.lp_en_support = 1,\
			.firmware_loaded = 0,\
			.serial_interface = SERIAL_INTERFACE_I2C,\
			.timebase_correction_pll = 0\
		}\
	};\
\
	SENSOR_DEVICE_DT_INST_DEFINE(inst, icm20948_init, NULL,\
			      &icm20948_data_##inst, &icm20948_config_##inst,\
			      POST_KERNEL, ICM20948_INIT_PRIORITY,\
			      &icm20948_driver_api);

DT_INST_FOREACH_STATUS_OKAY(ICM20948_DEFINE);
 