#include "DMA.h"


uint16_t data_size=0;

void MyDMA_Init(uint32_t addrA,uint32_t addrB,uint16_t size)
{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
		data_size=size;
		DMA_InitTypeDef  DMA_initstructer;
	DMA_initstructer.DMA_BufferSize=size; //DMA传输多少次数据才会停止
	DMA_initstructer.DMA_DIR=DMA_DIR_PeripheralSRC ;     //选择传输方向 这个是把外设寄存器当作源传输
	DMA_initstructer.DMA_M2M=DMA_M2M_Enable ;//选择软件触发还是外部触发
		DMA_initstructer.DMA_PeripheralBaseAddr=addrA; //源地址
	DMA_initstructer.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte  ;//传输一次的数据大小
	DMA_initstructer.DMA_PeripheralInc= DMA_PeripheralInc_Enable  ;  //传输完成一次后地址是否自增
	DMA_initstructer.DMA_MemoryBaseAddr=addrB;///目标地址
	DMA_initstructer.DMA_MemoryDataSize= DMA_MemoryDataSize_Byte ; //传输一次的数据大小
	DMA_initstructer.DMA_MemoryInc= DMA_MemoryInc_Enable ;
	DMA_initstructer.DMA_Mode=DMA_Mode_Normal ; //可以设置为连续搬运，就是当size=0时，有个数据重装载寄存器，让size重新变大，继续搬运

	DMA_initstructer.DMA_Priority=DMA_Priority_Medium;//优先级，因为dma通道只有一条线，多个通道同时传输数据，需要选择哪个通道来传输数据
		DMA_Init(DMA1_Channel1,&DMA_initstructer);
	DMA_Cmd(DMA1_Channel1,DISABLE);
	
}
void MyDMA_Transfer(void)
{
DMA_Cmd(DMA1_Channel1,DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1,data_size); //设置传输数据大小寄存器
	
	DMA_Cmd(DMA1_Channel1,ENABLE);
	//因为传输数据需要一定的时间，所以要等待直到数据传输完毕
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1)==RESET);
	//传输完成标志位不会自动清0
	DMA_ClearFlag(DMA1_FLAG_TC1);
}