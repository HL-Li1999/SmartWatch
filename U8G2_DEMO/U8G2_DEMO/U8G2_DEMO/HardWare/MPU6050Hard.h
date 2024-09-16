#ifndef __MPU6050HARD_H
#define __MPU6050HARD_H


void MPUHard_Init();
void WriteData2MPUHard(uint8_t regAdress, uint8_t data);
uint8_t ReadDataFromMPUHard(uint8_t regAdress);
uint8_t GetMPUHard_ID();
void GetMPUHard_Data(int16_t* AccX, int16_t* AccY, int16_t* AccZ,
	int16_t* GyroX, int16_t* GyroY, int16_t* GyroZ);


#endif