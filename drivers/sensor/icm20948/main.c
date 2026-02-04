#include <zephyr/drivers/i2c.h>
#include <zephyr/init.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/sys/util.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#include "icm20948.h"
#include "icm20948_img.dmp3a.h"

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
 