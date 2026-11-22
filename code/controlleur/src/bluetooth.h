/*
 * bluetooth.h
 *
 *  Created on: May 21, 2026
 *      Author: karim
 */

#ifndef SRC_BLUETOOTH_H_
#define SRC_BLUETOOTH_H_

#include <stdint.h>

void bluetooth_init(void);
char bluetooth_receive(void);
void bluetooth_send(char c);

#endif
