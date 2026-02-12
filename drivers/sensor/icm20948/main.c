#include <zephyr/drivers/i2c.h>
#include <zephyr/init.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/sys/util.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>

#include "Invn/Devices/Drivers/ICM20948/Icm20948Defs.h"

#include "main.h"
#include "sensor.h"
#include "trigger.h"


#define DT_DRV_COMPAT invensense_icm20948

#if DT_NUM_INST_STATUS_OKAY(DT_DRV_COMPAT) == 0
#warning "Custom ICM20948 driver enabled without any devices"
#endif

LOG_MODULE_REGISTER(icm20948, CONFIG_SENSOR_LOG_LEVEL);

static const struct sensor_driver_api icm20948_driver_api = {
	.channel_get = &icm20948_channel_get,
	.sample_fetch = &icm20948_sample_fetch,
	.trigger_set = &icm20948_trigger_set,
};

int icm20948_init(const struct device *dev)
{
	const struct icm20948_config *cfg = dev->config;
	struct inv_icm20948 *icm_device = dev->data;
	int rc = 0;

	LOG_INF("Initializing ICM20948 on I2C bus %p, address 0x%02x", cfg->i2c.bus, cfg->i2c.addr);

	icm_device->serif.context = (void *)&cfg->i2c;
	icm_device->serif.read_reg = (int(*)(void*, uint8_t, uint8_t*,uint32_t))i2c_burst_read_dt;
	icm_device->serif.write_reg = (int(*)(void*, uint8_t, const uint8_t*,uint32_t))i2c_burst_write_dt;
	icm_device->serif.max_read = INV_MAX_SERIAL_READ;
	icm_device->serif.max_write = INV_MAX_SERIAL_WRITE;
	icm_device->serif.is_spi = 0;

	

	rc |= icm20948_sensor_setup(icm_device, cfg);
	if (rc != 0) {
		LOG_ERR("Failed to initialize ICM20948: %d", rc);
		return rc;
	}


	// rc |= icm20948_run_selftest(icm_device);	
	// rc != icm20948_sensor_setup(icm_device, cfg);

	rc |= inv_icm20948_enable_sensor(icm_device, INV_ICM20948_SENSOR_ACCELEROMETER, 1);
	rc |= inv_icm20948_enable_sensor(icm_device, INV_ICM20948_SENSOR_GYROSCOPE, 1);
	rc |= inv_icm20948_enable_sensor(icm_device, INV_ICM20948_SENSOR_GEOMAGNETIC_FIELD, 1);
	if (rc != 0) {
		LOG_ERR("Failed to enable sensors: %d", rc);
		return rc;
	}

	gpio_pin_interrupt_configure_dt(&cfg->int_gpio, GPIO_INT_EDGE_TO_ACTIVE);

	return 0;
}
#define ICM20948_DEFINE(inst)\
\
	static struct inv_icm20948 icm20948_device_##inst = {0};\
\
	static const struct icm20948_config icm20948_config_##inst = {\
		.i2c = I2C_DT_SPEC_INST_GET(inst),\
		.int_gpio = GPIO_DT_SPEC_INST_GET(inst, int_gpios),\
		.gyro_div = DT_INST_PROP(inst, gyro_sr_div),\
		.secondary_div = DT_INST_PROP(inst, mag_sr_div),\
		.accel_div = DT_INST_PROP(inst, accel_sr_div),\
		.gyro_averaging = DT_INST_ENUM_IDX(inst, gyro_dlpf),\
		.accel_averaging = DT_INST_ENUM_IDX(inst, accel_dlpf),\
		.gyro_fullscale = DT_INST_ENUM_IDX(inst, gyro_fs_sel),\
		.accel_fullscale = DT_INST_ENUM_IDX(inst, accel_fs_sel),\
	};\
\
	SENSOR_DEVICE_DT_INST_DEFINE(inst, icm20948_init, NULL,\
			      &icm20948_device_##inst, &icm20948_config_##inst,\
			      POST_KERNEL, ICM20948_INIT_PRIORITY,\
			      &icm20948_driver_api);

DT_INST_FOREACH_STATUS_OKAY(ICM20948_DEFINE);
 