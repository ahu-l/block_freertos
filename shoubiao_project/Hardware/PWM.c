#include "PWM.h"
#include "stm32f10x.h"

void Pwm_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	TIM_InternalClockConfig(TIM2);
	TIM_TimeBaseInitTypeDef TIME_Structer;
	TIME_Structer.TIM_ClockDivision=TIM_CKD_DIV1 ;   //滤波使用的时钟周期设置
	TIME_Structer.TIM_CounterMode=TIM_CounterMode_Up  ;      //计数模式，向上还是向下
	TIME_Structer.TIM_Period=100-1;           //目标数
	TIME_Structer.TIM_Prescaler=720-1;					//预分频数
	TIME_Structer.TIM_RepetitionCounter=0;  //重复计数
	TIM_TimeBaseInit( TIM2, &TIME_Structer);
	
	
	GPIO_InitTypeDef gpio_initstructer;
	gpio_initstructer.GPIO_Mode= GPIO_Mode_AF_PP;//复用推挽输出
	gpio_initstructer.GPIO_Pin=GPIO_Pin_0  ;
	gpio_initstructer.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_initstructer);
	
	
	TIM_OCInitTypeDef tim_oc_structer;
	TIM_OCStructInit(&tim_oc_structer);
	tim_oc_structer.TIM_OCMode=TIM_OCMode_PWM1  ;				//输出模式选择
	tim_oc_structer.TIM_OCPolarity=TIM_OCPolarity_High    ;   //输出极性
	tim_oc_structer.TIM_OutputState=TIM_OutputState_Enable ;  //使能输出比较
	tim_oc_structer.TIM_Pulse=50;//CCR 寄存器的值，也就是与cnt比较的值   /TIM_SetCompare1(TIM2, Compare1);; 可以通过这个函数来修改
	TIM_OC1Init(TIM2, &tim_oc_structer);
	
	TIM_Cmd(TIM2,ENABLE);   //开启cnt计数器
	

}
void pwm_set_compare(uint16_t Compare1)
{
	TIM_SetCompare1(TIM2, Compare1);
}

void pwm_set_pre(uint16_t  Prescaler)
{
	 TIM_PrescalerConfig(TIM2,  Prescaler, TIM_PSCReloadMode_Immediate);
}