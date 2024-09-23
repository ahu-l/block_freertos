#include "my_iic.h"

#define SCL_GROUP GPIOB
#define SDA_GROUP GPIOB
#define SCL_PIN  GPIO_Pin_10
#define SDA_PIN  GPIO_Pin_11
#define SCL_SET(x)  GPIO_WriteBit(SCL_GROUP, SCL_PIN, (BitAction)(x));
#define SDA_SET(x)  GPIO_WriteBit(SDA_GROUP, SDA_PIN, (BitAction)(x));
void my_iic_init(void)
{
		GPIO_InitTypeDef GPIO_INITStruct;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//时钟的开启要在寄存器配置之前

	GPIO_INITStruct.GPIO_Mode= GPIO_Mode_Out_OD;
	GPIO_INITStruct.GPIO_Pin=SCL_PIN ;
	GPIO_INITStruct.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(SCL_GROUP,&GPIO_INITStruct);

	GPIO_INITStruct.GPIO_Pin=SDA_PIN ;
	GPIO_INITStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(SDA_GROUP,&GPIO_INITStruct);
	
	SCL_SET(1);//IIC空闲时两条总线都应该置为1
	SDA_SET(1);
}


void my_iic_start(void)
{
	SDA_SET(1);
	SCL_SET(1);


	SDA_SET(0);//这里应该加上延时，因为要保持4.7us
	SCL_SET(0);
	
}
void my_iic_stop(void)
{
	SDA_SET(0); //这里sda在前的原因防止此时scl为1，你将sda置为0，是10跳变，是开始信号
	SCL_SET(1);
	SDA_SET(1);
}
uint8_t my_iic_r_sda(void)
{
	uint8_t bitvalue;
	bitvalue=GPIO_ReadInputDataBit(SDA_GROUP,SDA_PIN);
	return bitvalue;
}
uint8_t my_iic_get_byte(void )
{
	
	uint8_t i;
		uint8_t BYTE=0;
	SDA_SET(1);

	for(i=0;i<8;i++)
	{
	SCL_SET(1);
	BYTE=BYTE|(my_iic_r_sda()<<(7-i));
	SCL_SET(0);
	}
	return BYTE;
}
uint8_t my_iic_get_ack(void )
{
	
	uint8_t i;
	SDA_SET(1);

	i=my_iic_r_sda();
	SCL_SET(1);

	SCL_SET(0);
	return i;
}
void my_iic_send_ack(uint8_t BYTE)
{


		SDA_SET(BYTE);
		SCL_SET(1);
		SCL_SET(0);

	//这个时钟是用来给从机发送的，让其发送ACK信号，只是这里我并没有接收ack信号
	//这个时钟是必须的，你要是在这个时钟发送数据就会产生冲突
}
void my_iic_send_byte(uint8_t BYTE)
{
	uint8_t i=0; //防止编译器自动优化
	for( i=0;i<8;i++)
	{
		SDA_SET(BYTE&(0X80>>i));
		SCL_SET(1);
		SCL_SET(0);
	}
	
//SCL_SET(1);

//	SCL_SET(0);
	//这个时钟是用来给从机发送的，让其发送ACK信号，只是这里我并没有接收ack信号
	//这个时钟是必须的，你要是在这个时钟发送数据就会产生冲突
	//我这里另外写了一个send——ack函数
}