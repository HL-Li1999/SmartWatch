#include "stm32f10x.h"                  // Device header
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "MyU8G2.h"
#include "queue.h"
#include "main.h"
#include <stdio.h>

#include "CreateTask.h"
#include "MainITF.h"
#include "MenuTask.h"
#include "LightTask.h"
#include "LedTask.h"
#include "ServoTask.h"
#include "MusicTask.h"
#include "MotorTask.h"
#include "GyroTask.h"


/*句柄*/
//问题1：为什么这里要再声明一次且不能用extern
TimerHandle_t MainClock_TimerHandle = NULL;		//主时钟定时器句柄
TaskHandle_t MainITF_TaskHandle = NULL;				//主界面任务句柄
QueueHandle_t Keys_QueueHandle = NULL;				//按键消息队列句柄
TaskHandle_t KeysSend_TaskHandle = NULL;			//按键消息发送任务句柄
TaskHandle_t Menu_TaskHandle = NULL;					//菜单任务句柄
TaskHandle_t Light_TaskHandle = NULL;					//手电筒任务句柄
TaskHandle_t Led_TaskHandle = NULL;						//Led灯任务句柄
TaskHandle_t Servo_TaskHandle = NULL;				//舵机任务句柄
TaskHandle_t Music_TaskHandle = NULL;					//音乐任务句柄
TaskHandle_t Motor_TaskHandle = NULL;					//电机任务句柄
TaskHandle_t Gyro_TaskHandle = NULL;					//陀螺仪任务句柄

uint32_t ticks;

/*函数声明*/
void MainClock_Callback(TimerHandle_t xTimer);
char* GetMinStr(uint32_t seconds);
char* GetSecStr(uint32_t seconds);
void KeysSend_Task();
void KeysReceive_Task();

	
void CreateAllTask()
{	
	/*主时钟*/
	//问题2：为什么在任务中创建时钟的话，会被打断呢？？补充：发现是其它任务不用循环while(1)的话会打断
	MainClock_TimerHandle = xTimerCreate("MainClock_Handle", 1000, pdTRUE, (void*)1, (TimerCallbackFunction_t)MainClock_Callback);
	if(MainClock_TimerHandle!=NULL)
		xTimerStart(MainClock_TimerHandle, 0);
	

	/*按键响应*/
	Keys_QueueHandle = xQueueCreate(1,sizeof(uint8_t));
	
	/*主界面*/
	xTaskCreate((TaskFunction_t)MainITF_Task, "MainITF_Task", 256, NULL, 22, &MainITF_TaskHandle);
	
	/*菜单*/
	xTaskCreate((TaskFunction_t)Menu_Task, "Menu_Task", 256, NULL, 22, &Menu_TaskHandle);
	
	/*手电筒*/
	xTaskCreate((TaskFunction_t)Light_Task, "Light_Task", 256, NULL, 22, &Light_TaskHandle);
	
	/*led灯*/
	xTaskCreate((TaskFunction_t)Led_Task, "Led_Task", 256, NULL, 22, &Led_TaskHandle);
	
	/*舵机*/
	xTaskCreate((TaskFunction_t)Servo_Task, "Servo_Task", 256, NULL, 22, &Servo_TaskHandle);
	
	/*音乐*/
	xTaskCreate((TaskFunction_t)Music_Task, "Music_Task", 256, NULL, 22, &Music_TaskHandle);
	
	/*电机*/
	xTaskCreate((TaskFunction_t)Motor_Task, "Motor_Task", 256, NULL, 22, &Motor_TaskHandle);
	
	/*陀螺仪*/
	xTaskCreate((TaskFunction_t)Gyro_Task, "Gyro_Task", 256, NULL, 22, &Gyro_TaskHandle);
	
	vTaskStartScheduler();
	
	
}

///***************************************************主时钟相关***************************************************///
/*定时器中断回调函数*/
void MainClock_Callback(TimerHandle_t xTimer)
{
	ticks = xTaskGetTickCount();
}

/*获取主时钟的分钟分量，范围为0~99*/
char str_min[10];
char* GetMinStr(uint32_t seconds)
{
	uint32_t min = seconds / 60;
	min %= 100;
	
	sprintf(str_min, "%02d", min);
	return str_min;
}

/*获取主时钟的秒数分量，范围为0~59*/
char str_sec[10];
char* GetSecStr(uint32_t seconds)
{
	uint32_t sec = seconds % 60;
	
	sprintf(str_sec, "%02d", sec);
	return str_sec;
}
///***************************************************按键相关***************************************************///

BaseType_t pxHigherPriorityTaskWoken = pdTRUE;
uint32_t lastKeyTime;

void EXTI0_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		if(GPIO_ReadInputDataBit(GPIOB, KeyExit)==RESET)
		{
			//中断中不建议使用延时，要使用延时的话需要配置定时器的优先级高于EXITA
			//vTaskDelay(20);
			//while(GPIO_ReadInputDataBit(GPIOB, KeyExit)==RESET);
			//vTaskDelay(20);
			
			if(ticks-lastKeyTime>40)
			{
				lastKeyTime = ticks;
			
				xQueueSendFromISR(Keys_QueueHandle, &ExitDown, &pxHigherPriorityTaskWoken);
				printf("KeyExit Down\r\n");		
			}
		}
		
		EXTI_ClearITPendingBit(EXTI_Line0);
	}	
}

void EXTI1_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line1) == SET)
	{
		if(GPIO_ReadInputDataBit(GPIOB, KeyEnter)==RESET)
		{		
			if(ticks-lastKeyTime>40)
			{
				lastKeyTime = ticks;
			
				xQueueSendFromISR(Keys_QueueHandle, &EnterDown, &pxHigherPriorityTaskWoken);
				printf("KeyEnter Down\r\n");		
			}
		}
		
		EXTI_ClearITPendingBit(EXTI_Line1);
	}	
}

void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line11) == SET)
	{
		if(GPIO_ReadInputDataBit(GPIOB, KeyLeft)==RESET)
		{
			if(ticks-lastKeyTime>40)
			{
				lastKeyTime = ticks;
			
				xQueueSendFromISR(Keys_QueueHandle, &LeftDown, &pxHigherPriorityTaskWoken);
				printf("KeyLeft Down\r\n");			
			}	
		}
		
		EXTI_ClearITPendingBit(EXTI_Line11);
	}	
	if (EXTI_GetITStatus(EXTI_Line10) == SET)
	{
		if(GPIO_ReadInputDataBit(GPIOB, KeyRight)==RESET)
		{
			if(ticks-lastKeyTime>40)
			{
				lastKeyTime = ticks;
			
				xQueueSendFromISR(Keys_QueueHandle, &RightDown, &pxHigherPriorityTaskWoken);
				printf("KeyRight Down\r\n");	
			}
			
		}
		
		EXTI_ClearITPendingBit(EXTI_Line10);
	}
}


