#include "stm32f10x.h"
#include "app_i2c.h"
#include "bh1750.h"
#include "uart.h"
#include "timer.h"
#include "stdio.h"
int main(void)
{ 
		timer2_Init();
    UART_Init();
    BH1750_Init();

    while (1)
    {
      float lux = BH1750_Readlux();
			char buffer[32];
        sprintf(buffer, "Light BH1750: %.2f Lux\n", lux);
        UART_SendString(buffer);
        Delay_1ms(1000);
    }
}