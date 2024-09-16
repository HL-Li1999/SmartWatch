#ifndef __MPU6050_H
#define __MPU6050_H


void MPU_Init();
void WriteData2MPU(uint8_t regAdress, uint8_t data);
uint8_t ReadDataFromMPU(uint8_t regAdress);
uint8_t GetMPU_ID();
void GetMPU_Data(int16_t* AccX, int16_t* AccY, int16_t* AccZ,
	int16_t* GyroX, int16_t* GyroY, int16_t* GyroZ);


#endif