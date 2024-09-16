#include "stm32f10x.h"                  // Device header
#include "Delay.h"



void MyI2C_Init(uint16_t SDA_Pin, uint16_t SCL_Pin)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = SDA_Pin | SCL_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, SDA_Pin | SCL_Pin);//SCL和SDA初始化后默认为高电平（释放总线状态）
}


uint8_t MyI2C_SDA_R(uint16_t SDA_Pin)
{
	uint8_t bit = GPIO_ReadInputDataBit(GPIOB, SDA_Pin);
	//Delay_us(10);
	return bit;
}


void MyI2C_SDA_W(uint8_t bit, uint16_t SDA_Pin)
{
	
	GPIO_WriteBit(GPIOB, SDA_Pin, (BitAction)bit);	
											//只有当bit==0x00时, (BitAction)bit才为0，其余全为1
	//Delay_us(10);							//延时保证从机有时间读取
}

uint8_t MyI2C_SCL_R(uint16_t SCL_Pin)
{
	uint8_t bit = GPIO_ReadInputDataBit(GPIOB, SCL_Pin);
	//Delay_us(10);
	return bit;
}


void MyI2C_SCL_W(uint8_t bit, uint16_t SCL_Pin)
{
	GPIO_WriteBit(GPIOB, SCL_Pin, (BitAction)bit);	
											//只有当bit==0x00时, (BitAction)bit才为0，其余全为1													
	//Delay_us(10);							//延时保证从机有时间读取
}

/*主机设置起始位，设置完成后SCL、SDA为低电平*/
void MyI2C_Start(uint16_t SDA_Pin, uint16_t SCL_Pin)
{
	MyI2C_SDA_W(1, SDA_Pin);								//释放SDA、SCL，先拉高SDA再拉高SCL使得此函数也适用于重新起始				
	MyI2C_SCL_W(1, SCL_Pin);
	MyI2C_SDA_W(0, SDA_Pin);								//先拉低SDA，再拉低SCL
	MyI2C_SCL_W(0, SCL_Pin);	
}

/*主机发送信息，发送完毕后SCL为低电平，SDA未知*/
void MyI2C_SendData(uint8_t Data, uint16_t SDA_Pin, uint16_t SCL_Pin)
{
	for(uint8_t i=0; i<8; i++)					//先写入高位，再写入低位
	{
		MyI2C_SDA_W(Data & (0x80 >> i), SDA_Pin);		//在SCL低电平时写入信息到SDA
		MyI2C_SCL_W(1, SCL_Pin);							//写入完毕后拉高SCL,等待从机读取	
		MyI2C_SCL_W(0, SCL_Pin);							//不管延时时间内从机有没有读到，将SCL拉低继续写入下一位
	}			
}

/*主机接收应答，接收完毕后SCL为低电平，SDA未知*/
uint8_t MyI2C_RecArc(uint16_t SDA_Pin, uint16_t SCL_Pin)
{
	MyI2C_SDA_W(1, SDA_Pin);								//先释放主机SDA，控制权交给从机
																					//拉高SCL前，从机写入应答响应到SDA
	MyI2C_SCL_W(1, SCL_Pin);								//拉高SCL，接收从机应答响应
	uint8_t arc = MyI2C_SDA_R(SDA_Pin);			//读取从机应答响应，无响应为1，有为0
	MyI2C_SCL_W(0, SCL_Pin);								//拉低SCL，置于空闲状态
	return arc;
}

/*主机接收数据，接收完毕后SCL为低电平，SDA未知*/
uint8_t MyI2C_RecData(uint16_t SDA_Pin, uint16_t SCL_Pin)
{
	uint8_t Data = 0x00;
	MyI2C_SDA_W(1, SDA_Pin);								//确保主机已释放SDA
	for(int i=0; i<8; i++)
	{
																					//在SCL低电平期间，从机写入数据到SDA
		MyI2C_SCL_W(1, SCL_Pin);							//拉高SCL，高电平期间从机不允许更改数据，主机读取数据			
		if(MyI2C_SDA_R(SDA_Pin) == 1)					//接收从机发送的数据
			Data = Data | (0x80 >> i);					//将Data第7-i位置1
		MyI2C_SCL_W(0, SCL_Pin);							//读取完毕后将SCL拉低(置可读)，给从机更改数据的时间
	}
	return Data;
}

/*主机发送应答，发送完毕后SCL为低电平，SDA未知*/
void MyI2C_SendArc(uint8_t bit, uint16_t SDA_Pin, uint16_t SCL_Pin)
{
	MyI2C_SDA_W(bit, SDA_Pin);							//在SCL低电平时写入应答位到SDA
	MyI2C_SCL_W(1, SCL_Pin);								//拉高SCL，给从机读取的时间
	MyI2C_SCL_W(0, SCL_Pin);								//发送完毕后拉低SCL
}

/*主机设置结束位，设置完成后SCL、SDA为高电平*/
void MyI2C_End(uint16_t SDA_Pin, uint16_t SCL_Pin)
{
	MyI2C_SDA_W(0, SDA_Pin);								//确保SDA为低电平
	MyI2C_SCL_W(1, SCL_Pin);								//先拉高SCL，再拉高SDA
	MyI2C_SDA_W(1, SDA_Pin);
}
