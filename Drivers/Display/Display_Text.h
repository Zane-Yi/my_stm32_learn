#ifndef DISPLAY_TEXT_H
#define DISPLAY_TEXT_H

#include "Fonts.h"
#include "stm32f1xx_hal.h"

void DisplayText_DrawAsciiChar(uint16_t x, uint16_t y, char character,
                               const sFONT *font, uint16_t color);
void DisplayText_DrawAsciiString(uint16_t x, uint16_t y, const char *text,
                                 const sFONT *font, uint16_t color);
void DisplayText_Printf(uint16_t x, uint16_t y, const sFONT *font,
                        uint16_t color, const char *fmt, ...);

#endif
