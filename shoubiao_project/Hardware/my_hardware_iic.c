#include "my_hardware_iic.h"
void my_hardware_iic_init(void)
{
		GPIO_InitTypeDef GPIO_INITStruct;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
	GPIO_INITStruct.GPIO_Mode= GPIO_Mode_AF_OD;
	GPIO_INITStruct.GPIO_Pin=  GPIO_Pin_10;
	GPIO_INITStruct.GPIO_Speed=GPIO_Speed_50MHz;

	
	
	
	GPIO_Init(GPIOB,&GPIO_INITStruct);

	GPIO_INITStruct.GPIO_Pin=  GPIO_Pin_11 ;
	GPIO_INITStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_INITStruct);
	I2C_InitTypeDef I2C_Initstructer;
	I2C_Initstructer.I2C_Ack=I2C_Ack_Enable ; //是否开启应答
	I2C_Initstructer.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit; //stm32的iic设备作为从机的地址是几位地址
	I2C_Initstructer.I2C_ClockSpeed=50000; //scl总线的时钟频率
	I2C_Initstructer.I2C_DutyCycle=I2C_DutyCycle_2;//scl总线的时钟占空比
	I2C_Initstructer.I2C_Mode=I2C_Mode_I2C; //配置成IIC模式
	I2C_Initstructer.I2C_OwnAddress1=0X00;//stm32的iic设备作为从机的地址
	I2C_Init(I2C2, &I2C_Initstructer);
	I2C_Cmd(I2C2,ENABLE);
	

	
}