#ifndef __MYU8G2_H
#define __MYU8G2_H
#include "u8g2.h"

extern u8g2_t u8g2;
//uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr);
void MyU8G2_Init();
void MyU8G2_SetFontBig();
void MyU8G2_SetFontTiny();

extern const uint8_t gImage_test[518];
extern const uint8_t gImage_lightoff[518];
extern const uint8_t gImage_lighton[518];
extern const uint8_t gImage_leftMove[30];
extern const uint8_t gImage_rightMove[30];

extern const uint8_t gImage_lightItem[];
extern const uint8_t gImage_ledItem[];
extern const uint8_t gImage_servosItem[];
extern const uint8_t gImage_musicItem[];
extern const uint8_t gImage_motorItem[]; 
extern const uint8_t gImage_gyroItem[];

extern const uint8_t gImage_GuYongZhe[];
extern const uint8_t gImage_DoLaAMeng[];
extern const uint8_t gImage_XiYangYang[];

#endif