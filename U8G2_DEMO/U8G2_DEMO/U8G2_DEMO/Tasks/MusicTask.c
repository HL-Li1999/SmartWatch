#include "stm32f10x.h"                  // Device header
#include "MyU8G2.h"
#include "CreateTask.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "MusicTask.h"

uint16_t play_index=1;
uint16_t Music_Sound=50;

const uint16_t music_GuYongZhe[]=
{
		M3,M3,S,S,M1,M2,M1,M3,M3,S, //�����¸ҵ�
		M1,M2,M1,M2,M3,L6,M1,L6,M1,L6,M1,M2,M1,L7,L7,S,S, //���ͷ���˿���Ĳ�ͬ�㷸�Ĵ�
	  M3,M3,S,S,M1,M2,M1,M3,M3,S, //����������
	  M1,M2,M1,M2,M3,L6,M1,L6,M1,L6,M1,M3,M2,L7,L7,S,S, //���ƾɵ���ż�������������
		L6,M1,M6,M6,M6,M5,M6,M6,M5,M6,M5,M6,M5,M3,M3,M3,S,S, //����˵Ҫ���Ź�ѱ��ÿһͷ����
		L6,M1,M6,M6,M6,M5,M6,M5,M7,M7,M7,M6,M7,M7,M6,M3,M3,S,S, //����˵Ҫ��������û�˰�С��
		M3,M5,M3,M2,M3,M2,M3,M2,S, //Ϊ�ι¶����ɹ���
		M3,M5,M3,M5,M3,M2,M3,M2,M3,M2,S, //��ֻ�в�����ֵ�ø���
		M1,M2,M3,L6,M1,M3,M2,M3,M2,M1,M1,L6,L6,S,S,//˭˵��������Ĳ���Ӣ��
		M6,M7,H1,H2,M7,H1,H1,S,	//��������߰���
		H1,M7,H1,H2,M7,H1,H1,S, //���㲻���ģ��
		H1,H2,H3,H2,H3,H2,H3,H3,H2,H3,H5,H3,S, //������Ź��������Ͽ�һ��
		M6,M7,H1,H2,M7,H1,H1,H1,M7,H1,H2,M7,H1,H1,S, //�������õ�����ȴ�Ҷ����˵�ǹ
	  H1,H2,H3,H2,H3,H2,H3,H3,H2,H3,H5,H3,S, //���������ô��ȱ��һ��
		H5,H3, //ȥ��
		H5,H3,S, //����
	  H5,H3,H5,H6,H3,H5,S, //�����ڵ�����
		H5,H3,//ս��
	  H5,H3,S, //ս��
		H5,H3,H5,H6,H3,H5,H5,H5,H3,H2,H2,H2,H1,H3,H3,H2,H2,H2,H1,H1,M6,M6,S,S, //���΢�������Ǻ�ҹ�е�������ŭ��
		H5,H5,H3,H2,H2,H2,H1,H3,H3,H2,H2,H2,H1,H1,M6,M6,S,S, //˭˵վ�ڹ������Ӣ��
};

const uint16_t music_DoLaAMeng[]=
{
	  L5,M1,M1,M3,M6,M3,M5,
		M5,M6,M5,M3,M4,M3,M2,
		L6,M2,M2,M4,M7,M7,M6,M5,
		M4,M4,M4,M3,L6,L7,L7,M1,
		M2,M2,M2,M2,
		L5,M1,M1,M3,M6,M3,M5,
		M5,M6,M5,M3,M4,M3,M2,
		L6,M2,M2,M4,M7,M7,M6,M5,
		M4,M4,M4,M3,L7,M2,
		M1,M1,M1,M1,
		M6,M6,M5,M4,M5,M6,M5,
		M2,M3,M4,M2,M5,M5,
		M6,M5,M2,M3,M4,M2,
		M5,M5,M5,M5,
		M6,S,M5,S,M4,S,S,
	  M2,M7,M6,M5,M6,M5,M4,
	  S,M5,M6,M3,M2,
		M1,M1,M1,M1,	
};

const uint16_t music_XiYangYang[]=
{
		M3,M5,M5,S,M2,M5,M5,S,M3,M5,M5,M2,M5,M5,S,M3,M5,M5,S,M2,M5,M5,S,M3,M5,M5,M2,M5,M5,
		S,S,S,S,S,H1,H1,H1,H1,M7,M5,M3,M5,M5,M5,M5,M5,S,M7,M7,M7,M7,M6,M5,M3,M5,H2,H2,H2,
		S,H3,H3,H3,H3,H2,H1,M6,M4,H2,H2,H2,S,M7,M7,M7,M7,M6,M5,M4,M3,M5,M5,M5,S,H1,H1,H1,H1,M7,M5,M3,
		M5,M5,M5,M5,S,H1,H1,H1,H1,M7,M5,M3,M5,H2,H2,H2,S,H3,H3,H3,H3,H2,H1,M6,M4,H2,H2,H2,
		S,M7,M7,M7,M7,H1,H2,H1,H1,H1,S,H1,M6,M6,H1,H1,M6,M6,H1,H1,H2,H2,H1,H1,M6,H1,S,H1,
		M5,M5,H1,H1,M5,M5,H1,H1,H2,H2,H1,H1,H2,H3,S,H1,M4,M4,H1,H1,M4,M4,H1,H1,H3,H3,H2,H3,H5,
		H5,H5,H5,H3,H2,H2,H2,H2,
};

void SetBeep(const uint16_t f, uint16_t sound)
{
	
	if(f==0)
	{ 
		//���f=0�򲻷�������
		TIM_SetAutoreload(TIM3,1);
		TIM_SetCompare1(TIM3,1);
	}
	else
	{ 
		//����ָ��Ƶ�ʵ�����
		TIM_SetAutoreload(TIM3,(1000000/f));  		//����ARR��ֵ
		uint16_t sound_num = ((100-sound)/100.0)*(1000000/f); //����ռ�ձȣ�sound��10~90��
																														//soundԽ��ռ�ձ�ԽС��Խ����
		TIM_SetCompare1(TIM3,sound_num); //
	}
}

void PlayMusic(uint8_t musicItem)
{	
	uint16_t musicLength;
	switch(musicItem)
	{
		case 1: 
			musicLength = sizeof(music_GuYongZhe)/sizeof(music_GuYongZhe[0]);
			break;
		case 2: 
			musicLength = sizeof(music_DoLaAMeng)/sizeof(music_DoLaAMeng[0]);
			break;
		case 3:
			musicLength = sizeof(music_XiYangYang)/sizeof(music_XiYangYang[0]);
			break;
	}

	uint8_t recData;
	uint8_t playFlag = 1;
	
	for(uint16_t i=0;i<musicLength;i++)
	{
		xQueueReceive(Keys_QueueHandle, &recData, 0);
		if(recData == EnterDown)
		{
			recData = 0;
			playFlag = 1 - playFlag;		
		}
		
		//���³���enter����һ�������л��������˳�
		if(recData != 0)
		{
			recData = 0;
			SetBeep(0,0);
			if(recData == LeftDown & play_index<3)
				play_index += play_index;
			if(recData == RightDown & play_index>1)
				play_index -= play_index;
			if(recData == ExitDown)
			{
				vTaskResume(Menu_TaskHandle);
				vTaskSuspend(NULL);
			}	
			
			
			return;
		}
			
		//����
		if(playFlag == 1)	
		{
			switch(musicItem)
			{
				case 1:
					SetBeep(music_GuYongZhe[i],Music_Sound);
					break;
				case 2:
					SetBeep(music_DoLaAMeng[i],Music_Sound);
					break;
				case 3:
					SetBeep(music_XiYangYang[i],Music_Sound);
					break;
			}
			
		}

		
		//��ͣ
		if(playFlag == 0)
		{
			SetBeep(0,0);
			i--;
		}
			
		
		vTaskDelay(500);
	}
	SetBeep(0,0);
}


void Music_Task(void* parameter)
{
	uint16_t musicIndex;
	uint8_t recData;
	while(1)
	{
		u8g2_ClearBuffer(&u8g2);
		MyU8G2_SetFontTiny();
		u8g2_DrawStr(&u8g2,35,10,"Playing:");
		
		switch(play_index)
		{
			case 1:
				u8g2_DrawXBMP(&u8g2,39,20,50,37,gImage_GuYongZhe);
				break;
			case 2:
				u8g2_DrawXBMP(&u8g2,39,20,50,38,gImage_DoLaAMeng);
				break;
			case 3:
				u8g2_DrawXBMP(&u8g2,39,20,50,35,gImage_XiYangYang);
				break;
		}
		
		u8g2_DrawXBMP(&u8g2,0,30,23,10,gImage_leftMove);
		u8g2_DrawXBMP(&u8g2,104,30,23,10,gImage_rightMove);
		u8g2_SendBuffer(&u8g2);
		
		xQueueReceive(Keys_QueueHandle, &recData, 0);
		if(recData == ExitDown)
		{
			recData = 0;
			vTaskResume(Menu_TaskHandle);
			vTaskSuspend(NULL);
		}	
		if(recData == EnterDown)
		{
			recData = 0;

			PlayMusic(play_index);
		}
		if(recData == RightDown)
		{
			recData = 0;
			
			if(play_index<3)
				play_index += 1;
		}
		if(recData == LeftDown)
		{
			recData = 0;
			
			if(play_index>1)
				play_index -= 1;
		}
		vTaskDelay(250);
		
	}
}