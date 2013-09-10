
#include	"config.h"
#include	"USART1.h"
#include	"delay.h"
#include	"pwm.h"



/*************	����˵��	**************

˫����ȫ˫���жϷ�ʽ�շ�ͨѶ����

ͨ��PC��MCU��������, MCU�յ���ͨ�����ڰ��յ�������ԭ������.

******************************************/

/*************	���س�������	**************/


/*************	���ر�������	**************/


/*************	���غ�������	**************/



/*************  �ⲿ�����ͱ������� *****************/

void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//�ṹ����
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//ģʽ,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//ʹ�ò�����,   BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//������, һ�� 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//�����ʼӱ�, ENABLE��DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//�ж�����,   ENABLE��DISABLE
	COMx_InitStructure.UART_Polity    = PolityLow;			//�ж����ȼ�, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//�л��˿�,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(����ʹ���ڲ�ʱ��)
	COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;		//�ڲ���·RXD��TXD, ���м�, ENABLE,DISABLE
	USART_Configuration(USART1, &COMx_InitStructure);		//��ʼ������1 USART1,USART2

	//PrintString1("STC15F2K60S2 UART1 Test Prgramme!\r\n");	//SUART1����һ���ַ���
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
//		if(COM1.RX_TimeOut > 0)		//��ʱ����
//		{
//			if(--COM1.RX_TimeOut == 0)
//			{
//				if(COM1.RX_Cnt > 0)
//				{
//					for(i=0; i<COM1.RX_Cnt; i++)	TX1_write2buff(RX1_Buffer[i]);	//�յ�������ԭ������
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



