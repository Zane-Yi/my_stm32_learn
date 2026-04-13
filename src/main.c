#include "stm32f1xx_hal.h"
#include "LED.h"
#include "KEY.h"
#include "BEEP.h"
#include "USART.h"
#include "Task_Scheduler.h"


int main (void)
{    
    HAL_Init();
    LED_GPIO_Init();
    KEY_GPIO_Init();
    BEEP_GPIO_Init();
    USART_Config();


    while (1)
    {
        System_Task_Runner();
    }
}
