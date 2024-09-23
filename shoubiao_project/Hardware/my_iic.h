#ifndef _MY_IIC_H
#define _MY_IIC_H
#include "stm32f10x.h"
void my_iic_init(void);

void my_iic_start(void);
uint8_t my_iic_get_ack(void );
void my_iic_send_ack(uint8_t BYTE);
uint8_t my_iic_r_sda(void);
uint8_t my_iic_get_byte(void );
void my_iic_stop(void);
void my_iic_send_byte(uint8_t BYTE);

#endif
