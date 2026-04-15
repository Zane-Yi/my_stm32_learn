#ifndef KEY_EVENT_H_
#define KEY_EVENT_H_

#include "stm32f1xx_hal.h"

typedef enum {
    KEY_EVENT_NONE = 0,
    KEY_EVENT_PRESSED,
    KEY_EVENT_RELEASED
} KeyEvent_t;

typedef void (*KeyEventCallback_t)(void* user_data);

typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
    uint8_t active_level;
    uint8_t last_state;
    KeyEventCallback_t on_pressed;
    KeyEventCallback_t on_released;
    void* user_data;
} KeyEventHandler_t;

void KeyEvent_Init(void);
void KeyEvent_Register(KeyEventHandler_t* handler, GPIO_TypeDef* port, uint16_t pin,
                       uint8_t active_level, KeyEventCallback_t on_pressed,
                       KeyEventCallback_t on_released, void* user_data);
void KeyEvent_Process(void);

#endif
