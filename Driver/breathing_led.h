#ifndef BREATHING_LED_H_
#define BREATHING_LED_H_

#include "stm32f1xx_hal.h"

typedef enum {
    LED_COLOR_RED = 0,
    LED_COLOR_GREEN,
    LED_COLOR_BLUE,
    LED_COLOR_MAX
} LedColor_t;

typedef struct {
    TIM_HandleTypeDef* timer;
    uint32_t channel;
    int16_t current;
    int16_t speed;
    uint16_t limit;
} BreathingLed_t;

void BreathingLed_Init(void);
void BreathingLed_Enable(void);
void BreathingLed_Disable(void);
void BreathingLed_SetColor(LedColor_t color);
LedColor_t BreathingLed_GetColor(void);
void BreathingLed_Update(void);

#endif
