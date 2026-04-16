#ifndef __BREATHE_H
#define __BREATHE_H

#include "stm32f1xx_hal.h"
#include <stdio.h>

extern TIM_HandleTypeDef htim3;

#define LED_RED_PIN     GPIO_PIN_5
#define LED_GREEN_PIN   GPIO_PIN_0
#define LED_BLUE_PIN    GPIO_PIN_1
#define LED_ALL_PINS    (LED_RED_PIN | LED_GREEN_PIN | LED_BLUE_PIN)

typedef enum {
    LED_COLOR_RED = 0,
    LED_COLOR_GREEN,
    LED_COLOR_BLUE,
    LED_COLOR_MAX
} LedColor_t;

typedef struct{
    TIM_HandleTypeDef* timer;
    GPIO_TypeDef* port;
    uint16_t pin;
    uint32_t channel;
    uint16_t limit;
    uint16_t current;
    int16_t speed;
}BreatheLed_t;

void Breathe_Update (void);
void BreatheLed_Init(void);
void BreatheLed_Enable(void);
void BreatheLed_Disable(void);
void BreatheLed_SwichColor(LedColor_t color);

#endif
