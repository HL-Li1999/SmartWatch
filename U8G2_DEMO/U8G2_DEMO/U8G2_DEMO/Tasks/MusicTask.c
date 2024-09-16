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
		M3,M3,S,S,M1,M2,M1,M3,M3,S, //都是勇敢的
		M1,M2,M1,M2,M3,L6,M1,L6,M1,L6,M1,M2,M1,L7,L7,S,S, //你额头的伤口你的不同你犯的错
	  M3,M3,S,S,M1,M2,M1,M3,M3,S, //都不必隐藏
	  M1,M2,M1,M2,M3,L6,M1,L6,M1,L6,M1,M3,M2,L7,L7,S,S, //你破旧的玩偶你的面具你的自我
		L6,M1,M6,M6,M6,M5,M6,M6,M5,M6,M5,M6,M5,M3,M3,M3,S,S, //他们说要带着光驯服每一头怪兽
		L6,M1,M6,M6,M6,M5,M6,M5,M7,M7,M7,M6,M7,M7,M6,M3,M3,S,S, //他们说要缝好你的伤没人爱小丑
		M3,M5,M3,M2,M3,M2,M3,M2,S, //为何孤独不可光荣
		M3,M5,M3,M5,M3,M2,M3,M2,M3,M2,S, //人只有不完美值得歌颂
		M1,M2,M3,L6,M1,M3,M2,M3,M2,M1,M1,L6,L6,S,S,//谁说污泥满身的不算英雄
		M6,M7,H1,H2,M7,H1,H1,S,	//爱你孤身走暗巷
		H1,M7,H1,H2,M7,H1,H1,S, //爱你不跪的模样
		H1,H2,H3,H2,H3,H2,H3,H3,H2,H3,H5,H3,S, //爱你对峙过绝望不肯哭一场
		M6,M7,H1,H2,M7,H1,H1,H1,M7,H1,H2,M7,H1,H1,S, //爱你破烂的衣裳却敢堵命运的枪
	  H1,H2,H3,H2,H3,H2,H3,H3,H2,H3,H5,H3,S, //爱你和我那么像缺口一样
		H5,H3, //去吗
		H5,H3,S, //配吗
	  H5,H3,H5,H6,H3,H5,S, //这褴褛的披风
		H5,H3,//战吗
	  H5,H3,S, //战啊
		H5,H3,H5,H6,H3,H5,H5,H5,H3,H2,H2,H2,H1,H3,H3,H2,H2,H2,H1,H1,M6,M6,S,S, //以最卑微的梦致那黑夜中的呜咽与怒吼
		H5,H5,H3,H2,H2,H2,H1,H3,H3,H2,H2,H2,H1,H1,M6,M6,S,S, //谁说站在光里才算英雄
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
		//如果f=0则不发出声音
		TIM_SetAutoreload(TIM3,1);
		TIM_SetCompare1(TIM3,1);
	}
	else
	{ 
		//发出指定频率的声音
		TIM_SetAutoreload(TIM3,(1000000/f));  		//设置ARR的值
		uint16_t sound_num = ((100-sound)/100.0)*(1000000/f); //设置占空比，sound：10~90。
																														//sound越大占空比越小，越大声
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
		
		//按下除了enter的任一按键：切换歌曲或退出
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
			
		//播放
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

		
		//暂停
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