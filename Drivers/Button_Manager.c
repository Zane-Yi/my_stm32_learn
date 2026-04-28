#include "Button_Manager.h"
#include <stddef.h>

#define BUTTON_DEBOUNCE_MS 20U

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
                            ButtonCallback_t on_released) 
{
    GPIO_PinState pin_state;
    uint8_t current_state;

    if (registered_count >= MAX_BUTTON_HANDLERS) return;

    handler->port = port;
    handler->pin = pin;
    handler->active_level = active_level;
    pin_state = HAL_GPIO_ReadPin(port, pin);
    current_state = (pin_state == active_level) ? 1U : 0U;
    handler->last_state = current_state;
    handler->last_raw_state = current_state;
    handler->last_change_tick = HAL_GetTick();
    handler->on_pressed = on_pressed;
    handler->on_released = on_released;

    button_list[registered_count++] = handler;
}
void ButtonManager_Process(void) 
{
    for (uint8_t i = 0; i < registered_count; i++) 
    {
        ButtonHandler_t* handler = button_list[i];
        uint32_t now;
        if (handler == NULL) continue;

        GPIO_PinState pin_state = HAL_GPIO_ReadPin(handler->port, handler->pin);
        uint8_t current_state = (pin_state == handler->active_level) ? 1U : 0U;
        now = HAL_GetTick();

        if (current_state != handler->last_raw_state) {
            handler->last_raw_state = current_state;
            handler->last_change_tick = now;
            continue;
        }

        if ((current_state != handler->last_state) &&
            ((now - handler->last_change_tick) >= BUTTON_DEBOUNCE_MS)) {
            if ((handler->last_state == 0U) && (current_state == 1U)) {
                if (handler->on_pressed != NULL) {
                    handler->on_pressed();
                }
            } else if ((handler->last_state == 1U) && (current_state == 0U)) {
                if (handler->on_released != NULL) {
                    handler->on_released();
                }
            }

            handler->last_state = current_state;
        }
    }
}
    
