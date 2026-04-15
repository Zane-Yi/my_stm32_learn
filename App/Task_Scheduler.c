#include "stm32f1xx_hal.h"
#include "KEY.h"
#include "LED.h"
#include "BEEP.h"
#include "USART.h"
#include "PWM.h"
#include "Task_Scheduler.h"

static uint8_t led_enabled = 0;

PWM_Device_t* Led_PWM_Toggle[] = {&Red_Led, &Blue_Led, &Green_Led};

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
            led_enabled ++;
            if(led_enabled >= 2) led_enabled = 0;
            __HAL_TIM_SET_COMPARE (Led_PWM_Toggle[color]->timer,Led_PWM_Toggle[color]->channel,0);

        }
        if(KEY_Process(&KEY_led) == KEY_ON)
        {
            __HAL_TIM_SET_COMPARE (Led_PWM_Toggle[color]->timer,Led_PWM_Toggle[color]->channel,0);
            color ++;
            if(color >= 3) color = 0;
        }
    }


    if ((HAL_GetTick() - last_led_task) >= 10)
    {
        last_led_task = HAL_GetTick();
        if (led_enabled == 1)
        {
            PWM_Update(Led_PWM_Toggle[color]);
        }
        
    }
    

    if (is_beeping  && HAL_GetTick() - beep_start_time >= 50)
    {
        BEEP_OFF;
        is_beeping = 0;
    }
}
    
    


