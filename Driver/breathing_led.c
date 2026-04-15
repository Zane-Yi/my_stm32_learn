#include "breathing_led.h"
#include "PWM.h"

static BreathingLed_t leds[LED_COLOR_MAX];
static LedColor_t current_color = LED_COLOR_RED;
static uint8_t is_enabled = 0;

void BreathingLed_Init(void) {
    leds[LED_COLOR_RED].timer = &htim3;
    leds[LED_COLOR_RED].channel = TIM_CHANNEL_2;
    leds[LED_COLOR_RED].current = 0;
    leds[LED_COLOR_RED].speed = 10;
    leds[LED_COLOR_RED].limit = 1000;

    leds[LED_COLOR_GREEN].timer = &htim3;
    leds[LED_COLOR_GREEN].channel = TIM_CHANNEL_3;
    leds[LED_COLOR_GREEN].current = 0;
    leds[LED_COLOR_GREEN].speed = 10;
    leds[LED_COLOR_GREEN].limit = 1000;

    leds[LED_COLOR_BLUE].timer = &htim3;
    leds[LED_COLOR_BLUE].channel = TIM_CHANNEL_4;
    leds[LED_COLOR_BLUE].current = 0;
    leds[LED_COLOR_BLUE].speed = 10;
    leds[LED_COLOR_BLUE].limit = 1000;

    is_enabled = 0;
}

void BreathingLed_Enable(void) {
    is_enabled = 1;
}

void BreathingLed_Disable(void) {
    is_enabled = 0;
    __HAL_TIM_SET_COMPARE(leds[current_color].timer, leds[current_color].channel, 0);
}

void BreathingLed_SetColor(LedColor_t color) {
    if (color >= LED_COLOR_MAX) return;

    __HAL_TIM_SET_COMPARE(leds[current_color].timer, leds[current_color].channel, 0);
    current_color = color;
    leds[current_color].current = 0;
}

LedColor_t BreathingLed_GetColor(void) {
    return current_color;
}

void BreathingLed_Update(void) {
    if (!is_enabled) return;

    BreathingLed_t* led = &leds[current_color];

    led->current += led->speed;

    if (led->current >= led->limit || led->current <= 0) {
        led->speed = -led->speed;
    }

    if (led->current > led->limit) led->current = led->limit;
    if (led->current < 0) led->current = 0;

    __HAL_TIM_SET_COMPARE(led->timer, led->channel, led->current);
}
