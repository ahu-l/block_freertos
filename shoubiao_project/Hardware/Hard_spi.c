#include "Hard_spi.h"
void SPI_SS_SET(uint8_t bitvalue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)bitvalue);
	
}
void SPI_SCK_SET(uint8_t bitvalue)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_5,(BitAction)bitvalue);
	
}
void Hard_SPI_Init(void)
{
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	GPIO_InitTypeDef gpio_initstructer;
	gpio_initstructer.GPIO_Mode= GPIO_Mode_Out_PP;//推挽输出
	gpio_initstructer.GPIO_Pin=GPIO_Pin_4;
	gpio_initstructer.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_initstructer);
	gpio_initstructer.GPIO_Mode= GPIO_Mode_AF_PP;//复用推挽输出
	gpio_initstructer.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_7 ; //复用到spi1的mosi和miso
	gpio_initstructer.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_initstructer);
	gpio_initstructer.GPIO_Mode= GPIO_Mode_IPU;//上拉输入
	gpio_initstructer.GPIO_Pin=GPIO_Pin_6 ;
	gpio_initstructer.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_initstructer);
	
	SPI_InitTypeDef SPI_Initstructer;
	SPI_Initstructer.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_128 ; //spi时钟分频系数
	SPI_Initstructer.SPI_CPHA=SPI_CPHA_1Edge;//选择SPI1的模式 0
	SPI_Initstructer.SPI_CPOL=SPI_CPOL_Low;
	SPI_Initstructer.SPI_CRCPolynomial=7;//CRC校验位无用
	SPI_Initstructer.SPI_DataSize=SPI_DataSize_8b;//单次传输数据的位数
	SPI_Initstructer.SPI_Direction=SPI_Direction_2Lines_FullDuplex;//选择半双工还是全双工
	SPI_Initstructer.SPI_FirstBit=SPI_FirstBit_MSB;//高位先行还是低位先行
	SPI_Initstructer.SPI_Mode=SPI_Mode_Master;//主模式还是从模式
	SPI_Initstructer.SPI_NSS=SPI_NSS_Soft;//无用 //还是有用

	SPI_Init(SPI1,&SPI_Initstructer);
	SPI_Cmd(SPI1,ENABLE);
	SPI_SS_SET(1); //默认高电平 这里用的gpio模拟片选信号
	
	
}

void soft_spi_start(void)
{
SPI_SS_SET(0);
	
	
}
void soft_spi_stop(void)
{
	
	SPI_SS_SET(1);
}


uint8_t Myspi_Swap_Byte(uint8_t ByteSend)
{
	
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)!=SET);//等待标志位TXE设置，设置代表数据发送缓冲器没有数据
	SPI_I2S_SendData(SPI1,ByteSend );
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)!=SET);//等待RXNE标志位，代表接收缓冲区收到数据
	return SPI_I2S_ReceiveData( SPI1);
}