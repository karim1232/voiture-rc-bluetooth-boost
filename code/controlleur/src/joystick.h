// joystick.h
#ifndef SRC_JOYSTICK_H_
#define SRC_JOYSTICK_H_

#include <stdint.h>

#define JOYSTICK_ADDR    0x20
#define JOYSTICK_CENTER  512
#define JOYSTICK_SEUIL   50

typedef enum {
    CMD_STOP    = 'S',
    CMD_FORWARD = 'F',
    CMD_BACK    = 'B',
    CMD_LEFT    = 'L',
    CMD_RIGHT   = 'R'
} JoystickCmd;

void joystick_init(void);
uint16_t joystick_read_x(void);
uint16_t joystick_read_y(void);
JoystickCmd joystick_get_command(void);
uint8_t joystick_get_speed(void);

#endif
