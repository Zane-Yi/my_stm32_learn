#include "page_manager.h"

#include "BEEP.h"
#include "LED.h"
#include "page_beep.h"
#include "page_led.h"
#include "page_menu.h"
#include "page_welcome.h"

typedef struct {
    PageId_t current_page;
    PageMenuItem_t selected_item;
    PageActionItem_t beep_selected_item;
    PageActionItem_t led_selected_item;
    uint8_t initialized;
    uint8_t dirty;
    uint8_t beep_enabled;
    uint8_t led_enabled;
} PageManagerState_t;

static PageManagerState_t s_page_state = {
    .current_page = PAGE_ID_WELCOME,
    .selected_item = PAGE_MENU_ITEM_BEEP,
    .beep_selected_item = PAGE_ACTION_ITEM_TOGGLE,
    .led_selected_item = PAGE_ACTION_ITEM_TOGGLE,
    .initialized = 0U,
    .dirty = 0U,
    .beep_enabled = 0U,
    .led_enabled = 0U,
};

void PageManager_Init(void)
{
    s_page_state.current_page = PAGE_ID_WELCOME;
    s_page_state.selected_item = PAGE_MENU_ITEM_BEEP;
    s_page_state.beep_selected_item = PAGE_ACTION_ITEM_TOGGLE;
    s_page_state.led_selected_item = PAGE_ACTION_ITEM_TOGGLE;
    s_page_state.initialized = 1U;
    s_page_state.dirty = 0U;
    s_page_state.beep_enabled = 0U;
    s_page_state.led_enabled = 0U;

    BEEP_OFF;
    LED_G_OFF;

    Page_Welcome_Setup();
}

void PageManager_Update(void)
{
    if (s_page_state.initialized == 0U) {
        PageManager_Init();
        return;
    }

    switch (s_page_state.current_page) {
    case PAGE_ID_WELCOME:
        Page_Welcome_Update();
        break;
    case PAGE_ID_MENU:
        Page_Menu_Update();
        break;
    case PAGE_ID_BEEP:
        Page_Beep_Update();
        break;
    case PAGE_ID_LED:
        Page_Led_Update();
        break;
    default:
        Page_Menu_Setup();
        break;
    }
}

void PageManager_Next(void)
{
    switch (s_page_state.current_page) {
    case PAGE_ID_WELCOME:
        Page_Welcome_Next();
        break;
    case PAGE_ID_MENU:
        Page_Menu_Next();
        break;
    case PAGE_ID_BEEP:
        Page_Beep_Next();
        break;
    case PAGE_ID_LED:
        Page_Led_Next();
        break;
    default:
        Page_Menu_Setup();
        break;
    }
}

void PageManager_Enter(void)
{
    switch (s_page_state.current_page) {
    case PAGE_ID_WELCOME:
        Page_Welcome_Enter();
        break;
    case PAGE_ID_MENU:
        Page_Menu_Enter();
        break;
    case PAGE_ID_BEEP:
        Page_Beep_Enter();
        break;
    case PAGE_ID_LED:
        Page_Led_Enter();
        break;
    default:
        Page_Menu_Setup();
        break;
    }
}

PageId_t PageManager_GetCurrentPage(void)
{
    return s_page_state.current_page;
}

void PageManager_SetCurrentPage(PageId_t page_id)
{
    s_page_state.current_page = page_id;
}

PageMenuItem_t PageManager_GetSelectedItem(void)
{
    return s_page_state.selected_item;
}

void PageManager_SetSelectedItem(PageMenuItem_t item)
{
    s_page_state.selected_item = item;
}

PageActionItem_t PageManager_GetBeepSelectedItem(void)
{
    return s_page_state.beep_selected_item;
}

void PageManager_SetBeepSelectedItem(PageActionItem_t item)
{
    s_page_state.beep_selected_item = item;
}

PageActionItem_t PageManager_GetLedSelectedItem(void)
{
    return s_page_state.led_selected_item;
}

void PageManager_SetLedSelectedItem(PageActionItem_t item)
{
    s_page_state.led_selected_item = item;
}

uint8_t PageManager_GetBeepEnabled(void)
{
    return s_page_state.beep_enabled;
}

void PageManager_SetBeepEnabled(uint8_t enabled)
{
    s_page_state.beep_enabled = enabled;
}

uint8_t PageManager_GetLedEnabled(void)
{
    return s_page_state.led_enabled;
}

void PageManager_SetLedEnabled(uint8_t enabled)
{
    s_page_state.led_enabled = enabled;
}

void PageManager_MarkDirty(void)
{
    s_page_state.dirty = 1U;
}

uint8_t PageManager_IsDirty(void)
{
    return s_page_state.dirty;
}

void PageManager_ClearDirty(void)
{
    s_page_state.dirty = 0U;
}
