#include "stm32f10x.h"                  // Device header
#include "MyU8G2.h"
#include "CreateTask.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>


void LightTwinkle()
{
	uint8_t recData = 0;
	while(1)
	{
		//再接收到任意按键按下则退出
		xQueueReceive(Keys_QueueHandle, &recData, 0);
		
		if(recData == 0)
		{
			GPIO_WriteBit(GPIOA, GPIO_Pin_0, 1);
			GPIO_WriteBit(GPIOA, GPIO_Pin_1, 0);
			vTaskDelay(500);
		
			GPIO_WriteBit(GPIOA, GPIO_Pin_0, 0);
			GPIO_WriteBit(GPIOA, GPIO_Pin_1, 1);
			vTaskDelay(500);
		}
		else
		{
			GPIO_WriteBit(GPIOA, GPIO_Pin_0, 0);
			GPIO_WriteBit(GPIOA, GPIO_Pin_1, 0);
			return;
		}
	}
}


void Led_Task(void* parameter)
{
	uint8_t Flag=0;
	uint8_t recData;

	while(1)
	{
		u8g2_ClearBuffer(&u8g2);
		MyU8G2_SetFontTiny();
		u8g2_DrawStr(&u8g2,10,10,"1.Push <Right> To");
		u8g2_DrawStr(&u8g2,10,20,"Turn On Light0");
		u8g2_DrawStr(&u8g2,10,32,"2.Push <Left>  To");
		u8g2_DrawStr(&u8g2,10,42,"Turn On Light1");
		u8g2_DrawStr(&u8g2,10,54,"3.Push <Enter> To");
		u8g2_DrawStr(&u8g2,10,64,"Start Twinkle");
		u8g2_SendBuffer(&u8g2);
		
		xQueueReceive(Keys_QueueHandle, &recData, 0);
		
		if(recData == EnterDown)
		{
			recData = 0;
			LightTwinkle();
		}
		if(recData == ExitDown)
		{
			recData = 0;
			vTaskResume(Menu_TaskHandle);
			vTaskSuspend(NULL);	
		}	
		if(recData == RightDown)
		{
			recData = 0;
			Flag = 1 - Flag;
			GPIO_WriteBit(GPIOA, GPIO_Pin_0, Flag);
		}
		if(recData == LeftDown)
		{
			recData = 0;
			Flag = 1 - Flag;
			GPIO_WriteBit(GPIOA, GPIO_Pin_1, Flag);
		}
		
		vTaskDelay(500);
	}	
}



