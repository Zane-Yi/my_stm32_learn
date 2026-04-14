#include "stm32f1xx_hal.h"
#include "KEY.h"
#include "LED.h"
#include "BEEP.h"
#include "USART.h"
#include "Task_Scheduler.h"

void (*LED_TOGGLE[]) (void) = {LED_R_Real_Toggle,LED_B_Real_Toggle,LED_G_Real_Toggle};


static  uint32_t    beep_start_time     = 0;
static  uint32_t    last_led_task       = 0;
static  uint32_t    last_key_task       = 0;
static  uint8_t     is_beeping          = 0;
static  uint8_t     color               = 0;


Botton_t KEY_beep = {GPIOA,GPIO_PIN_0,0,GPIO_PIN_SET};
Botton_t KEY_led = {GPIOC,GPIO_PIN_13,0, GPIO_PIN_RESET};


void Start_Beep_Task(void)
{

    BEEP_ON;
    beep_start_time = HAL_GetTick();
    is_beeping = 1;
    printf("Beep ON Tick:%lu\r\n", HAL_GetTick());
}


void System_Task_Runner(void)
{
    if ((HAL_GetTick() - last_key_task) >= 10)
    {
        last_key_task = HAL_GetTick();
        if(KEY_Process(&KEY_beep) == KEY_ON)
        {
            Start_Beep_Task();
        }
        if(KEY_Process(&KEY_led) == KEY_ON)
        {
            LED_R_OFF; LED_G_OFF; LED_B_OFF;
            color = (color + 1) % 3;
            LED_TOGGLE[color]();
            printf("Color change Tick:%lu\r\n", HAL_GetTick());
        }
    }
    if ((HAL_GetTick() - last_led_task) >= 500)
    {
        LED_TOGGLE[color]();
        last_led_task = HAL_GetTick();
    }

    if (is_beeping  && HAL_GetTick() - beep_start_time >= 50)
    {
        BEEP_OFF;
        is_beeping = 0;
    }
}
    
    


