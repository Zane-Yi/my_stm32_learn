#ifndef PAGE_MANAGER_H
#define PAGE_MANAGER_H

#include "stm32f1xx_hal.h"

typedef enum {
    PAGE_ID_WELCOME = 0,
    PAGE_ID_MENU,
    PAGE_ID_BEEP,
    PAGE_ID_LED
} PageId_t;

typedef enum {
    PAGE_MENU_ITEM_BEEP = 0,
    PAGE_MENU_ITEM_LED,
    PAGE_MENU_ITEM_COUNT
} PageMenuItem_t;

typedef enum {
    PAGE_ACTION_ITEM_TOGGLE = 0,
    PAGE_ACTION_ITEM_BACK,
    PAGE_ACTION_ITEM_COUNT
} PageActionItem_t;

void PageManager_Init(void);
void PageManager_Update(void);
void PageManager_Next(void);
void PageManager_Enter(void);

PageId_t PageManager_GetCurrentPage(void);
void PageManager_SetCurrentPage(PageId_t page_id);

PageMenuItem_t PageManager_GetSelectedItem(void);
void PageManager_SetSelectedItem(PageMenuItem_t item);

PageActionItem_t PageManager_GetBeepSelectedItem(void);
void PageManager_SetBeepSelectedItem(PageActionItem_t item);

PageActionItem_t PageManager_GetLedSelectedItem(void);
void PageManager_SetLedSelectedItem(PageActionItem_t item);

uint8_t PageManager_GetBeepEnabled(void);
void PageManager_SetBeepEnabled(uint8_t enabled);

uint8_t PageManager_GetLedEnabled(void);
void PageManager_SetLedEnabled(uint8_t enabled);

void PageManager_MarkDirty(void);
uint8_t PageManager_IsDirty(void);
void PageManager_ClearDirty(void);

#endif
