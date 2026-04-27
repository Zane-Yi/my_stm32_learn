#ifndef __USART_H
#define __USART_H

#define TX_BUF_SIZE 256
#define RX_BUF_SIZE 256

#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <stdbool.h>

#define USARTx                  USART1
#define USART_GPIO_PORT         GPIOA
#define USART_TX_GPIO_PIN       GPIO_PIN_9
#define USART_RX_GPIO_PIN       GPIO_PIN_10
#define USART_CLK               RCC_APB1Periph_USART1
#define USART_IRQn              USART1_IRQn
#define USART_IRQHandler        USART1_IRQHandler

void USART_Config(void);
bool DRV_USART_GetByte(uint8_t *byte);

#endif