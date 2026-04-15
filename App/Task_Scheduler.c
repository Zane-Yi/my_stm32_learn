#include "stm32f1xx_hal.h"
#include "BEEP.h"
#include "USART.h"
#include "Task_Scheduler.h"
#include "breathing_led.h"
#include "key_event.h"

static uint32_t beep_start_time = 0;
static uint32_t last_led_task = 0;
static uint32_t last_key_task = 0;
static uint8_t is_beeping = 0;

static KeyEventHandler_t key_beep_handler;
static KeyEventHandler_t key_led_handler;

void OnKey_Beep_Pressed(void* user_data) {
    (void)user_data;

    BEEP_ON;
    beep_start_time = HAL_GetTick();
    is_beeping = 1;
    printf("Beep ON Tick:%lu\r\n", HAL_GetTick());

    static uint8_t toggle = 0;
    toggle = !toggle;
    if (toggle) {
        BreathingLed_Enable();
    } else {
        BreathingLed_Disable();
    }
}

void OnKey_Led_Pressed(void* user_data) {
    (void)user_data;

    LedColor_t current = BreathingLed_GetColor();
    current = (current + 1) % LED_COLOR_MAX;
    BreathingLed_SetColor(current);
}

void Task_Scheduler_Init(void) {
    KeyEvent_Init();
    BreathingLed_Init();

    KeyEvent_Register(&key_beep_handler, GPIOA, GPIO_PIN_0, GPIO_PIN_SET,
                      OnKey_Beep_Pressed, NULL, NULL);

    KeyEvent_Register(&key_led_handler, GPIOC, GPIO_PIN_13, GPIO_PIN_RESET,
                      OnKey_Led_Pressed, NULL, NULL);
}

void System_Task_Runner(void) {
    if ((HAL_GetTick() - last_key_task) >= 10) {
        last_key_task = HAL_GetTick();
        KeyEvent_Process();
    }

    if ((HAL_GetTick() - last_led_task) >= 10) {
        last_led_task = HAL_GetTick();
        BreathingLed_Update();
    }

    if (is_beeping && HAL_GetTick() - beep_start_time >= 50) {
        BEEP_OFF;
        is_beeping = 0;
    }
}
    
    


