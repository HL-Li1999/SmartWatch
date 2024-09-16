//#include "stm32f10x.h"                  // Device header
//#include "FreeRTOS.h"
//#include "task.h"
//#include "bsp_usart.h"


//TaskHandle_t startTask_handle = NULL;
//TaskHandle_t testTask_handle = NULL;

//void test_task(void *pvParameters)
//{
//	BaseType_t xReturn;
//	size_t heapSize;
//	uint32_t* mallocAddress;
//	while(1)
//	{
//		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == RESET)
//		{
//			vTaskDelay(20);
//			while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == RESET);
//			vTaskDelay(20);
//			
//			heapSize = xPortGetFreeHeapSize();
//			printf("未分配内存大小为%d\r\n", heapSize);
//			
//			mallocAddress = pvPortMalloc(1024);
//			printf("分配内存地址为%#x\r\n", (int)mallocAddress);
//			
//			heapSize = xPortGetFreeHeapSize();
//			printf("未分配内存大小为%d\r\n", heapSize);
//		}
//		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == RESET)
//		{
//			vTaskDelay(20);
//			while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == RESET);
//			vTaskDelay(20);
//			
//			printf("释放内存\r\n");
//			vPortFree(mallocAddress);
//			
//			heapSize = xPortGetFreeHeapSize();
//			printf("未分配内存大小为%d\r\n", heapSize);
//		}
//	}
//}

//void start_task(void *pvParameters)
//{
//	taskENTER_CRITICAL();	
//	
//	xTaskCreate(test_task, "test_task", 512, NULL, 2, &testTask_handle);
//	
//	vTaskDelete(startTask_handle);

//	taskEXIT_CRITICAL();
//}

//void MemManager_Demo()
//{	
//	USART_Config();
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//	xTaskCreate(start_task, "start_task", 512, NULL, 1, &startTask_handle);
//	
//	if(startTask_handle != NULL)
//	{
//		printf("start_task创建成功\r\n");
//		vTaskStartScheduler();
//	}
//}
