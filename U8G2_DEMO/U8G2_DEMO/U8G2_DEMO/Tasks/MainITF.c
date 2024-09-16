#include "stm32f10x.h"                  // Device header
#include "MyU8G2.h"
//#include "main.h"
#include "CreateTask.h"
#include "main.h"
#include<stdio.h>

void MainITF_Task(void* parameter)
{
	vTaskSuspend(Menu_TaskHandle);
	vTaskSuspend(Light_TaskHandle);
	vTaskSuspend(Led_TaskHandle);
	vTaskSuspend(Servo_TaskHandle);
	vTaskSuspend(Music_TaskHandle);
	vTaskSuspend(Motor_TaskHandle);
	vTaskSuspend(Gyro_TaskHandle);
	
	BaseType_t xReturn;
	uint8_t recData;
	while(1)
	{
		MyU8G2_SetFontBig();
		u8g2_ClearBuffer(&u8g2);
		u8g2_DrawStr(&u8g2, 75, 50, GetSecStr(ticks/1000));
		u8g2_DrawStr(&u8g2, 60, 45, ":");
		u8g2_DrawStr(&u8g2, 5, 50, GetMinStr(ticks/1000));
		u8g2_SendBuffer(&u8g2);
		
		xReturn = xQueueReceive(Keys_QueueHandle, &recData, 0);
		if(xReturn)
		{
			if(recData == EnterDown)
			{
				recData = 0;
				vTaskResume(Menu_TaskHandle);
				vTaskSuspend(NULL);
			}	
		}
		vTaskDelay(250);
	}
}
