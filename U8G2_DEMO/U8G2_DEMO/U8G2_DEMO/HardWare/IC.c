#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void IC_Init()
{
	PWM_Init();

	/*初始化输入捕获GPIO端口*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   				//输入捕获不用复用输入驱动，采用原输入驱动
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/*选择时钟源*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	TIM_InternalClockConfig(TIM3);            						//TIM3选择内部时钟源
	
	/*初始化时基单元*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//定义结构体变量
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式，选择向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;               //计数周期，即ARR的值，采用测周法测量频率时，ARR越大误差越小(1/65535)
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;               //预分频器，即PSC的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            //重复计数器，高级定时器才会用到
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);             
	
	/*初始化TIM3，选择输入捕获初始化*/
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;					//选择TIM3_CH1，因为PA6对应TIM3_CH1
	TIM_ICInitStruct.TIM_ICFilter = 0xF;							//选择输入滤波器滤波值，越高滤波越强
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising; 		//极性，选择上升沿触发
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;				//捕获器前面的分频器分频，因为打算测量原信号所以不分频
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;	//设置TIM3_CH1的捕获输入为TI1FP1
	TIM_ICInit(TIM3, &TIM_ICInitStruct);
	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;					//选择TIM3_CH2
	TIM_ICInitStruct.TIM_ICFilter = 0xF;							//选择输入滤波器滤波值，越高滤波越强
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling; 		//极性，选择下降沿触发
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;				//捕获器前面的分频器分频，因为打算测量原信号所以不分频
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_IndirectTI;	//设置TIM3_CH2的捕获输入为TI1FP1，注意不是设置TIM3_CH1捕获输入为TI1FP2
	TIM_ICInit(TIM3, &TIM_ICInitStruct);
	
	
	
	/*采用测周法测量，每个中断令CNT置0，可调用中断函数或主从触发模式*/
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
	
	/*TIM使能*/
	TIM_Cmd(TIM3, ENABLE);											//使能TIM3，定时器开始运行
	
	
	PWM_SetFreq(1000);
	//注意PWM的占空比初始值被设置为0了，需要给一个初始值
	PWM_SetDuty(50);
}

uint32_t Get_Freq()
{
	return 1000000/TIM_GetCapture1(TIM3);
}

uint8_t Get_Duty()
{
	//在TIM3_CH1上升沿捕获一次，此时CCR1捕获CNT并通过从触发令CNT置0
	//在TIM3_CH2下降沿又捕获一次，此时CCR2捕获CNT但不会令CNT置0
	//占空比Duty = CCR2 / CCR1 * 100
	return TIM_GetCapture2(TIM3) * 100 / TIM_GetCapture1(TIM3);
}