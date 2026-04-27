#include "stm32f1xx_hal.h"
#include "BEEP.h"
#include "USART.h"
#include "Task_Scheduler.h"
#include "Touch_Paint_Page.h"
#include "breathe_led.h"
#include "Button_Manager.h"

/* static uint32_t last_led_task = 0; */
static uint32_t last_key_task = 0;
static uint32_t last_ui_task = 0;

static ButtonHandler_t key_beep_handler;
static ButtonHandler_t key_led_handler;

void OnKey_Next_Pressed(void) 
{
    TouchPaintPage_HandleNext();
}

void OnKey_Toggle_Pressed(void) 
{
    TouchPaintPage_HandleConfirm();
}

void Task_Scheduler_Init(void) 
{
    ButtonManager_Init();
    /* BreatheLed_Init(); */

    ButtonManager_Register(&key_beep_handler, GPIOA, GPIO_PIN_0, GPIO_PIN_SET,
                          OnKey_Next_Pressed, NULL);

    ButtonManager_Register(&key_led_handler, GPIOC, GPIO_PIN_13, GPIO_PIN_RESET,
                          OnKey_Toggle_Pressed, NULL);
}

void System_Task_Runner(void) 
{
    //按键扫描
    if ((HAL_GetTick() - last_key_task) >= 10) {
        last_key_task = HAL_GetTick();
        ButtonManager_Process(); 
    }

    /*
    if ((HAL_GetTick() - last_led_task) >= 10) {
        last_led_task = HAL_GetTick();
        Breathe_Update();
    }
    */

    if ((HAL_GetTick() - last_ui_task) >= 20) {
        last_ui_task = HAL_GetTick();
        TouchPaintPage_Update();
    }
}
