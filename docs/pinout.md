# Pinout (Nucleo F446RE)

Clocks set in `code/*/src/sys/init.c`: SYSCLK = 180 MHz, APB1 = 45 MHz, APB2 = 90 MHz, APB1TIMCLK = 180 MHz.

## Car (`code/robot/`)

| Function | Pin | Arduino | To | Detail |
|---|---|---|---|---|
| Left direction | PB3 | D3 | Shield IN1_1 | GPIO, low = forward |
| Left PWM | PB4 | D5 | Shield IN1_2 | TIM3_CH1, AF2 |
| Right direction | PB10 | D6 | Shield IN2_1 | GPIO, low = forward |
| Right PWM | PC7 | D9 | Shield IN2_2 | TIM3_CH2, AF2 |
| USART1 TX | PA9 | D8 | HC-05 RXD | AF7, 9600 baud |
| USART1 RX | PA10 | D2 | HC-05 TXD | AF7 |
| Console | PA2 / PA3 | | YP-05 | USART2, printf |
| Power / ground | 3V3 / GND | | HC-05 | ground shared with the motor supply |

PWM: 180 MHz / (PSC + 1 = 180) / (ARR + 1 = 1000) = 1 kHz, with CCR = percentage × 10.

The logic is powered from the Nucleo's USB (shield jumper fitted) and the motors from an external 5 V supply on the terminal block.

## Controller (`code/controlleur/`)

| Function | Pin | Arduino | To | Detail |
|---|---|---|---|---|
| I2C1 SCL | PB8 | D15 | Joystick SCL | AF4, open-drain, 100 kHz |
| I2C1 SDA | PB9 | D14 | Joystick SDA | AF4, open-drain |
| USART1 TX | PA9 | D8 | HC-05 RXD | AF7, 9600 baud |
| USART1 RX | PA10 | D2 | HC-05 TXD | AF7 |
| Console | PA2 / PA3 | | YP-05 | USART2 |
| Power / ground | 3V3 / GND | | HC-05 + joystick | |

I2C: CR2 = 45, CCR = 225, TRISE = 46 (APB1 = 45 MHz).

## Joystick (0x20)

| Register | Content |
|---|---|
| 0x03 / 0x04 | X, MSB / LSB |
| 0x05 / 0x06 | Y, MSB / LSB |

Value = `(msb << 2) | (lsb >> 6)`. At rest it reads about 510 on X and 498 on Y, and Y drops toward 0 when pushed forward. The first read always returns 1023 and is discarded.
