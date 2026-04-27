#ifndef TOUCH_PAINT_PAGE_H
#define TOUCH_PAINT_PAGE_H

#include "stm32f1xx_hal.h"

typedef enum {
    PAGE_STATE_START = 0,
    PAGE_STATE_MENU
} PageState_t;

typedef enum {
    MENU_ITEM_BEEP = 0,
    MENU_ITEM_LED,
    MENU_ITEM_COUNT
} MenuItem_t;

void TouchPaintPage_Init(void);
void TouchPaintPage_Update(void);
void TouchPaintPage_HandleNext(void);
void TouchPaintPage_HandleConfirm(void);

#endif
