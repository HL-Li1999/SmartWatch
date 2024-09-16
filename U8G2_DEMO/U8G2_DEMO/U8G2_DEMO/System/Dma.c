#include "stm32f10x.h"                  // Device header

uint32_t Transfer_Size;

//stm32为32位平台，地址用uint32_t表示
void Dma_Init(uint32_t Adrr1, uint32_t Adrr2, uint32_t Size)
{
	Transfer_Size = Size;
	


	DMA_Cmd(DMA1_Channel1, DISABLE);
}

void Dma_Transfer()
{
	DMA_Cmd(DMA1_Channel1, DISABLE);										//DMA失能，在写入传输计数器之前，需要DMA暂停工作
	DMA_SetCurrDataCounter(DMA1_Channel1, Transfer_Size);					//写入传输计数器，指定将要转运的次数
	DMA_Cmd(DMA1_Channel1, ENABLE);											//DMA使能，开始工作
	
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);						//等待传输完成
	DMA_ClearFlag(DMA1_FLAG_TC1);
	
}