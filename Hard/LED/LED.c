#include "LED.h"


void LED_GPIO_Init (void){

    GPIO_InitTypeDef LED_GPIO_Init;

    __HAL_RCC_GPIOB_CLK_ENABLE();

    LED_GPIO_Init.Mode  = GPIO_MODE_OUTPUT_PP;
    LED_GPIO_Init.Pin   = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_5;
    LED_GPIO_Init.Pull  = GPIO_NOPULL;
    LED_GPIO_Init.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOB,&LED_GPIO_Init);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_5, GPIO_PIN_SET);

}

void LED_R_Real_Toggle(void) {
    LED_R_Toggle();
}

void LED_G_Real_Toggle(void) {
    LED_G_Toggle();
}

void LED_B_Real_Toggle(void) {
    LED_B_Toggle();
}
