#ifndef PWM_H_
#define PWM_H_

//#include "stc15f2k60s2.h"
#define CCP_S0 0X10
#define CCP_S1 0X20

void InitPWM();//configur pwm frequency and chose io
void UpdateDC(int dc);//Update pwm duty cycle
//int Str2Int(char* pStr, int n);
//void ClearString(char* pStr, int n);
#endif