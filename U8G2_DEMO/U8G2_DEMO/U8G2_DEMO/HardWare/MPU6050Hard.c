#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h"

#define MPU6050_ADDRESS 0xD0

/*等待标志位变为有效，超时时间为TimeOut*/
void WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint16_t TimeOut = 1000;
	while(I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
	{
		TimeOut --;
		if(TimeOut == 0)
			break;
	}
}


/*给MPU寄存器写入值*/
void WriteData2MPUHard(uint8_t regAddress, uint8_t data)
{	
	/*硬件I2C自动接收应答响应，响应失败会调用特定中断，因此不用判断标志位*/
	I2C_GenerateSTART(I2C2, ENABLE);
	WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);						//EV5，启动完毕
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);	
	WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);		//EV6，发送地址
	
	I2C_SendData(I2C2,regAddress);
	WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);				//EV8，发送数据
	
	I2C_SendData(I2C2,data);
	WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);					//EV8_2，发送的最后一个数据
	
	I2C_GenerateSTOP(I2C2, ENABLE);
}

/*读取MPU寄存器的值*/
uint8_t ReadDataFromMPUHard(uint8_t regAddress)
{	
	/*硬件I2C自动接收应答响应，响应失败会调用特定中断，因此不用判断标志位*/
	I2C_GenerateSTART(I2C2, ENABLE);
	WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);						//EV5，启动完毕
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);							//选择发送模式
	WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);		//EV6，发送地址完毕
	
	I2C_SendData(I2C2,regAddress);
	WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);					//EV8_2，发送的最后一个数据
		
	I2C_GenerateSTART(I2C2, ENABLE);									//重新起始
	WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);						//EV5，启动完毕

	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Receiver);	//选择接收模式
	WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);		//EV6，发送地址
	
	//看手册图246，EV6_1在只接收一个字节数据时在EV6后自动生成，此时要清除响应和停止条件的产生位
	I2C_AcknowledgeConfig(I2C2, DISABLE);								//在接收最后一个字节之前提前将应答失能
	I2C_GenerateSTOP(I2C2, ENABLE);										//在接收最后一个字节之前提前申请停止条件
	
	WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);					//EV7，接收到数据
	uint8_t data = I2C_ReceiveData(I2C2);
	
	I2C_AcknowledgeConfig(I2C2, ENABLE);								//将应答恢复为使能，为了不影响后续可能产生的读取多字节操作
	
	return data;

}

void MPUHard_Init()
{
	/*GPIO初始化*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;			//I2C采用开漏输出，硬件I2C采用复用输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);					
	
	/*I2C初始化*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);	
	I2C_InitTypeDef I2C_InitStructure;						
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;				
	I2C_InitStructure.I2C_ClockSpeed = 50000;				//时钟速度，选择为50KHz
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;		//时钟占空比，选择Tlow/Thigh = 2
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;				//应答，选择使能
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	
															//应答地址，选择7位，从机模式下才有效
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;				//自身地址，从机模式下才有效
	I2C_Init(I2C2, &I2C_InitStructure);						
	
	/*I2C使能*/
	I2C_Cmd(I2C2, ENABLE);									
	
	/*MPU6050寄存器初始化，需要对照MPU6050手册的寄存器描述配置，此处仅配置了部分重要的寄存器*/
	WriteData2MPUHard(MPU6050_PWR_MGMT_1, 0x01);				//电源管理寄存器1，取消睡眠模式，选择时钟源为X轴陀螺仪
	WriteData2MPUHard(MPU6050_PWR_MGMT_2, 0x00);				//电源管理寄存器2，保持默认值0，所有轴均不待机
	WriteData2MPUHard(MPU6050_SMPLRT_DIV, 0x09);				//采样率分频寄存器，配置采样率
	WriteData2MPUHard(MPU6050_CONFIG, 0x06);					//配置寄存器，配置DLPF
	WriteData2MPUHard(MPU6050_GYRO_CONFIG, 0x18);				//陀螺仪配置寄存器，选择满量程为±2000°/s
	WriteData2MPUHard(MPU6050_ACCEL_CONFIG, 0x18);				//加速度计配置寄存器，选择满量程为±16g
}

/*获取MPU6050的ID*/
uint8_t GetMPUHard_ID()
{
	return ReadDataFromMPUHard(MPU6050_WHO_AM_I);
}

/*获取MPU6050的数据*/
void GetMPUHard_Data(int16_t* AccX, int16_t* AccY, int16_t* AccZ,
					int16_t* GyroX, int16_t* GyroY, int16_t* GyroZ)
{
	uint8_t DataH, DataL;								//定义数据高8位和低8位的变量
	
	DataH = ReadDataFromMPUHard(MPU6050_ACCEL_XOUT_H);		//读取加速度计X轴的高8位数据
	DataL = ReadDataFromMPUHard(MPU6050_ACCEL_XOUT_L);		//读取加速度计X轴的低8位数据
	*AccX = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = ReadDataFromMPUHard(MPU6050_ACCEL_YOUT_H);		//读取加速度计Y轴的高8位数据
	DataL = ReadDataFromMPUHard(MPU6050_ACCEL_YOUT_L);		//读取加速度计Y轴的低8位数据
	*AccY = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = ReadDataFromMPUHard(MPU6050_ACCEL_ZOUT_H);		//读取加速度计Z轴的高8位数据
	DataL = ReadDataFromMPUHard(MPU6050_ACCEL_ZOUT_L);		//读取加速度计Z轴的低8位数据
	*AccZ = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = ReadDataFromMPUHard(MPU6050_GYRO_XOUT_H);		//读取陀螺仪X轴的高8位数据
	DataL = ReadDataFromMPUHard(MPU6050_GYRO_XOUT_L);		//读取陀螺仪X轴的低8位数据
	*GyroX = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = ReadDataFromMPUHard(MPU6050_GYRO_YOUT_H);		//读取陀螺仪Y轴的高8位数据
	DataL = ReadDataFromMPUHard(MPU6050_GYRO_YOUT_L);		//读取陀螺仪Y轴的低8位数据
	*GyroY = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = ReadDataFromMPUHard(MPU6050_GYRO_ZOUT_H);		//读取陀螺仪Z轴的高8位数据
	DataL = ReadDataFromMPUHard(MPU6050_GYRO_ZOUT_L);		//读取陀螺仪Z轴的低8位数据
	*GyroZ = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
}




