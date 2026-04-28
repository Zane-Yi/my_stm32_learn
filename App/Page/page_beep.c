#include "page_beep.h"

#include "BEEP.h"
#include "Display.h"
#include "page_common.h"
#include "page_manager.h"
#include "page_menu.h"

void Page_Beep_Setup(void)
{
    PageManager_SetCurrentPage(PAGE_ID_BEEP);
    PageManager_SetBeepSelectedItem(PAGE_ACTION_ITEM_TOGGLE);
    PageManager_MarkDirty();
}

void Page_Beep_Update(void)
{
    if (PageManager_IsDirty() == 0U) {
        return;
    }

    Display_FillRect(0U, 0U, DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_COLOR_BLACK);
    PageCommon_DrawTitle("BEEP PAGE");
    PageCommon_DrawHint(56U,
                        PageManager_GetBeepEnabled() != 0U ? "STATE: ON" : "STATE: OFF");
    PageCommon_DrawMenuItem(104U,
                            PageManager_GetBeepEnabled() != 0U ? "1. BEEP OFF" : "1. BEEP ON",
                            PageManager_GetBeepSelectedItem() == PAGE_ACTION_ITEM_TOGGLE);
    PageCommon_DrawMenuItem(144U,
                            "2. BACK",
                            PageManager_GetBeepSelectedItem() == PAGE_ACTION_ITEM_BACK);

    PageManager_ClearDirty();
}

void Page_Beep_Next(void)
{
    PageManager_SetBeepSelectedItem((PageActionItem_t)
        PageCommon_SelectNext((uint8_t)PageManager_GetBeepSelectedItem(), (uint8_t)PAGE_ACTION_ITEM_COUNT));
    PageManager_MarkDirty();
}

void Page_Beep_Enter(void)
{
    uint8_t enabled;

    if (PageManager_GetBeepSelectedItem() == PAGE_ACTION_ITEM_BACK) {
        Page_Menu_Setup();
        return;
    }

    enabled = PageCommon_Toggle(PageManager_GetBeepEnabled());
    PageManager_SetBeepEnabled(enabled);
    if (enabled != 0U) {
        BEEP_ON;
    } else {
        BEEP_OFF;
    }

    PageManager_MarkDirty();
}
