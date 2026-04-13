#ifndef BEEP_H_
#define BEEP_H_

#include "stm32f1xx_hal.h"



#define BEEP_OFF        do{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);}while (0)
#define BEEP_ON       do{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);}while (0)
#define BEEP_Toggle()  do{HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_0);}while (0)



void BEEP_GPIO_Init (void);




#endif
