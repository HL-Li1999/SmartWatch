#include "stm32f10x.h"                  // Device header

uint16_t Res[2];
void Adc_Init()
{
	/*********************************************GPIO部分*********************************************/
	/*初始化ADC输入的GPIO*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	/*********************************************ADC部分*********************************************/	
	/*初始化ADC转换器*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);									//ADC转换器时钟选择，频率=72M/6
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;    				//使用独立模式，不使用双AD模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;					//开启连续转换模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ADC转换器采用软件触发，不采用外部触发
	ADC_InitStructure.ADC_NbrOfChannel = 2;								//通道数，为1，仅在扫描模式下，才需要指定大于1的数，在非扫描模式下，只能是1
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;						//开启扫描模式
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/*规则通道配置*/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
	
	ADC_Cmd(ADC1, ENABLE);
		
		
	/*********************************************DMA部分*********************************************/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_InitTypeDef DMA_InitStruct;
	DMA_InitStruct.DMA_BufferSize = 2;					     				//传输数据个数，包括规则组序数1和序数2
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;			 				//数据传输方向，此处设置为从外设到存储器
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;				 				//是否为存储器到存储器的传输，实际就是时软件触发还是硬件触发(外设寄存器到存储器)
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)Res;	 					//存储器基地址
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;		//存储器的数据单位宽度，这里是数组元素宽度
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;					//存储器开启地址自增，外设寄存器不用自增，存储器要(除非只传一个元素)
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;							//开启循环传输
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;			//外设基地址
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设寄存器的数据单位宽度，这里只需要低位16位
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//外设寄存器关闭地址自增
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;						
	DMA_Init(DMA1_Channel1, &DMA_InitStruct);								//M2M通道可以任选，外设则需要选择特定的通道，ADC对应1通道
	
	DMA_Cmd(DMA1_Channel1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);												//ADC1触发DMA使能
	
	/*ADC校准*/
	ADC_ResetCalibration(ADC1);												//固定流程
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	//软件触发ADC开始工作，由于ADC处于连续转换模式，故触发一次后ADC就可以一直连续不断地工作
}

///*单次模式下，通过下述获取传输值*/
//uint16_t ADC_GetCount(uint8_t ADC_Channel)
//{
//	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5); 
//																		//修改规则组名单
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);								//软件触发ADC转换器
//	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);				//等待此次AD转换完成，完成时EOC标志位置0
//	return ADC_GetConversionValue(ADC1);
//}
	