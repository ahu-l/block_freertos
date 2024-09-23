#include "BKP_RTC.h"
void BKP_Init(void)
{

RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);
RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);  //使能pwr和bkp外设
PWR_BackupAccessCmd(ENABLE); //使能对BKP和RTC的访问

}
void BKP_WriteData(uint16_t BKP_DR,uint16_t Data)
{

BKP_WriteBackupRegister( BKP_DR, Data);
}
uint16_t BKP_ReadData(uint16_t  BKP_DR)
{
	

return  BKP_ReadBackupRegister(BKP_DR);
	
}
void RTC_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);  //使能pwr和bkp外设
	PWR_BackupAccessCmd(ENABLE); //使能对BKP和RTC的访问
	RCC_LSEConfig( RCC_LSE_ON); //启动内部时钟
	while( RCC_GetFlagStatus( RCC_FLAG_LSERDY)!=SET); //等待内部时钟启动完成
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//选择lse时钟源作为rcc的时钟源输入
	RCC_RTCCLKCmd(ENABLE);
	RTC_WaitForSynchro();//就是等待读写总线的时许与rtc的时序同步
	RTC_WaitForLastTask();//等待上一次读写任务的完成
	RTC_SetPrescaler(32768-1); //设置prl寄存器
		RTC_WaitForLastTask();//等待上一次读写任务的完成
	RTC_SetCounter(1672588795);
		RTC_WaitForLastTask();
	
}