#include "stm32f10x.h"                  // Device header
#include "Delay.h"

uint32_t KeyNum;

void LightControllerInit()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		
	GPIO_InitTypeDef GPIO_InitStructre;
	GPIO_InitStructre.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructre.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_4;
	GPIO_InitStructre.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructre);
}

void Light0On()
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
}

void Light0Off()
{
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
}

void Light4On()
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
}

void Light4Off()
{
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
}

void BottonControllerInit()
{
	GPIO_InitTypeDef GPIO_InitStructre;
	GPIO_InitStructre.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructre.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructre.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructre);
}

void LightTurn()
{
	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_0)==0)
	{
		Light0Off();
		Light4On();
	}
	else
	{
		Light0On();
		Light4Off();
	}
}


void BottonControl()
{
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7)==0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7)==0);
		Delay_ms(20);
		LightTurn();
	}
}