// joystick.c
#include "joystick.h"
#include "i2c.h"
#include <stdint.h>
#include <stdio.h>

#define REG_X_MSB  0x03
#define REG_X_LSB  0x04
#define REG_Y_MSB  0x05
#define REG_Y_LSB  0x06

/* Variables globales pour lire X Y une seule fois */
static uint16_t g_x = 512;
static uint16_t g_y = 512;

void joystick_init(void)
{
    i2c_init();
    /* Premiere lecture a vider */
    joystick_read_x();
    joystick_read_y();
}

uint16_t joystick_read_x(void)
{
    uint8_t msb = i2c_read_reg(JOYSTICK_ADDR, REG_X_MSB);
    uint8_t lsb = i2c_read_reg(JOYSTICK_ADDR, REG_X_LSB);
    g_x = (uint16_t)((msb << 2) | (lsb >> 6));
    return g_x;
}

uint16_t joystick_read_y(void)
{
    uint8_t msb = i2c_read_reg(JOYSTICK_ADDR, REG_Y_MSB);
    uint8_t lsb = i2c_read_reg(JOYSTICK_ADDR, REG_Y_LSB);
    g_y = (uint16_t)((msb << 2) | (lsb >> 6));
    return g_y;
}

/* Lire X et Y une seule fois par cycle */
static void joystick_update(void)
{
    joystick_read_x();
    joystick_read_y();
}

JoystickCmd joystick_get_command(void)
{
    joystick_update();  /* lecture unique ici */

    int16_t dx = (int16_t)g_x - JOYSTICK_CENTER;
    int16_t dy = (int16_t)g_y - JOYSTICK_CENTER;

    /* Valeurs absolues */
    int16_t absdx = dx < 0 ? -dx : dx;
    int16_t absdy = dy < 0 ? -dy : dy;

    printf("X=%u Y=%u dx=%d dy=%d\r\n", g_x, g_y, dx, dy);

    /* Zone morte */
    if (absdx < JOYSTICK_SEUIL && absdy < JOYSTICK_SEUIL)
        return CMD_STOP;

    /* Comparer les axes pour trouver le dominant */
    if (absdy >= absdx) {
        /* Y dominant */
        if (dy < 0) return CMD_FORWARD;
        else        return CMD_BACK;
    } else {
        /* X dominant */
        if (dx > 0) return CMD_RIGHT;
        else        return CMD_LEFT;
    }
}

uint8_t joystick_get_speed(void)
{
    /* Utiliser g_x et g_y deja lus dans get_command */
    int16_t dx = (int16_t)g_x - JOYSTICK_CENTER;
    int16_t dy = (int16_t)g_y - JOYSTICK_CENTER;

    /* Valeurs absolues correctes */
    int16_t absdx = dx < 0 ? -dx : dx;
    int16_t absdy = dy < 0 ? -dy : dy;

    /* Prendre le maximum des deux axes */
    int16_t val = absdx > absdy ? absdx : absdy;

    uint8_t speed = (uint8_t)(val * 100 / 512);
    if (speed > 100) speed = 100;
    return speed;
}
