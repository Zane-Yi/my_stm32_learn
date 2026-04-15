#ifndef __PWM_H
#define __PWM_H

#include "stm32f1xx_hal.h"
#include <stdio.h>

typedef struct{
    TIM_HandleTypeDef* timer;
    GPIO_TypeDef* port;
    uint16_t pin;
    uint32_t channel;
    uint16_t limit;
    uint16_t current;
    int16_t speed;
}PWM_Device_t;

void PWM_Init(void);
void PWM_Update(PWM_Device_t* device);

#endif