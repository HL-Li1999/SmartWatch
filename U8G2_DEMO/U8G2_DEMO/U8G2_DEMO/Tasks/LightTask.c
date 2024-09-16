#include "stm32f10x.h"                  // Device header
#include "MyU8G2.h"
#include "CreateTask.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

uint8_t LightFlag;

void LightOn()
{
	u8g2_ClearBuffer(&u8g2);
	u8g2_DrawBox(&u8g2,0,0,128,64);
	u8g2_DrawXBMP(&u8g2,48,0,64,64,gImage_lighton);
	u8g2_SendBuffer(&u8g2);
}

void LightOff()
{
	u8g2_ClearBuffer(&u8g2);
	u8g2_DrawXBMP(&u8g2,48,0,64,64,gImage_lightoff);
	u8g2_SendBuffer(&u8g2);
}

void Light_Task(void* parameter)
{	
	LightOff();
	uint8_t LightFlag;
	uint8_t recData;
	while(1)
	{
		xQueueReceive(Keys_QueueHandle, &recData, 0);
		if(recData == ExitDown)
		{
			recData = 0;
			vTaskResume(Menu_TaskHandle);
			vTaskSuspend(NULL);
		}
		if(recData == EnterDown)
		{
			if(LightFlag == 0)
			{
				LightOn();
				recData = 0;
				LightFlag = 1;
			}
			else if(LightFlag == 1)
			{
				LightOff();
				recData = 0;
				LightFlag = 0;
			}
		}
		
		vTaskDelay(250);
	}
}

