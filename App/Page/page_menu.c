#include "page_menu.h"

#include "Display.h"
#include "page_beep.h"
#include "page_common.h"
#include "page_manager.h"
#include "page_led.h"

void Page_Menu_Setup(void)
{
    PageManager_SetCurrentPage(PAGE_ID_MENU);
    PageManager_MarkDirty();
}

void Page_Menu_Update(void)
{
    if (PageManager_IsDirty() == 0U) {
        return;
    }

    Display_FillRect(0U, 0U, DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_COLOR_BLACK);
    PageCommon_DrawTitle("MAIN MENU");
    PageCommon_DrawMenuItem(88U,
                            "1. BEEP",
                            PageManager_GetSelectedItem() == PAGE_MENU_ITEM_BEEP);
    PageCommon_DrawMenuItem(128U,
                            "2. LED",
                            PageManager_GetSelectedItem() == PAGE_MENU_ITEM_LED);

    PageManager_ClearDirty();
}

void Page_Menu_Next(void)
{
    PageManager_SetSelectedItem((PageMenuItem_t)
        PageCommon_SelectNext((uint8_t)PageManager_GetSelectedItem(), (uint8_t)PAGE_MENU_ITEM_COUNT));
    PageManager_MarkDirty();
}

void Page_Menu_Enter(void)
{
    if (PageManager_GetSelectedItem() == PAGE_MENU_ITEM_BEEP) {
        Page_Beep_Setup();
    } else {
        Page_Led_Setup();
    }
}
