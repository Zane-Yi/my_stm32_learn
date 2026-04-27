#ifndef XPT2046_H
#define XPT2046_H

#include "stm32f1xx_hal.h"

typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t raw_x;
    uint16_t raw_y;
    uint8_t pressed;
} XPT2046_State_t;

void XPT2046_Init(void);
uint8_t XPT2046_IsPressed(void);
uint8_t XPT2046_Read(XPT2046_State_t *state);

#endif
