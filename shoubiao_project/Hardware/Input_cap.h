#ifndef _INPUT_CAP_H
#define _INPUT_CAP_H
#include "stm32f10x.h"
uint32_t IC_GetDuty(void);

uint32_t IC_GetFreq(void);

void input_cap_init(void);
#endif
