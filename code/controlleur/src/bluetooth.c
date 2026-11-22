/*
 * bluetooth.c
 *
 *  Created on: May 21, 2026
 *      Author: karim
 */
#include "sys/devices.h"
#include "sys/cm4.h"
#include "sys/clock.h"
#include "bluetooth.h"

/* ================================================
   HC-05 sur USART1
   PA9  → AF7 = USART1_TX
   PA10 → AF7 = USART1_RX
   APB2CLK = 90MHz
   BRR = 90MHz / 9600 = 9375
   ================================================ */

void bluetooth_init(void)
{
    enable_GPIOA();
    enable_USART1();

    /* PA9 → AF7 USART1_TX */
    GPIOA.AFRH  = (GPIOA.AFRH & ~(0xF<<4))  | (7<<4);
    GPIOA.MODER = (GPIOA.MODER & ~(3<<18))   | (2<<18);

    /* PA10 → AF7 USART1_RX */
    GPIOA.AFRH  = (GPIOA.AFRH & ~(0xF<<8))  | (7<<8);
    GPIOA.MODER = (GPIOA.MODER & ~(3<<20))   | (2<<20);

    /* Baudrate 9600 : 90MHz / 9600 = 9375 */
    USART1.BRR = 9375;

    /* 8 bits, 1 stop bit, pas de parité */
    USART1.CR2 = 0;
    USART1.CR3 = 0;

    /* RE=1, TE=1, UE=1 */
    USART1.CR1 = (1<<2) | (1<<3) | (1<<13);
}

char bluetooth_receive(void)
{
    /* Attendre RXNE=1 (bit 5) */
    while ((USART1.SR & (1<<5)) == 0);
    return (char)(USART1.DR & 0xFF);
}

void bluetooth_send(char c)
{
    /* Attendre TXE=1 (bit 7) */
    while ((USART1.SR & (1<<7)) == 0);
    USART1.DR = c;
}
