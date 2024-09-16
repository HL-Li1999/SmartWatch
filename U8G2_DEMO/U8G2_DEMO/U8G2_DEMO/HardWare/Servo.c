#include "stm32f10x.h"                  // Device header
#include "OLED.h"

void Servo_Init()
{
	OLED_Init();
	
	/*舵机输出GPIO初始化*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;				//Pin_1复用输出驱动，即使用TIM2_CH2，初始化为上拉输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	/*按键输入GPIO初始化*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;					
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);		
	
	/*内部时钟配置*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_InternalClockConfig(TIM2);
	
	/*时基单元初始化*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//定义结构体变量
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//计数器模式，选择向上计数
	TIM_TimeBaseInitStructure.TIM_Period =  10000 - 1;				//计数周期，即ARR的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 36 - 1;				//预分频器，即PSC的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;			//重复计数器，高级定时器才会用到
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);				//将结构体变量交给TIM_TimeBaseInit，配置TIM2的时基单元
	//CNT每(PSC+1)/(72*10^6)s加1，CNT每计数(ARR+1)个触发一次中断(有配置中断的话)，即每(ARR+1)*(PSC+1)/(72*10^6)s触发一次中断
	//舵机要求高电平时间：0.5ms~2.5ms，可生成一个5ms的波，即(PSC+1)*(ARR+1)==3.6*10^5
	
	/*输出比较单元初始化*/
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCStructInit(&TIM_OCInitStruct);
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 0;									//初始的CCR值
	TIM_OC2Init(TIM2, &TIM_OCInitStruct);							//TIM2的第2通道，应该用OC2Init()
			
	/*TIM使能*/
	TIM_Cmd(TIM2, ENABLE);											//使能TIM2，定时器开始运行	
	
	OLED_ShowNum(1,1,0,5);	
}

void SetAngle(uint16_t angle)
{		
	//舵机角度:0~180，占空比:0%~50%，CCR:0%*ARR~50%*ARR
	TIM_SetCompare2(TIM2, angle*5000/180);		//PA1是TIM2的第2通道，因此用SwtCompare2()
	
	OLED_ShowNum(1,1,angle,5);	
	
}
