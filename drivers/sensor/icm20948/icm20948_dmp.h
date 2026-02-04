#pragma once

#include <zephyr/types.h>

#define DMP_CFG_FIFO_SIZE                   (4222)
 
// data output control
#define DMP_DATA_OUT_CTL1           (4 * 16)
#define DMP_DATA_OUT_CTL2           (4 * 16 + 2)
#define DMP_DATA_INTR_CTL           (4 * 16 + 12)
#define DMP_FIFO_WATERMARK          (31 * 16 + 14)
 
// motion event control
#define DMP_MOTION_EVENT_CTL        (4 * 16 + 14)
 
// indicates to DMP which sensors are available
/*  1: gyro samples available
2: accel samples available
8: secondary samples available  */
#define DMP_DATA_RDY_STATUS         (8 * 16 + 10)
 
// batch mode
#define DMP_BM_BATCH_CNTR           (27 * 16)
#define DMP_BM_BATCH_THLD           (19 * 16 + 12)
#define DMP_BM_BATCH_MASK           (21 * 16 + 14)
 
// sensor output data rate
#define DMP_ODR_ACCEL               (11 * 16 + 14)
#define DMP_ODR_GYRO                (11 * 16 + 10)
#define DMP_ODR_CPASS               (11 * 16 +  6)
#define DMP_ODR_ALS                 (11 * 16 +  2)
#define DMP_ODR_QUAT6               (10 * 16 + 12)
#define DMP_ODR_QUAT9               (10 * 16 +  8)
#define DMP_ODR_PQUAT6              (10 * 16 +  4)
#define DMP_ODR_GEOMAG              (10 * 16 +  0)
#define DMP_ODR_PRESSURE            (11 * 16 + 12)
#define DMP_ODR_GYRO_CALIBR         (11 * 16 +  8)
#define DMP_ODR_CPASS_CALIBR        (11 * 16 +  4)
 
// sensor output data rate counter
#define DMP_ODR_CNTR_ACCEL          (9 * 16 + 14)
#define DMP_ODR_CNTR_GYRO           (9 * 16 + 10)
#define DMP_ODR_CNTR_CPASS          (9 * 16 +  6)
#define DMP_ODR_CNTR_ALS            (9 * 16 +  2)
#define DMP_ODR_CNTR_QUAT6          (8 * 16 + 12)
#define DMP_ODR_CNTR_QUAT9          (8 * 16 +  8)
#define DMP_ODR_CNTR_PQUAT6         (8 * 16 +  4)
#define DMP_ODR_CNTR_GEOMAG         (8 * 16 +  0)
#define DMP_ODR_CNTR_PRESSURE       (9 * 16 + 12)
#define DMP_ODR_CNTR_GYRO_CALIBR    (9 * 16 +  8)
#define DMP_ODR_CNTR_CPASS_CALIBR   (9 * 16 +  4)
 
// mounting matrix
#define DMP_CPASS_MTX_00            (23 * 16)
#define DMP_CPASS_MTX_01            (23 * 16 + 4)
#define DMP_CPASS_MTX_02            (23 * 16 + 8)
#define DMP_CPASS_MTX_10            (23 * 16 + 12)
#define DMP_CPASS_MTX_11            (24 * 16)
#define DMP_CPASS_MTX_12            (24 * 16 + 4)
#define DMP_CPASS_MTX_20            (24 * 16 + 8)
#define DMP_CPASS_MTX_21            (24 * 16 + 12)
#define DMP_CPASS_MTX_22            (25 * 16)
 
#define DMP_GYRO_SF                 (19 * 16)
#define DMP_ACCEL_FB_GAIN           (34 * 16)
#define DMP_ACCEL_ONLY_GAIN         (16 * 16 + 12)
 
// bias calibration
#define DMP_GYRO_BIAS_X             (139 * 16 +  4)
#define DMP_GYRO_BIAS_Y             (139 * 16 +  8)
#define DMP_GYRO_BIAS_Z             (139 * 16 + 12)
#define DMP_GYRO_ACCURACY           (138 * 16 +  2)
#define DMP_GYRO_BIAS_SET           (138 * 16 +  6)
#define DMP_GYRO_LAST_TEMPR         (134 * 16)
#define DMP_GYRO_SLOPE_X            ( 78 * 16 +  4)
#define DMP_GYRO_SLOPE_Y            ( 78 * 16 +  8)
#define DMP_GYRO_SLOPE_Z            ( 78 * 16 + 12)
 
#define DMP_ACCEL_BIAS_X            (110 * 16 +  4)
#define DMP_ACCEL_BIAS_Y            (110 * 16 +  8)
#define DMP_ACCEL_BIAS_Z            (110 * 16 + 12)
#define DMP_ACCEL_ACCURACY          (97 * 16)
#define DMP_ACCEL_CAL_RESET         (77 * 16)
#define DMP_ACCEL_VARIANCE_THRESH   (93 * 16)
#define DMP_ACCEL_CAL_RATE          (94 * 16 + 4)
#define DMP_ACCEL_PRE_SENSOR_DATA   (97 * 16 + 4)
#define DMP_ACCEL_COVARIANCE        (101 * 16 + 8)
#define DMP_ACCEL_ALPHA_VAR         (91 * 16)
#define DMP_ACCEL_A_VAR             (92 * 16)
#define DMP_ACCEL_CAL_INIT          (94 * 16 + 2)
#define DMP_ACCEL_CAL_SCALE_COVQ_IN_RANGE   (194 * 16)
#define DMP_ACCEL_CAL_SCALE_COVQ_OUT_RANGE  (195 * 16)
#define DMP_ACCEL_CAL_TEMPERATURE_SENSITIVITY   (194 * 16 + 4)
#define DMP_ACCEL_CAL_TEMPERATURE_OFFSET_TRIM   (194 * 16 + 12)
 
#define DMP_CPASS_BIAS_X            (126 * 16 +  4)
#define DMP_CPASS_BIAS_Y            (126 * 16 +  8)
#define DMP_CPASS_BIAS_Z            (126 * 16 + 12)
#define DMP_CPASS_ACCURACY          (37 * 16)
#define DMP_CPASS_BIAS_SET          (34 * 16 + 14)
#define DMP_MAR_MODE                (37 * 16 + 2)
#define DMP_CPASS_COVARIANCE        (115 * 16)
#define DMP_CPASS_COVARIANCE_CUR    (118 * 16 +  8)
#define DMP_CPASS_REF_MAG_3D        (122 * 16)
#define DMP_CPASS_CAL_INIT          (114 * 16)
#define DMP_CPASS_EST_FIRST_BIAS    (113 * 16)
#define DMP_MAG_DISTURB_STATE       (113 * 16 + 2)
#define DMP_CPASS_VAR_COUNT         (112 * 16 + 6)
#define DMP_CPASS_COUNT_7           ( 87 * 16 + 2)
#define DMP_CPASS_MAX_INNO          (124 * 16)
#define DMP_CPASS_BIAS_OFFSET       (113 * 16 + 4)
#define DMP_CPASS_CUR_BIAS_OFFSET   (114 * 16 + 4)
#define DMP_CPASS_PRE_SENSOR_DATA   ( 87 * 16 + 4)
 
// Compass Cal params to be adjusted according to sampling rate
#define DMP_CPASS_TIME_BUFFER       (112 * 16 + 14)
#define DMP_CPASS_RADIUS_3D_THRESH_ANOMALY  (112 * 16 + 8)
 
#define DMP_CPASS_STATUS_CHK        (25 * 16 + 12)
 
// 9-axis
#define DMP_MAGN_THR_9X             (80 * 16)
#define DMP_MAGN_LPF_THR_9X         (80 * 16 +  8)
#define DMP_QFB_THR_9X              (80 * 16 + 12)
 
// DMP running counter
#define DMP_DMPRATE_CNTR            (18 * 16 + 4)
 
// pedometer
#define DMP_PEDSTD_BP_B             (49 * 16 + 12)
#define DMP_PEDSTD_BP_A4            (52 * 16)
#define DMP_PEDSTD_BP_A3            (52 * 16 +  4)
#define DMP_PEDSTD_BP_A2            (52 * 16 +  8)
#define DMP_PEDSTD_BP_A1            (52 * 16 + 12)
#define DMP_PEDSTD_SB               (50 * 16 +  8)
#define DMP_PEDSTD_SB_TIME          (50 * 16 + 12)
#define DMP_PEDSTD_PEAKTHRSH        (57 * 16 +  8)
#define DMP_PEDSTD_TIML             (50 * 16 + 10)
#define DMP_PEDSTD_TIMH             (50 * 16 + 14)
#define DMP_PEDSTD_PEAK             (57 * 16 +  4)
#define DMP_PEDSTD_STEPCTR          (54 * 16)
#define DMP_PEDSTD_STEPCTR2         (58 * 16 +  8)
#define DMP_PEDSTD_TIMECTR          (60 * 16 +  4)
#define DMP_PEDSTD_DECI             (58 * 16)
#define DMP_PEDSTD_SB2              (60 * 16 + 14)
#define DMP_STPDET_TIMESTAMP        (18 * 16 +  8)
#define DMP_PEDSTEP_IND             (19 * 16 +  4)
#define DMP_PED_Y_RATIO             (17 * 16 +  0)
 
// SMD
#define DMP_SMD_VAR_TH              (141 * 16 + 12)
#define DMP_SMD_VAR_TH_DRIVE        (143 * 16 + 12)
#define DMP_SMD_DRIVE_TIMER_TH      (143 * 16 +  8)
#define DMP_SMD_TILT_ANGLE_TH       (179 * 16 + 12)
#define DMP_BAC_SMD_ST_TH           (179 * 16 +  8)
#define DMP_BAC_ST_ALPHA4           (180 * 16 + 12)
#define DMP_BAC_ST_ALPHA4A          (176 * 16 + 12)
 
// Wake on Motion
#define DMP_WOM_ENABLE              (64 * 16 + 14)
#define DMP_WOM_STATUS              (64 * 16 + 6)
#define DMP_WOM_THRESHOLD           (64 * 16)
#define DMP_WOM_CNTR_TH             (64 * 16 + 12)
 
// Activity Recognition
#define DMP_BAC_RATE                (48  * 16 + 10)
#define DMP_BAC_STATE               (179 * 16 +  0)
#define DMP_BAC_STATE_PREV          (179 * 16 +  4)
#define DMP_BAC_ACT_ON              (182 * 16 +  0)
#define DMP_BAC_ACT_OFF             (183 * 16 +  0)
#define DMP_BAC_STILL_S_F           (177 * 16 +  0)
#define DMP_BAC_RUN_S_F             (177 * 16 +  4)
#define DMP_BAC_DRIVE_S_F           (178 * 16 +  0)
#define DMP_BAC_WALK_S_F            (178 * 16 +  4)
#define DMP_BAC_SMD_S_F             (178 * 16 +  8)
#define DMP_BAC_BIKE_S_F            (178 * 16 + 12)
#define DMP_BAC_E1_SHORT            (146 * 16 +  0)
#define DMP_BAC_E2_SHORT            (146 * 16 +  4)
#define DMP_BAC_E3_SHORT            (146 * 16 +  8)
#define DMP_BAC_VAR_RUN             (148 * 16 + 12)
#define DMP_BAC_TILT_INIT           (181 * 16 +  0)
#define DMP_BAC_MAG_ON              (225 * 16 +  0)
#define DMP_BAC_PS_ON               (74  * 16 +  0)
#define DMP_BAC_BIKE_PREFERENCE     (173 * 16 +  8)
#define DMP_BAC_MAG_I2C_ADDR        (229 * 16 +  8)
#define DMP_BAC_PS_I2C_ADDR         (75  * 16 +  4)
#define DMP_BAC_DRIVE_CONFIDENCE    (144 * 16 +  0)
#define DMP_BAC_WALK_CONFIDENCE     (144 * 16 +  4)
#define DMP_BAC_SMD_CONFIDENCE      (144 * 16 +  8)
#define DMP_BAC_BIKE_CONFIDENCE     (144 * 16 + 12)
#define DMP_BAC_STILL_CONFIDENCE    (145 * 16 +  0)
#define DMP_BAC_RUN_CONFIDENCE      (145 * 16 +  4)
#define DMP_BAC_MODE_CNTR           (150 * 16)
#define DMP_BAC_STATE_T_PREV        (185 * 16 +  4)
#define DMP_BAC_ACT_T_ON            (184 * 16 +  0)
#define DMP_BAC_ACT_T_OFF           (184 * 16 +  4)
#define DMP_BAC_STATE_WRDBS_PREV    (185 * 16 +  8)
#define DMP_BAC_ACT_WRDBS_ON        (184 * 16 +  8)
#define DMP_BAC_ACT_WRDBS_OFF       (184 * 16 + 12)
#define DMP_BAC_ACT_ON_OFF          (190 * 16 +  2)
#define DMP_PREV_BAC_ACT_ON_OFF     (188 * 16 +  2)
#define DMP_BAC_CNTR                (48  * 16 +  2)
 
// Flip/Pick-up
#define DMP_FP_VAR_ALPHA            (245 * 16 +  8)
#define DMP_FP_STILL_TH             (246 * 16 +  4)
#define DMP_FP_MID_STILL_TH         (244 * 16 +  8)
#define DMP_FP_NOT_STILL_TH         (246 * 16 +  8)
#define DMP_FP_VIB_REJ_TH           (241 * 16 +  8)
#define DMP_FP_MAX_PICKUP_T_TH      (244 * 16 + 12)
#define DMP_FP_PICKUP_TIMEOUT_TH    (248 * 16 +  8)
#define DMP_FP_STILL_CONST_TH       (246 * 16 + 12)
#define DMP_FP_MOTION_CONST_TH      (240 * 16 +  8)
#define DMP_FP_VIB_COUNT_TH         (242 * 16 +  8)
#define DMP_FP_STEADY_TILT_TH       (247 * 16 +  8)
#define DMP_FP_STEADY_TILT_UP_TH    (242 * 16 + 12)
#define DMP_FP_Z_FLAT_TH_MINUS      (243 * 16 +  8)
#define DMP_FP_Z_FLAT_TH_PLUS       (243 * 16 + 12)
#define DMP_FP_DEV_IN_POCKET_TH     (76  * 16 + 12)
#define DMP_FP_PICKUP_CNTR          (247 * 16 +  4)
#define DMP_FP_RATE                 (240 * 16 + 12)
 
// Gyro FSR
#define DMP_GYRO_FULLSCALE          (72 * 16 + 12)
 
// Accel FSR
#define DMP_ACC_SCALE               (30 * 16 + 0)
#define DMP_ACC_SCALE2              (79 * 16 + 4)
 
// EIS authentication
#define DMP_EIS_AUTH_INPUT          (160 * 16 +   4)
#define DMP_EIS_AUTH_OUTPUT         (160 * 16 +   0)
 
// B2S
#define DMP_B2S_RATE                (48  * 16 +   8)
// mounting matrix
#define DMP_B2S_MTX_00              (208 * 16)
#define DMP_B2S_MTX_01              (208 * 16 + 4)
#define DMP_B2S_MTX_02              (208 * 16 + 8)
#define DMP_B2S_MTX_10              (208 * 16 + 12)
#define DMP_B2S_MTX_11              (209 * 16)
#define DMP_B2S_MTX_12              (209 * 16 + 4)
#define DMP_B2S_MTX_20              (209 * 16 + 8)
#define DMP_B2S_MTX_21              (209 * 16 + 12)
#define DMP_B2S_MTX_22              (210 * 16)
 
#define DMP_START_ADDRESS   ((unsigned short)0x1000)
#define DMP_MEM_BANK_SIZE   256
#define DMP_LOAD_START      0x90
 
#define DMP_CODE_SIZE 14301
 
// BAC states
#define DMP_BAC_DRIVE   0x01
#define DMP_BAC_WALK    0x02
#define DMP_BAC_RUN     0x04
#define DMP_BAC_BIKE    0x08
#define DMP_BAC_TILT    0x10
#define DMP_BAC_STILL   0x20

// data output control reg 1
#define ACCEL_SET		0x8000
#define GYRO_SET		0x4000
#define CPASS_SET		0x2000
#define ALS_SET			0x1000
#define QUAT6_SET		0x0800
#define QUAT9_SET		0x0400
#define PQUAT6_SET		0x0200
#define GEOMAG_SET		0x0100
#define PRESSURE_SET	0x0080
#define GYRO_CALIBR_SET	0x0040
#define CPASS_CALIBR_SET 0x0020
#define PED_STEPDET_SET	0x0010
#define HEADER2_SET		0x0008
#define PED_STEPIND_SET 0x0007

// data output control reg 2
#define ACCEL_ACCURACY_SET		0x4000
#define GYRO_ACCURACY_SET		0x2000
#define CPASS_ACCURACY_SET		0x1000
#define COMPASS_CAL_INPUT_SET	0x1000
#define FLIP_PICKUP_SET			0x0400
#define BATCH_MODE_EN			0x0100
#define ACT_RECOG_SET			0x0080

// motion event control reg
#define INV_BAC_WEARABLE_EN		0x8000
#define INV_PEDOMETER_EN		0x4000
#define INV_PEDOMETER_INT_EN	0x2000
#define INV_SMD_EN				0x0800
#define INV_BTS_EN				0x0020
#define FLIP_PICKUP_EN			0x0010
#define GEOMAG_EN   			0x0008
#define INV_ACCEL_CAL_EN		0x0200
#define INV_GYRO_CAL_EN			0x0100
#define INV_COMPASS_CAL_EN		0x0080
#define INV_NINE_AXIS_EN        0x0040
#define INV_BRING_AND_LOOK_T0_SEE_EN  0x0004  // Aded by ONn for 20648

extern const unsigned char icm20948_dmp_firmware[14301];

typedef struct {
    struct header1{
        uint8_t accel_set:1;
        uint8_t gyro_set:1;
        uint8_t cpass_set:1;
        uint8_t als_set:1;
        uint8_t quat6_set:1;
        uint8_t quat9_set:1;
        uint8_t pquat6_set:1;
        uint8_t geomag_set:1;
        uint8_t pressure_set:1;
        uint8_t gyro_calibr_set:1;
        uint8_t cpass_calibr_set:1;
        uint8_t ped_stepdet_set:1;
        uint8_t header2_set:1;
        uint8_t ped_stepind_set:3;
    } header1;
    struct header2{
        uint8_t rsvd0:1;
        uint8_t accel_accuracy_set:1;
        uint8_t gyro_accuracy_set:1;
        uint8_t cpass_accuracy_set:1;
        uint8_t rsvd1:1;
        uint8_t flip_pickup_set:1;
        uint8_t rsvd2:1;
        uint8_t batch_mode_en:1;
        uint8_t act_recog_set:1;
    } header2;
} fifo_packet_t;

// data packet size reg 1
#define HEADER_SZ		2
#define ACCEL_DATA_SZ	6
#define GYRO_DATA_SZ	6
#define CPASS_DATA_SZ	6
#define ALS_DATA_SZ		8
#define QUAT6_DATA_SZ	12
#define QUAT9_DATA_SZ	14
#define PQUAT6_DATA_SZ	6
#define GEOMAG_DATA_SZ	14
#define PRESSURE_DATA_SZ		6
#define GYRO_BIAS_DATA_SZ	6
#define CPASS_CALIBR_DATA_SZ	12
#define PED_STEPDET_TIMESTAMP_SZ	4
#define FOOTER_SZ		2

// data packet size reg 2
#define HEADER2_SZ			2
#define ACCEL_ACCURACY_SZ	2
#define GYRO_ACCURACY_SZ	2
#define CPASS_ACCURACY_SZ	2
#define FSYNC_SZ			2
#define FLIP_PICKUP_SZ      2
#define ACT_RECOG_SZ        6
#define ODR_CNT_GYRO_SZ	2