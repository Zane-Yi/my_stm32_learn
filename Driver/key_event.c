#include "key_event.h"
#include <stddef.h>

#define MAX_KEY_HANDLERS 8

static KeyEventHandler_t* key_handlers[MAX_KEY_HANDLERS];
static uint8_t handler_count = 0;

void KeyEvent_Init(void) {
    handler_count = 0;
    for (uint8_t i = 0; i < MAX_KEY_HANDLERS; i++) {
        key_handlers[i] = NULL;
    }
}

void KeyEvent_Register(KeyEventHandler_t* handler, GPIO_TypeDef* port, uint16_t pin,
                       uint8_t active_level, KeyEventCallback_t on_pressed,
                       KeyEventCallback_t on_released, void* user_data) {
    if (handler_count >= MAX_KEY_HANDLERS) return;

    handler->port = port;
    handler->pin = pin;
    handler->active_level = active_level;
    handler->last_state = 0;
    handler->on_pressed = on_pressed;
    handler->on_released = on_released;
    handler->user_data = user_data;

    key_handlers[handler_count++] = handler;
}

void KeyEvent_Process(void) {
    for (uint8_t i = 0; i < handler_count; i++) {
        KeyEventHandler_t* handler = key_handlers[i];
        if (handler == NULL) continue;

        GPIO_PinState pin_state = HAL_GPIO_ReadPin(handler->port, handler->pin);
        uint8_t current_state = (pin_state == handler->active_level) ? 1 : 0;

        if (handler->last_state == 0 && current_state == 1) {
            if (handler->on_pressed != NULL) {
                handler->on_pressed(handler->user_data);
            }
        } else if (handler->last_state == 1 && current_state == 0) {
            if (handler->on_released != NULL) {
                handler->on_released(handler->user_data);
            }
        }

        handler->last_state = current_state;
    }
}
