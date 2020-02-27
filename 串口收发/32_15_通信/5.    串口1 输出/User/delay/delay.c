/************************************************************************************
Copyright,  LCE STUDIO   
�������ƣ���ʱ����(����Ƶ��18.432MHz)
����˵��: ��ʱ������΢��
����汾��1.0 ��2014/06��
����������rob1983
�Ա����̣�http://shop67639654.taobao.com 
************************************************************************************/

#include "iap15w4k58s4.h"	    // ��Ƭ��STC15F2K60S2ͷ�ļ�,���Բ��ټ���reg51.h
#include <intrins.h>			    // �����ͷ�ļ���,��ʹ��_nop_�⺯��
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



