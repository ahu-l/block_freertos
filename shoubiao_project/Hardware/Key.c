#include "stm32f10x.h"    
#include "FreeRtos.h"// Device header
#include "queue.h"
uint8_t key_count=0;
extern uint16_t time_count;
extern QueueHandle_t queue1;
/**
  * 函    数：按键初始化
  * 参    数：无
  * 返 回 值：无
  */
void Key_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开启GPIOB的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						//将PB1和PB11引脚初始化为上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);
	EXTI_InitTypeDef EXTI_InitStructure;						//定义结构体变量
	EXTI_InitStructure.EXTI_Line = EXTI_Line11|EXTI_Line1|EXTI_Line3|EXTI_Line5;					//选择配置外部中断的14号线
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;					//指定外部中断线使能
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//指定外部中断线为中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//指定外部中断线为下降沿触发
	EXTI_Init(&EXTI_InitStructure);				



	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//配置 中断优先级分组
	NVIC_InitTypeDef nvic_structer;
	nvic_structer.NVIC_IRQChannel= EXTI1_IRQn;
	nvic_structer.NVIC_IRQChannelCmd=ENABLE;
	nvic_structer.NVIC_IRQChannelPreemptionPriority=10;
	nvic_structer.NVIC_IRQChannelSubPriority=0;

	NVIC_Init(&nvic_structer);
		nvic_structer.NVIC_IRQChannelPreemptionPriority=11;
	nvic_structer.NVIC_IRQChannel= EXTI3_IRQn;
	NVIC_Init(&nvic_structer);
		nvic_structer.NVIC_IRQChannelPreemptionPriority=12;
	nvic_structer.NVIC_IRQChannel= EXTI9_5_IRQn                ;
	NVIC_Init(&nvic_structer);
		nvic_structer.NVIC_IRQChannelPreemptionPriority=13;
	nvic_structer.NVIC_IRQChannel=EXTI15_10_IRQn              ;
	NVIC_Init(&nvic_structer);
}

/**
  * 函    数：按键获取键码
  * 参    数：无
  * 返 回 值：按下按键的键码值，范围：0~2，返回0代表没有按键按下
  * 注意事项：此函数是阻塞式操作，当按键按住不放时，函数会卡住，直到按键松手
  */



void EXTI1_IRQHandler  (void)
{
	if (EXTI_GetITStatus(EXTI_Line1) == SET)		//判断是否是外部中断14号线触发的中断
	{
		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
		for(int i = 0; i<5000; i++){};
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
		{time_count=0;
			key_count=2;
		xQueueSendFromISR(queue1,&key_count,NULL); 				//计数值自增一次
		}
		EXTI_ClearITPendingBit(EXTI_Line1);		//清除外部中断14号线的中断标志位
													//中断标志位必须清除
													//否则中断将连续不断地触发，导致主程序卡死
	}
}
void  EXTI3_IRQHandler  (void)
{
		if (EXTI_GetITStatus(EXTI_Line3) == SET)		//判断是否是外部中断14号线触发的中断
	{
		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
		for(int i = 0; i<5000; i++){};
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 0)
		{   time_count=0;
				key_count=4;
		xQueueSendFromISR(queue1,&key_count,NULL); 						//计数值自增一次
		}
		EXTI_ClearITPendingBit(EXTI_Line3);		//清除外部中断14号线的中断标志位
													//中断标志位必须清除
													//否则中断将连续不断地触发，导致主程序卡死
	}
}
void EXTI9_5_IRQHandler(void)
{
	uint16_t ERR;
		if (EXTI_GetITStatus(EXTI_Line5) == SET)		//判断是否是外部中断14号线触发的中断
	{
		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
		for(int i = 0; i<5000; i++){};
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0)
		{  time_count=0;
		   key_count=3;					//计数值自增一次
					ERR=xQueueSendFromISR(queue1,&key_count,NULL); 		
			if (ERR != pdTRUE)
{
		key_count=10;
}

		}
		EXTI_ClearITPendingBit(EXTI_Line5);		//清除外部中断14号线的中断标志位
													//中断标志位必须清除
													//否则中断将连续不断地触发，导致主程序卡死
	}
}
void EXTI15_10_IRQHandler (void)
{	if (EXTI_GetITStatus(EXTI_Line11) == SET)		//判断是否是外部中断14号线触发的中断
	{

		for(int i = 0; i<5000; i++){};
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0)
		{
			
			time_count=0;
		key_count=1;					//计数值自增一次
	xQueueOverwriteFromISR(queue1,&key_count,NULL); 		
		}
		EXTI_ClearITPendingBit(EXTI_Line11);		//清除外部中断14号线的中断标志位
													//中断标志位必须清除
													//否则中断将连续不断地触发，导致主程序卡死
	}
}