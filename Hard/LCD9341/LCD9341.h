#ifndef LCD9341_H
#define LCD9341_H

#include "stm32f1xx_hal.h"

#define ILI9341_RST_PORT GPIOG
#define ILI9341_RST_PIN  GPIO_PIN_11
#define ILI9341_BK_PORT  GPIOG
#define ILI9341_BK_PIN   GPIO_PIN_6

#define ILI9341_ID 0x9341U
#define ST7789V_ID 0x8552U

#define ILI9341_WIDTH  240U
#define ILI9341_HEIGHT 320U

#define FSMC_ADDR_ILI9341_CMD  ((uint32_t)0x6C000000)
#define FSMC_ADDR_ILI9341_DATA ((uint32_t)0x6D000000)

void ILI9341_Init(void);
void ILI9341_Clear(uint16_t color);
void ILI9341_FillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                      uint16_t color);

#endif
