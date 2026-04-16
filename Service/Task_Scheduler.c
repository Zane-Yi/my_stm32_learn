#include "stm32f1xx_hal.h"
#include "BEEP.h"
#include "USART.h"
#include "Task_Scheduler.h"
#include "breathe_led.h"
#include "Button_Manager.h"

static uint32_t beep_start_time = 0;
static uint32_t last_led_task = 0;
static uint32_t last_key_task = 0;
static uint8_t is_beeping = 0;
static uint8_t led_toggle = 0;
static uint8_t color_index = 0; 

static ButtonHandler_t key_beep_handler;
static ButtonHandler_t key_led_handler;

void OnKey_Beep_Pressed(void) {
    BEEP_ON;
    beep_start_time = HAL_GetTick();
    is_beeping = 1;

    led_toggle = !led_toggle;
    if (led_toggle) {
        BreatheLed_Enable();
    } else {
        BreatheLed_Disable();
    }
}

void OnKey_Led_Pressed(void) 
{
    color_index++;
    color_index %= LED_COLOR_MAX; 
    BreatheLed_SwichColor((LedColor_t)color_index);
}

void Task_Scheduler_Init(void) {
    ButtonManager_Init();
    BreatheLed_Init();

    ButtonManager_Register(&key_beep_handler, GPIOA, GPIO_PIN_0, GPIO_PIN_SET,
                          OnKey_Beep_Pressed, NULL);

    ButtonManager_Register(&key_led_handler, GPIOC, GPIO_PIN_13, GPIO_PIN_RESET,
                          OnKey_Led_Pressed, NULL);
}

void System_Task_Runner(void) {
    //按键扫描
    if ((HAL_GetTick() - last_key_task) >= 10) {
        last_key_task = HAL_GetTick();
        ButtonManager_Process(); 
    }

    if ((HAL_GetTick() - last_led_task) >= 10) {
        last_led_task = HAL_GetTick();
        Breathe_Update();
    }

    if (is_beeping && (HAL_GetTick() - beep_start_time >= 50)) {
        BEEP_OFF;
        is_beeping = 0;
    }
}