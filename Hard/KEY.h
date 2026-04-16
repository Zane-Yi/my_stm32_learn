#ifndef KEY_H_
#define KEY_H_

#include "stm32f1xx_hal.h"

#define KEY_ON      1
#define KEY_OFF     0

typedef struct{
    GPIO_TypeDef* port;
    uint16_t pin;
    uint8_t last_state;
    uint8_t active_level;
}Botton_t;


void KEY_GPIO_Init (void);





#endif
