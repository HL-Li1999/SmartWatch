#include "stm32f10x.h"                  // Device header

void PWM_Init()
{		
//	/*端口重映射*/
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);	
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  		//复用PA15引脚时需要取消调试功能，转为普通的GPIO功能
//																	//复用PB3、PB4同理
		
	/*GPIO初始化*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;				//PWM复用输出驱动
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);							
	
	/*内部时钟配置*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_InternalClockConfig(TIM2);
	
	/*时基单元初始化*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//定义结构体变量
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//计数器模式，选择向上计数
	TIM_TimeBaseInitStructure.TIM_Period =  100 - 1;				//计数周期，即ARR的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;				//预分频器，即PSC的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;			//重复计数器，高级定时器才会用到
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);				//将结构体变量交给TIM_TimeBaseInit，配置TIM2的时基单元	
	//CNT每(PSC+1)/(72*10^6)s加1，CNT每计数(ARR+1)个触发一次中断(有配置中断的话)，即每(ARR+1)*(PSC+1)/(72*10^6)s触发一次中断
	
	/*输出比较单元初始化*/
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCStructInit(&TIM_OCInitStruct);
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 0;									//初始的CCR值
	TIM_OC1Init(TIM2, &TIM_OCInitStruct);	
	
	/*不用配置NVIC，NVIC用于配置各中断的优先级，即各中断函数的调用顺序*/
	/*同理也不用配置中断输出*/
		
	/*TIM使能*/
	TIM_Cmd(TIM2, ENABLE);			//使能TIM2，定时器开始运行
	
	/*加到main函数中
	while(1)
	{
		//占空比逐渐增大的周期为1ms的信号，也即每1ms增大1%
		//占空比为CRR/(ARR+1)
		for(int i=0; i<=100; i++)
		{
			TIM_SetCompare1(TIM2, i);
			Delay_ms(10);	
		}
		//占空比逐渐减小的周期为1ms的信号
		for(int i=0; i<=100; i++)
		{
			TIM_SetCompare1(TIM2, 100 - i);
			Delay_ms(10);	
		}
	}*/
}

/*设置占空比：0%~100%*/
void PWM_SetDuty(uint16_t compare)
{
	TIM_SetCompare1(TIM2, compare);
}

/*设置频率：初始值为1000Hz*/
void PWM_SetFreq(uint16_t frequance)
{
	uint16_t prescaler = 720000/frequance - 1;			//注意C语言的^为位异或而非指数
	TIM_PrescalerConfig(TIM2, prescaler, TIM_PSCReloadMode_Immediate);
}

