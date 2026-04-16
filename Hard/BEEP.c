#include "BEEP.h"


void BEEP_GPIO_Init (void){

    GPIO_InitTypeDef BEEP_GPIO_Init;

    __HAL_RCC_GPIOC_CLK_ENABLE();

    BEEP_GPIO_Init.Mode  = GPIO_MODE_OUTPUT_PP;
    BEEP_GPIO_Init.Pin   = GPIO_PIN_0;
    BEEP_GPIO_Init.Pull  = GPIO_PULLDOWN;
    BEEP_GPIO_Init.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOC,&BEEP_GPIO_Init);

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);

}

