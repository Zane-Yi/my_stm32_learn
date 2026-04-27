#ifndef FONTS_H
#define FONTS_H

#include "stm32f1xx.h"

typedef struct {
    const uint8_t *table;
    uint16_t Width;
    uint16_t Height;
} sFONT;

extern sFONT Font24x32;
extern sFONT Font16x24;
extern sFONT Font8x16;

#endif
