#include "button.h"

#include "irq.h"
#include "sys/devices.h"

void init_blue_button() {
  /* setup PC13 as input, no pull */
  enable_GPIOC();
  GPIOC.MODER = GPIOC.MODER & ~(3<<26);
  GPIOC.PUPDR = GPIOC.PUPDR & ~(3<<26);
}

void init_red_button() {
  /* setup PB8 as input, no pull */
  enable_GPIOB();
  GPIOB.MODER = GPIOB.MODER & ~(3<<16);
  GPIOB.PUPDR = GPIOB.PUPDR & ~(3<<16);
}

uint32_t poll_blue_button() {
  /* read PC13 input state */
  return ((GPIOC.IDR >> 13) & 1);
}

uint32_t poll_red_button() {
  /* read PB8 input state */
  return ((GPIOB.IDR >> 8) & 1);
}

void select_PC13_as_EXTI13() {

  enable_SYSCFG();
  SYSCFG.EXTICR4 = (SYSCFG.EXTICR4 & ~ (0xf << 4) ) | (2 << 4);
}

void select_PB8_as_EXTI8() {
  enable_SYSCFG();
  SYSCFG.EXTICR3 = (SYSCFG.EXTICR3 & ~ (0xf << 0) ) | (1 << 0);
}

void setup_IRQ_on_EXTI13_events() {
  /* EXTI is always enabled (no RCC mask) */

  EXTI.IMR  |= (1<<13); /* IRQ enable */
  EXTI.FTSR |= (1<<13); /* trigger on fallgin edge */
  EXTI.RTSR |= (1<<13); /* trigger on rising edge */
  EXTI.PR    = (1<<13); /* write 1 to reset pending flag */
}

void setup_IRQ_on_EXTI8_events() {
  /* EXTI is always enabled (no RCC mask) */

  EXTI.IMR  |= (1<<8); /* IRQ enable */
  EXTI.FTSR |= (1<<8); /* trigger on fallgin edge */
  EXTI.RTSR |= (1<<8); /* trigger on rising edge */
  EXTI.PR    = (1<<8); /* write 1 to reset pending flag */
}

void enable_EXTI15_10_IRQ_simple() {
  NVIC.ISER[1]=(1<<8);
}

void enable_EXTI9_5_IRQ_simple() {
  NVIC.ISER[0]=(1<<23);
}

void enable_EXTI15_10_IRQ() {
    enable_IRQ_by_number(40);
}

void enable_EXTI9_5_IRQ() {
    enable_IRQ_by_number(23);
}

void init_IRQ_PC13() {
  init_blue_button();
  select_PC13_as_EXTI13();
  setup_IRQ_on_EXTI13_events();
  enable_EXTI15_10_IRQ();
}

void init_IRQ_PB8() {
  init_red_button();
  select_PB8_as_EXTI8();
  setup_IRQ_on_EXTI8_events();
  enable_EXTI9_5_IRQ();
}
