#include "stm32f10x.h"
#include "IIC.h"
#include "Timer.h"
#include "PWM.h"
#include "Input_cap.h"
#include "ADC.h"
#include "DMA.h"
#include "Serial.h"
#include  "MPU6050.h"
#include "W25Q64.h"
#include "MyRTC.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "FreeRtos_demo.h"
#include "Key.h"
#include "menu.h"
extern void xPortSysTickHandler(void);
/**
  * @brief  main函数 初始化oled 初始化按键中断 初始化rtc时钟 初始化menu链表 开启freertos
	stm32的程序存储在flash里面双击Target 1拉到最末尾倒数第二个数是SRAM的大小，倒数第一个数是FLASH大小
	程序在FLASH上运行，SRAM保存程序运行的变量
	通过调整魔术棒里面的东西来改变程序下载到flash的位置，并且有个问题，就是你复位默认是到0x8000000读取resethandle的函数地址
	如果将 程序的地址保存在别的位置，会导致程序运行失败
  * @retval 无
  */

int main(void)
{
		SCB->VTOR = FLASH_BASE | 0X6000;  //现在程序的起始地址是8006000，需要将vector的地址加上偏移地址，才能找到对应的中断函数
		OLED_Init();
		Key_Init();
		MyRTC_Init();
		Menu_Init();
		FreeRTOS_Start();
	while(1){

		
	}
}
void SysTick_Handler(void)
{
if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
{
 xPortSysTickHandler();
}
}
