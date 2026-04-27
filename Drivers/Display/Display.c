#include "Display.h"

#include <stdarg.h>
#include <stdio.h>

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

void Display_DrawAsciiChar(uint16_t x, uint16_t y, char character,
                           const sFONT *font, uint16_t color)
{
    uint8_t row;
    uint8_t column;
    uint8_t bytes_per_row;
    uint16_t glyph_index;
    const uint8_t *glyph_data;

    if ((font == NULL) || (character < 32) || (character > 126)) {
        return;
    }

    bytes_per_row = (uint8_t)((font->Width + 7U) / 8U);
    glyph_index = (uint16_t)(character - 32);
    glyph_data = &font->table[glyph_index * (font->Height * bytes_per_row)];

    for (row = 0U; row < font->Height; row++) {
        for (column = 0U; column < font->Width; column++) {
            uint8_t data_byte;

            data_byte = glyph_data[row * bytes_per_row + (column / 8U)];
            if ((data_byte & (uint8_t)(0x80U >> (column % 8U))) != 0U) {
                Display_FillRect((uint16_t)(x + column),
                                 (uint16_t)(y + row),
                                 1U,
                                 1U,
                                 color);
            }
        }
    }
}

void Display_DrawAsciiString(uint16_t x, uint16_t y, const char *text,
                             const sFONT *font, uint16_t color)
{
    uint16_t cursor_x;
    uint16_t cursor_y;

    if ((text == NULL) || (font == NULL)) {
        return;
    }

    cursor_x = x;
    cursor_y = y;
    while (*text != '\0') {
        if (*text == '\n') {
            cursor_x = x;
            cursor_y = (uint16_t)(cursor_y + font->Height);
            text++;
            continue;
        }

        if (((uint32_t)cursor_x + (uint32_t)font->Width) > DISPLAY_WIDTH) {
            cursor_x = x;
            cursor_y = (uint16_t)(cursor_y + font->Height);
        }

        if (((uint32_t)cursor_y + (uint32_t)font->Height) > DISPLAY_HEIGHT) {
            break;
        }

        Display_DrawAsciiChar(cursor_x, cursor_y, *text, font, color);
        cursor_x = (uint16_t)(cursor_x + font->Width);
        text++;
    }
}

void Display_Printf(uint16_t x, uint16_t y, const sFONT *font,
                    uint16_t color, const char *fmt, ...)
{
    char buffer[64];
    va_list args;

    if ((fmt == NULL) || (font == NULL)) {
        return;
    }

    va_start(args, fmt);
    (void)vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    Display_DrawAsciiString(x, y, buffer, font, color);
}
