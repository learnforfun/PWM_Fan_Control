#include "pwm.h"
#include "stc15f2k60s2.h"
typedef unsigned char BYTE;
void InitPWM()
{
//freq = sysclk/2; io = p1.1
 	ACC = P_SW1;
	ACC &= ~(CCP_S0 | CCP_S1);
	P_SW1 = ACC;
	CL = 0;
	CH = 0;
	CMOD = 0X02;

	PCA_PWM0 = 0X00;
	CCAP0H = CCAP0L = 0xcc;
	CCAPM0 = 0X42;

	CR = 1;
}
void UpdateDC(int newDC)
{
	CR = 0;
	CCAP0H = CCAP0L = newDC;
	CR = 1;
}
//int Str2Int(char* pStr, int n)
//{
// 	int result = 0;
//	int fact = 1;
//	int i = n-1;
//	for(; i >= 0; i--)
//	{		 
//		result += fact*(pStr[i]-'0');
//		fact *= 10;		
//	}
//	if(result <0 || result >100)//if input illegal,set result 20,means that duty cycle is 20%
//		result = 20;
//	return result;
//}
//void ClearString(char* pStr, int n)
//{
// 	int i = 0;
//	for(; i < n; i++)
//		pStr[i] = '\0';
//}