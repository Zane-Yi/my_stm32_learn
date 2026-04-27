#ifndef DISPLAY_H
#define DISPLAY_H

#include "Fonts.h"
#include "LCD9341.h"
#include "stm32f1xx_hal.h"

#define DISPLAY_COLOR_BLACK 0x0000U
#define DISPLAY_COLOR_WHITE 0xFFFFU
#define DISPLAY_COLOR_RED   0xF800U
#define DISPLAY_COLOR_GREEN 0x07E0U
#define DISPLAY_COLOR_BLUE  0x001FU
#define DISPLAY_COLOR_CYAN  0x07FFU

#define DISPLAY_WIDTH  ILI9341_WIDTH
#define DISPLAY_HEIGHT ILI9341_HEIGHT

void Display_Init(void);
void Display_Clear(uint16_t color);
void Display_FillScreen(uint16_t color);
void Display_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void Display_DrawHLine(uint16_t x, uint16_t y, uint16_t width, uint16_t color);
void Display_DrawVLine(uint16_t x, uint16_t y, uint16_t height, uint16_t color);
void Display_DrawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                      uint16_t color);
void Display_FillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                      uint16_t color);
void Display_ClearLine(uint16_t y, uint16_t height, uint16_t color);
void Display_DrawAsciiChar(uint16_t x, uint16_t y, char character,
                           const sFONT *font, uint16_t color);
void Display_DrawAsciiString(uint16_t x, uint16_t y, const char *text,
                             const sFONT *font, uint16_t color);
void Display_Printf(uint16_t x, uint16_t y, const sFONT *font,
                    uint16_t color, const char *fmt, ...);

#endif
