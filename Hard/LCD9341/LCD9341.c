#include "LCD9341.h"



static SRAM_HandleTypeDef hsram4;
static uint16_t lcd_id;

static inline void ILI9341_WriteCmd(uint16_t cmd) {
    *(__IO uint16_t *)FSMC_ADDR_ILI9341_CMD = cmd;
}

static inline void ILI9341_WriteData(uint16_t data) {
    *(__IO uint16_t *)FSMC_ADDR_ILI9341_DATA = data;
}

static inline uint16_t ILI9341_ReadData(void) {
    return *(__IO uint16_t *)FSMC_ADDR_ILI9341_DATA;
}

static void ILI9341_Reset(void) {
    HAL_GPIO_WritePin(ILI9341_RST_PORT, ILI9341_RST_PIN, GPIO_PIN_RESET);
    HAL_Delay(50);
    HAL_GPIO_WritePin(ILI9341_RST_PORT, ILI9341_RST_PIN, GPIO_PIN_SET);
    HAL_Delay(50);
}

static void ILI9341_BacklightControl(GPIO_PinState state)
{
    /* 该板背光为低电平点亮，所以这里对外统一使用亮/灭状态。 */
    HAL_GPIO_WritePin(
        ILI9341_BK_PORT,
        ILI9341_BK_PIN,
        (state == GPIO_PIN_SET) ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

static uint16_t get_id_cmd (uint16_t cmd) {
    uint16_t temp_id;

    ILI9341_WriteCmd(cmd);
    (void)ILI9341_ReadData();
    (void)ILI9341_ReadData();
    temp_id = ILI9341_ReadData();
    temp_id <<= 8;
    temp_id |= ILI9341_ReadData();
    return temp_id;
}

static uint16_t ILI9341_ReadID(void) {
    uint16_t id;
    id = get_id_cmd(0x04);
    if (id == ST7789V_ID) {
        return ST7789V_ID;
    }
    id = get_id_cmd(0xD3);
    return id;
}

static void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    GPIO_InitStruct.Pin = ILI9341_BK_PIN | ILI9341_RST_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    HAL_GPIO_WritePin(ILI9341_RST_PORT, ILI9341_RST_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ILI9341_BK_PORT, ILI9341_BK_PIN, GPIO_PIN_RESET);
}

static void MX_FSMC_Init(void)
{
    FSMC_NORSRAM_TimingTypeDef Timing = {0};
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_FSMC_CLK_ENABLE();

    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9 |
                          GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_4 |
                          GPIO_PIN_5;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
                          GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |
                          GPIO_PIN_15 | GPIO_PIN_2;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    hsram4.Instance = FSMC_NORSRAM_DEVICE;
    hsram4.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
    hsram4.Init.NSBank = FSMC_NORSRAM_BANK4;
    hsram4.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
    hsram4.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
    hsram4.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
    hsram4.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
    hsram4.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;

    Timing.AddressSetupTime = 5;
    Timing.DataSetupTime = 15;
    Timing.AccessMode = FSMC_ACCESS_MODE_B;

    HAL_SRAM_Init(&hsram4, &Timing, NULL);
}

static void ILI9341_REG_Config(void)
{
    lcd_id = ILI9341_ReadID();

    if (lcd_id == ILI9341_ID) {
        ILI9341_WriteCmd(0xCF);
        ILI9341_WriteData(0x00);
        ILI9341_WriteData(0x81);
        ILI9341_WriteData(0x30);

        ILI9341_WriteCmd(0xED);
        ILI9341_WriteData(0x64);
        ILI9341_WriteData(0x03);
        ILI9341_WriteData(0x12);
        ILI9341_WriteData(0x81);

        ILI9341_WriteCmd(0xE8);
        ILI9341_WriteData(0x85);
        ILI9341_WriteData(0x10);
        ILI9341_WriteData(0x78);

        ILI9341_WriteCmd(0xCB);
        ILI9341_WriteData(0x39);
        ILI9341_WriteData(0x2C);
        ILI9341_WriteData(0x00);
        ILI9341_WriteData(0x34);
        ILI9341_WriteData(0x06);

        ILI9341_WriteCmd(0xF7);
        ILI9341_WriteData(0x20);

        ILI9341_WriteCmd(0xEA);
        ILI9341_WriteData(0x00);
        ILI9341_WriteData(0x00);

        ILI9341_WriteCmd(0xB1);
        ILI9341_WriteData(0x00);
        ILI9341_WriteData(0x1B);

        ILI9341_WriteCmd(0xB6);
        ILI9341_WriteData(0x0A);
        ILI9341_WriteData(0xA2);

        ILI9341_WriteCmd(0xC0);
        ILI9341_WriteData(0x35);

        ILI9341_WriteCmd(0xC1);
        ILI9341_WriteData(0x11);

        ILI9341_WriteCmd(0xC5);
        ILI9341_WriteData(0x45);
        ILI9341_WriteData(0x45);

        ILI9341_WriteCmd(0xC7);
        ILI9341_WriteData(0xA2);

        ILI9341_WriteCmd(0x36);
        ILI9341_WriteData(0xC8);

        ILI9341_WriteCmd(0x3A);
        ILI9341_WriteData(0x55);

        ILI9341_WriteCmd(0x11);
        HAL_Delay(120);
        ILI9341_WriteCmd(0x29);
        return;
    }

    if (lcd_id == ST7789V_ID) {
        /* 当前仓库里没有完整 ST7789V 初始化表，先保留最小上电流程。 */
        ILI9341_WriteCmd(0xCF);
        ILI9341_WriteData(0x00);
        ILI9341_WriteData(0xC1);
        ILI9341_WriteData(0x30);
        ILI9341_WriteCmd(0x11);
        HAL_Delay(120);
        ILI9341_WriteCmd(0x29);
    }
}

void ILI9341_Init(void)
{
    MX_GPIO_Init();
    MX_FSMC_Init();
    ILI9341_BacklightControl(GPIO_PIN_SET);
    ILI9341_Reset();
    ILI9341_REG_Config();
}

void ILI9341_Clear(uint16_t color)
{
    uint32_t i;

    ILI9341_WriteCmd(0x2A);
    ILI9341_WriteData(0);
    ILI9341_WriteData(0);
    ILI9341_WriteData(0);
    ILI9341_WriteData(ILI9341_WIDTH - 1U);

    ILI9341_WriteCmd(0x2B);
    ILI9341_WriteData(0);
    ILI9341_WriteData(0);
    ILI9341_WriteData(0);
    ILI9341_WriteData(ILI9341_HEIGHT - 1U);

    ILI9341_WriteCmd(0x2C);
    for (i = 0; i < ILI9341_WIDTH * ILI9341_HEIGHT; i++) {
        ILI9341_WriteData(color);
    }
}

void ILI9341_FillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                      uint16_t color)
{
    uint32_t pixel_count;
    uint32_t x_limit;
    uint32_t y_limit;
    uint16_t end_x;
    uint16_t end_y;

    if ((x >= ILI9341_WIDTH) || (y >= ILI9341_HEIGHT) || (width == 0U) ||
        (height == 0U)) {
        return;
    }

    x_limit = (uint32_t)x + (uint32_t)width - 1U;
    if (x_limit >= ILI9341_WIDTH) {
        end_x = ILI9341_WIDTH - 1U;
    } else {
        end_x = (uint16_t)x_limit;
    }

    y_limit = (uint32_t)y + (uint32_t)height - 1U;
    if (y_limit >= ILI9341_HEIGHT) {
        end_y = ILI9341_HEIGHT - 1U;
    } else {
        end_y = (uint16_t)y_limit;
    }

    ILI9341_WriteCmd(0x2A);
    ILI9341_WriteData((uint16_t)(x >> 8));
    ILI9341_WriteData((uint16_t)(x & 0xFFU));
    ILI9341_WriteData((uint16_t)(end_x >> 8));
    ILI9341_WriteData((uint16_t)(end_x & 0xFFU));

    ILI9341_WriteCmd(0x2B);
    ILI9341_WriteData((uint16_t)(y >> 8));
    ILI9341_WriteData((uint16_t)(y & 0xFFU));
    ILI9341_WriteData((uint16_t)(end_y >> 8));
    ILI9341_WriteData((uint16_t)(end_y & 0xFFU));

    ILI9341_WriteCmd(0x2C);
    pixel_count = (uint32_t)(end_x - x + 1U) * (uint32_t)(end_y - y + 1U);
    while (pixel_count-- > 0U) {
        ILI9341_WriteData(color);
    }
}
