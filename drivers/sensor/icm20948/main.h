#pragma once

#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/types.h>
#include <zephyr/devicetree.h>

#include "Invn/Devices/Drivers/ICM20948/Icm20948.h"
#include "Invn/Devices/Drivers/ICM20948/Icm20948MPUFifoControl.h"

struct icm20948_config {
    const struct i2c_dt_spec i2c;
    const struct gpio_dt_spec int_gpio;
    const unsigned char gyro_div;
    const unsigned short secondary_div;
    const short accel_div;
    const unsigned char gyro_averaging;
    const unsigned char accel_averaging;
    const uint8_t gyro_fullscale;
    const uint8_t accel_fullscale;

};

#define ICM20948_INIT_PRIORITY 91

int icm20948_init(const struct device *dev);

int icm20948_sample_fetch(const struct device *dev, enum sensor_channel chan);
int icm20948_channel_get(const struct device *dev, enum sensor_channel chan, struct sensor_value *val);
