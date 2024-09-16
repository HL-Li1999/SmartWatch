#include "stm32f10x.h"                  // Device header
#include "OLED.h"


int16_t RotateVal;

void Encoder1Init()
{
	OLED_Init();
	
	/*初始化GPIO*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	
	GPIO_InitTypeDef GPIO_InitStructre;
	GPIO_InitStructre.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructre.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructre.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructre);
	
	
	/*初始化AFIO，并选择中断引脚*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0); //GPIO_PinSource0：对GPIO口相应引脚的复用功能进行配置时才会用到，
															   //比如这里复用为外部中断输入端口
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);
	
	
	/*初始化EXIT*/
	EXTI_InitTypeDef EXTI_InitStructre;
	EXTI_InitStructre.EXTI_Line = EXTI_Line0 | EXTI_Line1;
	EXTI_InitStructre.EXTI_LineCmd = ENABLE;
	EXTI_InitStructre.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructre.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructre);
	
	
	/*初始化NVIC*/	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	NVIC_InitTypeDef NVIC_InitStructre;
	
	NVIC_InitStructre.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructre.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructre.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructre.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructre);
	
	NVIC_InitStructre.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructre.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructre.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructre.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructre);
	
	OLED_ShowNum(1,1,0,5);
}

void Encoder2Init()
{
	/*********************************************GPIO部分*********************************************/
	/*初始化编码器信号输入引脚*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*********************************************TIM部分*********************************************/
	/*初始化TIM输入捕获*/	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	
	/*配置TIM3_CH1输入捕获*/
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; 
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;  //捕获寄存器捕获信号为TI1FP1
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	/*配置TIM3_CH2输入捕获*/
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2; 
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //捕获寄存器捕获信号为TI2FP1
		
	/*初始化TIM时基单元*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;      			//捕获信号不分频
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
	
	/*配置TIM编码器接口*/
	//每个TIM都拥有一个编码器接口，使用TIM_CH1和TIM_CH2
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
																	//注意此时参数的Rising和Falling已经不代表上升沿和下降沿了，而是代表输入通道是否反相
																	//此函数必须在输入捕获初始化之后进行，否则输入捕获的配置会覆盖此函数的部分配置
	
	/*使能TIM3*/
	TIM_Cmd(TIM3, ENABLE);
}

int16_t Encoder_GetCounter()
{
	//uint16_t：0~65536，int16_t:-32,768~32768
	//uint16_t->int16_t
	return (int16_t)TIM_GetCounter(TIM3);
}

void EXTI0_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line0) == SET)		
	{
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)
		{
			if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
				RotateVal ++;						
			OLED_ShowSignedNum(1,1,RotateVal,5);
		}				
		EXTI_ClearITPendingBit(EXTI_Line0);		
	}
}


void EXTI1_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line1) == SET)		
	{
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
		{
			if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)
				RotateVal --;						
			OLED_ShowSignedNum(1,1,RotateVal,5);
		}				
		EXTI_ClearITPendingBit(EXTI_Line1);		
	}
}