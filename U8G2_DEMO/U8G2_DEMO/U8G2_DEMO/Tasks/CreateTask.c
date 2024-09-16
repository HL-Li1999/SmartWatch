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


/*���*/
//����1��Ϊʲô����Ҫ������һ���Ҳ�����extern
TimerHandle_t MainClock_TimerHandle = NULL;		//��ʱ�Ӷ�ʱ�����
TaskHandle_t MainITF_TaskHandle = NULL;				//������������
QueueHandle_t Keys_QueueHandle = NULL;				//������Ϣ���о��
TaskHandle_t KeysSend_TaskHandle = NULL;			//������Ϣ����������
TaskHandle_t Menu_TaskHandle = NULL;					//�˵�������
TaskHandle_t Light_TaskHandle = NULL;					//�ֵ�Ͳ������
TaskHandle_t Led_TaskHandle = NULL;						//Led��������
TaskHandle_t Servo_TaskHandle = NULL;				//���������
TaskHandle_t Music_TaskHandle = NULL;					//����������
TaskHandle_t Motor_TaskHandle = NULL;					//���������
TaskHandle_t Gyro_TaskHandle = NULL;					//������������

uint32_t ticks;

/*��������*/
void MainClock_Callback(TimerHandle_t xTimer);
char* GetMinStr(uint32_t seconds);
char* GetSecStr(uint32_t seconds);
void KeysSend_Task();
void KeysReceive_Task();

	
void CreateAllTask()
{	
	/*��ʱ��*/
	//����2��Ϊʲô�������д���ʱ�ӵĻ����ᱻ����أ������䣺����������������ѭ��while(1)�Ļ�����
	MainClock_TimerHandle = xTimerCreate("MainClock_Handle", 1000, pdTRUE, (void*)1, (TimerCallbackFunction_t)MainClock_Callback);
	if(MainClock_TimerHandle!=NULL)
		xTimerStart(MainClock_TimerHandle, 0);
	

	/*������Ӧ*/
	Keys_QueueHandle = xQueueCreate(1,sizeof(uint8_t));
	
	/*������*/
	xTaskCreate((TaskFunction_t)MainITF_Task, "MainITF_Task", 256, NULL, 22, &MainITF_TaskHandle);
	
	/*�˵�*/
	xTaskCreate((TaskFunction_t)Menu_Task, "Menu_Task", 256, NULL, 22, &Menu_TaskHandle);
	
	/*�ֵ�Ͳ*/
	xTaskCreate((TaskFunction_t)Light_Task, "Light_Task", 256, NULL, 22, &Light_TaskHandle);
	
	/*led��*/
	xTaskCreate((TaskFunction_t)Led_Task, "Led_Task", 256, NULL, 22, &Led_TaskHandle);
	
	/*���*/
	xTaskCreate((TaskFunction_t)Servo_Task, "Servo_Task", 256, NULL, 22, &Servo_TaskHandle);
	
	/*����*/
	xTaskCreate((TaskFunction_t)Music_Task, "Music_Task", 256, NULL, 22, &Music_TaskHandle);
	
	/*���*/
	xTaskCreate((TaskFunction_t)Motor_Task, "Motor_Task", 256, NULL, 22, &Motor_TaskHandle);
	
	/*������*/
	xTaskCreate((TaskFunction_t)Gyro_Task, "Gyro_Task", 256, NULL, 22, &Gyro_TaskHandle);
	
	vTaskStartScheduler();
	
	
}

///***************************************************��ʱ�����***************************************************///
/*��ʱ���жϻص�����*/
void MainClock_Callback(TimerHandle_t xTimer)
{
	ticks = xTaskGetTickCount();
}

/*��ȡ��ʱ�ӵķ��ӷ�������ΧΪ0~99*/
char str_min[10];
char* GetMinStr(uint32_t seconds)
{
	uint32_t min = seconds / 60;
	min %= 100;
	
	sprintf(str_min, "%02d", min);
	return str_min;
}

/*��ȡ��ʱ�ӵ�������������ΧΪ0~59*/
char str_sec[10];
char* GetSecStr(uint32_t seconds)
{
	uint32_t sec = seconds % 60;
	
	sprintf(str_sec, "%02d", sec);
	return str_sec;
}
///***************************************************�������***************************************************///

BaseType_t pxHigherPriorityTaskWoken = pdTRUE;
uint32_t lastKeyTime;

void EXTI0_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		if(GPIO_ReadInputDataBit(GPIOB, KeyExit)==RESET)
		{
			//�ж��в�����ʹ����ʱ��Ҫʹ����ʱ�Ļ���Ҫ���ö�ʱ�������ȼ�����EXITA
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


