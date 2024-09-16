#ifndef __MENUTASK_H
#define __MENUTASK_H

typedef struct
{
	uint16_t id;
	uint16_t w;
	uint16_t h;
	uint16_t x;
	uint16_t y;
	const uint8_t* img;
}Item;



void Init_AllItem();
void Menu_Task(void* parameter);

#endif