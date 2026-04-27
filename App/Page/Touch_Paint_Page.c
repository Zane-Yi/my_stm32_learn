#include "Touch_Paint_Page.h"

#include "BEEP.h"
#include "Display.h"
#include "LED.h"

#include <string.h>



static uint8_t s_page_initialized = 0U;
static uint8_t s_page_dirty = 0U;
static uint8_t s_beep_enabled = 0U;
static uint8_t s_led_enabled = 0U;
static PageState_t s_page_state = PAGE_STATE_START;
static MenuItem_t s_selected_item = MENU_ITEM_BEEP;

static void TouchPaintPage_DrawLabel(uint16_t x,
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

static void TouchPaintPage_DrawMenuItem(uint16_t y, const char *text, uint8_t selected)
{
    if (selected != 0U) {
        Display_FillRect(24U, (uint16_t)(y - 4U), 192U, 24U, DISPLAY_COLOR_CYAN);
        TouchPaintPage_DrawLabel(32U, y, text, DISPLAY_COLOR_BLACK, DISPLAY_COLOR_CYAN);
    } else {
        Display_FillRect(24U, (uint16_t)(y - 4U), 192U, 24U, DISPLAY_COLOR_BLACK);
        TouchPaintPage_DrawLabel(32U, y, text, DISPLAY_COLOR_WHITE, DISPLAY_COLOR_BLACK);
    }
}

static void TouchPaintPage_Render(void)
{
    Display_FillRect(0U, 0U, DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_COLOR_BLACK);

    if (s_page_state == PAGE_STATE_START) {
        TouchPaintPage_DrawLabel(72U,
                                 112U,
                                 "hello world",
                                 DISPLAY_COLOR_WHITE,
                                 DISPLAY_COLOR_BLACK);
        return;
    }

    TouchPaintPage_DrawLabel(72U,
                             24U,
                             "MAIN MENU",
                             DISPLAY_COLOR_CYAN,
                             DISPLAY_COLOR_BLACK);
    TouchPaintPage_DrawMenuItem(80U,
                                s_beep_enabled != 0U ? "1.BEEP: ON" : "1.BEEP: OFF",
                                s_selected_item == MENU_ITEM_BEEP);
    TouchPaintPage_DrawMenuItem(120U,
                                s_led_enabled != 0U ? "2.LED : ON" : "2.LED : OFF",
                                s_selected_item == MENU_ITEM_LED);
}

void TouchPaintPage_Init(void)
{
    s_page_initialized = 1U;
    s_page_dirty = 1U;
    TouchPaintPage_Render();
    s_page_dirty = 0U;
}

void TouchPaintPage_Update(void)
{
    if (!s_page_initialized) {
        TouchPaintPage_Init();
        return;
    }

    if (s_page_dirty != 0U) {
        TouchPaintPage_Render();
        s_page_dirty = 0U;
    }
}

void TouchPaintPage_HandleNext(void)
{
    if (s_page_state == PAGE_STATE_START) {
        s_page_state = PAGE_STATE_MENU;
        s_page_dirty = 1U;
        return;
    }

    s_selected_item = (MenuItem_t)((s_selected_item + 1U) % MENU_ITEM_COUNT);
    s_page_dirty = 1U;
}

void TouchPaintPage_HandleConfirm(void)
{
    if (s_page_state == PAGE_STATE_START) {
        return;
    }

    if (s_selected_item == MENU_ITEM_BEEP) {
        s_beep_enabled = (uint8_t)!s_beep_enabled;
        if (s_beep_enabled != 0U) {
            BEEP_ON;
        } else {
            BEEP_OFF;
        }
    } else {
        s_led_enabled = (uint8_t)!s_led_enabled;
        if (s_led_enabled != 0U) {
            LED_G_ON;
        } else {
            LED_G_OFF;
        }
    }

    s_page_dirty = 1U;
}
