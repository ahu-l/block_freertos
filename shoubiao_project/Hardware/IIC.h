#ifndef _IIC_H
#define _IIC_H
#include "stm32f10x.h"

void iic_init(void);

void iic_start(void);
uint8_t iic_get_ack(void );
void iic_send_ack(uint8_t BYTE);
uint8_t iic_r_sda(void);
uint8_t iic_get_byte(void );
void iic_stop(void);
void iic_send_byte(uint8_t BYTE);
void iic_cmd(uint8_t Command);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowIMG(uint8_t Line, uint8_t Column,uint8_t W, uint8_t H, const char *String);
void OLED_ShowIMG_zero(void );
void OLED_Showtime(void);
void OLED_ShowIMG_point(void);
#endif
