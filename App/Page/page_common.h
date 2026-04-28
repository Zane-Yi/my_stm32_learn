#ifndef PAGE_COMMON_H
#define PAGE_COMMON_H

#include "stm32f1xx_hal.h"

uint8_t PageCommon_SelectNext(uint8_t current, uint8_t count);
uint8_t PageCommon_Toggle(uint8_t value);

void PageCommon_DrawLabel(uint16_t x,
                          uint16_t y,
                          const char *text,
                          uint16_t text_color,
                          uint16_t background_color);
void PageCommon_DrawMenuItem(uint16_t y, const char *text, uint8_t selected);
void PageCommon_DrawTitle(const char *title);
void PageCommon_DrawHint(uint16_t y, const char *text);

#endif
