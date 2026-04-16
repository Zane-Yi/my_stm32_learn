#include "Button_Manager.h"
#include <stddef.h>

static ButtonHandler_t* button_list[MAX_BUTTON_HANDLERS];
static uint8_t registered_count = 0;

void ButtonManager_Init(void) 
{
    registered_count = 0;
    for (uint8_t i = 0; i < MAX_BUTTON_HANDLERS; i++) 
    {
        button_list[i] = NULL;
    }
}
void ButtonManager_Register(ButtonHandler_t* handler, GPIO_TypeDef* port, uint16_t pin,
                           uint8_t active_level, ButtonCallback_t on_pressed,
                           ButtonCallback_t on_released) {
    if (registered_count >= MAX_BUTTON_HANDLERS) return;

    handler->port = port;
    handler->pin = pin;
    handler->active_level = active_level;
    handler->last_state = 0;
    handler->on_pressed = on_pressed;
    handler->on_released = on_released;

    button_list[registered_count++] = handler;
}
void ButtonManager_Process(void) {
    for (uint8_t i = 0; i < registered_count; i++) 
    {
        ButtonHandler_t* handler = button_list[i];
        if (handler == NULL) continue;

        GPIO_PinState pin_state = HAL_GPIO_ReadPin(handler->port, handler->pin);
        uint8_t current_state = (pin_state == handler->active_level) ? 1 : 0;

        if (handler->last_state == 0 && current_state == 1) {
            if (handler->on_pressed != NULL) {
                handler->on_pressed();
            }
        } 
        else if (handler->last_state == 1 && current_state == 0) {
            if (handler->on_released != NULL) {
                handler->on_released();
            }
        }

        handler->last_state = current_state;
    }
}
    