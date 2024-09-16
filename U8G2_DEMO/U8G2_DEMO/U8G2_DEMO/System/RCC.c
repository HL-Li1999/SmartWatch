#include "RCC.h"
void RCC_HSE_Configuration(void)
{
	RCC_DeInit();											//��RCC����ΪĬ��ֵ
	RCC_HSEConfig(RCC_HSE_ON);								//�����ⲿ���پ���  ״̬��HSE����ON 
	if(RCC_WaitForHSEStartUp()== SUCCESS) 					//�ȴ�HSE���� SUCCESS:HSE�����ȶ��Ҿ���	
	{ 
		RCC_HCLKConfig(RCC_SYSCLK_Div1);					//����AHBʱ�� RCC_SYSCLK_Div1��ϵͳʱ��
		RCC_PCLK1Config(RCC_HCLK_Div2);						//���õ���AHBʱ��(PCLK1) RCC_HCLK_Div2��APB1ʱ��= HCLK / 2
		RCC_PCLK2Config(RCC_HCLK_Div1);						//���ø���AHBʱ��(PCLK2) RCC_HCLK_Div1��APB2ʱ��= HCLK 
		
		FLASH_SetLatency(FLASH_Latency_2); 					//����FLASH�洢����ʱʱ������  FLASH_Latency_2��2��ʱ����
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);//ѡ��FLASHԤ��ȡ����ģʽ 	FLASH_PrefetchBuffer_Enable ������ȡ����ʹ��
		//����PLLʱ��Դ����Ƶϵ�� 
		//RCC_PLLSource_HSE_Div1:PLL��ʱ��Ƶ��= HSE��ʱ��Ƶ�� ��������PLL��ԴΪ�ⲿʱ�ӻ����ڲ�ʱ��
		//RCC_PLLMul_9��PLL������ʱ��x 9 
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
		RCC_PLLCmd(ENABLE);//ʹ��PLL
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); //�ȴ�PLL�ȶ�
		//����ϵͳʱ��SYSCLK RCC_SYSCLKSource_PLLCLK��PLL��Ϊϵͳʱ��
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		while(RCC_GetSYSCLKSource() != 0x08);				//��������ϵͳʱ�ӵ�ʱ��Դ 0x08��PLLʱ��Դ		
	} 
}
