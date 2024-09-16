#ifndef __MYI2C_H
#define __MYI2C_H


void MyI2C_Init(uint16_t SDA_Pin, uint16_t SCL_Pin);

uint8_t MyI2C_SDA_R(uint16_t SDA_Pin);

void MyI2C_SDA_W(uint8_t bit, uint16_t SDA_Pin);

uint8_t MyI2C_SCL_R(uint16_t SCL_Pin);

void MyI2C_SCL_W(uint8_t bit, uint16_t SCL_Pin);

void MyI2C_Start(uint16_t SDA_Pin, uint16_t SCL_Pin);

void MyI2C_SendData(uint8_t Data, uint16_t SDA_Pin, uint16_t SCL_Pin);

uint8_t MyI2C_RecArc(uint16_t SDA_Pin, uint16_t SCL_Pin);

uint8_t MyI2C_RecData(uint16_t SDA_Pin, uint16_t SCL_Pin);

void MyI2C_SendArc(uint8_t bit, uint16_t SDA_Pin, uint16_t SCL_Pin);

void MyI2C_End(uint16_t SDA_Pin, uint16_t SCL_Pin);


#endif
