#include "rfid.h"

void PinCS_Init(void) 
{
  GPIO_InitTypeDef gpioInit;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	gpioInit.GPIO_Mode=GPIO_Mode_Out_PP;
	gpioInit.GPIO_Speed=GPIO_Speed_50MHz;
	gpioInit.GPIO_Pin=GPIO_Pin_12;
	GPIO_Init(GPIOB, &gpioInit);
  GPIO_SetBits(GPIOB, GPIO_Pin_12);
}

void RFID_WriteReg(uint8_t reg, uint8_t value)
{
    RFID_CS_LOW();
    RFID_SendByte((reg << 1) & 0x7E);
    RFID_SendByte(value);
    RFID_CS_HIGH();
}


uint8_t RFID_ReadReg(uint8_t reg)
{
    uint8_t result;
    RFID_CS_LOW();
    RFID_SendByte(((reg << 1) & 0x7E) | 0x80);
    result = SPI_SendByte(MFRC522_DUMMY);
    RFID_CS_HIGH();
    return result;
}

uint8_t RFID_ReceiveByte(void)
{
    return SPI_SendByte(MFRC522_DUMMY);
}

uint8_t RFID_SendByte(uint8_t byte)
{
    return SPI_SendByte(byte);
}

void RFID_Init(void)
{
    SPI_Config();
    PinCS_Init();
   
    RFID_WriteReg(MFRC522_REG_COMMAND, PCD_RESETPHASE);
  
    RFID_WriteReg(MFRC522_REG_T_MODE, 0x8D);
    RFID_WriteReg(MFRC522_REG_T_PRESCALER, 0x3E);
    RFID_WriteReg(MFRC522_REG_T_RELOAD_L, 30);
    RFID_WriteReg(MFRC522_REG_T_RELOAD_H, 0);

    RFID_WriteReg(MFRC522_REG_RF_CFG, 0x70);
    
		RFID_WriteReg(MFRC522_REG_TX_AUTO, 0x40);
		RFID_WriteReg(MFRC522_REG_MODE, 0x3D);

    RFID_AntennaOn();
}

void RFID_SetBitMask(uint8_t reg, uint8_t mask)
{
    RFID_WriteReg(reg, RFID_ReadReg(reg) | mask);
}


void RFID_ClearBitMask(uint8_t reg, uint8_t mask)
{
    RFID_WriteReg(reg, RFID_ReadReg(reg) & ~mask);
}

void RFID_AntennaOn(void)
{
    uint8_t value = RFID_ReadReg(MFRC522_REG_TX_CONTROL);
    if ((value & 0x03) != 0x03)
    {
        RFID_SetBitMask(MFRC522_REG_TX_CONTROL, value | 0x03);
    }
}

void RFID_AntennaOff(void)
{
    RFID_ClearBitMask(MFRC522_REG_TX_CONTROL, 0x03);
}

void RFID_Read(uint8_t *buffer, uint8_t length) 
{
    uint8_t i;
    
    if (length == 0) return;
    
    RFID_CS_LOW();
    
    for (i = 0; i < length; i++) 
    {
        buffer[i] = RFID_ReceiveByte();
    }
    
    RFID_CS_HIGH();
}

void RFID_Write(uint8_t *buffer, uint8_t length) 
{
    uint8_t i;
    
    if (length == 0) return;
    
    RFID_CS_LOW();
    
    for (i = 0; i < length; i++) 
    {
        RFID_SendByte(buffer[i]);
    }
    RFID_CS_HIGH();
}

uint8_t RFID_ToCard(uint8_t command, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, uint8_t *backLen)
{
    uint8_t status = 0;
    uint8_t irqEn = 0x00;
    uint8_t waitIRq = 0x00;
    uint8_t lastBits;
    uint8_t n;
    int i;

    switch (command)
    {
    case PCD_AUTHENT:
        irqEn = 0x12;
        waitIRq = 0x10;
        break;
    case PCD_TRANSCEIVE:
        irqEn = 0x77;
        waitIRq = 0x30;
        break;
    default:
        break;
    }

    RFID_WriteReg(MFRC522_REG_COMM_IE_N, irqEn | 0x80);
    RFID_ClearBitMask(MFRC522_REG_COMM_IRQ, 0x80);
    RFID_SetBitMask(MFRC522_REG_FIFO_LEVEL, 0x80);

    RFID_WriteReg(MFRC522_REG_COMMAND, PCD_IDLE);

    for (i = 0; i < sendLen; i++)
    {
        RFID_WriteReg(MFRC522_REG_FIFO_DATA, sendData[i]);
    }

    RFID_WriteReg(MFRC522_REG_COMMAND, command);
    if (command == PCD_TRANSCEIVE)
    {
        RFID_SetBitMask(MFRC522_REG_BIT_FRAMING, 0x80);
    }

    i = 2000; 
    do
    {
        n = RFID_ReadReg(MFRC522_REG_COMM_IRQ);
        i--;
    } while ((i != 0) && !(n & 0x01) && !(n & waitIRq));

    RFID_ClearBitMask(MFRC522_REG_BIT_FRAMING, 0x80);

    if (i != 0)
    {
        if (!(RFID_ReadReg(MFRC522_REG_ERROR) & 0x1B))
        {
            status = 1; 

            if (n & irqEn & 0x01)
            {
                status = 0; 
            }

            if (command == PCD_TRANSCEIVE)
            {
                n = RFID_ReadReg(MFRC522_REG_FIFO_LEVEL);
                lastBits = RFID_ReadReg(MFRC522_REG_CONTROL) & 0x07;
                if (lastBits)
                {
                    *backLen = (n - 1) * 8 + lastBits;
                }
                else
                {
                    *backLen = n * 8;
                }

                if (n == 0)
                {
                    n = 1;
                }
                if (n > MFRC522_MAX_LEN)
                {
                    n = MFRC522_MAX_LEN;
                }

                for (i = 0; i < n; i++)
                {
                    backData[i] = RFID_ReadReg(MFRC522_REG_FIFO_DATA);
                }
            }
        }
        else
        {
            status = 0;
        }
    }

    return status;
}

uint8_t RFID_Request(uint8_t reqMode, uint8_t *TagType)
{
    uint8_t status;
    uint8_t backBits;
    uint8_t buffer[MFRC522_MAX_LEN];

    RFID_WriteReg(MFRC522_REG_BIT_FRAMING, 0x07); 

    buffer[0] = reqMode;
    status = RFID_ToCard(PCD_TRANSCEIVE, buffer, 1, buffer, &backBits);

    if ((status == 1) && (backBits == 0x10))
    {
        *TagType = buffer[0];
        *(TagType + 1) = buffer[1];
    }
    else
    {
        status = 0;
    }

    return status;
}

uint8_t RFID_Anticoll(uint8_t *serNum)
{
    uint8_t status;
    uint8_t i;
    uint8_t serNumCheck = 0;
    uint8_t backBits;
    uint8_t buffer[MFRC522_MAX_LEN];

    RFID_WriteReg(MFRC522_REG_BIT_FRAMING, 0x00);

    buffer[0] = PICC_ANTICOLL;
    buffer[1] = 0x20;
    status = RFID_ToCard(PCD_TRANSCEIVE, buffer, 2, buffer, &backBits);

    if (status == 1)
    {
        for (i = 0; i < 4; i++)
        {
            serNumCheck ^= buffer[i];
            serNum[i] = buffer[i];
        }
        if (serNumCheck != buffer[4])
        {
            status = 0;
        }
    }

    return status;
}

void RFID_Halt(void)
{
    uint8_t buffer[4];
    uint8_t backLen;

    buffer[0] = PICC_HALT;
    buffer[1] = 0;
    
    buffer[2] = 0;
    buffer[3] = 0;

    RFID_ToCard(PCD_TRANSCEIVE, buffer, 4, buffer, &backLen);
}


void RFID_Debug_ReadCard(void)
{
    uint8_t status;
    uint8_t tagType[2];
    uint8_t serialNum[5];
    uint8_t i;

    for (i = 0; i < 5; i++)
    {
        serialNum[i] = 0;
    }

    status = RFID_Request(PICC_REQIDL, tagType);
    if (status == 1)
    {
        USART1_Send_String("Card detected! Type: ");
        USART1_Send_Hex(tagType[0]);
        USART1_Send_Hex(tagType[1]);
        USART1_Send_String("\r\n");
        
        status = RFID_Anticoll(serialNum);
        if (status == 1)
        {
            USART1_Send_String("Card UID: ");
            for (i = 0; i < 4; i++)
            {
                USART1_Send_Hex(serialNum[i]);
                USART1_Send_Char(' ');
            }
            USART1_Send_String("\r\n");
        }
        else
        {
            USART1_Send_String("Anticoll error\r\n");
        }
        
        RFID_Halt(); 
    }
}
