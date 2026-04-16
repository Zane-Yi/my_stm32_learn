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

