#include "Serial.h"

uint8_t receive_data;
uint8_t receive_flag=0;
void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef gpio_initstructer;
	gpio_initstructer.GPIO_Mode= GPIO_Mode_AF_PP;//复用推挽输出
	gpio_initstructer.GPIO_Pin=GPIO_Pin_9  ; //TX 引脚
	gpio_initstructer.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_initstructer);
	
	gpio_initstructer.GPIO_Mode= GPIO_Mode_IPU;//上拉输入
	gpio_initstructer.GPIO_Pin=GPIO_Pin_10  ; //RX引脚
	gpio_initstructer.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_initstructer);
	
	USART_InitTypeDef Usart_Initstructer;
	Usart_Initstructer.USART_BaudRate=9600;//波特率
	Usart_Initstructer.USART_HardwareFlowControl=USART_HardwareFlowControl_None ;//流控制 不懂
	Usart_Initstructer.USART_Mode=USART_Mode_Rx|USART_Mode_Tx; //选择输入还是输出
	Usart_Initstructer.USART_Parity=USART_Parity_No; //选择奇偶校验还是无校验
	Usart_Initstructer.USART_StopBits=USART_StopBits_1;//选择停止位的位数
	Usart_Initstructer.USART_WordLength=USART_WordLength_8b; //选择数据位的个数
	USART_Init(USART1,&Usart_Initstructer);
	
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE); //开启接收中断
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//配置 中断优先级分组
	NVIC_InitTypeDef nvic_structer;
	nvic_structer.NVIC_IRQChannel= USART1_IRQn  ;
	nvic_structer.NVIC_IRQChannelCmd=ENABLE;
	nvic_structer.NVIC_IRQChannelPreemptionPriority=1;
	nvic_structer.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&nvic_structer);
	
	USART_Cmd(USART1,ENABLE);
}

void send_byte(uint8_t data)
{
	USART_SendData(USART1, data);
	while(USART_GetFlagStatus( USART1,USART_FLAG_TXE)==RESET); //这个标志位是dr将数据传送到发送数据移位寄存器的完成标志位 
}
void USART1_IRQHandler (void)
{
	
	if(USART_GetFlagStatus(USART1,USART_IT_RXNE)==SET)
	{
		receive_data=USART_ReceiveData(USART1);
		receive_flag=1;
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}

}
uint8_t Usart_GetData(void)
{
	return receive_data;
}
uint8_t Usart_GetFlag(void)
{
	if(receive_flag==1)
	{
		receive_flag=0;
		return 1;
	}
	return 0;
}

