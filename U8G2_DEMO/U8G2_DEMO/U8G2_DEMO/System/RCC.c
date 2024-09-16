#include "RCC.h"
void RCC_HSE_Configuration(void)
{
	RCC_DeInit();											//将RCC设置为默认值
	RCC_HSEConfig(RCC_HSE_ON);								//设置外部高速晶振  状态：HSE晶振ON 
	if(RCC_WaitForHSEStartUp()== SUCCESS) 					//等待HSE起振 SUCCESS:HSE晶振稳定且就绪	
	{ 
		RCC_HCLKConfig(RCC_SYSCLK_Div1);					//设置AHB时钟 RCC_SYSCLK_Div1：系统时钟
		RCC_PCLK1Config(RCC_HCLK_Div2);						//设置低速AHB时钟(PCLK1) RCC_HCLK_Div2：APB1时钟= HCLK / 2
		RCC_PCLK2Config(RCC_HCLK_Div1);						//设置高速AHB时钟(PCLK2) RCC_HCLK_Div1：APB2时钟= HCLK 
		
		FLASH_SetLatency(FLASH_Latency_2); 					//设置FLASH存储器延时时钟周期  FLASH_Latency_2：2延时周期
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);//选择FLASH预读取缓存模式 	FLASH_PrefetchBuffer_Enable ：欲读取缓存使能
		//配置PLL时钟源及倍频系数 
		//RCC_PLLSource_HSE_Div1:PLL的时钟频率= HSE的时钟频率 可以设置PLL来源为外部时钟或者内部时钟
		//RCC_PLLMul_9：PLL的输入时钟x 9 
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
		RCC_PLLCmd(ENABLE);//使能PLL
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); //等待PLL稳定
		//设置系统时钟SYSCLK RCC_SYSCLKSource_PLLCLK：PLL作为系统时钟
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		while(RCC_GetSYSCLKSource() != 0x08);				//返回用作系统时钟的时钟源 0x08：PLL时钟源		
	} 
}
