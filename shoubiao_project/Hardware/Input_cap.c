#include "Input_cap.h"
void input_cap_init(void)
{
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	TIM_InternalClockConfig(TIM3);
	TIM_TimeBaseInitTypeDef TIME_Structer;
	TIME_Structer.TIM_ClockDivision=TIM_CKD_DIV1 ;   //滤波使用的时钟周期设置
	TIME_Structer.TIM_CounterMode=TIM_CounterMode_Up  ;      //计数模式，向上还是向下
	TIME_Structer.TIM_Period=65536-1;           //目标数尽可能的大，因为他会被重置，
	TIME_Structer.TIM_Prescaler=72-1;					//预分频数
	TIME_Structer.TIM_RepetitionCounter=0;  //重复计数
	TIM_TimeBaseInit( TIM3, &TIME_Structer);
	
	
	GPIO_InitTypeDef gpio_initstructer;
	gpio_initstructer.GPIO_Mode= GPIO_Mode_IPU ;//上拉输入
	gpio_initstructer.GPIO_Pin=GPIO_Pin_6  ;
	gpio_initstructer.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_initstructer);
	
	TIM_ICInitTypeDef tim_ic_structer;
	tim_ic_structer.TIM_Channel=TIM_Channel_1 ;   //输入通道选择
	tim_ic_structer.TIM_ICFilter= 0XF; //输入滤波器配置
	tim_ic_structer.TIM_ICPolarity=TIM_ICPolarity_Rising ;//触发沿选择
	tim_ic_structer.TIM_ICPrescaler=TIM_ICPSC_DIV1 ;//输入预分频
	tim_ic_structer.TIM_ICSelection=TIM_ICSelection_DirectTI ;//这里是选择从哪来，比如一通道的输入和二通道的输入
	
	TIM_ICStructInit(&tim_ic_structer);
	TIM_PWMIConfig(TIM3, &tim_ic_structer);
	//主从触发模式选择
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
	TIM_SelectInputTrigger(TIM3,TIM_TS_TI1FP1);
	
	TIM_Cmd(TIM3,ENABLE);   //开启cnt计数器
	
}
uint32_t IC_GetFreq(void)
{
return 1000000/TIM_GetCapture1(TIM3);
}
uint32_t IC_GetDuty(void)
{
return TIM_GetCapture2(TIM3)*100/TIM_GetCapture1(TIM3);
}