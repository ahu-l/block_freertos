#ifndef _PWM_H
#define _PWM_H

#include "stm32f10x.h"
void Pwm_Init(void);
void pwm_set_compare(uint16_t Compare1);
void pwm_set_pre(uint16_t  Prescaler);

#endif
