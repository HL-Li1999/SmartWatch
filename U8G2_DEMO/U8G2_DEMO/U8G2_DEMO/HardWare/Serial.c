#include "stm32f10x.h"                  // Device header
#include <stdio.h>

uint8_t RecFlag;
//uint8_t RecData;
uint8_t RecPackState;
uint8_t RecPackData[] = {0x00, 0x00, 0x00, 0x00};


void Serial_Init()
{
	/*GPIO初始化*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;						//串口通信发送端复用输出驱动
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;						//空闲时电平为高电平
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	/*USART初始化*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);				//只有USTART1为APB2，其余USTART均为APB1
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  		//无奇偶校验位则为8位，有则为9位
	USART_Init(USART1, &USART_InitStructure);
	
	
	/*USART中断配置*/
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	
	/*NVIC配置*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	
	/*USART使能*/
	USART_Cmd(USART1, ENABLE);
}

/*串口助手HEX模式接收*/
void Send_Byte(uint8_t Data)
{
	USART_SendData(USART1, Data);										//参数输入为uint16_t，但实际发送为后9位，因为设置了无奇偶校验位所以这里为8位
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		//等待发送完成
	/*下次写入数据寄存器会自动清除发送完成标志位，故此循环后，无需清除标志位*/
}

/*串口助手HEX模式接收*/
void Send_Array(uint8_t* Data, uint8_t Length)
{
	for(uint8_t i=0; i<Length; i++)
		Send_Byte(Data[i]);
}

/*串口助手文本模式接收*/
void Send_String(char* Data)
{
	for(uint8_t i=0; Data[i]!='\0'; i++)
		Send_Byte(Data[i]);
}

uint32_t Pow(uint32_t x, uint32_t y)
{
	uint32_t res = 1;
	for(uint8_t i=0; i<y; i++)
		res *= x;
	return res;
}

/*串口助手文本模式接收*/
void Send_Num(uint32_t Data, uint8_t Length)
{
	for(uint8_t i=0; i<Length; i++)
	{
		Send_Byte(Data/Pow(10,Length-i-1)%10 + '0');
	}		
		
}


void USART1_IRQHandler()
{
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
	{		
		uint8_t RecData = USART_ReceiveData(USART1);				//读取数据后RXNE标志位会自动置0
		
		if(RecPackState == 0)
		{
			if(RecData == 0xFF)
			{
				RecPackState = 1;
			}
		}
		else if(RecPackState >= 1 && RecPackState <= 4)
		{
			RecPackData[RecPackState-1] = RecData;
			RecPackState ++;
		}
		else if(RecPackState >= 5)
		{
			if(RecData == 0xFE)
			{
				RecPackState = 0;
				RecFlag = 1;
			}
		}
	}
	
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);		//清除中断标志位，否则会一直触发
}

uint8_t Get_RecFlag()
{
	if(RecFlag==1)
	{
		RecFlag = 0;
		return 1;
	}
	return 0;
}

/*uint8_t GetRecData()
{
	return RecData;
}*/

void SendPackage(uint8_t* Data)
{
	Send_Byte(0xFF);
	Send_Array(Data,4);
	Send_Byte(0xFE);
}

void Get_RecPackData()
{

}

uint8_t Get_RecPackState()
{
	return RecPackState;
}

/*使用串口调试需要重定义fputc函数*/
/*#define USE_REGISTER

int fputc(int ch, FILE *f)
{
	#ifdef USE_REGISTER
	while((USART1->SR&0x40)==0);
	USART1->DR = (u8)ch;
	#else
	USART_SendData(USART1, (unsigned char)ch);
	while(!USART_GetFlagStatus(USART1, USART_FLAG_TC))
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
	#endif
	
	return ch;
}*/

