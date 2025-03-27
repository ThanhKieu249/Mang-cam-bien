#include "tim2.h"

void Timer2_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseInitTypeDef timerInit;
    timerInit.TIM_CounterMode = TIM_CounterMode_Up;
    timerInit.TIM_Period = 0xFFFF;
    timerInit.TIM_Prescaler = 72 - 1;
    TIM_TimeBaseInit(TIM2, &timerInit);
    TIM_Cmd(TIM2, ENABLE);
}

void Delay_ms(uint32_t time_ms)
{
    for (uint32_t i = 0; i < time_ms; i++)
    {
        TIM_SetCounter(TIM2, 0);
        while (TIM_GetCounter(TIM2) < 1000);
    }
}