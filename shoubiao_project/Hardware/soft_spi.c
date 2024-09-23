#include "soft_spi.h"

static void SPI_SS_SET(uint8_t bitvalue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)bitvalue);
	
}
static void SPI_SCK_SET(uint8_t bitvalue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_5,(BitAction)bitvalue);
	
}
void SPI_MOSI_SET(uint8_t bitvalue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_7,(BitAction)bitvalue);
	
}
uint8_t SPI_MISO_GET(void)
{
	return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6);
}
void soft_spi_init(void)
{	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitTypeDef gpio_initstructer;
	gpio_initstructer.GPIO_Mode= GPIO_Mode_Out_PP;//推挽输出
	gpio_initstructer.GPIO_Pin=GPIO_Pin_4 |GPIO_Pin_5|GPIO_Pin_7 ;
	gpio_initstructer.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_initstructer);

	gpio_initstructer.GPIO_Mode= GPIO_Mode_IPU;//上路输入
	gpio_initstructer.GPIO_Pin=GPIO_Pin_6 ;
	gpio_initstructer.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_initstructer);
SPI_SS_SET(1); //默认高电平
	SPI_SCK_SET(0); //默认低电平 具体可以看模式0 SPI基本时序
	
}
static void soft_spi_start(void)
{
SPI_SS_SET(0);
	
	
}
static void soft_spi_stop(void)
{
	
	SPI_SS_SET(1);
}


static uint8_t Myspi_Swap_Byte(uint8_t ByteSend)
{
	
	
	uint8_t i;
	for(i=0;i<8;i++)
	{
		SPI_MOSI_SET(ByteSend&0X80);
		ByteSend<<=1;
		SPI_SCK_SET(1); //读取数据
		if(SPI_MISO_GET()==1)
		{
			ByteSend|=0X01;
		}
		SPI_SCK_SET(0); //写入数据   
	}
	return ByteSend;
	
}