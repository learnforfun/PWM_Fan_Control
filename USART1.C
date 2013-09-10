
#include "USART1.h"
#include "pwm.h"
#include	"stdio.h"

COMx_Define	COM1;
u8	xdata TX1_Buffer[COM_TX1_Lenth];	//发送缓冲
u8 	xdata RX1_Buffer[COM_RX1_Lenth];	//接收缓冲

//////////////////
//	My Var
int  dcs[101] = {0xff,
				 0xfe,0xfb,0xf9,0xf6,0xf4,0xf1,0xef,0xec,0xe9,0xe7,
				 0xe4,0xe2,0xdf,0xdd,0xda,0xd8,0xd5,0xd2,0xd0,0xcd,
				 0xcb,0xc8,0xc6,0xc3,0xc0,0xbe,0xbb,0xb9,0xb6,0xb4,
				 0xb1,0xaf,0xac,0xa9,0xa7,0xa4,0xa2,0x9f,0x9d,0x9a,
				 0x98,0x95,0x92,0x90,0x8d,0x8b,0x88,0x86,0x83,0x80,
				 0x7e,0x7b,0x79,0x76,0x74,0x71,0x6f,0x6c,0x69,0x67,
				 0x64,0x62,0x5f,0x5d,0x5a,0x58,0x55,0x52,0x50,0x4d,
				 0x4b,0x48,0x46,0x43,0x40,0x3e,0x3b,0x39,0x36,0x34,
				 0x31,0x2f,0x2c,0x29,0x27,0x24,0x22,0x1f,0x1d,0x1a,
				 0x18,0x15,0x12,0x10,0x0d,0x0b,0x08,0x06,0x03,0x00
				};
char index = 1;	  
int result = 0;
#define STR_LEN 50
char szTemp[STR_LEN] = {'\0'};
/////////////////
u8 USART_Configuration(u8 UARTx, COMx_InitDefine *COMx)
{
	u8	i;
	u32	j;
	
	if(UARTx == USART1)
	{
		COM1.id = 1;
		COM1.TX_read    = 0;
		COM1.TX_write   = 0;
		COM1.B_TX_busy  = 0;
		COM1.RX_Cnt     = 0;
		COM1.RX_TimeOut = 0;
		COM1.B_RX_OK    = 0;
		for(i=0; i<COM_TX1_Lenth; i++)	TX1_Buffer[i] = 0;
		for(i=0; i<COM_RX1_Lenth; i++)	RX1_Buffer[i] = 0;

		if(COMx->UART_Mode > UART_9bit_BRTx)	return 1;	//模式错误
		if(COMx->UART_Polity == PolityHigh)		PS = 1;	//高优先级中断
		else									PS = 0;	//低优先级中断
		SCON = (SCON & 0x3f) | COMx->UART_Mode;
		if((COMx->UART_Mode == UART_9bit_BRTx) ||(COMx->UART_Mode == UART_8bit_BRTx))	//可变波特率
		{
			j = (MAIN_Fosc / 4) / COMx->UART_BaudRate;	//按1T计算
			if(j >= 65536UL)	return 2;	//错误
			j = 65536UL - j;
			if(COMx->UART_BRT_Use == BRT_Timer1)
			{
				TR1 = 0;
				AUXR &= ~0x01;		//S1 BRT Use Timer1;
				TMOD &= ~(1<<6);	//Timer1 set As Timer
				TMOD &= ~0x30;		//Timer1_16bitAutoReload;
				AUXR |=  (1<<6);	//Timer1 set as 1T mode
				TH1 = (u8)(j>>8);
				TL1 = (u8)j;
				ET1 = 0;	//禁止中断
				TMOD &= ~0x40;	//定时
				INT_CLKO &= ~0x02;	//不输出时钟
				TR1  = 1;
			}
			else if(COMx->UART_BRT_Use == BRT_Timer2)
			{
				AUXR &= ~(1<<4);	//Timer stop
				AUXR |= 0x01;		//S1 BRT Use Timer2;
				AUXR &= ~(1<<3);	//Timer2 set As Timer
				AUXR |=  (1<<2);	//Timer2 set as 1T mode
				TH2 = (u8)(j>>8);
				TL2 = (u8)j;
				IE2  &= ~(1<<2);	//禁止中断
				AUXR &= ~(1<<3);	//定时
				AUXR |=  (1<<4);	//Timer run enable
			}
			else return 2;	//错误
		}
		else if(COMx->UART_Mode == UART_ShiftRight)
		{
			if(COMx->BaudRateDouble == ENABLE)	AUXR |=  (1<<5);	//固定波特率SysClk/2
			else								AUXR &= ~(1<<5);	//固定波特率SysClk/12
		}
		else if(COMx->UART_Mode == UART_9bit)	//固定波特率SysClk*2^SMOD/64
		{
			if(COMx->BaudRateDouble == ENABLE)	PCON |=  (1<<7);	//固定波特率SysClk/32
			else								PCON &= ~(1<<7);	//固定波特率SysClk/64
		}
		if(COMx->UART_Interrupt == ENABLE)	ES = 1;	//允许中断
		else								ES = 0;	//禁止中断
		if(COMx->UART_RxEnable == ENABLE)	REN = 1;	//允许接收
		else								REN = 0;	//禁止接收
		P_SW1 = (P_SW1 & 0x3f) | (COMx->UART_P_SW & 0xc0);	//切换IO
		if(COMx->UART_RXD_TXD_Short == ENABLE)	PCON2 |=  (1<<4);	//内部短路RXD与TXD, 做中继, ENABLE,DISABLE
		else									PCON2 &= ~(1<<4);
		return	0;
	}
	return 3;	//其它错误
}


/*************** 装载串口发送缓冲 *******************************/

void TX1_write2buff(u8 dat)	//写入发送缓冲，指针+1
{
	TX1_Buffer[COM1.TX_write] = dat;	//装发送缓冲
	if(++COM1.TX_write >= COM_TX1_Lenth)	COM1.TX_write = 0;

	if(COM1.B_TX_busy == 0)		//空闲
	{  
		COM1.B_TX_busy = 1;		//标志忙
		TI = 1;					//触发发送中断
	}
}

void PrintString1(u8 *puts)
{
    for (; *puts != 0;	puts++)  TX1_write2buff(*puts); 	//遇到停止符0结束
}



/********************* UART1中断函数************************/
void UART1_int (void) interrupt UART1_VECTOR
{
//	int i, j;
	if(RI)
	{
		RI = 0;
		if(COM1.B_RX_OK == 0)
		{
			if(COM1.RX_Cnt >= COM_RX1_Lenth)	COM1.RX_Cnt = 0;
			RX1_Buffer[COM1.RX_Cnt++] = SBUF;
//			if(SBUF == '.')
//			{
//				PrintString1("Dot\r\n");
//				sprintf(szTemp, "Change Duty Cycle to: %d%%\r\n#READY#\r\n", (int)COM1.RX_Cnt);
//				PrintString1(szTemp);
//				UpdateDC(result);
//				fact = 1;
//				result = 0;
//				COM1.RX_Cnt = 0;
//			}
//			else
//			{
//				 
//				result += fact*(SBUF-'0');
//				sprintf(szTemp, "%d\r\n", result);
//				PrintString1(szTemp);
//				fact *= 10;
//				sprintf(szTemp, "count: %c\r\n", SBUF);
//				PrintString1(szTemp);
//				PrintString1("#\r\n");
//			}
			switch(index)
			{
			 	case 1:
				{
				 	result += (SBUF-'0')*10;
					index = 0;break;
				}
				case 0:
				{
				 	result += (SBUF-'0');
					index = 1;
					sprintf(szTemp, "Change Duty Cycle to: %d%%\r\n#READY#\r\n", result);
					PrintString1(szTemp);
					UpdateDC(dcs[result]);
					result = 0;
				}
			}
			COM1.RX_TimeOut = TimeOutSet1;
		}
		
	}

	if(TI)
	{
		TI = 0;
		if(COM1.TX_read != COM1.TX_write)
		{
		 	SBUF = TX1_Buffer[COM1.TX_read];
			if(++COM1.TX_read >= COM_TX1_Lenth)		COM1.TX_read = 0;
		}
		else	COM1.B_TX_busy = 0;
	}
}




