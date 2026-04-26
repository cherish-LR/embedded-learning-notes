#include "stm32f10x.h"                  // Device header

uint16_t AD_Valua [4];

void AD_Init(void)
{
	//1.开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	//2.配置GPIO
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,4,ADC_SampleTime_55Cycles5);
	
	//3.选择规则组的输入通道
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);
	
	//4.用结构体初始化 ADC
	ADC_InitTypeDef ADC_Initstruct;
	ADC_Initstruct.ADC_Mode =ADC_Mode_Independent ;
	ADC_Initstruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_Initstruct.ADC_ExternalTrigConv =ADC_ExternalTrigConv_None ;
	ADC_Initstruct.ADC_ContinuousConvMode = ENABLE;
	ADC_Initstruct.ADC_NbrOfChannel = 4;
	ADC_Initstruct.ADC_ScanConvMode = ENABLE;
	ADC_Init(ADC1,&ADC_Initstruct);
	
	

	
	
	DMA_InitTypeDef DMA_Initstruct;
	DMA_Initstruct.DMA_PeripheralBaseAddr =(uint32_t)& ADC1->DR ;
	DMA_Initstruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_Initstruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	
	DMA_Initstruct.DMA_MemoryBaseAddr =(uint32_t)AD_Valua ;
	DMA_Initstruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_Initstruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	
	DMA_Initstruct.DMA_DIR = DMA_DIR_PeripheralSRC;
	
	DMA_Initstruct.DMA_BufferSize = 4;
	
	DMA_Initstruct.DMA_Mode = DMA_Mode_Circular;
	
	DMA_Initstruct.DMA_M2M = DMA_M2M_Disable;
	
	DMA_Initstruct.DMA_Priority = DMA_Priority_Medium;
	
	DMA_Init(DMA1_Channel1,&DMA_Initstruct);
	
	ADC_DMACmd(ADC1,ENABLE);
	
	DMA_Cmd(DMA1_Channel1,ENABLE);
	
	
	
	//5. 开启ADC电源
	ADC_Cmd(ADC1,ENABLE);
	//6. 对ADC校准
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1) == SET);
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}



