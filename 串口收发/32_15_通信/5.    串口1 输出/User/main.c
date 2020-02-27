/************************************************************************************
Copyright,  LCE STUDIO   
�������ƣ����ڷ������ݣ�����Ƶ��18.432MHz�� 
����˵��: ͨ������1���͡�Everything is possible!�� 
          ������9600������λ8����żЧ���ޣ�ֹͣλ1��������������  
����汾��1.0 ��2015/06��
����������rob1983
�Ա����̣�http://shop67639654.taobao.com 
************************************************************************************/


#include "iap15w4k58s4.h"	    // ��Ƭ��iap15w4k58s4ͷ�ļ�,���Բ��ټ���reg51.h
#include <intrins.h>			    // �����ͷ�ļ���,��ʹ��_nop_�⺯��
#include "key.h"
#include "led.h"
#include "delay.h"		        // ��ʱ����ͷ�ļ�
#include "uart.h"		          // ����ͨ�ź���ͷ�ļ�
#define MAXBUF 20

int count = 1;
char RecBuf[MAXBUF];
unsigned char Index;
void InitGPIO();
void Timer0Init();

void main()		                                        // ������     
{
	InitGPIO();
    DelayMS(100);							   
	UartInit();
	Timer0Init();
	ET0 = 1;
	ES = 1;
	EA = 1;
	DelayUS(100);
	SendString("����һ��15��32˫��ͨ�ŵĲ���\r\n");
	DelayUS(100);
	
    while (1)		                                      // ��ѭ�� 
    {
		switch (count%20)
		{
			case 0:
				if (IsPressedKEY1())
				{
					SendData('A');
					LED1ON;
				}
				break;
			case 3:
				if (IsPressedKEY2())
				{
					SendData('B');
					LED2ON;
				}
				break;
			case 6:
				if (IsPressedKEY3())
				{
					SendData('C');
					LED3ON;
				}
				break;
			case 8:
				if (IsPressedKEY4())
				{
					LEDOFF;
				}
				break;
			default: break;
		}
		
		if ('A' == RecBuf[Index-1])
		{
			LEDON;
		}
		if ('B' == RecBuf[Index-11])
		{
			LEDOFF;
		}
    }
}

void RecDataInterrupt() interrupt 4 using 1
{
	LED1ON;
	ES = 0;
	if (RI)
	{
		RI = 0;
		RecBuf[Index++] = SBUF;
		if (Index > 20)
		{
			Index = 0;
		}
	}
	ES = 1;
	LEDOFF;
}

void Timer1(void) interrupt 1 using 2
{
	ET0 = 0;
	TR0 = 0;
	count == 30 ? count = 0 : count++;
	TR0 = 1;
	ET0 = 1;
}

void Timer0Init(void)		//1ms@12.000MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x20;		//���ö�ʱ��ֵ
	TH0 = 0xD1;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
}


void InitGPIO()
{
	P0M0 = 0;
	P0M1 = 0;
	P1M0 = 0;
	P1M1 = 0;
	P2M0 = 0;
	P2M1 = 0;
	P3M0 = 0;
	P3M1 = 0;
	P4M0 = 0;
	P4M1 = 0;
	P5M0 = 0;
	P5M1 = 0;
}