#include <stdio.h>
#include <math.h>
#include "sys/cm4.h"
#include "sys/devices.h"
#include "sys/init.h"
#include "sys/clock.h"
#include "mouvement.h"
#include "bluetooth.h"

void delay(int ms) {
    for (volatile int i = 0; i < ms * 15000; i++);
}


int main() {
	printf("\e[2J\e[1;1H\r\n*** Welcome to Nucleo F446 ! ***\r\n");
	printf("   %08lx-%08lx-%08lx\r\n",U_ID[0],U_ID[1],U_ID[2]);
	printf("SYSCLK = %9lu Hz\r\n",get_SYSCLK());
	printf("AHBCLK = %9lu Hz\r\n",get_AHBCLK());
	printf("APB1CLK= %9lu Hz\r\n",get_APB1CLK());
	printf("APB2CLK= %9lu Hz\r\n",get_APB2CLK());
	printf("APB1TIMCLK= %9lu Hz\r\n",get_APB1TIMCLK());
	printf("\r\n");


	printf("Voiture init...\r\n");

	initialize_PINs();
	setup_PWM();
    bluetooth_init();

    printf("En attente commandes...\r\n");

    while (1) {
        char cmd = bluetooth_receive();
        printf("Recu: %c\r\n", cmd);

        switch (cmd) {
        	case 'F':
        		printf("AVANT\r\n");
        		motor_forward(70);
        		break;
        	case 'B':
        		printf("ARRIERE\r\n");
        		motor_back(70);
        		break;
        	case 'L':
        		printf("GAUCHE\r\n");
        		motor_left(50);
        		break;
        	case 'R':
        		printf("DROITE\r\n");
        		motor_right(50);
        		break;
        	case 'S':
        		printf("STOP\r\n");
        		motor_stop();
        		break;
        	default:
        		break;
        }
    }
    return 0;
}
