#include "stm32f10x.h"
#include "dht11.h"
#include "timer.h"
#include "uart.h"
#include "stdio.h"

int main() {
    UART_Init();
    timer2_Init();
		char buffer[50];
		uint8_t humidity, temperature;
    while (1) {
				DHT11_Init(&temperature,&humidity);
				sprintf(buffer, "Temperature: %d*C, Humidity: %d%%\n", temperature, humidity);
        UART_SendString(buffer);
        Delay_1ms(1000);
    }
}
