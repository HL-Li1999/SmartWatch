#include "stm32f10x.h"                  // Device header
#include "MyU8G2.h"
#include "CreateTask.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

void SetAngle(uint8_t angle)
{		
	//舵机角度:0~180，占空比:0%~50%，CCR:0%*ARR~50%*ARR
	TIM_SetCompare4(TIM4, angle*5000/180);		//PA2是TIM3的第4通道，因此用SwtCompare4()	
	printf("angle=%d/n",angle);
}


void Servo_Task(void* parameter)
{
	uint8_t angle;
	uint8_t recData;
	while(1)
	{
		u8g2_ClearBuffer(&u8g2);
		MyU8G2_SetFontTiny();
		u8g2_DrawStr(&u8g2,10,10,"1.Push <Right> To");
		u8g2_DrawStr(&u8g2,10,20,"Turn Right");
		u8g2_DrawStr(&u8g2,10,32,"2.Push <Left>  To");
		u8g2_DrawStr(&u8g2,10,42,"Turn Left");
		u8g2_SendBuffer(&u8g2);
		
		xQueueReceive(Keys_QueueHandle, &recData, 0);
		
		if(recData == ExitDown)
		{
			recData = 0;
			vTaskResume(Menu_TaskHandle);
			vTaskSuspend(NULL);
		}	
		if(recData == RightDown)
		{
			recData = 0;
			angle += 30;
			if(angle>180)
				angle = 0;
			SetAngle(angle);
		}
		if(recData == LeftDown)
		{
			recData = 0;		
			angle -= 30;
			if(angle>180)
				angle = 180;
			SetAngle(angle);
		}
		vTaskDelay(250);
	}
}