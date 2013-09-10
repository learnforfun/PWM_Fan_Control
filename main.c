
#include	"config.h"
#include	"USART1.h"
#include	"delay.h"
#include	"pwm.h"



/*************	功能说明	**************

双串口全双工中断方式收发通讯程序。

通过PC向MCU发送数据, MCU收到后通过串口把收到的数据原样返回.

******************************************/

/*************	本地常量声明	**************/


/*************	本地变量声明	**************/


/*************	本地函数声明	**************/



/*************  外部函数和变量声明 *****************/

void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//结构定义
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//模式,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//使用波特率,   BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率, 一般 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//中断允许,   ENABLE或DISABLE
	COMx_InitStructure.UART_Polity    = PolityLow;			//中断优先级, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//切换端口,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(必须使用内部时钟)
	COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;		//内部短路RXD与TXD, 做中继, ENABLE,DISABLE
	USART_Configuration(USART1, &COMx_InitStructure);		//初始化串口1 USART1,USART2

	//PrintString1("STC15F2K60S2 UART1 Test Prgramme!\r\n");	//SUART1发送一个字符串
	PrintString1("Fan Speed Control System Start!\r\n");
}


/**********************************************/
void main(void)
{
//	int	i;
	
	UART_config();
	PrintString1("Initialize Duty Cycle to 20%\r\n");
	PrintString1("Input New Duty Cycle To Change Speed(0~99)\r\n");
	InitPWM();
	EA = 1;
	//UpdateDC(0x19);

	while (1)
	{
//		delay_ms(1);
//		if(COM1.RX_TimeOut > 0)		//超时计数
//		{
//			if(--COM1.RX_TimeOut == 0)
//			{
//				if(COM1.RX_Cnt > 0)
//				{
//					for(i=0; i<COM1.RX_Cnt; i++)	TX1_write2buff(RX1_Buffer[i]);	//收到的数据原样返回
//					i = Str2Int(RX1_Buffer, COM1.RX_Cnt);
//					//sprintf(szTemp, "Num: %d\r\n", i);
//					//PrintString1(szTemp);
//					//sprintf(szTemp, "Current duty cycle is: %d%%\r\n#READY#\r\n", i);
//					//PrintString1(szTemp);
//					//i = 256-256*i/100;
//					//UpdateDC(i);
//					//sprintf(szTemp, "Current duty cycle is: %x%%\r\n#READY#\r\n", i); 
//					//PrintString1(szTemp);
//				}
//				COM1.RX_Cnt = 0;
//			}
//		}
		P55 = 0;
		delay_ms(250);
		delay_ms(250);
//		delay_ms(250);
		P55 = 1;
		delay_ms(250);
		delay_ms(250);
//		delay_ms(250);
	}
}



