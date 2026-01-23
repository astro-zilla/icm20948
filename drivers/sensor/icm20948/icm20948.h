#pragma once

#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/types.h>
#include <zephyr/devicetree.h>

#define ICM20948_BANK0_REG_COUNT            (65)
#define ICM20948_BANK1_REG_COUNT            (14)
#define ICM20948_BANK2_REG_COUNT            (20)
#define ICM20948_BANK3_REG_COUNT            (25)

#define ICM20948_WHO_AM_I_DEFAULT           (0xEA)
#define ICM20948_EXT_SLV_SENS_DATA_COUNT    (25)

#define ICM20948_ADDR_REG_BANK_SEL          (0x7F)
#define ICM20948_REG_BANK_SEL_SHIFT         (4)

typedef enum {
    ICM20948_BANK0 = 0x00 << ICM20948_REG_BANK_SEL_SHIFT,
    ICM20948_BANK1 = 0x01 << ICM20948_REG_BANK_SEL_SHIFT,
    ICM20948_BANK2 = 0x02 << ICM20948_REG_BANK_SEL_SHIFT,
    ICM20948_BANK3 = 0x03 << ICM20948_REG_BANK_SEL_SHIFT
} icm20948_reg_bank_sel_t;

typedef enum {
    ICM20948_BANK0_WHO_AM_I = 0x00,
    ICM20948_BANK0_USER_CTRL = 0x03,
    ICM20948_BANK0_LP_CONFIG = 0x05,
    ICM20948_BANK0_PWR_MGMT_1 = 0x06,
    ICM20948_BANK0_PWR_MGMT_2 = 0x07,
    ICM20948_BANK0_INT_PIN_CFG = 0x0F,
    ICM20948_BANK0_INT_ENABLE = 0x10,
    ICM20948_BANK0_INT_ENABLE_1 = 0x11,
    ICM20948_BANK0_INT_ENABLE_2 = 0x12,
    ICM20948_BANK0_INT_ENABLE_3 = 0x13,
    ICM20948_BANK0_I2C_MST_STATUS = 0x17,
    ICM20948_BANK0_INT_STATUS = 0x19,
    ICM20948_BANK0_INT_STATUS_1 = 0x1A,
    ICM20948_BANK0_INT_STATUS_2 = 0x1B,
    ICM20948_BANK0_INT_STATUS_3 = 0x1C,
    ICM20948_BANK0_DELAY_TIMEH = 0x28,
    ICM20948_BANK0_DELAY_TIMEL = 0x29,
    ICM20948_BANK0_ACCEL_XOUT_H = 0x2D,
    ICM20948_BANK0_ACCEL_XOUT_L = 0x2E,
    ICM20948_BANK0_ACCEL_YOUT_H = 0x2F,
    ICM20948_BANK0_ACCEL_YOUT_L = 0x30,
    ICM20948_BANK0_ACCEL_ZOUT_H = 0x31,
    ICM20948_BANK0_ACCEL_ZOUT_L = 0x32,
    ICM20948_BANK0_GYRO_XOUT_H = 0x33,
    ICM20948_BANK0_GYRO_XOUT_L = 0x34,
    ICM20948_BANK0_GYRO_YOUT_H = 0x35,
    ICM20948_BANK0_GYRO_YOUT_L = 0x36,
    ICM20948_BANK0_GYRO_ZOUT_H = 0x37,
    ICM20948_BANK0_GYRO_ZOUT_L = 0x38,
    ICM20948_BANK0_TEMP_OUT_H = 0x39,
    ICM20948_BANK0_TEMP_OUT_L = 0x3A,
    ICM20948_BANK0_EXT_SLV_SENS_DATA_00 = 0x3B,
    ICM20948_BANK0_EXT_SLV_SENS_DATA_01 = 0x3C,
    ICM20948_BANK0_EXT_SLV_SENS_DATA_02 = 0x3D,
    ICM20948_BANK0_EXT_SLV_SENS_DATA_03 = 0x3E,
    ICM20948_BANK0_EXT_SLV_SENS_DATA_04 = 0x3F,
    ICM20948_BANK0_EXT_SLV_SENS_DATA_05 = 0x40,
    ICM20948_BANK0_EXT_SLV_SENS_DATA_06 = 0x41,
    ICM20948_BANK0_EXT_SLV_SENS_DATA_07 = 0x42,
    ICM20948_BANK0_EXT_SLV_SENS_DATA_08 = 0x43,
    ICM20948_BANK0_EXT_SLV_SENS_DATA_09 = 0x44,
    ICM20948_BANK0_EXT_SLV_SENS_DATA_10 = 0x45,
    ICM20948_BANK0_EXT_SLV_SENS_DATA_11 = 0x46,
    ICM20948_BANK0_EXT_SLV_SENS_DATA_12 = 0x47,
    ICM20948_BANK0_EXT_SLV_SENS_DATA_13 = 0x48,
    ICM20948_BANK0_EXT_SLV_SENS_DATA_14 = 0x49,
    ICM20948_BANK0_EXT_SLV_SENS_DATA_15 = 0x4A,
    ICM20948_BANK0_EXT_SLV_SENS_DATA_16 = 0x4B,
    ICM20948_BANK0_EXT_SLV_SENS_DATA_17 = 0x4C,
    ICM20948_BANK0_EXT_SLV_SENS_DATA_18 = 0x4D,
    ICM20948_BANK0_EXT_SLV_SENS_DATA_19 = 0x4E,
    ICM20948_BANK0_EXT_SLV_SENS_DATA_20 = 0x4F,
    ICM20948_BANK0_EXT_SLV_SENS_DATA_21 = 0x50,
    ICM20948_BANK0_EXT_SLV_SENS_DATA_22 = 0x51,
    ICM20948_BANK0_EXT_SLV_SENS_DATA_23 = 0x52,
    ICM20948_BANK0_FIFO_EN_1 = 0x66,
    ICM20948_BANK0_FIFO_EN_2 = 0x67,
    ICM20948_BANK0_FIFO_RST = 0x68,
    ICM20948_BANK0_FIFO_MODE = 0x69,
    ICM20948_BANK0_FIFO_COUNTH = 0x70,
    ICM20948_BANK0_FIFO_COUNTL = 0x71,
    ICM20948_BANK0_FIFO_R_W = 0x72,
    ICM20948_BANK0_DATA_RDY_STATUS = 0x74,
    ICM20948_BANK0_FIFO_CFG = 0x76,
} icm20948_reg_bank0_addr_t;

typedef enum {
    ICM20948_BANK1_SELF_TEST_X_GYRO = 0x02,
    ICM20948_BANK1_SELF_TEST_Y_GYRO = 0x03,
    ICM20948_BANK1_SELF_TEST_Z_GYRO = 0x04,
    ICM20948_BANK1_SELF_TEST_X_ACCEL = 0x0E,
    ICM20948_BANK1_SELF_TEST_Y_ACCEL = 0x0F,
    ICM20948_BANK1_SELF_TEST_Z_ACCEL = 0x10,
    ICM20948_BANK1_XA_OFFS_H = 0x14,
    ICM20948_BANK1_XA_OFFS_L = 0x15,
    ICM20948_BANK1_YA_OFFS_H = 0x17,
    ICM20948_BANK1_YA_OFFS_L = 0x18,
    ICM20948_BANK1_ZA_OFFS_H = 0x1A,
    ICM20948_BANK1_ZA_OFFS_L = 0x1B,
    ICM20948_BANK1_TIMEBASE_CORRECTION_PLL = 0x28,
} icm20948_reg_bank_1_addr_t;

typedef enum {
    ICM20948_BANK2_GYRO_SMPLRT_DIV = 0x00,
    ICM20948_BANK2_GYRO_CONFIG_1 = 0x01,
    ICM20948_BANK2_GYRO_CONFIG_2 = 0x02,
    ICM20948_BANK2_XG_OFFS_USRH = 0x03,
    ICM20948_BANK2_XG_OFFS_USRL = 0x04,
    ICM20948_BANK2_YG_OFFS_USRH = 0x05,
    ICM20948_BANK2_YG_OFFS_USRL = 0x06,
    ICM20948_BANK2_ZG_OFFS_USRH = 0x07,
    ICM20948_BANK2_ZG_OFFS_USRL = 0x08,
    ICM20948_BANK2_ODR_ALIGN_EN = 0x09,
    ICM20948_BANK2_ACCEL_SMPLRT_DIV_1 = 0x10,
    ICM20948_BANK2_ACCEL_SMPLRT_DIV_2 = 0x11,
    ICM20948_BANK2_ACCEL_INTEL_CTRL = 0x12,
    ICM20948_BANK2_ACCEL_WOM_THR = 0x13,
    ICM20948_BANK2_ACCEL_CONFIG = 0x14,
    ICM20948_BANK2_ACCEL_CONFIG_2 = 0x15,
    ICM20948_BANK2_FSYNC_CONFI = 0x52,
    ICM20948_BANK2_TEMP_CONFIG = 0x53,
    ICM20948_BANK2_MOD_CTRL_USR = 0x54,
} icm20948_reg_bank2_addr_t;

typedef enum {
    ICM20948_BANK3_I2C_MST_ODR_CONFIG = 0x00,
    ICM20948_BANK3_I2C_MST_CTRL = 0x01,
    ICM20948_BANK3_I2C_MST_DELAY_CTRL = 0x02,
    ICM20948_BANK3_I2C_SLV0_ADDR = 0x03,
    ICM20948_BANK3_I2C_SLV0_REG = 0x04,
    ICM20948_BANK3_I2C_SLV0_CTRL = 0x05,
    ICM20948_BANK3_I2C_SLV0_DO = 0x06,
    ICM20948_BANK3_I2C_SLV1_ADDR = 0x07,
    ICM20948_BANK3_I2C_SLV1_REG = 0x08,
    ICM20948_BANK3_I2C_SLV1_CTRL = 0x09,
    ICM20948_BANK3_I2C_SLV1_DO = 0x0A,
    ICM20948_BANK3_I2C_SLV2_ADDR = 0x0B,
    ICM20948_BANK3_I2C_SLV2_REG = 0x0C,
    ICM20948_BANK3_I2C_SLV2_CTRL = 0x0D,
    ICM20948_BANK3_I2C_SLV2_DO = 0x0E,
    ICM20948_BANK3_I2C_SLV3_ADDR = 0x0F,
    ICM20948_BANK3_I2C_SLV3_REG = 0x10,
    ICM20948_BANK3_I2C_SLV3_CTRL = 0x11,
    ICM20948_BANK3_I2C_SLV3_DO = 0x12,
    ICM20948_BANK3_I2C_SLV4_ADDR = 0x13,
    ICM20948_BANK3_I2C_SLV4_REG = 0x14,
    ICM20948_BANK3_I2C_SLV4_CTRL = 0x15,
    ICM20948_BANK3_I2C_SLV4_DO = 0x16,
    ICM20948_BANK3_I2C_SLV4_DI = 0x17,
} icm20948_reg_bank_3_addr_t;

typedef union {
    struct {
        uint8_t WHO_AM_I;

        union {
            struct {
                uint8_t RSVD                : 1;
                uint8_t I2C_MST_RST         : 1;
                uint8_t SRAM_RST            : 1;
                uint8_t DMP_RST             : 1;
                uint8_t I2C_IF_DS           : 1;
                uint8_t I2C_MST_EN          : 1;
                uint8_t FIFO_EN             : 1;
                uint8_t DMP_EN              : 1;
            } bits;
            uint8_t byte;
        } USER_CTRL;

        union {
            struct {
                uint8_t RSVD0               : 4;
                uint8_t GYRO_CYCLE          : 1;
                uint8_t ACCEL_CYCLE         : 1;
                uint8_t I2C_MST_CYCLE       : 1;
                uint8_t RSVD1               : 1;
            } bits;
            uint8_t byte;
        } LP_CONFIG;

        union {
            struct {
                uint8_t CLKSEL              : 3;
                uint8_t TEMP_DIS            : 1;
                uint8_t RSVD                : 1;
                uint8_t LP_EN               : 1;
                uint8_t SLEEP               : 1;
                uint8_t DEVICE_RESET        : 1;
            } bits;
            uint8_t byte;
        } PWR_MGMT_1;

        union {
            struct {
                uint8_t DISABLE_GYRO        : 3;
                uint8_t DISABLE_ACCEL       : 3;
                uint8_t RSVD                : 2;
            } bits;
            uint8_t byte;
        } PWR_MGMT_2;

        union {
            struct {
                uint8_t RSVD                : 1;
                uint8_t BYPASS_EN           : 1;
                uint8_t FSYNC_INT_MODE_EN   : 1;
                uint8_t ACTL_FSYNC          : 1;
                uint8_t INT_ANYRD_2CLEAR    : 1;
                uint8_t INT1_LATCH__EN      : 1;
                uint8_t INT1_OPEN           : 1;
                uint8_t INT1_ACTL           : 1;
            } bits;
            uint8_t byte;
        } INT_PIN_CFG;

        union {
            struct {
                uint8_t I2C_MST_INT_EN      : 1;
                uint8_t DMP_INT1_EN         : 1;
                uint8_t PLL_RDY_EN          : 1;
                uint8_t WOM_INT_EN          : 1;
                uint8_t RSVD                : 3;
                uint8_t REG_WOF_EN          : 1;
            } bits;
            uint8_t byte;
        } INT_ENABLE;

        union {
            struct {
                uint8_t RAW_DATA_0_RDY_EN   : 1;
                uint8_t RSVD                : 7;
            } bits;
            uint8_t byte;
        } INT_ENABLE_1;

        union {
            struct {
                uint8_t FIFO_OVERFLOW_EN    : 5;
                uint8_t RSVD                : 3;
            } bits;
            uint8_t byte;
        } INT_ENABLE_2;

        union {
            struct {
                uint8_t FIFO_W_EN           : 5;
                uint8_t RSVD                : 3;
            } bits;
            uint8_t byte;
        } INT_ENABLE_3;

        union {
            struct {
                uint8_t I2C_SLV0_NACK       : 1;
                uint8_t I2C_SLV1_NACK       : 1;
                uint8_t I2C_SLV2_NACK       : 1;
                uint8_t I2C_SLV3_NACK       : 1;
                uint8_t I2C_SLV4_NACK       : 1;
                uint8_t I2C_LOST_ARB        : 1;
                uint8_t I2C_SLV4_DONE       : 1;
                uint8_t PASS_THROUGH        : 1;
            } bits;
            uint8_t byte;
        } I2C_MST_STATUS;

        uint8_t RESERVED;

        union {
            struct {
                uint8_t I2C_MST_INT         : 1;
                uint8_t DMP_INT1            : 1;
                uint8_t PLL_RDY_INT         : 1;
                uint8_t WOM_INT             : 1;
                uint8_t RSVD                : 4;
            } bits;
            uint8_t byte;
        } INT_STATUS;

        union {
            struct {
                uint8_t RAW_DATA_0_RDY_INT  : 1;
                uint8_t RSVD                : 7;
            } bits;
            uint8_t byte;
        } INT_STATUS_1;

        union {
            struct {
                uint8_t FIFO_OVERFLOW_INT   : 5;
                uint8_t RSVD                : 3;
            } bits;
            uint8_t byte;
        } INT_STATUS_2;

        union {
            struct {
                uint8_t FIFO_WM_INT         : 5;
                uint8_t RSVD                : 3;
            } bits;
            uint8_t byte;
        } INT_STATUS_3;

        uint8_t DELAY_TIMEH;
        uint8_t DELAY_TIMEL;
        uint8_t ACCEL_XOUT_H;
        uint8_t ACCEL_XOUT_L;
        uint8_t ACCEL_YOUT_H;
        uint8_t ACCEL_YOUT_L;
        uint8_t ACCEL_ZOUT_H;
        uint8_t ACCEL_ZOUT_L;
        uint8_t GYRO_XOUT_H;
        uint8_t GYRO_XOUT_L;
        uint8_t GYRO_YOUT_H;
        uint8_t GYRO_YOUT_L;
        uint8_t GYRO_ZOUT_H;
        uint8_t GYRO_ZOUT_L;
        uint8_t TEMP_OUT_H;
        uint8_t TEMP_OUT_L;

        uint8_t EXT_SLV_SENS_DATA[ICM20948_EXT_SLV_SENS_DATA_COUNT];

        union {
            struct {
                uint8_t SLV_0_FIFO_EN       : 1;
                uint8_t SLV_1_FIFO_EN       : 1;
                uint8_t SLV_2_FIFO_EN       : 1;
                uint8_t SLV_3_FIFO_EN       : 1;
                uint8_t RSVD                : 4;
            } bits;
            uint8_t byte;
        } FIFO_EN_1;

        union {
            struct {
                uint8_t TEMP_FIFO_EN        : 1;
                uint8_t GYRO_X_FIFO_EN      : 1;
                uint8_t GYRO_Y_FIFO_EN      : 1;
                uint8_t GYRO_Z_FIFO_EN      : 1;
                uint8_t ACCEL_FIFO_EN       : 1;
                uint8_t RSVD                : 3;
            } bits;
            uint8_t byte;
        } FIFO_EN_2;

        union {
            struct {
                uint8_t FIFO_RESET          : 5;
                uint8_t RSVD                : 3;
            } bits;
            uint8_t byte;
        } FIFO_RST;

        union {
            struct {
                uint8_t FIFO_MODE           : 5;
                uint8_t RSVD                : 3;
            } bits;
            uint8_t byte;
        } FIFO_MODE;

        union {
            struct {
                uint8_t FIFO_COUNTH         : 5;
                uint8_t RSVD                : 3;
            } bits;
            uint8_t byte;
        } FIFO_COUNTH;

        uint8_t FIFO_COUNTL;
        uint8_t FIFO_R_W;
        uint8_t DATA_RDY_STATUS;
        uint8_t FIFO_CFG;

        union {
            struct {
                uint8_t RSVD0               : 4;
                uint8_t USER_BANK           : 2;
                uint8_t RSVD1               : 2;
            } bits;
            uint8_t byte;
        } REG_BANK_SEL;
    } bytes;
    uint8_t arr[ICM20948_BANK0_REG_COUNT];
} icm20948_reg_bank_0_t;

typedef union {
    struct {
        uint8_t SELF_TEST_X_GYRO;
        uint8_t SELF_TEST_Y_GYRO;
        uint8_t SELF_TEST_Z_GYRO;
        uint8_t SELF_TEST_X_ACCEL;
        uint8_t SELF_TEST_Y_ACCEL;
        uint8_t SELF_TEST_Z_ACCEL;
        uint8_t XA_OFFS_H;

        union {
            struct {
                uint8_t RSVD                : 1;
                uint8_t XA_OFFS             : 7;
            } bits;
            uint8_t byte;
        } XA_OFFS_L;

        uint8_t YA_OFFS_H;

        union {
            struct {
                uint8_t RSVD                : 1;
                uint8_t YA_OFFS             : 7;
            } bits;
            uint8_t byte;
        } YA_OFFS_L;

        uint8_t ZA_OFFS_H;

        union {
            struct {
                uint8_t RSVD                : 1;
                uint8_t ZA_OFFS             : 7;
            } bits;
            uint8_t byte;
        } ZA_OFFS_L;

        uint8_t TIMEBASE_CORRECTION_PLL;

        union {
            struct {
                uint8_t RSVD0               : 4;
                uint8_t USER_BANK           : 2;
                uint8_t RSVD1               : 2;
            } bits;
            uint8_t byte;
        } REG_BANK_SEL;
    } bytes;
    uint8_t arr[ICM20948_BANK1_REG_COUNT];
} icm20948_reg_bank_1_t;

typedef union {
    struct {
        uint8_t GYRO_SMPLRT_DIV;

        union {
            struct {
                uint8_t GYRO_FCHOICE        : 1;
                uint8_t GYRO_FS_SEL         : 2;
                uint8_t GYRO_DLPFCFG        : 3;
                uint8_t RSVD                : 2;
            } bits;
            uint8_t byte;
        } GYRO_CONFIG_1;

        union {
            struct {
                uint8_t GYRO_AVGCFG         : 3;
                uint8_t ZGYRO_CTEN          : 1;
                uint8_t YGYRO_CTEN          : 1;
                uint8_t XGYRO_CTEN          : 1;
                uint8_t RSVD                : 2;
            } bits;
            uint8_t byte;
        } GYRO_CONFIG_2;

        uint8_t XG_OFFS_USRH;
        uint8_t XG_OFFS_USRL;
        uint8_t YG_OFFS_USRH;
        uint8_t YG_OFFS_USRL;
        uint8_t ZG_OFFS_USRH;
        uint8_t ZG_OFFS_USRL;

        union {
            struct {
                uint8_t ODR_ALIGN_EN        : 1;
                uint8_t RSVD                : 7;
            } bits;
            uint8_t byte;
        } ODR_ALIGN_EN;

        union {
            struct {
                uint8_t ACCEL_SMPLRT_DIV    : 4;
                uint8_t RSVD                : 4;
            } bits;
            uint8_t byte;
        } ACCEL_SMPLRT_DIV_1;

        uint8_t ACCEL_SMPLRT_DIV_2;

        union {
            struct {
                uint8_t ACCEL_INTEL_MODE_INT    : 1;
                uint8_t ACCEL_INTEL_EN      : 1;
                uint8_t RSVD                : 6;
            } bits;
            uint8_t byte;
        } ACCEL_INTEL_CTRL;

        uint8_t ACCEL_WOM_THR;

        union {
            struct {
                uint8_t ACCEL_FCHOICE       : 1;
                uint8_t ACCEL_FS_SEL        : 2;
                uint8_t ACCEL_DLPFCFG       : 3;
                uint8_t RSVD                : 2;
            } bits;
            uint8_t byte;
        } ACCEL_CONFIG;

        union {
            struct {
                uint8_t DEC3_CFG            : 2;
                uint8_t AZ_ST_EN_REG        : 1;
                uint8_t AY_ST_EN_REG        : 1;
                uint8_t AX_ST_EN_REG        : 1;
                uint8_t RSVD                : 3;
            } bits;
            uint8_t byte;
        } ACCEL_CONFIG_2;

        union {
            struct {
                uint8_t EXT_SYNC_SET        : 4;
                uint8_t WOF_EDGE_INT        : 1;
                uint8_t WOF_DEGLITCH_EN     : 1;
                uint8_t RSVD                : 1;
                uint8_t DELAY_TIME_EN       : 1;
            } bits;
            uint8_t byte;
        } FSYNC_CONFIG;

        union {
            struct {
                uint8_t TEMP_DLPFCFG        : 3;
                uint8_t RSVD                : 5;
            } bits;
            uint8_t byte;
        } TEMP_CONFIG;

        union {
            struct {
                uint8_t REG_LP_DMP_EN       : 1;
                uint8_t RSVD                : 7;
            } bits;
            uint8_t byte;
        } MOD_CTRL_USR;

        union {
            struct {
                uint8_t RSVD0               : 4;
                uint8_t USER_BANK           : 2;
                uint8_t RSVD1               : 2;
            } bits;
            uint8_t byte;
        } REG_BANK_SEL;

    } bytes;
    uint8_t arr[ICM20948_BANK2_REG_COUNT];
} icm20948_reg_bank_2_t;

typedef union {
    struct {
        union {
            struct {
                uint8_t I2C_MST_ODR_CONFIG  : 4;
                uint8_t RSVD                : 4;
            } bits;
            uint8_t byte;
        } I2C_MST_ODR_CONFIG;

        union {
            struct {
                uint8_t I2C_MST_CLK         : 4;
                uint8_t I2C_MST_P_NSR       : 1;
                uint8_t RSVD                : 2;
                uint8_t MULT_MST_EN         : 1;
            } bits;
            uint8_t byte;
        } I2C_MST_CTRL;

        union {
            struct {
                uint8_t I2C_SLV0_DELAY_EN   : 1;
                uint8_t I2C_SLV1_DELAY_EN   : 1;
                uint8_t I2C_SLV2_DELAY_EN   : 1;
                uint8_t I2C_SLV3_DELAY_EN   : 1;
                uint8_t I2C_SLV4_DELAY_EN   : 1;
                uint8_t RSVD                : 2;
                uint8_t DELAY_ES_SHADOW     : 1;
            } bits;
            uint8_t byte;
        } I2C_MST_DELAY_CTRL;

        // Slave 0
        union {
            struct {
                uint8_t I2C_ID_0            : 7;
                uint8_t I2C_SLV0_RNW        : 1;
            } bits;
            uint8_t byte;
        } I2C_SLV0_ADDR;

        uint8_t I2C_SLV0_REG;

        union {
            struct {
                uint8_t I2C_SLV0_LENG       : 4;
                uint8_t I2C_SLV0_GRP        : 1;
                uint8_t I2C_SLV0_REG_DIS    : 1;
                uint8_t I2C_SLV0_BYTE_SW    : 1;
                uint8_t I2C_SLV0_EN         : 1;
            } bits;
            uint8_t byte;
        } I2C_SLV0_CTRL;

        uint8_t I2C_SLV0_DO;

        // Slave 1
        union {
            struct {
                uint8_t I2C_ID_1            : 7;
                uint8_t I2C_SLV1_RNW        : 1;
            } bits;
            uint8_t byte;
        } I2C_SLV1_ADDR;

        uint8_t I2C_SLV1_REG;

        union {
            struct {
                uint8_t I2C_SLV1_LENG       : 4;
                uint8_t I2C_SLV1_GRP        : 1;
                uint8_t I2C_SLV1_REG_DIS    : 1;
                uint8_t I2C_SLV1_BYTE_SW    : 1;
                uint8_t I2C_SLV1_EN         : 1;
            } bits;
            uint8_t byte;
        } I2C_SLV1_CTRL;

        uint8_t I2C_SLV1_DO;

        // Slave 2
        union {
            struct {
                uint8_t I2C_ID_2            : 7;
                uint8_t I2C_SLV2_RNW        : 1;
            } bits;
            uint8_t byte;
        } I2C_SLV2_ADDR;

        uint8_t I2C_SLV2_REG;

        union {
            struct {
                uint8_t I2C_SLV2_LENG       : 4;
                uint8_t I2C_SLV2_GRP        : 1;
                uint8_t I2C_SLV2_REG_DIS    : 1;
                uint8_t I2C_SLV2_BYTE_SW    : 1;
                uint8_t I2C_SLV2_EN         : 1;
            } bits;
            uint8_t byte;
        } I2C_SLV2_CTRL;

        uint8_t I2C_SLV2_DO;

        // Slave 3
        union {
            struct {
                uint8_t I2C_ID_3            : 7;
                uint8_t I2C_SLV3_RNW        : 1;
            } bits;
            uint8_t byte;
        } I2C_SLV3_ADDR;

        uint8_t I2C_SLV3_REG;

        union {
            struct {
                uint8_t I2C_SLV3_LENG       : 4;
                uint8_t I2C_SLV3_GRP        : 1;
                uint8_t I2C_SLV3_REG_DIS    : 1;
                uint8_t I2C_SLV3_BYTE_SW    : 1;
                uint8_t I2C_SLV3_EN         : 1;
            } bits;
            uint8_t byte;
        } I2C_SLV3_CTRL;

        uint8_t I2C_SLV3_DO;

        // Slave 4
        union {
            struct {
                uint8_t I2C_ID_4            : 7;
                uint8_t I2C_SLV4_RNW        : 1;
            } bits;
            uint8_t byte;
        } I2C_SLV4_ADDR;

        uint8_t I2C_SLV4_REG;

        union {
            struct {
                uint8_t I2C_SLV4_LENG       : 4;
                uint8_t I2C_SLV4_GRP        : 1;
                uint8_t I2C_SLV4_REG_DIS    : 1;
                uint8_t I2C_SLV4_BYTE_SW    : 1;
                uint8_t I2C_SLV4_EN         : 1;
            } bits;
            uint8_t byte;
        } I2C_SLV4_CTRL;

        uint8_t I2C_SLV4_DO;

        union {
            struct {
                uint8_t RSVD0               : 4;
                uint8_t USER_BANK           : 2;
                uint8_t RSVD1               : 2;
            } bits;
            uint8_t byte;
        } REG_BANK_SEL;

    } bytes;
    uint8_t arr[ICM20948_BANK3_REG_COUNT];
} icm20948_reg_bank_3_t;

typedef struct {
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;

    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;

    int16_t temp;
} icm20948_converted_t;

struct icm20948_data{
    icm20948_reg_bank_0_t bank0;
    icm20948_reg_bank_1_t bank1;
    icm20948_reg_bank_2_t bank2;
    icm20948_reg_bank_3_t bank3;
    icm20948_reg_bank_sel_t reg_bank_sel;
    icm20948_converted_t converted;
};


struct icm20948_config {
    const struct i2c_dt_spec i2c;

    uint8_t gyro_sr_div;
    uint8_t gyro_dlpf;
    uint8_t gyro_fs_sel;
    bool gyro_fchoice;

    uint8_t accel_sr_div;
    uint8_t accel_dlpf;
    uint8_t accel_fs_sel;
    bool accel_fchoice;

    const struct gpio_dt_spec int_gpio;
};

int icm20948_init(const struct device *dev);

int icm20948_read(const struct device *dev, icm20948_reg_bank_sel_t bank, uint8_t reg_addr, uint8_t *data, size_t length);
int icm20948_write(const struct device *dev, icm20948_reg_bank_sel_t bank, uint8_t reg_addr, const uint8_t *data, size_t length);

int icm20948_bank_sel(const struct device *dev, icm20948_reg_bank_sel_t bank);

int icm20948_sample_fetch(const struct device *dev, enum sensor_channel chan);
int icm20948_channel_get(const struct device *dev, enum sensor_channel chan, struct sensor_value *val);