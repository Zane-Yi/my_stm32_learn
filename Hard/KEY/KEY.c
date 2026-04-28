#include "KEY.h"

void KEY_GPIO_Init (void)
{

    GPIO_InitTypeDef KEY_GPIO_Init;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();


    KEY_GPIO_Init.Mode  = GPIO_MODE_INPUT;
    KEY_GPIO_Init.Pin   = GPIO_PIN_0;
    KEY_GPIO_Init.Pull  = GPIO_PULLDOWN;
    KEY_GPIO_Init.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOA,&KEY_GPIO_Init);

    KEY_GPIO_Init.Pin   = GPIO_PIN_13;
    KEY_GPIO_Init.Pull  = GPIO_PULLUP;

    HAL_GPIO_Init(GPIOC,&KEY_GPIO_Init);

}
static ButtonHandler_t  g_key1 = {
    .port = GPIOA,
    .pin = GPIO_PIN_0,
    .active_level = GPIO_PIN_SET,

};

static ButtonHandler_t  g_key2 = {
    .port = GPIOC,
    .pin = GPIO_PIN_13,
    .active_level = GPIO_PIN_RESET,

};

ButtonHandler_t* KEY_GetKey1 (void) {
    return &g_key1;
}

ButtonHandler_t* KEY_GetKey2 (void) {
    return &g_key2;
}