# pragma once

#include <zephyr/types.h>

#define AK09916_REG_COUNT       (18)

#define AK09916_I2C_ADDR        ()
#define AK09916_COMPANY_ID      (0x48)
#define AK09916_DEVICE_ID       (0x09)

typedef enum {
    AK09916_ADDR_COMPANY_ID = 0x00,
    AK09916_ADDR_DEVICE_ID = 0x01,
    AK09916_ADDR_STATUS_1 = 0x10,
    AK09916_ADDR_MAG_XOUT_L = 0x11,
    AK09916_ADDR_MAG_XOUT_H = 0x12,
    AK09916_ADDR_MAG_YOUT_L = 0x13,
    AK09916_ADDR_MAG_YOUT_H = 0x14,
    AK09916_ADDR_MAG_ZOUT_L = 0x15,
    AK09916_ADDR_MAG_ZOUT_H = 0x16,
    AK09916_ADDR_STATUS_2 = 0x18,
    AK09916_ADDR_CONTROL_2 = 0x31,
    AK09916_ADDR_CONTROL_3 = 0x32
} ak09916_reg_addr_t;

typedef union {
    struct {
        uint8_t COMPANY_ID;
        uint8_t DEVICE_ID;
        uint8_t RSV1;
        uint8_t RSV2;
        union {
            struct {
                uint8_t DRDY               : 1;
                uint8_t DOR                : 1;
                uint8_t RSVD               : 6;
            } bits;
            uint8_t byte;
        } STATUS_1;
        uint8_t MAG_XOUT_L;
        uint8_t MAG_XOUT_H;
        uint8_t MAG_YOUT_L;
        uint8_t MAG_YOUT_H;
        uint8_t MAG_ZOUT_L;
        uint8_t MAG_ZOUT_H;
        uint8_t TMPS;
        union {
            struct {
                uint8_t RSVD               : 3;
                uint8_t HOFL               : 1;
                uint8_t RSV28              : 1;
                uint8_t RSV29              : 1;
                uint8_t RSV30              : 1;
                uint8_t RSVD1              : 1;
            } bits;
            uint8_t byte;
        } STATUS_2;
        uint8_t CONTROL_1;
        union {
            struct {
                uint8_t MODE0              : 1;
                uint8_t MODE1              : 1;
                uint8_t MODE2              : 1;
                uint8_t MODE3              : 1;
                uint8_t MODE4              : 1;
                uint8_t RSVD               : 3;
            } bits;
            uint8_t  byte;
        } CONTROL_2;
        union {
            struct {
                uint8_t SRST               : 1;
                uint8_t RSVD               : 7;
            } bits;
            uint8_t byte;
        } CONTROL_3;
    } bytes;
    uint8_t arr[AK09916_REG_COUNT];
} ak09916_reg_t;

int ak09916_convert_mag(struct sensor_value *val int16_t raw_val, inst16_t scale, uint8_t st2);
int ak09916_init(const struct device *dev);
