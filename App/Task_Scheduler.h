#ifndef __TASK_CONFIG_H
#define __TASK_CONFIG_H

#include "stm32f1xx_hal.h"

extern PWM_Device_t Red_Led;
extern PWM_Device_t Green_Led;
extern PWM_Device_t Blue_Led;

void System_Task_Runner(void);
void Start_Beep_Task(void);
void led_toggle(void);

#endif
