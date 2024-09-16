#ifndef __CREATETASK_H
#define __CREATETASK_H

#include "FreeRTOS.h"
#include "timers.h"
#include "queue.h"

extern uint32_t ticks;
extern TaskHandle_t MainITF_TaskHandle;				//主界面任务句柄
extern QueueHandle_t Keys_QueueHandle;				//按键消息队列句柄
extern TaskHandle_t KeysSend_TaskHandle;			//按键消息发送任务句柄
extern TaskHandle_t Menu_TaskHandle;					//菜单任务句柄
extern TaskHandle_t Light_TaskHandle;					//手电筒任务句柄
extern TaskHandle_t Led_TaskHandle;						//Led灯任务句柄
extern TaskHandle_t Servo_TaskHandle;				//舵机任务句柄
extern TaskHandle_t Music_TaskHandle;					//音乐任务句柄
extern TaskHandle_t Motor_TaskHandle;					//电机任务句柄
extern TaskHandle_t Gyro_TaskHandle;					//陀螺仪任务句柄


char* GetMinStr(uint32_t seconds);
char* GetSecStr(uint32_t seconds);

void CreateAllTask();

#endif