#include "stm32f10x.h"                  // Device header
#include "OLED.h"

int16_t ETRCount;
void ETRTimerInit()
{
	OLED_Init();
	
	/*打开内部时钟TIM2*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	
	/*初始化GPIO(外部时钟源需要)*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	/*选择时钟源*/	
	TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_Inverted, 0x0F); 
																	//最后一个滤波器参数加到最大0x0F，可滤除时钟信号抖动
																	//注意TIM2的ETR引脚固定为PA0，无法随意更改
	
	/*初始化时基单元*/
	TIM_TimeBaseInitTypeDef TimeBase_InitStructure;
	TimeBase_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//时钟分频：不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TimeBase_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;    //计数模式：向上计数
	TimeBase_InitStructure.TIM_Period = 10 - 1;						//计数周期，最大值为65535，达到周期后会重新计数并触发一次中断函数
	TimeBase_InitStructure.TIM_Prescaler = 1 - 1;					//预分频器，最大值为65535
	TimeBase_InitStructure.TIM_RepetitionCounter = 0;				//重复计数器，高级定时器才会用到
	TIM_TimeBaseInit(TIM2,&TimeBase_InitStructure);	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);							//TIM_TimeBaseInit函数末尾，手动产生了一次更新事件，若不清除则会从1开始计数
	
	
	
	/*配置中断输出*/
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	/*初始化NVIC*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	/*运行控制*/
	TIM_Cmd(TIM2, ENABLE);										 //使能TIM2，定时器开始运行
	
	OLED_ShowNum(1, 1, 0, 5);
	
	/*放main()中	
	while(1)
	{
		OLED_ShowNum(1, 1, TIM_GetCounter(TIM2), 5);
		OLED_ShowNum(2, 1, ETRCount, 5);
	}*/
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		ETRCount ++;
		TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	}
}
