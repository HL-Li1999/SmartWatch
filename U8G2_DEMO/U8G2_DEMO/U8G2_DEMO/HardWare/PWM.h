#ifndef __PWM_H
#define __PWM_H

void PWM_Init();
void PWM_SetDuty(uint16_t compare);
void PWM_SetFreq(uint16_t frequance);
#endif