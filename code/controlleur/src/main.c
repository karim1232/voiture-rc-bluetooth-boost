// main.c controleur
#include <stdio.h>
#include "sys/cm4.h"
#include "sys/devices.h"
#include "sys/init.h"
#include "sys/clock.h"
#include "bluetooth.h"
#include "joystick.h"

void delay(int ms) {
    for (volatile int i = 0; i < ms * 15000; i++);
}

int main() {
    printf("Controleur init...\r\n");

    bluetooth_init();
    joystick_init();

    printf("Go!\r\n");

    JoystickCmd last_cmd = CMD_STOP;

    while (1) {
        JoystickCmd cmd   = joystick_get_command();
        uint8_t     speed = joystick_get_speed();

        if (cmd != last_cmd) {
            bluetooth_send((char)cmd);
            printf("Envoye: %c vitesse: %d\r\n", (char)cmd, speed);
            last_cmd = cmd;
        }

        delay(50);
    }

    return 0;
}
