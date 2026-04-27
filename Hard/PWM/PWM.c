#include "PWM.h"
#include "breathe_led.h"

TIM_HandleTypeDef htim3;


void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim) {
    if (htim->Instance == TIM3) {
        __HAL_RCC_TIM3_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_AFIO_CLK_ENABLE();

        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;         // 配置引脚为复用推挽输出 (AF_PP)
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Pin = LED_ALL_PINS;

        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        // 野火霸道 V2 PB5 接的是 TIM3_CH2，必须开启部分重映射
        __HAL_AFIO_REMAP_TIM3_PARTIAL();
    }
}

void PWM_Init(void) {
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 71;           // 72MHz / 72 = 1MHz
    htim3.Init.Period = 1000 - 1; // 计数到 999，即 1000 个周期
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    HAL_TIM_PWM_Init(&htim3);

    // PWM 通道配置
    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;                 // 初始亮度全灭
    sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
    
    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2);
    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3);
    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4);

    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);

}
