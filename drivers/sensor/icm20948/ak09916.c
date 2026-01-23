#include "ak09916.h"
#include "icm20948.h"

static int ak09916_execute_io(const struct device *dev, ak09916_reg_addr_t reg, bool rnw) {
    const struct icm20948_config *cfg = dev->config;
    struct icm20948_data *drv_data = dev->data;

    drv_data.bank3.bytes.I2C_SLV0_ADDR.bits.I2C_ID_0 = AK09916_I2C_ADDR;
    drv_data.bank3.bytes.I2C_SLV0_ADDR.bits.RNW = rnw;
    icm20948_write(dev, ICM20948_BANK3, ICM20948_BANK3_I2C_SLV0_ADDR, drv_data.bank3.I2C_SLV0_ADDR, 1);
    drv_data.bank3.bytes.I2C_SLV0_REG = reg; 
    icm20948_write(dev, ICM20948_BANK3, ICM20948_BANK3_I2C_SLV0_REG, drv_data.bank3.bytes.I2C_SLV0_REG, 1);
    
}

static int ak09916_read(const struct device *dev, ak09916_reg_addr_t addr, uint8_t *data) {
    const struct icm20948_config *cfg = dev->config;
    struct icm20948_data *drv_data = dev->data;
    int ret;

    ret = 
}

int ak09916_convert_mag(struct sensor_value *val int16_t raw_val, inst16_t scale, uint8_t st2) {

}

int ak09916_init(const struct device *dev) {

}