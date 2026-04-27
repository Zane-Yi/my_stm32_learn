#include "XPT2046.h"

#define XPT2046_IRQ_PORT   GPIOF
#define XPT2046_IRQ_PIN    GPIO_PIN_9
#define XPT2046_CS_PORT    GPIOF
#define XPT2046_CS_PIN     GPIO_PIN_10
#define XPT2046_CLK_PORT   GPIOG
#define XPT2046_CLK_PIN    GPIO_PIN_7
#define XPT2046_MOSI_PORT  GPIOF
#define XPT2046_MOSI_PIN   GPIO_PIN_11
#define XPT2046_MISO_PORT  GPIOF
#define XPT2046_MISO_PIN   GPIO_PIN_6

#define XPT2046_CMD_X      0x90U
#define XPT2046_CMD_Y      0xD0U
#define XPT2046_TOUCH_LEVEL GPIO_PIN_RESET

#define XPT2046_LCD_WIDTH  240U
#define XPT2046_LCD_HEIGHT 320U
#define XPT2046_SCAN_MODE  6U

typedef struct {
    float dX_X;
    float dX_Y;
    float dX;
    float dY_X;
    float dY_Y;
    float dY;
} XPT2046_TouchPara_t;

static const XPT2046_TouchPara_t s_touch_para[8] = {
    {-0.006464f, -0.073259f, 280.358032f,  0.074878f,  0.002052f,  -6.545977f},
    { 0.086314f,  0.001891f, -12.836658f, -0.003722f, -0.065799f, 254.715714f},
    { 0.002782f,  0.061522f, -11.595689f,  0.083393f,  0.005159f, -15.650089f},
    { 0.089743f, -0.000289f, -20.612209f, -0.001374f,  0.064451f, -16.054003f},
    { 0.000767f, -0.068258f, 250.891769f, -0.085559f, -0.000195f, 334.747650f},
    {-0.084744f,  0.000047f, 323.163147f, -0.002109f, -0.066371f, 260.985809f},
    {-0.001848f,  0.066984f, -12.807136f, -0.084858f, -0.000805f, 333.395386f},
    {-0.085470f, -0.000876f, 334.023163f, -0.003390f,  0.064725f,  -6.211169f},
};

static void XPT2046_PinWrite(GPIO_TypeDef *port, uint16_t pin, GPIO_PinState state)
{
    HAL_GPIO_WritePin(port, pin, state);
}

static GPIO_PinState XPT2046_PinRead(GPIO_TypeDef *port, uint16_t pin)
{
    return HAL_GPIO_ReadPin(port, pin);
}

static void XPT2046_DelayUS(__IO uint32_t count)
{
    uint32_t i;

    for (i = 0U; i < count; i++) {
        uint8_t delay = 12U;
        while (delay-- > 0U) {
        }
    }
}

static void XPT2046_WriteCMD(uint8_t cmd)
{
    uint8_t i;

    XPT2046_PinWrite(XPT2046_MOSI_PORT, XPT2046_MOSI_PIN, GPIO_PIN_RESET);
    XPT2046_PinWrite(XPT2046_CLK_PORT, XPT2046_CLK_PIN, GPIO_PIN_RESET);

    for (i = 0U; i < 8U; i++) {
        XPT2046_PinWrite(
            XPT2046_MOSI_PORT,
            XPT2046_MOSI_PIN,
            ((cmd >> (7U - i)) & 0x01U) != 0U ? GPIO_PIN_SET : GPIO_PIN_RESET);
        XPT2046_DelayUS(5U);
        XPT2046_PinWrite(XPT2046_CLK_PORT, XPT2046_CLK_PIN, GPIO_PIN_SET);
        XPT2046_DelayUS(5U);
        XPT2046_PinWrite(XPT2046_CLK_PORT, XPT2046_CLK_PIN, GPIO_PIN_RESET);
    }
}

static uint16_t XPT2046_ReadCMD(void)
{
    uint8_t i;
    uint16_t buffer = 0U;

    XPT2046_PinWrite(XPT2046_MOSI_PORT, XPT2046_MOSI_PIN, GPIO_PIN_RESET);
    XPT2046_PinWrite(XPT2046_CLK_PORT, XPT2046_CLK_PIN, GPIO_PIN_SET);

    for (i = 0U; i < 12U; i++) {
        XPT2046_PinWrite(XPT2046_CLK_PORT, XPT2046_CLK_PIN, GPIO_PIN_RESET);
        if (XPT2046_PinRead(XPT2046_MISO_PORT, XPT2046_MISO_PIN) == GPIO_PIN_SET) {
            buffer |= (uint16_t)(1U << (11U - i));
        }
        XPT2046_PinWrite(XPT2046_CLK_PORT, XPT2046_CLK_PIN, GPIO_PIN_SET);
    }

    return buffer;
}

static uint16_t XPT2046_ReadAdc(uint8_t channel)
{
    XPT2046_WriteCMD(channel);
    return XPT2046_ReadCMD();
}

static void XPT2046_ReadAdcXY(int16_t *x, int16_t *y)
{
    int16_t x_value;
    int16_t y_value;

    x_value = (int16_t)XPT2046_ReadAdc(XPT2046_CMD_X);
    XPT2046_DelayUS(1U);
    y_value = (int16_t)XPT2046_ReadAdc(XPT2046_CMD_Y);

    *x = x_value;
    *y = y_value;
}

static uint8_t XPT2046_ReadAdcSmoothXY(int16_t *x, int16_t *y)
{
    uint8_t count = 0U;
    uint8_t i;
    int16_t raw_x;
    int16_t raw_y;
    int16_t samples_x[10] = {0};
    int16_t samples_y[10] = {0};
    int32_t min_x;
    int32_t max_x;
    int32_t min_y;
    int32_t max_y;
    int32_t sum_x;
    int32_t sum_y;

    do {
        XPT2046_ReadAdcXY(&raw_x, &raw_y);
        samples_x[count] = raw_x;
        samples_y[count] = raw_y;
        count++;
    } while ((XPT2046_IsPressed() != 0U) && (count < 10U));

    if (count != 10U) {
        return 0U;
    }

    min_x = max_x = samples_x[0];
    min_y = max_y = samples_y[0];
    sum_x = 0;
    sum_y = 0;

    for (i = 0U; i < 10U; i++) {
        if (samples_x[i] < min_x) {
            min_x = samples_x[i];
        } else if (samples_x[i] > max_x) {
            max_x = samples_x[i];
        }

        if (samples_y[i] < min_y) {
            min_y = samples_y[i];
        } else if (samples_y[i] > max_y) {
            max_y = samples_y[i];
        }

        sum_x += samples_x[i];
        sum_y += samples_y[i];
    }

    *x = (int16_t)((sum_x - min_x - max_x) >> 3);
    *y = (int16_t)((sum_y - min_y - max_y) >> 3);

    return 1U;
}

static uint8_t XPT2046_TransformCoordinate(int16_t raw_x, int16_t raw_y,
                                           uint16_t *x, uint16_t *y)
{
    float mapped_x;
    float mapped_y;
    const XPT2046_TouchPara_t *para;

    para = &s_touch_para[XPT2046_SCAN_MODE];
    mapped_x = (para->dX_X * (float)raw_x) + (para->dX_Y * (float)raw_y) + para->dX;
    mapped_y = (para->dY_X * (float)raw_x) + (para->dY_Y * (float)raw_y) + para->dY;

    if (mapped_x < 0.0f) {
        mapped_x = 0.0f;
    } else if (mapped_x >= (float)XPT2046_LCD_WIDTH) {
        mapped_x = (float)(XPT2046_LCD_WIDTH - 1U);
    }

    if (mapped_y < 0.0f) {
        mapped_y = 0.0f;
    } else if (mapped_y >= (float)XPT2046_LCD_HEIGHT) {
        mapped_y = (float)(XPT2046_LCD_HEIGHT - 1U);
    }

    *x = (uint16_t)mapped_x;
    *y = (uint16_t)mapped_y;

    return 1U;
}

void XPT2046_Init(void)
{
    GPIO_InitTypeDef gpio_init = {0};

    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    gpio_init.Pin = XPT2046_CLK_PIN;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(XPT2046_CLK_PORT, &gpio_init);

    gpio_init.Pin = XPT2046_MOSI_PIN;
    HAL_GPIO_Init(XPT2046_MOSI_PORT, &gpio_init);

    gpio_init.Pin = XPT2046_CS_PIN;
    HAL_GPIO_Init(XPT2046_CS_PORT, &gpio_init);

    gpio_init.Pin = XPT2046_MISO_PIN;
    gpio_init.Mode = GPIO_MODE_INPUT;
    gpio_init.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(XPT2046_MISO_PORT, &gpio_init);

    gpio_init.Pin = XPT2046_IRQ_PIN;
    gpio_init.Mode = GPIO_MODE_INPUT;
    gpio_init.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(XPT2046_IRQ_PORT, &gpio_init);

    XPT2046_PinWrite(XPT2046_CS_PORT, XPT2046_CS_PIN, GPIO_PIN_RESET);
    XPT2046_PinWrite(XPT2046_CLK_PORT, XPT2046_CLK_PIN, GPIO_PIN_SET);
    XPT2046_PinWrite(XPT2046_MOSI_PORT, XPT2046_MOSI_PIN, GPIO_PIN_RESET);
}

uint8_t XPT2046_IsPressed(void)
{
    return (uint8_t)(XPT2046_PinRead(XPT2046_IRQ_PORT, XPT2046_IRQ_PIN) == XPT2046_TOUCH_LEVEL);
}

uint8_t XPT2046_Read(XPT2046_State_t *state)
{
    int16_t raw_x;
    int16_t raw_y;
    uint16_t mapped_x;
    uint16_t mapped_y;

    if (state == NULL) {
        return 0U;
    }

    state->pressed = 0U;

    if (!XPT2046_IsPressed()) {
        return 0U;
    }

    if (!XPT2046_ReadAdcSmoothXY(&raw_x, &raw_y)) {
        return 0U;
    }

    XPT2046_TransformCoordinate(raw_x, raw_y, &mapped_x, &mapped_y);

    state->raw_x = (uint16_t)raw_x;
    state->raw_y = (uint16_t)raw_y;
    state->x = mapped_x;
    state->y = mapped_y;
    state->pressed = 1U;

    return 1U;
}
