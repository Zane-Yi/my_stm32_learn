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
    uint8_t KEY_Process(Botton_t* bin)
    {

        GPIO_PinState pin_state = HAL_GPIO_ReadPin(bin->port,bin->pin);

        uint8_t current_state = (pin_state == bin->active_level) ? 1:0;
        uint8_t result = KEY_OFF;

        if (bin->last_state == 0 && current_state == 1)
        {
            result = KEY_ON;
        }
        bin->last_state = current_state;
        return result;
        
    }
