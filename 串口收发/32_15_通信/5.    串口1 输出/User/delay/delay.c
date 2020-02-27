/************************************************************************************
Copyright,  LCE STUDIO   
程序名称：延时函数(晶振频率18.432MHz)
功能说明: 延时毫秒与微秒
程序版本：1.0 （2014/06）
阿里旺旺：rob1983
淘宝店铺：http://shop67639654.taobao.com 
************************************************************************************/

#include "iap15w4k58s4.h"	    // 单片机STC15F2K60S2头文件,可以不再加入reg51.h
#include <intrins.h>			    // 加入此头文件后,可使用_nop_库函数
#include <stdio.h>
#include "delay.h"

//void NOP30(unsigned char times)
//{
//	while(times--)
//	{
//		_nop_();_nop_();_nop_();_nop_();_nop_();
//		_nop_();_nop_();_nop_();_nop_();_nop_();
//		_nop_();_nop_();_nop_();_nop_();_nop_();
//		_nop_();_nop_();_nop_();_nop_();_nop_();
//		_nop_();_nop_();_nop_();_nop_();_nop_();
//		_nop_();_nop_();_nop_();_nop_();_nop_();
//	}
//}

void Delay1us()		//@12.000MHz
{
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}


void Delay1ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 12;
	j = 169;
	do
	{
		while (--j);
	} while (--i);
}

void DelayMS(int t)
{
	while(t--)
	{
		Delay1ms();
	}
}

void DelayUS(int t)
{
	while(t--)
	{
		Delay1us();
	}
}



