/************************************************************************************
Copyright,  LCE STUDIO   
程序名称：串口发送数据（晶振频率18.432MHz） 
功能说明: 通过串口1发送“Everything is possible!” 
          波特率9600，数据位8，奇偶效验无，停止位1，数据流控制无  
程序版本：1.0 （2015/06）
阿里旺旺：rob1983
淘宝店铺：http://shop67639654.taobao.com 
************************************************************************************/


#include "iap15w4k58s4.h"	    // 单片机iap15w4k58s4头文件,可以不再加入reg51.h
#include <intrins.h>			    // 加入此头文件后,可使用_nop_库函数
#include "key.h"
#include "led.h"
#include "delay.h"		        // 延时函数头文件
#include "uart.h"		          // 串行通信函数头文件
#define MAXBUF 20

int count = 1;
char RecBuf[MAXBUF];
unsigned char Index;
void InitGPIO();
void Timer0Init();

void main()		                                        // 主函数     
{
	InitGPIO();
    DelayMS(100);							   
	UartInit();
	Timer0Init();
	ET0 = 1;
	ES = 1;
	EA = 1;
	DelayUS(100);
	SendString("这是一个15与32双机通信的测试\r\n");
	DelayUS(100);
	
    while (1)		                                      // 主循环 
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
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x20;		//设置定时初值
	TH0 = 0xD1;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
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