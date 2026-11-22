// i2c.h
#ifndef SRC_I2C_H_
#define SRC_I2C_H_

#include <stdint.h>

void i2c_init(void);
uint8_t i2c_read_reg(uint8_t addr, uint8_t reg);

#endif
