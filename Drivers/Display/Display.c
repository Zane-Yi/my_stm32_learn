#include "Display.h"

#include "LCD9341.h"

void Display_Init(void)
{
    ILI9341_Init();
}

void Display_Clear(uint16_t color)
{
    ILI9341_Clear(color);
}

void Display_FillScreen(uint16_t color)
{
    Display_Clear(color);
}

void Display_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    Display_FillRect(x, y, 1U, 1U, color);
}

void Display_DrawHLine(uint16_t x, uint16_t y, uint16_t width, uint16_t color)
{
    Display_FillRect(x, y, width, 1U, color);
}

void Display_DrawVLine(uint16_t x, uint16_t y, uint16_t height, uint16_t color)
{
    Display_FillRect(x, y, 1U, height, color);
}

void Display_DrawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                      uint16_t color)
{
    if ((width == 0U) || (height == 0U)) {
        return;
    }

    Display_DrawHLine(x, y, width, color);
    if (height > 1U) {
        Display_DrawHLine(x, (uint16_t)(y + height - 1U), width, color);
    }

    if (height > 2U) {
        Display_DrawVLine(x, (uint16_t)(y + 1U), (uint16_t)(height - 2U), color);
        if (width > 1U) {
            Display_DrawVLine((uint16_t)(x + width - 1U),
                              (uint16_t)(y + 1U),
                              (uint16_t)(height - 2U),
                              color);
        }
    }
}

void Display_FillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                      uint16_t color)
{
    ILI9341_FillRect(x, y, width, height, color);
}

void Display_ClearLine(uint16_t y, uint16_t height, uint16_t color)
{
    if ((height == 0U) || (y >= DISPLAY_HEIGHT)) {
        return;
    }

    if (((uint32_t)y + (uint32_t)height) > DISPLAY_HEIGHT) {
        height = (uint16_t)(DISPLAY_HEIGHT - y);
    }

    Display_FillRect(0U, y, DISPLAY_WIDTH, height, color);
}
