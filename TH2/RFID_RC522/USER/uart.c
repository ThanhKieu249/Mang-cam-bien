#include "uart.h"


void USART1_Init(uint32_t baud_rate)
{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	
    GPIO_InitTypeDef GPIO_InitStruct; 
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = baud_rate;  
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    
    USART_Init(USART1, &USART_InitStruct);
    USART_Cmd(USART1, ENABLE);
}

void USART1_Send_Char(char chr)
{
   while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
   USART_SendData(USART1, chr);
}

void USART1_Send_String(char* str)
{
    while (*str)
    {
        USART1_Send_Char(*str++);
    }
}

void USART1_Send_Data(uint8_t* data, uint8_t length)
{
    for (int i = 0; i < length; i++) {
        while( !(USART1->SR & ( 1 << 7)));
        USART1->DR = data[i];
    }
}

void USART1_Send_Number(int16_t num)
{
    if (num < 0) {
        USART1_Send_Char('-');
        num = -num;
    }
    uint8_t length = 0;
    uint8_t temp[10];
    if (num == 0) {
        USART1_Send_Char('0');
        return;
    } else {
        while (num != 0) {
            uint8_t value = num % 10;
            temp[length++] = value + '0';
            num /= 10;
        }
        for (int i = length - 1; i >= 0; i--) {
            USART1_Send_Char(temp[i]);
        }
    }
}

void USART1_Send_Float(float num)
{
    if (num < 0) {
        USART1_Send_Char('-');
        num = -num;
    }
    int16_t integer = (int16_t)num;
    float decimal = num - integer;
    USART1_Send_Number(integer);
    USART1_Send_Char('.');
    decimal *= 1000;
    USART1_Send_Number((int16_t)decimal);
}

void USART1_Send_Hex(uint8_t num)
{
    uint8_t temp;
    temp = num >> 4;
    if(temp > 9) {
        temp += 0x37;
    } else {
        temp += 0x30;
    }
    USART1_Send_Char(temp);
    temp = num & 0x0F;
    if(temp > 9) {
        temp += 0x37;
    } else {
        temp += 0x30;
    }
    USART1_Send_Char(temp);
}
