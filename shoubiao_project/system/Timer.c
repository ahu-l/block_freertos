#include "Timer.h"
#include "stm32f10x.h"

void timer_init(void)
{
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_InternalClockConfig(TIM2);
	TIM_TimeBaseInitTypeDef TIME_Structer;
	TIME_Structer.TIM_ClockDivision=TIM_CKD_DIV1 ;   //滤波使用的时钟周期设置
	TIME_Structer.TIM_CounterMode=TIM_CounterMode_Up  ;      //计数模式，向上还是向下
	TIME_Structer.TIM_Period=10000-1;           //目标数
	TIME_Structer.TIM_Prescaler=7200-1;					//预分频数
	TIME_Structer.TIM_RepetitionCounter=0;  //重复计数
	TIM_TimeBaseInit( TIM2, &TIME_Structer);
	// TIMx->EGR = TIM_PSCReloadMode_Immediate;     因为这一句会立即装载预分频数，所以说会开启事件
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //开启TIM设置中断
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
//	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//配置 中断优先级分组
	NVIC_InitTypeDef nvic_structer;
	nvic_structer.NVIC_IRQChannel=  TIM2_IRQn  ;
	nvic_structer.NVIC_IRQChannelCmd=ENABLE;
	nvic_structer.NVIC_IRQChannelPreemptionPriority=1;
	nvic_structer.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&nvic_structer);
	TIM_Cmd(TIM2,ENABLE);   //开启cnt计数器
	
	
	
}
void TIM2_IRQHandler(void)
{
	
	
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
	{
		TIM_ClearFlag(TIM2,TIM_FLAG_Update);

	}
}

void SysTick_Init(void)
{
    /* SystemFrequency / 1000    1ms中断一次
    * SystemFrequency / 100000  10us中断一次
    * SystemFrequency / 1000000 1us中断一次
    */
    if (SysTick_Config(SystemCoreClock / 10)) {
        /* Capture error */
        while (1);
    }
}
