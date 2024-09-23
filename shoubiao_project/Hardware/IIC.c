#include "IIC.h"
#include "oled_font.h"
#include "MyRTC.h"
//iic先发数据的高位
#define SCL_GROUP GPIOB
#define SDA_GROUP GPIOB
#define SCL_PIN  GPIO_Pin_8
#define SDA_PIN  GPIO_Pin_9
#define SCL_SET(x)  GPIO_WriteBit(SCL_GROUP, SCL_PIN, (BitAction)(x));
#define SDA_SET(x)  GPIO_WriteBit(SDA_GROUP, SDA_PIN, (BitAction)(x));
void iic_init(void)
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


void iic_start(void)
{
	SDA_SET(1);
	SCL_SET(1);


	SDA_SET(0);//这里应该加上延时，因为要保持4.7us
	SCL_SET(0);
	
}
void iic_stop(void)
{
	SDA_SET(0); //这里sda在前的原因防止此时scl为1，你将sda置为0，是10跳变，是开始信号
	SCL_SET(1);
	SDA_SET(1);
}
uint8_t iic_r_sda(void)
{
	uint8_t bitvalue;
	bitvalue=GPIO_ReadInputDataBit(SDA_GROUP,SDA_PIN);
	return bitvalue;
}
uint8_t iic_get_byte(void )
{
	
	uint8_t i;
		uint8_t BYTE=0;
	SDA_SET(1);

	for(i=0;i<8;i++)
	{
	SCL_SET(1);
	BYTE=BYTE|(iic_r_sda()<<(7-i));
	SCL_SET(0);
	}
	return BYTE;
}
uint8_t iic_get_ack(void )
{
	
	uint8_t i;
	SDA_SET(1);

	i=iic_r_sda();
	SCL_SET(1);

	SCL_SET(0);
	return i;
}
void iic_send_ack(uint8_t BYTE)
{


		SDA_SET(BYTE);
		SCL_SET(1);
		SCL_SET(0);

	//这个时钟是用来给从机发送的，让其发送ACK信号，只是这里我并没有接收ack信号
	//这个时钟是必须的，你要是在这个时钟发送数据就会产生冲突
}
void iic_send_byte(uint8_t BYTE)
{
	uint8_t i=0; //防止编译器自动优化
	for( i=0;i<8;i++)
	{
		SDA_SET(BYTE&(0X80>>i));
		SCL_SET(1);
		SCL_SET(0);
	}
	
		SCL_SET(1);
		SCL_SET(0);
	//这个时钟是用来给从机发送的，让其发送ACK信号，只是这里我并没有接收ack信号
	//这个时钟是必须的，你要是在这个时钟发送数据就会产生冲突
	//我这里另外写了一个send——ack函数
}
void iic_cmd(uint8_t Command)
{
	iic_start();
	iic_send_byte(0x78);		//从机地址
	iic_send_byte(0x00);		//写命令
	iic_send_byte(Command); 
	iic_stop();
}
void OLED_WriteData(uint8_t Data)
{
	iic_start();
	iic_send_byte(0x78);		//从机地址
	iic_send_byte(0x40);		//写数据
	iic_send_byte(Data);
	iic_stop();
}
/**
  * @brief  设置光标的起始位置
  * @param  Y横坐标
  * @param  X纵坐标
  * @retval 无
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	iic_cmd(0xB0 | Y);					//设置Y位置
	iic_cmd(0x10 | ((X & 0xF0) >> 4));	//设置X位置高4位
	iic_cmd(0x00 | (X & 0x0F));			//设置X位置低4位
}
/**
  * @brief  通过readtime，获取rtc时间，然后显示到oled屏幕上
  * @retval 无
  */
void OLED_Clear(void)
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_WriteData(0x00);
		}
	}
}
/**
  * @brief  显示字符
  * @retval 无
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//设置光标位置在上半部分
	for (i = 0; i < 8; i++)
	{
	OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//显示上半部分内容
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//设置光标位置在下半部分
	for (i = 0; i < 8; i++)
	{
OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//显示下半部分内容
	}
}
/**
  * @brief  显示图片
  * @param  Line 起始行位置，范围：1~64
  * @param  Column 起始列位置，范围：1~128
  * @param   W 图片的宽度
  * @param   H 图片的高度 但是要除以8 早知道我在函数里面除以8了，还简单
  * @retval 无
  */
void OLED_ShowIMG(uint8_t Line, uint8_t Column,uint8_t W, uint8_t H, const char *String)
{
	uint8_t i;
	for(uint8_t j=0;j<H;j++)
	{
		OLED_SetCursor((Line - 1+j) , (Column - 1));	
	for (i = 0; i<W; i++)
	{
	
		OLED_WriteData(String[i+W*j]);
	}
}
}
/**
  * @brief  通过readtime，获取rtc时间，然后显示到oled屏幕上
  * @retval 无
  */
void OLED_Showtime(void)
{
	MyRTC_ReadTime();
	OLED_ShowIMG(1,1,32,8,time_zifu[MyRTC_Time[3]/10]);  //时 
	OLED_ShowIMG(1,30,32,8,time_zifu[MyRTC_Time[3]%10]);
	OLED_ShowIMG(1,69,32,8,time_zifu[MyRTC_Time[4]/10]); //分
	OLED_ShowIMG(1,97,32,8,time_zifu[MyRTC_Time[4]%10]);
}
/**
  * @brief  OLED显示中间的两个点（用来实现呼吸的效果）
  * @retval 无
  */
void OLED_ShowIMG_point(void)
{
				OLED_ShowIMG(1,61,8,8,point);
}
/**
  * @brief  OLED清除中间的屏幕（用来实现呼吸的效果）
  * @retval 无
  */
void OLED_ShowIMG_zero(void )
{
	uint8_t i;
	for(uint8_t j=0;j<8;j++)
	{
		OLED_SetCursor((j) , (60));	
	for (i = 0; i<8; i++)
	{
	
		OLED_WriteData(0x00);
	}
}
}
/**
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}

/**
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

/**
  * @brief  OLED显示数字（十进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~4294967295
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
	OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十进制，带符号数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-2147483648~2147483647
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
	OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十六进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFFFFFF
  * @param  Length 要显示数字的长度，范围：1~8
  * @retval 无
  */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
		OLED_ShowChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
		OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}

/**
  * @brief  OLED显示数字（二进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
  */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
	}
}

/**
  * @brief  OLED初始化
  * @param  无
  * @retval 无
  */
void OLED_Init(void)
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//上电延时
	{
		for (j = 0; j < 1000; j++);
	}
	
	iic_init();			//端口初始化
	
	iic_cmd(0xAE);	//关闭显示
	
	iic_cmd(0xD5);	//设置显示时钟分频比/振荡器频率
	iic_cmd(0x80);
	
	iic_cmd(0xA8);	//设置多路复用率
	iic_cmd(0x3F);
	
	iic_cmd(0xD3);	//设置显示偏移
	iic_cmd(0x00);
	
	iic_cmd(0x40);	//设置显示开始行
	
	iic_cmd(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
	
	iic_cmd(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置

	iic_cmd(0xDA);	//设置COM引脚硬件配置
	iic_cmd(0x12);
	
	iic_cmd(0x81);	//设置对比度控制
	iic_cmd(0xCF);

	iic_cmd(0xD9);	//设置预充电周期
	iic_cmd(0xF1);

	iic_cmd(0xDB);	//设置VCOMH取消选择级别
	iic_cmd(0x30);

	iic_cmd(0xA4);	//设置整个显示打开/关闭

	iic_cmd(0xA6);	//设置正常/倒转显示

	iic_cmd(0x8D);	//设置充电泵
	iic_cmd(0x14);

	iic_cmd(0xAF);	//开启显示
		
	OLED_Clear();				//OLED清屏
}
