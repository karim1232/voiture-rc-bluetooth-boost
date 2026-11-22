# Brochage (Nucleo F446RE)

Horloges réglées dans `code/*/src/sys/init.c` : SYSCLK = 180 MHz, APB1 = 45 MHz, APB2 = 90 MHz, APB1TIMCLK = 180 MHz.

## Voiture (`code/robot/`)

| Fonction | Broche | Arduino | Vers | Détail |
|---|---|---|---|---|
| Direction gauche | PB3 | D3 | Shield IN1_1 | GPIO, bas = avant |
| PWM gauche | PB4 | D5 | Shield IN1_2 | TIM3_CH1, AF2 |
| Direction droite | PB10 | D6 | Shield IN2_1 | GPIO, bas = avant |
| PWM droite | PC7 | D9 | Shield IN2_2 | TIM3_CH2, AF2 |
| USART1 TX | PA9 | D8 | HC-05 RXD | AF7, 9600 bauds |
| USART1 RX | PA10 | D2 | HC-05 TXD | AF7 |
| Console | PA2 / PA3 | | YP-05 | USART2, printf |
| Alim / masse | 3V3 / GND | | HC-05 | GND commun avec l'alim moteurs |

PWM : 180 MHz / (PSC + 1 = 180) / (ARR + 1 = 1000) = 1 kHz, avec CCR = pourcentage × 10.

La logique est alimentée par l'USB de la Nucleo (jumper du shield en place) et les moteurs par une alimentation 5 V externe branchée sur le bornier.

## Manette (`code/controlleur/`)

| Fonction | Broche | Arduino | Vers | Détail |
|---|---|---|---|---|
| I2C1 SCL | PB8 | D15 | Joystick SCL | AF4, open-drain, 100 kHz |
| I2C1 SDA | PB9 | D14 | Joystick SDA | AF4, open-drain |
| USART1 TX | PA9 | D8 | HC-05 RXD | AF7, 9600 bauds |
| USART1 RX | PA10 | D2 | HC-05 TXD | AF7 |
| Console | PA2 / PA3 | | YP-05 | USART2 |
| Alim / masse | 3V3 / GND | | HC-05 + joystick | |

I2C : CR2 = 45, CCR = 225, TRISE = 46 (APB1 = 45 MHz).

## Joystick (0x20)

| Registre | Contenu |
|---|---|
| 0x03 / 0x04 | X, MSB / LSB |
| 0x05 / 0x06 | Y, MSB / LSB |

Valeur = `(msb << 2) | (lsb >> 6)`. Au repos on lit environ 510 en X et 498 en Y, et Y descend vers 0 quand on pousse vers l'avant. La première lecture renvoie toujours 1023, on la jette.
