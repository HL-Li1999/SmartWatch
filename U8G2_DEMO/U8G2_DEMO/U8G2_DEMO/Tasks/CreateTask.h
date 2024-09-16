#ifndef __CREATETASK_H
#define __CREATETASK_H

#include "FreeRTOS.h"
#include "timers.h"
#include "queue.h"

extern uint32_t ticks;
extern TaskHandle_t MainITF_TaskHandle;				//������������
extern QueueHandle_t Keys_QueueHandle;				//������Ϣ���о��
extern TaskHandle_t KeysSend_TaskHandle;			//������Ϣ����������
extern TaskHandle_t Menu_TaskHandle;					//�˵�������
extern TaskHandle_t Light_TaskHandle;					//�ֵ�Ͳ������
extern TaskHandle_t Led_TaskHandle;						//Led��������
extern TaskHandle_t Servo_TaskHandle;				//���������
extern TaskHandle_t Music_TaskHandle;					//����������
extern TaskHandle_t Motor_TaskHandle;					//���������
extern TaskHandle_t Gyro_TaskHandle;					//������������


char* GetMinStr(uint32_t seconds);
char* GetSecStr(uint32_t seconds);

void CreateAllTask();

#endif