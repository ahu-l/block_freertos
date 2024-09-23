#include "ADC.h"



void ADCone_Init(void)
{
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//从apb2来的是72兆时钟，但是adc时钟最大不能超过14兆，需要分频
	GPIO_InitTypeDef gpio_initstructer;
	gpio_initstructer.GPIO_Mode= GPIO_Mode_AIN ;//切断输入，直接与adc连接具体可以看gpio内部图
	gpio_initstructer.GPIO_Pin=GPIO_Pin_0  ;
	gpio_initstructer.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_initstructer);
	//uint8_t Rank 就是通道对应在表的第几个，就是他有一个扫描表，你可以设置哪个通道放在表的具体位置，来决定谁先被扫描
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_41Cycles5);
	ADC_InitTypeDef ADC_initstructer;
	ADC_initstructer.ADC_ContinuousConvMode=DISABLE;  //是否连续采集 如果不是需要一直调用采集触发
	ADC_initstructer.ADC_DataAlign=ADC_DataAlign_Right ; //数据只有12位，但是寄存器16位需要选择是左对齐还是右对齐
	ADC_initstructer.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None ;//外部触发源选择 但是ADC的触发模式有软件内部和外部 ，这里不使用外部
	ADC_initstructer.ADC_Mode=ADC_Mode_Independent ; //选择单adc还是双adc
	ADC_initstructer.ADC_NbrOfChannel=1;  //扫描表里面的通道个数
	ADC_initstructer.ADC_ScanConvMode=DISABLE;//是否开启扫描模式，这里只有一个通道，所以不开启

	ADC_Init(ADC1,&ADC_initstructer);
	
	ADC_Cmd(ADC1,ENABLE);//开启ADC
	
	
	//校准adc
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1)==SET);//初始化校准寄存器
	ADC_StartCalibration(ADC1);//开始校准寄存器
	while(ADC_GetCalibrationStatus(ADC1)==SET);
	
}
uint16_t AD_GetValue(void)
{
ADC_SoftwareStartConvCmd(ADC1,ENABLE);//开启软件触发ADC
while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
	return ADC_GetConversionValue(ADC1);

}