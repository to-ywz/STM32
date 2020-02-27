#ifndef _KEY_H_
#define _KEY_H_

#include "iap15w4k58s4.h"
#include "intrins.h"
#include "delay.h"

sbit KEY1     =  P2^0;        // 按键1
sbit KEY2     =  P2^1;        // 按键2 	 
sbit KEY3     =  P2^2;        // 按键3
sbit KEY4     =  P2^3;        // 按键4

#define KEY_ON  0
#define KEY_OFF 1
#define UNLOCK	0
#define LOCKED  1

int IsPressedKEY1();
int IsPressedKEY2();
int IsPressedKEY3();
int IsPressedKEY4();

#endif