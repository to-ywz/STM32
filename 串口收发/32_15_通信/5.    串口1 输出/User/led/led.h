#ifndef _LED_H
#define _LED_H

#include "iap15w4k58s4.h"
#include "intrins.h"
#include "delay.h"

sbit LED1     =  P5^0;        // 定义LED1
sbit LED2     =  P5^1;        // 定义LED2
sbit LED3     =  P5^2;        // 定义LED3

#define LED1ON  LED1 = 0;\
				LED2 = 1;\
				LED3 = 1;
				
#define LED2ON  LED1 = 1;\
				LED2 = 0;\
				LED3 = 1;

#define LED3ON  LED1 = 1;\
				LED2 = 1;\
				LED3 = 0;

#define LEDON   LED1 = 0;\
				LED2 = 0;\
				LED3 = 0;

#define LEDOFF  LED1 = 1;\
				LED2 = 1;\
				LED3 = 1;

#endif 