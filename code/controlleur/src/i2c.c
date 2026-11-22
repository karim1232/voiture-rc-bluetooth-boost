// i2c.c
#include <stdio.h>
#include "sys/devices.h"
#include "sys/cm4.h"
#include "i2c.h"

/* ================================================
   I2C1 sur STM32 Nucleo F446RE
   PB8 → AF4 = I2C1_SCL (D15)
   PB9 → AF4 = I2C1_SDA (D14)
   APB1CLK = 45MHz
   ================================================ */

void i2c_init(void)
{
    enable_GPIOB();

    /* Activer I2C1 */
    RCC.APB1ENR |= (1UL<<21);
    __DSB();
    __DSB();

    /* PB8 → AF4 I2C1_SCL */
    GPIOB.AFRH  = (GPIOB.AFRH & ~(0xF<<0))  | (4<<0);
    GPIOB.MODER = (GPIOB.MODER & ~(3<<16))   | (2<<16);
    GPIOB.OTYPER |= (1<<8);
    GPIOB.PUPDR = (GPIOB.PUPDR & ~(3<<16));

    /* PB9 → AF4 I2C1_SDA */
    GPIOB.AFRH  = (GPIOB.AFRH & ~(0xF<<4))  | (4<<4);
    GPIOB.MODER = (GPIOB.MODER & ~(3<<18))   | (2<<18);
    GPIOB.OTYPER |= (1<<9);
    GPIOB.PUPDR = (GPIOB.PUPDR & ~(3<<18));

    /* Reset I2C1 */
    I2C1.CR1 |= (1<<15);
    I2C1.CR1 &= ~(1<<15);

    /* FREQ = 45MHz */
    I2C1.CR2 = 45;

    /* CCR = 225 → 100kHz */
    I2C1.CCR = 225;

    /* TRISE = 46 */
    I2C1.TRISE = 46;

    /* Enable I2C1 */
    I2C1.CR1 |= (1<<0);

    printf("I2C1 CR1=%lu AFRH=0x%08lX MODER=0x%08lX\r\n",
           I2C1.CR1, GPIOB.AFRH, GPIOB.MODER);
}

static void i2c_start(void)
{
    I2C1.CR1 |= (1<<8);
    while (!(I2C1.SR1 & (1<<0)));
}

static void i2c_stop(void)
{
    I2C1.CR1 |= (1<<9);
}

static void i2c_send_addr(uint8_t addr, uint8_t rw)
{
    I2C1.DR = (addr << 1) | rw;
    while (!(I2C1.SR1 & (1<<1)));
    (void)I2C1.SR1;
    (void)I2C1.SR2;
}

static void i2c_send_byte(uint8_t data)
{
    while (!(I2C1.SR1 & (1<<7)));
    I2C1.DR = data;
    while (!(I2C1.SR1 & (1<<2)));
}

static uint8_t i2c_read_byte(void)
{
    I2C1.CR1 &= ~(1<<10);
    i2c_stop();
    while (!(I2C1.SR1 & (1<<6)));
    return (uint8_t)I2C1.DR;
}

uint8_t i2c_read_reg(uint8_t addr, uint8_t reg)
{
    i2c_start();
    i2c_send_addr(addr, 0);
    i2c_send_byte(reg);
    i2c_start();
    i2c_send_addr(addr, 1);
    return i2c_read_byte();
}
