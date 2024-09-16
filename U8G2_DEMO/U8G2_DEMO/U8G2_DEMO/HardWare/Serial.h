#ifndef __SERIAL_H
#define __SERIAL_H

extern uint8_t RecPackData[];

void Serial_Init();
void Send_Byte(uint8_t Data);
void Send_Array(uint8_t* Data, uint8_t Length);
void Send_String(char* Data);
void Send_Num(uint32_t Data, uint8_t Length);
uint8_t Get_RecFlag();
uint8_t GetRecData();
void SendPackage(uint8_t* Data);
void Get_RecPackData();
#endif