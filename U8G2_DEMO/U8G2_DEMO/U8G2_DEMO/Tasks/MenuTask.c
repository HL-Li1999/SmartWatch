#include "stm32f10x.h"                  // Device header
#include "MyU8G2.h"
#include "CreateTask.h"
#include "main.h"
#include <stdio.h>
#include "MenuTask.h"

Item leftMove,rightMove,light,led,servo,musicbeep,motor,gyro;
static char* itemName[] = {"light","led","servo","music","motor","gyro"};
uint8_t idNow;

void InitItem(Item* item, uint8_t id, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t* img)
{
	item->id = id;
	item->x = x;
	item->y = y;
	item->w = w;
	item->h = h;
	item->img = img;
}

void InitAllItems()
{	
	InitItem(&leftMove,  -1, 0,   10, 23, 10, gImage_leftMove);
	InitItem(&rightMove, -1, 104, 10, 23, 10, gImage_rightMove);
	
	InitItem(&light,   0, 50,      20,   30, 30, gImage_lightItem);
	InitItem(&led,     1, 50+55,   24+5, 30, 30, gImage_ledItem);
	InitItem(&servo,   2, 50+55*2, 24+5, 30, 30, gImage_servosItem);
	InitItem(&musicbeep,   3, 50+55*3, 24+5, 30, 30, gImage_musicItem);
	InitItem(&motor,   4, 50+55*4, 24+5, 30, 30, gImage_motorItem);
	InitItem(&gyro,    5, 50+55*5, 24+5, 30, 30, gImage_gyroItem);
}


void DrawUI()
{
	MyU8G2_SetFontTiny();
	u8g2_ClearBuffer(&u8g2);
	
	u8g2_DrawFrame(&u8g2, 45, 14, 40, 40);	
	u8g2_DrawStr(&u8g2, 50, 10 , itemName[idNow]);
	u8g2_DrawXBMP(&u8g2,leftMove.x,leftMove.y,leftMove.w,leftMove.h,leftMove.img);
	u8g2_DrawXBMP(&u8g2,rightMove.x,rightMove.y,rightMove.w,rightMove.h,rightMove.img);
	
	u8g2_DrawXBMP(&u8g2,light.x,light.y,light.w,light.h,light.img);
	u8g2_DrawXBMP(&u8g2,led.x,led.y,led.w,led.h,led.img);
	u8g2_DrawXBMP(&u8g2,servo.x,servo.y,servo.w,servo.h,servo.img);
	u8g2_DrawXBMP(&u8g2,musicbeep.x,musicbeep.y,musicbeep.w,musicbeep.h,musicbeep.img);
	u8g2_DrawXBMP(&u8g2,motor.x,motor.y,motor.w,motor.h,motor.img);
	u8g2_DrawXBMP(&u8g2,gyro.x,gyro.y,gyro.w,gyro.h,gyro.img);
	
	u8g2_SendBuffer(&u8g2);
}

void ItemMove_Right(uint8_t distance, uint8_t step)
{
	for(uint8_t i=0;i<distance/step;i++)
	{
		light.x += step;
		led.x += step;
		servo.x += step;
		musicbeep.x += step;
		motor.x += step;
		gyro.x += step;
		
		switch(idNow){
			case 1:
				light.y -= (step/5);
				led.y += (step/5);
				break;
			case 2:
				led.y -= (step/5);
				servo.y += (step/5);
				break;
			case 3:
				servo.y -= (step/5);
				musicbeep.y += (step/5);
				break;
			case 4:
				musicbeep.y -= (step/5);
				motor.y += (step/5);
				break;
			case 5:
				motor.y -= (step/5);
				gyro.y += (step/5);
				break;
		}
				
		DrawUI();
		vTaskDelay(5);
	}
}

void ItemMove_Left(uint8_t distance, uint8_t step)
{
	for(uint8_t i=0;i<distance/step;i++)
	{
		light.x -= step;
		led.x -= step;
		servo.x -= step;
		musicbeep.x -= step;
		motor.x -= step;
		gyro.x -= step;
		
		switch(idNow){
			case 0:
				light.y += (step/5);
				led.y -= (step/5);
				break;
			case 1:
				led.y += (step/5);
				servo.y -= (step/5);
				break;
			case 2:
				servo.y += (step/5);
				musicbeep.y -= (step/5);
				break;	
			case 3:
				musicbeep.y += (step/5);
				motor.y -= (step/5);
				break;
			case 4:
				motor.y += (step/5);
				gyro.y -= (step/5);
				break;
		}
					
		DrawUI();
		vTaskDelay(5);
	}
}

void Menu_Task(void* parameter)
{
	InitAllItems();
	idNow = 0;
	uint8_t itemLength = sizeof(itemName) / sizeof(itemName[0]);
	
	uint8_t recData;
	while(1)
	{
		DrawUI();
		
		xQueueReceive(Keys_QueueHandle, &recData, 0);
		if(recData == ExitDown)
		{
			recData = 0;
			vTaskResume(MainITF_TaskHandle);
			vTaskSuspend(NULL);
		}
		if(recData == EnterDown)
		{
			recData = 0;
			switch(idNow){
				case 0:
					vTaskResume(Light_TaskHandle);
					break;
				case 1:
					vTaskResume(Led_TaskHandle);
					break;
				case 2:
					vTaskResume(Servo_TaskHandle);
					break;
				case 3:
					vTaskResume(Music_TaskHandle);
					break;	
				case 4:
					vTaskResume(Motor_TaskHandle);
					break;
				case 5:
					vTaskResume(Gyro_TaskHandle);
					break;
			}
						
			vTaskSuspend(NULL);
		}
		if(recData == RightDown)
		{
			if(idNow>0)
			{	
				recData = 0;
				ItemMove_Right(55,5);
				idNow--;
			}
		}
		if(recData == LeftDown)
		{
			if(idNow<itemLength-1)
			{
				recData = 0;			
				ItemMove_Left(55,5);
				idNow++;
			}
		}
			
		vTaskDelay(250);
	}
}