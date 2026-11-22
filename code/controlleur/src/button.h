/*
 * button.h
 *
 */

#ifndef SRC_SYS_BUTTON_H_
#define SRC_SYS_BUTTON_H_

#include<stdint.h>

void init_blue_button();
void init_red_button();

uint32_t poll_blue_button();
uint32_t poll_red_button();

void select_PC13_as_EXTI13();
void select_PB8_as_EXTI8();

void setup_IRQ_on_EXTI13_events();
void setup_IRQ_on_EXTI8_events();

void enable_EXTI15_10_IRQ_simple();
void enable_EXTI9_5_IRQ_simple();

void enable_EXTI15_10_IRQ();
void enable_EXTI9_5_IRQ();

void init_IRQ_PC13();
void init_IRQ_PB8();


#endif /* SRC_SYS_BUTTON_H_ */
