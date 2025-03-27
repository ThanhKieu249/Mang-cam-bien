#include "stm32f10x.h"                 
#include "stm32f10x_spi.h"           
#include "stm32f10x_rcc.h"             
#include "spi.h"
#include "uart.h"

#define RFID_CS_LOW() GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define RFID_CS_HIGH() GPIO_SetBits(GPIOB, GPIO_Pin_12)

#define PCD_IDLE							0x00   
#define PCD_AUTHENT						0x0E   
#define PCD_RECEIVE						0x08   
#define PCD_TRANSMIT					0x04   
#define PCD_TRANSCEIVE				0x0C   
#define PCD_RESETPHASE				0x0F  
#define PCD_CALCCRC						0x03   

#define PICC_REQIDL						0x26   
#define PICC_REQALL						0x52   
#define PICC_ANTICOLL					0x93   
#define PICC_SElECTTAG				0x93   
#define PICC_AUTHENT1A				0x60   
#define PICC_AUTHENT1B				0x61   
#define PICC_READ						  0x30   
#define PICC_WRITE						0xA0   
#define PICC_DECREMENT				0xC0   
#define PICC_INCREMENT				0xC1   
#define PICC_RESTORE					0xC2   
#define PICC_TRANSFER					0xB0  
#define PICC_HALT						  0x50  

#define MFRC522_REG_RESERVED00		0x00    
#define MFRC522_REG_COMMAND				0x01    
#define MFRC522_REG_COMM_IE_N			0x02    
#define MFRC522_REG_DIV1_EN				0x03    
#define MFRC522_REG_COMM_IRQ			0x04    
#define MFRC522_REG_DIV_IRQ				0x05
#define MFRC522_REG_ERROR					0x06    
#define MFRC522_REG_STATUS1				0x07    
#define MFRC522_REG_STATUS2				0x08    
#define MFRC522_REG_FIFO_DATA			0x09
#define MFRC522_REG_FIFO_LEVEL		0x0A
#define MFRC522_REG_WATER_LEVEL		0x0B
#define MFRC522_REG_CONTROL				0x0C
#define MFRC522_REG_BIT_FRAMING		0x0D
#define MFRC522_REG_COLL				  0x0E
#define MFRC522_REG_RESERVED01		0x0F

#define MFRC522_REG_RESERVED10		0x10
#define MFRC522_REG_MODE					0x11
#define MFRC522_REG_TX_MODE				0x12
#define MFRC522_REG_RX_MODE				0x13
#define MFRC522_REG_TX_CONTROL		0x14
#define MFRC522_REG_TX_AUTO				0x15
#define MFRC522_REG_TX_SELL				0x16
#define MFRC522_REG_RX_SELL				0x17
#define MFRC522_REG_RX_THRESHOLD	0x18
#define MFRC522_REG_DEMOD				  0x19
#define MFRC522_REG_RESERVED11		0x1A
#define MFRC522_REG_RESERVED12		0x1B
#define MFRC522_REG_MIFARE				0x1C
#define MFRC522_REG_RESERVED13		0x1D
#define MFRC522_REG_RESERVED14		0x1E
#define MFRC522_REG_SERIALSPEED		0x1F
 
#define MFRC522_REG_RESERVED20				0x20  
#define MFRC522_REG_CRC_RESULT_M			0x21
#define MFRC522_REG_CRC_RESULT_L			0x22
#define MFRC522_REG_RESERVED21				0x23
#define MFRC522_REG_MOD_WIDTH			 		0x24
#define MFRC522_REG_RESERVED22				0x25
#define MFRC522_REG_RF_CFG				 		0x26
#define MFRC522_REG_GS_N				  	  0x27
#define MFRC522_REG_CWGS_PREG			 		0x28
#define MFRC522_REG__MODGS_PREG				0x29
#define MFRC522_REG_T_MODE					  0x2A
#define MFRC522_REG_T_PRESCALER				0x2B
#define MFRC522_REG_T_RELOAD_H				0x2C
#define MFRC522_REG_T_RELOAD_L				0x2D
#define MFRC522_REG_T_COUNTER_VALUE_H	0x2E
#define MFRC522_REG_T_COUNTER_VALUE_L	0x2F

#define MFRC522_REG_RESERVED30				0x30
#define MFRC522_REG_TEST_SEL1					0x31
#define MFRC522_REG_TEST_SEL2					0x32
#define MFRC522_REG_TEST_PIN_EN				0x33
#define MFRC522_REG_TEST_PIN_VALUE		0x34
#define MFRC522_REG_TEST_BUS					0x35
#define MFRC522_REG_AUTO_TEST					0x36
#define MFRC522_REG_VERSION						0x37
#define MFRC522_REG_ANALOG_TEST				0x38
#define MFRC522_REG_TEST_ADC1					0x39  
#define MFRC522_REG_TEST_ADC2					0x3A   
#define MFRC522_REG_TEST_ADC0					0x3B   
#define MFRC522_REG_RESERVED31				0x3C   
#define MFRC522_REG_RESERVED32				0x3D
#define MFRC522_REG_RESERVED33				0x3E   
#define MFRC522_REG_RESERVED34				0x3F

#define MFRC522_DUMMY					0x00

#define MFRC522_MAX_LEN					16


void       PinCS_Init(void);
void       RFID_Init(void);

uint8_t    RFID_SendByte(uint8_t byte);
uint8_t    RFID_ReceiveByte(void);

uint8_t    RFID_ReadReg(uint8_t reg);
void       RFID_WriteReg(uint8_t reg, uint8_t value);

void       RFID_Read(uint8_t *buffer, uint8_t length);
void       RFID_Write(uint8_t *buffer, uint8_t length);

void       RFID_SetBitMask(uint8_t reg, uint8_t mask);
void       RFID_ClearBitMask(uint8_t reg, uint8_t mask);

void       RFID_AntennaOn(void);
void       RFID_AntennaOff(void);

uint8_t    RFID_ToCard(uint8_t command, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, uint8_t *backLen);
uint8_t    RFID_Anticoll(uint8_t *serNum);
uint8_t    RFID_Request(uint8_t reqMode, uint8_t *TagType);

void       RFID_Halt(void);
void	     RFID_Debug_ReadCard(void);

