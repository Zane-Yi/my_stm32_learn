#include "Init.h"
#include "stm32f1xx_hal.h"
#include "USART.h"
#include "KEY.h"
#include "BEEP.h"
#include "PWM.h"
#include "LED.h"
#include "Button_Manager.h"
#include "breathe_led.h"
#include "Task_Scheduler.h"

void All_Init (void)
{
    HAL_Init();
    LED_GPIO_Init();
    KEY_GPIO_Init();
    BEEP_GPIO_Init();
    USART_Config();
    PWM_Init();
    BreatheLed_Init();
    ButtonManager_Init();
    Task_Scheduler_Init();
}