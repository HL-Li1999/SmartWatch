#include "stm32f10x.h"                  // Device header
//#include "OLED.h"

int16_t CountTime;
void TimerInit()
{
	//OLED_Init();
	
	/*打开内部时钟TIM2*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	/*选择时钟源*/
	TIM_InternalClockConfig(TIM2); //选择为内部时钟源
	
	/*初始化时基单元*/
	TIM_TimeBaseInitTypeDef TimeBase_InitStructure;
	TimeBase_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//时钟分频：不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TimeBase_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;    //计数模式：向上计数
	TimeBase_InitStructure.TIM_Period = 10000 - 1;					//计数周期，最大值为65535
	TimeBase_InitStructure.TIM_Prescaler = 7200 - 1;				//预分频器，最大值为65535
	TimeBase_InitStructure.TIM_RepetitionCounter = 0;				//重复计数器，高级定时器才会用到
	TIM_TimeBaseInit(TIM2,&TimeBase_InitStructure);	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);							//TIM_TimeBaseInit函数末尾，手动产生了一次更新事件(即调用一次中断输出函数)，若不清除则会从1开始计数
	
	/*配置中断输出*/
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);						//在需要调用中断输出函数时才需要配置
	
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
	
	//OLED_ShowNum(1, 1, 0, 5);
}

//void TIM2_IRQHandler(void)
//{
//	if(TIM_GetFlagStatus(TIM2, TIM_FLAG_Update) == SET)
//	{
//		CountTime ++;
//		OLED_ShowNum(1, 1, CountTime, 5);
//		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
//	}	
//}


