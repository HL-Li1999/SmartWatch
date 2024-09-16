#include "stm32f10x.h"                  // Device header
#include "MyU8G2.h"
#include "CreateTask.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "MyI2C.h"
#include "MPU6050.h"
#include <stdio.h>

void Get_AvrData(int16_t* AccX, int16_t* AccY, int16_t* AccZ,
		int16_t* GyroX, int16_t* GyroY, int16_t* GyroZ)
{
	int16_t AccX1, AccY1, AccZ1, GyroX1, GyroY1, GyroZ1;
	for(uint8_t i=0; i<100; i++)
	{
		GetMPU_Data(AccX, AccY, AccZ, GyroX, GyroY, GyroZ);
		AccX1 = AccX1 + *AccX;
		AccY1 = AccY1 + *AccY;
		AccZ1 = AccZ1 + *AccZ;
		GyroX1 = GyroX1 + *GyroX;
		GyroY1 = GyroY1 + *GyroY;
		GyroZ1 = GyroZ1 + *GyroZ;		
	}
	AccX1 = AccX1/100;
	AccY1 = AccY1/100;
	AccZ1 = AccZ1/100;
	GyroX1 = GyroX1/100;
	GyroY1 = GyroY1/100;
	GyroZ1 = GyroZ1/100;
	*AccX = AccX1;
	*AccY = AccY1;
	*AccZ = AccZ1;
	*GyroX = GyroX1;
	*GyroY = GyroY1;
	*GyroZ = GyroZ1;
}


void Gyro_Task(void)
{
	
	uint8_t recData, WristTimes;
	uint64_t lastTime;
	char Acc[50];
	char Gyro[50];
	char WristStr[50];
	
	int16_t AccX0, AccY0, AccZ0, GyroX0, GyroY0, GyroZ0;
	int16_t AccX, AccY, AccZ, GyroX, GyroY, GyroZ;
	Get_AvrData(&AccX0, &AccY0, &AccZ0, &GyroX0, &GyroY0, &GyroZ0);
	WristTimes = 0;
	
	while(1)
	{
		Get_AvrData(&AccX, &AccY, &AccZ, &GyroX, &GyroY, &GyroZ);
		AccX = AccX - AccX0;
		AccY = AccY - AccY0;
		AccZ = AccZ - AccZ0;
		GyroX = GyroX - GyroX0;
		GyroY = GyroY - GyroY0;
		GyroZ = GyroZ - GyroZ0;
		
		if((GyroY>=100 || GyroY<=-100) && ticks - lastTime > 2000)
		{
			lastTime = ticks;
			WristTimes += 1;		
		}
		
		sprintf(Acc, "Acc:%d, %d, %d", AccX, AccY, AccZ);
		sprintf(Gyro, "Gyro:%d, %d, %d", GyroX, GyroY, GyroZ);
		sprintf(WristStr, "Wrist Flip: %d", WristTimes);
		
		u8g2_ClearBuffer(&u8g2);
		MyU8G2_SetFontTiny();
		u8g2_DrawStr(&u8g2,10,10,Acc);
		u8g2_DrawStr(&u8g2,10,32,Gyro);
		u8g2_DrawStr(&u8g2,10,54,WristStr);
		u8g2_SendBuffer(&u8g2);
				
		
		xQueueReceive(Keys_QueueHandle, &recData, 0);
		
		if(recData == ExitDown)
		{
			recData = 0;
			vTaskResume(Menu_TaskHandle);
			vTaskSuspend(NULL);
		}	
		
		vTaskDelay(250);
		
	}
	
	
	
}