#include "stm32f10x.h"                  // Device header
#include "MyU8G2.h"
#include "CreateTask.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



void SetVecocity(int8_t velocity)
{		
	if(velocity==0)
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_4, 1);
		GPIO_WriteBit(GPIOA, GPIO_Pin_5, 1);
	}
	if(velocity>0)
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_4, 1);
		GPIO_WriteBit(GPIOA, GPIO_Pin_5, 0);
		//速度：1~10，CCR = 占空比*ARR = 占空比*10000 = (速度/10)*10000
		TIM_SetCompare4(TIM2, velocity*1000);	
	}		
	if(velocity<0)	
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_4, 0);
		GPIO_WriteBit(GPIOA, GPIO_Pin_5, 1);
		velocity = -velocity;
		//速度：1~10，CCR = 占空比*ARR = 占空比*10000 = (速度/10)*10000
		TIM_SetCompare4(TIM2, velocity*1000);	
	}	
}


void Motor_Task(void* parameter)
{
	int8_t velocity;
	uint8_t recData;
	while(1)
	{	
		char velStr[5];
		sprintf(velStr, "%s%d", "3.Velocity = ", velocity);
		
		u8g2_ClearBuffer(&u8g2);
		MyU8G2_SetFontTiny();
		u8g2_DrawStr(&u8g2,10,10,"1.Push <Right> Acc");
		u8g2_DrawStr(&u8g2,10,22,"2.Push <Left> Dec");
		u8g2_DrawStr(&u8g2,10,32,velStr);		
		u8g2_SendBuffer(&u8g2);
			
		xQueueReceive(Keys_QueueHandle, &recData, 0);
		
		if(recData == ExitDown)
		{
			recData = 0;
			SetVecocity(0);
			vTaskResume(Menu_TaskHandle);
			vTaskSuspend(NULL);
		}	
		if(recData == RightDown)
		{
			recData = 0;
			if(velocity<10)
				velocity ++;
			SetVecocity(velocity);
			
		}
		if(recData == LeftDown)
		{
			recData = 0;
			if(velocity>-10)
				velocity --;
			SetVecocity(velocity);
		}
		vTaskDelay(250);
	}
}