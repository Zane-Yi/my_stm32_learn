#include "page_led.h"

#include "Display.h"
#include "LED.h"
#include "page_common.h"
#include "page_manager.h"
#include "page_menu.h"

void Page_Led_Setup(void)
{
    PageManager_SetCurrentPage(PAGE_ID_LED);
    PageManager_SetLedSelectedItem(PAGE_ACTION_ITEM_TOGGLE);
    PageManager_MarkDirty();
}

void Page_Led_Update(void)
{
    if (PageManager_IsDirty() == 0U) {
        return;
    }

    Display_FillRect(0U, 0U, DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_COLOR_BLACK);
    PageCommon_DrawTitle("LED PAGE");
    PageCommon_DrawHint(56U,
                        PageManager_GetLedEnabled() != 0U ? "STATE: ON" : "STATE: OFF");
    PageCommon_DrawMenuItem(104U,
                            PageManager_GetLedEnabled() != 0U ? "1. LED OFF" : "1. LED ON",
                            PageManager_GetLedSelectedItem() == PAGE_ACTION_ITEM_TOGGLE);
    PageCommon_DrawMenuItem(144U,
                            "2. BACK",
                            PageManager_GetLedSelectedItem() == PAGE_ACTION_ITEM_BACK);
    PageManager_ClearDirty();
}

void Page_Led_Next(void)
{
    PageManager_SetLedSelectedItem((PageActionItem_t)
        PageCommon_SelectNext((uint8_t)PageManager_GetLedSelectedItem(), (uint8_t)PAGE_ACTION_ITEM_COUNT));
    PageManager_MarkDirty();
}

void Page_Led_Enter(void)
{
    uint8_t enabled;

    if (PageManager_GetLedSelectedItem() == PAGE_ACTION_ITEM_BACK) {
        Page_Menu_Setup();
        return;
    }

    enabled = PageCommon_Toggle(PageManager_GetLedEnabled());
    PageManager_SetLedEnabled(enabled);
    if (enabled != 0U) {
        LED_G_ON;
    } else {
        LED_G_OFF;
    }

    PageManager_MarkDirty();
}
