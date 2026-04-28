#ifndef BUTTON_MANAGER_H
#define BUTTON_MANAGER_H

#include "stm32f1xx_hal.h"

#define MAX_BUTTON_HANDLERS 8

typedef enum {
    BUTTON_EVENT_NONE = 0,
    BUTTON_EVENT_PRESSED,
    BUTTON_EVENT_RELEASED
} ButtonEvent_t;

typedef void (*ButtonCallback_t)(void);

typedef struct {
    GPIO_TypeDef* port;            
    uint16_t pin;                 
    uint8_t active_level;         
    uint8_t last_state;           
    uint8_t last_raw_state;       
    uint32_t last_change_tick;    
    ButtonCallback_t on_pressed;  
    ButtonCallback_t on_released; 
} ButtonHandler_t;

void ButtonManager_Init(void);
void ButtonManager_Register(ButtonHandler_t* handler, GPIO_TypeDef* port, uint16_t pin,
                           uint8_t active_level, ButtonCallback_t on_pressed,
                           ButtonCallback_t on_released);
void ButtonManager_Process(void);

#endif
