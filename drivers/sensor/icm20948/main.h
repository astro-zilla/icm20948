#pragma once

#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/types.h>
#include <zephyr/devicetree.h>

#include "Invn/Devices/Drivers/ICM20948/Icm20948.h"

struct icm20948_data {
    struct inv_icm20948 icm_device;
};

struct icm20948_config {
    const struct i2c_dt_spec i2c;
    const struct gpio_dt_spec int_gpio;
    const struct base_driver_t base_driver;
};


#define ICM20948_INIT_PRIORITY 91

int icm20948_init(const struct device *dev);

int icm20948_sample_fetch(const struct device *dev, enum sensor_channel chan);
int icm20948_channel_get(const struct device *dev, enum sensor_channel chan, struct sensor_value *val);
