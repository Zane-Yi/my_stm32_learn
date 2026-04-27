#include "Init.h"
#include "stm32f1xx_hal.h"
#include "USART.h"
#include "KEY.h"
#include "BEEP.h"
#include "PWM.h"
#include "LED.h"
#include "Display.h"
#include "Button_Manager.h"
#include "breathe_led.h"
#include "Task_Scheduler.h"
#include "Touch_Paint_Page.h"

void All_Init (void)
{
    HAL_Init();
    USART_Config();
    Display_Init();
    LED_GPIO_Init();
    KEY_GPIO_Init();
    BEEP_GPIO_Init();
    /* PWM_Init(); */
    TouchPaintPage_Init();
    Task_Scheduler_Init();
}
