#include "page_welcome.h"

#include "Display.h"
#include "page_common.h"
#include "page_manager.h"
#include "page_menu.h"

void Page_Welcome_Setup(void)
{
    PageManager_SetCurrentPage(PAGE_ID_WELCOME);
    PageManager_MarkDirty();
}

void Page_Welcome_Update(void)
{
    if (PageManager_IsDirty() == 0U) {
        return;
    }

    Display_FillRect(0U, 0U, DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_COLOR_BLACK);
    PageCommon_DrawLabel(72U,
                         120U,
                         "hello world",
                         DISPLAY_COLOR_CYAN,
                         DISPLAY_COLOR_BLACK);
    PageManager_ClearDirty();
}

void Page_Welcome_Next(void)
{
}

void Page_Welcome_Enter(void)
{
    Page_Menu_Setup();
}
