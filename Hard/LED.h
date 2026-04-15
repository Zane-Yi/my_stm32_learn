#ifndef LED_H_
#define LED_H_

#include "stm32f1xx_hal.h"


#define LED_R_ON        do{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET);}while (0)
#define LED_R_OFF       do{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);}while (0)
#define LED_R_Toggle()    do{HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_5);}while (0)

#define LED_G_ON        do{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);}while (0)
#define LED_G_OFF       do{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);}while (0)
#define LED_G_Toggle()    do{HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);}while (0)

#define LED_B_ON        do{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);}while (0)
#define LED_B_OFF       do{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);}while (0)
#define LED_B_Toggle()  do{HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_1);}while (0)

void LED_R_Real_Toggle(void);
void LED_G_Real_Toggle(void);
void LED_B_Real_Toggle(void);



void LED_GPIO_Init (void);




#endif
