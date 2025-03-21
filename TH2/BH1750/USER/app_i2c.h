#include "stm32f10x.h"
 void I2C1_config(void);
 void I2C1_write(unsigned char HW_address, unsigned char sub, unsigned char data);
 char I2C1_read(unsigned char HW_address, unsigned char sub);
 void I2C1_read_buf(unsigned char HW_address, unsigned char sub, unsigned char *p_buff, unsigned char buf_size);
