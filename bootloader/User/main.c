#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "LED.h"
#include "string.h"
#include "Key.h"
#define FLASH_SAVE_ADDR  0x08006000		//设置 FLASH 地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)
typedef  void (*iapfun)(void);			//定义一个函数类型的参数.


__asm void MSR_MSP(uint32_t addr) {
    MSR MSP, r0 			//将8006000的第一个数给MSR寄存器
    BX r14   //返回调用位置也就是说到jump2app
}
iapfun jump2app;   //0x8006000第一个地址是

//跳转到应用程序段
//appxaddr:用户代码起始地址.
void iap_load_app(uint32_t appxaddr) {
	if(((*(__IO uint32_t*)appxaddr)&0x2FFE0000)==0x20000000) 	//检查栈顶地址是否合法.
	{ 
		jump2app=(iapfun)*(__IO uint32_t*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		 
		MSR_MSP(*(__IO uint32_t*)appxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		jump2app();									//跳转到APP. 将0x8006004复用位函数地址指针然后执行函数就相当于跳转了
		
	}
}


int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	Serial_Init();		//串口初始化
	Key_Init();
	uint16_t i=0;
	while(1)
	{

		for(i=0;i<300;i++)
		{
					OLED_ShowNum(1,1,i,3);
			if(Key_GetNum())
			{

				iap_load_app(FLASH_SAVE_ADDR);
				break;
			}
		}
	

		}
	
	}