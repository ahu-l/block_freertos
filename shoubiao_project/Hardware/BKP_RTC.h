#ifndef _BKP_RTC_H
#define _BKP_RTC_H
#include "stm32f10x.h"



void BKP_Init(void);
void BKP_WriteData(uint16_t BKP_DR,uint16_t Data);
uint16_t BKP_ReadData(uint16_t  BKP_DR);
void RTC_Init(void);

#endif
