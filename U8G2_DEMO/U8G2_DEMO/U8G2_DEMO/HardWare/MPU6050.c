#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "MPU6050_Reg.h"

#define MPU6050_Adress 0xD0
#define MPU_SDA_Pin GPIO_Pin_12
#define MPU_SCL_Pin GPIO_Pin_13



/*给MPU寄存器写入值*/
void WriteData2MPU(uint8_t regAdress, uint8_t data)
{
	MyI2C_Start(MPU_SDA_Pin, MPU_SCL_Pin);
	MyI2C_SendData(MPU6050_Adress, MPU_SDA_Pin, MPU_SCL_Pin);
	MyI2C_RecArc(MPU_SDA_Pin, MPU_SCL_Pin);
	MyI2C_SendData(regAdress, MPU_SDA_Pin, MPU_SCL_Pin);
	MyI2C_RecArc(MPU_SDA_Pin, MPU_SCL_Pin);
	MyI2C_SendData(data, MPU_SDA_Pin, MPU_SCL_Pin);
	MyI2C_RecArc(MPU_SDA_Pin, MPU_SCL_Pin);
	MyI2C_End(MPU_SDA_Pin, MPU_SCL_Pin);
}

/*读取MPU寄存器的值*/
uint8_t ReadDataFromMPU(uint8_t regAdress)
{
	/*指定要读的寄存器地址*/
	MyI2C_Start(MPU_SDA_Pin, MPU_SCL_Pin);
	MyI2C_SendData(MPU6050_Adress, MPU_SDA_Pin, MPU_SCL_Pin);
	MyI2C_RecArc(MPU_SDA_Pin, MPU_SCL_Pin);
	MyI2C_SendData(regAdress, MPU_SDA_Pin, MPU_SCL_Pin);
	MyI2C_RecArc(MPU_SDA_Pin, MPU_SCL_Pin);
	
	/*切换为读模式需要重新起始*/
	MyI2C_Start(MPU_SDA_Pin, MPU_SCL_Pin);
	MyI2C_SendData(MPU6050_Adress | 0x01, MPU_SDA_Pin, MPU_SCL_Pin);
	MyI2C_RecArc(MPU_SDA_Pin, MPU_SCL_Pin);
	uint8_t data = MyI2C_RecData(MPU_SDA_Pin, MPU_SCL_Pin);
	MyI2C_SendArc(1, MPU_SDA_Pin, MPU_SCL_Pin);
	MyI2C_End(MPU_SDA_Pin, MPU_SCL_Pin);
	
	return data;
}

void MPU_Init()
{
	MyI2C_Init(MPU_SDA_Pin, MPU_SCL_Pin);
	
	/*MPU6050寄存器初始化，需要对照MPU6050手册的寄存器描述配置，此处仅配置了部分重要的寄存器*/
	WriteData2MPU(MPU6050_PWR_MGMT_1, 0x01);	//电源管理寄存器1，取消睡眠模式，选择时钟源为X轴陀螺仪
	WriteData2MPU(MPU6050_PWR_MGMT_2, 0x00);	//电源管理寄存器2，保持默认值0，所有轴均不待机
	WriteData2MPU(MPU6050_SMPLRT_DIV, 0x09);	//采样率分频寄存器，配置采样率
	WriteData2MPU(MPU6050_CONFIG, 0x06);		//配置寄存器，配置DLPF
	WriteData2MPU(MPU6050_GYRO_CONFIG, 0x18);	//陀螺仪配置寄存器，选择满量程为±2000°/s
	WriteData2MPU(MPU6050_ACCEL_CONFIG, 0x18);	//加速度计配置寄存器，选择满量程为±16g
}

/*获取MPU6050的ID*/
uint8_t GetMPU_ID()
{
	return ReadDataFromMPU(0x75);
}

/*获取MPU6050的数据*/
void GetMPU_Data(int16_t* AccX, int16_t* AccY, int16_t* AccZ,
					int16_t* GyroX, int16_t* GyroY, int16_t* GyroZ)
{
	uint8_t DataH, DataL;								//定义数据高8位和低8位的变量
	
	DataH = ReadDataFromMPU(MPU6050_ACCEL_XOUT_H);		//读取加速度计X轴的高8位数据
	DataL = ReadDataFromMPU(MPU6050_ACCEL_XOUT_L);		//读取加速度计X轴的低8位数据
	*AccX = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = ReadDataFromMPU(MPU6050_ACCEL_YOUT_H);		//读取加速度计Y轴的高8位数据
	DataL = ReadDataFromMPU(MPU6050_ACCEL_YOUT_L);		//读取加速度计Y轴的低8位数据
	*AccY = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = ReadDataFromMPU(MPU6050_ACCEL_ZOUT_H);		//读取加速度计Z轴的高8位数据
	DataL = ReadDataFromMPU(MPU6050_ACCEL_ZOUT_L);		//读取加速度计Z轴的低8位数据
	*AccZ = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = ReadDataFromMPU(MPU6050_GYRO_XOUT_H);		//读取陀螺仪X轴的高8位数据
	DataL = ReadDataFromMPU(MPU6050_GYRO_XOUT_L);		//读取陀螺仪X轴的低8位数据
	*GyroX = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = ReadDataFromMPU(MPU6050_GYRO_YOUT_H);		//读取陀螺仪Y轴的高8位数据
	DataL = ReadDataFromMPU(MPU6050_GYRO_YOUT_L);		//读取陀螺仪Y轴的低8位数据
	*GyroY = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = ReadDataFromMPU(MPU6050_GYRO_ZOUT_H);		//读取陀螺仪Z轴的高8位数据
	DataL = ReadDataFromMPU(MPU6050_GYRO_ZOUT_L);		//读取陀螺仪Z轴的低8位数据
	*GyroZ = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
}




