#include "dht11.h"
#include "timer.h"
#include "stm32f10x.h"
void DHT11_Init(uint8_t *tem, uint8_t *hum) 
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef gpioInit;
    gpioInit.GPIO_Pin = GPIO_Pin_12;
    gpioInit.GPIO_Mode = GPIO_Mode_Out_OD;
    gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpioInit);

    GPIO_ResetBits(GPIOB, GPIO_Pin_12);
    Delay_1ms(20);
    GPIO_SetBits(GPIOB, GPIO_Pin_12);
    
		TIM_SetCounter(TIM2,0);
		while(TIM_GetCounter(TIM2) < 10){
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
				break;
			}
		}
		uint16_t u16Tim;
		uint8_t u8Buff[5];
		
		u16Tim = TIM_GetCounter(TIM2);
		if(u16Tim >= 10){
			while(1){
			}
		}
		
		TIM_SetCounter(TIM2,0);
		while(TIM_GetCounter(TIM2) < 45){
			if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
				break;
			}
		}
		
		u16Tim = TIM_GetCounter(TIM2);
		if((u16Tim >= 45)||(u16Tim <= 5)){
			while(1){
			}
		}
		
		TIM_SetCounter(TIM2,0);
		while(TIM_GetCounter(TIM2) < 90){
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
				break;
			}
		}
		
		u16Tim = TIM_GetCounter(TIM2);
		if((u16Tim >= 90)||(u16Tim <= 70)){
			while(1){
			}
		}
		
		TIM_SetCounter(TIM2,0);
		while(TIM_GetCounter(TIM2) < 95){
			if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
				break;
			}
		}
		
		u16Tim = TIM_GetCounter(TIM2);
		if((u16Tim >= 95)||(u16Tim <= 75)){
			while(1){
			}
		}
		//byte 1
		for (int i=0;i<8;++i){
			TIM_SetCounter(TIM2,0);
			while(TIM_GetCounter(TIM2) < 65){
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
				break;
				}
			}
		
			u16Tim = TIM_GetCounter(TIM2);
			if((u16Tim >= 65)||(u16Tim <= 45)){
			while(1){
				}
			}
			
			TIM_SetCounter(TIM2,0);
			while(TIM_GetCounter(TIM2) < 80){
			if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
				break;
				}
			}
		
			u16Tim = TIM_GetCounter(TIM2);
			if((u16Tim >= 80)||(u16Tim <= 10)){
			while(1){
				}
			}
			u8Buff[0] <<=1;
			if(u16Tim>45) u8Buff[0] |= 1;
			else u8Buff[0] &= ~1;
		}
			
		//byte 2
		for (int i=0;i<8;++i){
			TIM_SetCounter(TIM2,0);
			while(TIM_GetCounter(TIM2) < 65){
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
				break;
				}
			}
		
			u16Tim = TIM_GetCounter(TIM2);
			if((u16Tim >= 65)||(u16Tim <= 45)){
			while(1){
				}
			}
			
			TIM_SetCounter(TIM2,0);
			while(TIM_GetCounter(TIM2) < 80){
			if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
				break;
				}
			}
		
			u16Tim = TIM_GetCounter(TIM2);
			if((u16Tim >= 80)||(u16Tim <= 10)){
			while(1){
				}
			}

			u8Buff[1] <<=1;
			if(u16Tim>45) u8Buff[1] |= 1;
			else u8Buff[1] &= ~1;
		}
		//byte 3
		for (int i=0;i<8;++i){
			TIM_SetCounter(TIM2,0);
			while(TIM_GetCounter(TIM2) < 65){
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
				break;
				}
			}
		
			u16Tim = TIM_GetCounter(TIM2);
			if((u16Tim >= 65)||(u16Tim <= 45)){
			while(1){
				}
			}
			
			TIM_SetCounter(TIM2,0);
			while(TIM_GetCounter(TIM2) < 80){
			if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
				break;
				}
			}
		
			u16Tim = TIM_GetCounter(TIM2);
			if((u16Tim >= 80)||(u16Tim <= 10)){
			while(1){
				}
			}

			u8Buff[2] <<=1;
			if(u16Tim>45) u8Buff[2] |= 1;
			else u8Buff[2] &= ~1;
		} 
			//byte 4
		for (int i=0;i<8;++i){
			TIM_SetCounter(TIM2,0);
			while(TIM_GetCounter(TIM2) < 65){
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
				break;
				}
			}
		
			u16Tim = TIM_GetCounter(TIM2);
			if((u16Tim >= 65)||(u16Tim <= 45)){
			while(1){
				}
			}
			
			TIM_SetCounter(TIM2,0);
			while(TIM_GetCounter(TIM2) < 80){
			if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
				break;
				}
			}
		
			u16Tim = TIM_GetCounter(TIM2);
			if((u16Tim >= 80)||(u16Tim <= 10)){
			while(1){
				}
			}

			u8Buff[3] <<=1;
			if(u16Tim>45) u8Buff[3] |= 1;
			else u8Buff[3] &= ~1;
		}
		//byte 5
		for (int i=0;i<8;++i){
			TIM_SetCounter(TIM2,0);
			while(TIM_GetCounter(TIM2) < 65){
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
				break;
				}
			}
		
			u16Tim = TIM_GetCounter(TIM2);
			if((u16Tim >= 65)||(u16Tim <= 45)){
			while(1){
				}
			}
			
			TIM_SetCounter(TIM2,0);
			while(TIM_GetCounter(TIM2) < 80){
			if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){
				break;
				}
			}
		
			u16Tim = TIM_GetCounter(TIM2);
			if((u16Tim >= 80)||(u16Tim <= 10)){
			while(1){
				}
			}

			u8Buff[4] <<=1;
			if(u16Tim>45) u8Buff[4] |= 1;
			else u8Buff[4] &= ~1;
		}
		uint8_t u8CheckSum = u8Buff[0] + u8Buff[1] + u8Buff[2] + u8Buff[3];
		if(u8CheckSum != u8Buff[4]) while(1){};
		*tem=u8Buff[2];
		*hum=u8Buff[0];
}
