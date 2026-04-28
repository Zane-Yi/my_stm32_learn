#ifndef KEY_H_
#define KEY_H_

#include "stm32f1xx_hal.h"
#include "Button_Manager.h"

void KEY_GPIO_Init (void);

ButtonHandler_t* KEY_GetKey1 (void);
ButtonHandler_t* KEY_GetKey2 (void);




#endif
