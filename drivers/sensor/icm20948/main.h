#pragma once

#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/types.h>
#include <zephyr/devicetree.h>

typedef struct {
    float accel_x;
    float accel_y;
    float accel_z;

    float gyro_x;
    float gyro_y;
    float gyro_z;

    float temp;
} icm20948_converted_t;

struct icm20948_data{
    icm20948_converted_t converted;
};


struct icm20948_config {
    const struct i2c_dt_spec i2c;
    const struct gpio_dt_spec int_gpio;

    uint8_t gyro_sr_div;
    uint8_t gyro_dlpf;
    uint8_t gyro_fs_sel;
    bool gyro_fchoice;

    uint8_t accel_sr_div;
    uint8_t accel_dlpf;
    uint8_t accel_fs_sel;
    bool accel_fchoice;

    bool fifo_enable;
};

#define ICM20948_INIT_PRIORITY 91

int icm20948_init(const struct device *dev);

int icm20948_sample_fetch(const struct device *dev, enum sensor_channel chan);
int icm20948_channel_get(const struct device *dev, enum sensor_channel chan, struct sensor_value *val);
