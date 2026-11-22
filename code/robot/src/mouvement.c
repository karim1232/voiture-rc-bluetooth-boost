/*
 * mouvement.c
 *
 *  Created on: May 12, 2026
 *      Author: karim
 */

#include "sys/devices.h"
#include "sys/cm4.h"
#include "sys/clock.h"
#include "mouvement.h"

/* ================================================
   Dual DC Motor Shield V2 — STM32 Nucleo F446RE

   D3  = PB3  → IN1_1  direction gauche
   D5  = PB4  → TIM3_CH1 PWM vitesse gauche
   D6  = PB10 → IN2_1  direction droite
   D9  = PC7  → TIM3_CH2 PWM vitesse droite

   APB1TIMCLK = 180MHz
   PSC = 179, ARR = 999 → 1kHz PWM
   CCR = 0 à 1000 (0% à 100%)
   ================================================ */

/* ------ INIT GPIO ------ */
void initialize_PINs(void)
{
    enable_GPIOB();
    enable_GPIOC();

    /* PB3 → output (direction gauche) : MODER[7:6] = 01 */
    GPIOB.MODER = (GPIOB.MODER & ~(3<<6))  | (1<<6);

    /* PB10 → output (direction droite) : MODER[21:20] = 01 */
    GPIOB.MODER = (GPIOB.MODER & ~(3<<20)) | (1<<20);

    /* PB4 → AF2 = TIM3_CH1 (PWM gauche) */
    GPIOB.AFRL  = (GPIOB.AFRL  & ~(0xF<<16)) | (2<<16); /* AFRL[19:16] = 2 */
    GPIOB.MODER = (GPIOB.MODER & ~(3<<8))    | (2<<8);  /* MODER[9:8] = 10 */

    /* PC7 → AF2 = TIM3_CH2 (PWM droite) */
    GPIOC.AFRL  = (GPIOC.AFRL  & ~(0xF<<28)) | (2<<28); /* AFRL[31:28] = 2 */
    GPIOC.MODER = (GPIOC.MODER & ~(3<<14))   | (2<<14); /* MODER[15:14] = 10 */
}

/* ------ INIT PWM TIM3 ------ */
void setup_PWM(void)
{
    enable_TIM3();

    /* 180MHz / 180 = 1MHz */
    TIM3.PSC = 180 - 1;

    /* 1MHz / 1000 = 1kHz PWM, CCR entre 0 et 1000 */
    TIM3.ARR = 1000 - 1;

    /* CCMR1 : CH1 PWM mode 1 (OC1M=110, OC1PE=1)
               CH2 PWM mode 1 (OC2M=110, OC2PE=1) */
    TIM3.CCMR1 = (6<<4) | (1<<3) | (6<<12) | (1<<11);

    /* Vitesse initiale = 0 */
    TIM3.CCR1 = 0;
    TIM3.CCR2 = 0;

    /* CCER : activer CH1 (CC1E=1) et CH2 (CC2E=1) */
    TIM3.CCER = (1<<0) | (1<<4);

    /* CR1 : ARPE=1, CEN=1 */
    TIM3.CR1 = (1<<7) | (1<<0);

    /* EGR : forcer update pour charger PSC et ARR */
    TIM3.EGR = (1<<0);
}

/* ------ DIRECTION ------ */

/* avant=1 → LOW (IN1_1=0 = avant selon table vérité) */
void set_dir_left(int avant)
{
    if (avant)
        GPIOB.BSRR = (1<<(3+16));  /* PB3 = 0 */
    else
        GPIOB.BSRR = (1<<3);       /* PB3 = 1 */
}

void set_dir_right(int avant)
{
    if (avant)
        GPIOB.BSRR = (1<<(10+16)); /* PB10 = 0 */
    else
        GPIOB.BSRR = (1<<10);      /* PB10 = 1 */
}

/* ------ VITESSE ------ */

/* percent : 0 à 100 */
void set_speed_left(uint32_t percent)
{
    TIM3.CCR1 = percent * 10; /* 0-100 → 0-1000 */
}

void set_speed_right(uint32_t percent)
{
    TIM3.CCR2 = percent * 10;
}

/* ------ COMMANDES HAUT NIVEAU ------ */

void motor_forward(uint32_t speed)
{
    set_dir_left(1);
    set_dir_right(1);
    set_speed_left(speed);
    set_speed_right(speed);
}

void motor_back(uint32_t speed)
{
    set_dir_left(0);
    set_dir_right(0);
    set_speed_left(speed);
    set_speed_right(speed);
}

/* Tourner gauche : roues droites en avant, gauche en arrière */
void motor_left(uint32_t speed)
{
    set_dir_left(0);
    set_dir_right(1);
    set_speed_left(speed);
    set_speed_right(speed);
}

/* Tourner droite : roues gauches en avant, droite en arrière */
void motor_right(uint32_t speed)
{
    set_dir_left(1);
    set_dir_right(0);
    set_speed_left(speed);
    set_speed_right(speed);
}

void motor_stop(void)
{
    /* Mettre direction et vitesse a 0 */
    set_dir_left(1);
    set_dir_right(1);
    set_speed_left(0);
    set_speed_right(0);
}
