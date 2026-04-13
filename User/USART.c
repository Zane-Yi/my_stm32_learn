#include "USART.h"
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>

#define TX_BUF_SIZE 256

struct{
    u_int8_t data[TX_BUF_SIZE];
    volatile u_int16_t head;
    volatile u_int16_t tail;
}tx_fifo = {{0},0,0};

volatile uint8_t is_busy = 0;

UART_HandleTypeDef huart1;

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(huart->Instance==USART1)
    {
        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
  
        GPIO_InitStruct.Pin = GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;//tx复用推挽输出
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;//rx浮空输入
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
}

void USART_Config(void) {
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B; 
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    
    HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);//中断优先级
    HAL_NVIC_EnableIRQ(USART1_IRQn);//开启中断

    HAL_UART_Init(&huart1);
}

int _write(int file, char *ptr, int len)
{
    for (int i = 0; i < len; i++)
    {
        u_int8_t next_head = ((tx_fifo.head + 1) % TX_BUF_SIZE);
        while (next_head == tx_fifo.tail);
        tx_fifo.data[tx_fifo.head] = ptr[i];
        tx_fifo.head = next_head;
    }
    __disable_irq();
    if (is_busy == 0)
    {
        is_busy = 1;
        u_int8_t byte = tx_fifo.data[tx_fifo.tail];
        tx_fifo.tail = (tx_fifo.tail +1) % TX_BUF_SIZE;
        HAL_UART_Transmit_IT(&huart1, &byte, 1);
    }
    __enable_irq();
    return len;

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        if (tx_fifo.head != tx_fifo.tail)
        {
            u_int8_t byte = tx_fifo.data[tx_fifo.tail];
            tx_fifo.tail = (tx_fifo.tail +1) % TX_BUF_SIZE;
            HAL_UART_Transmit_IT(&huart1, &byte, 1);
        }
        else
        {
            is_busy = 0;
        }
    }
}




















int _close(int file) { return -1; }
int _lseek(int file, int ptr, int dir) { return 0; }
int _read(int file, char *ptr, int len) { return 0; }
int _fstat(int file, struct stat *st) { st->st_mode = S_IFCHR; return 0; }
int _isatty(int file) { return 1; }