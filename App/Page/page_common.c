#include "page_common.h"

#include <string.h>

#include "Display.h"

uint8_t PageCommon_SelectNext(uint8_t current, uint8_t count)
{
    if (count == 0U) {
        return current;
    }

    return (uint8_t)((current + 1U) % count);
}

uint8_t PageCommon_Toggle(uint8_t value)
{
    return (uint8_t)!value;
}

void PageCommon_DrawLabel(uint16_t x,
                          uint16_t y,
                          const char *text,
                          uint16_t text_color,
                          uint16_t background_color)
{
    uint16_t text_width;

    if (text == NULL) {
        return;
    }

    text_width = (uint16_t)(strlen(text) * Font8x16.Width);
    Display_FillRect(x,
                     y,
                     text_width,
                     Font8x16.Height,
                     background_color);
    Display_DrawAsciiString(x, y, text, &Font8x16, text_color);
}

void PageCommon_DrawMenuItem(uint16_t y, const char *text, uint8_t selected)
{
    if (selected != 0U) {
        Display_FillRect(24U, (uint16_t)(y - 4U), 192U, 24U, DISPLAY_COLOR_CYAN);
        PageCommon_DrawLabel(32U, y, text, DISPLAY_COLOR_BLACK, DISPLAY_COLOR_CYAN);
    } else {
        Display_FillRect(24U, (uint16_t)(y - 4U), 192U, 24U, DISPLAY_COLOR_BLACK);
        PageCommon_DrawLabel(32U, y, text, DISPLAY_COLOR_WHITE, DISPLAY_COLOR_BLACK);
    }
}

void PageCommon_DrawTitle(const char *title)
{
    PageCommon_DrawLabel(72U,
                         24U,
                         title,
                         DISPLAY_COLOR_CYAN,
                         DISPLAY_COLOR_BLACK);
}

void PageCommon_DrawHint(uint16_t y, const char *text)
{
    PageCommon_DrawLabel(24U,
                         y,
                         text,
                         DISPLAY_COLOR_WHITE,
                         DISPLAY_COLOR_BLACK);
}
