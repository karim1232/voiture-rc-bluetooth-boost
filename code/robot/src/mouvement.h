/*
 * mouvement.h
 *
 *  Created on: May 12, 2026
 *      Author: karim
 */

#ifndef SRC_MOUVEMENT_H_
#define SRC_MOUVEMENT_H_

#include <stdint.h>

/* Init */
void initialize_PINs(void);
void setup_PWM(void);

/* Vitesse gauche/droite séparées (0-100%) */
void set_speed_left(uint32_t percent);
void set_speed_right(uint32_t percent);

/* Direction gauche/droite */
void set_dir_left(int avant);
void set_dir_right(int avant);

/* Commandes haut niveau */
void motor_forward(uint32_t speed);
void motor_back(uint32_t speed);
void motor_left(uint32_t speed);
void motor_right(uint32_t speed);
void motor_stop(void);

#endif /* SRC_MOUVEMENT_H_ */
