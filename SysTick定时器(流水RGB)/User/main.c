/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   控制多彩流水灯
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "bsp_systick.h"

void Delay(__IO u32 nCount); 

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	/* LED 端口初始化 */
	LED_GPIO_Config();
	SysTick_Init();
	/* 控制LED灯 */
	while (1)
	{
		LED1( ON );			 // 亮 
		Delay_ms(1000);
		LED1( OFF );		  // 灭
		Delay_ms(1000);

		LED2( ON );			// 亮 
		Delay_ms(1000);
		LED2( OFF );		  // 灭

		LED3( ON );			 // 亮 
		Delay_ms(1000);
		LED3( OFF );		  // 灭	

		/*轮流显示 红绿蓝黄紫青白 颜色*/
		LED_RED;
		Delay_ms(1000);
		
		LED_GREEN;
		Delay_ms(1000);
		
		LED_BLUE;
		Delay_ms(1000);
		
		LED_YELLOW;
		Delay_ms(1000);
		
		LED_PURPLE;
		Delay_ms(1000);
				
		LED_CYAN;
		Delay_ms(1000);
		
		LED_WHITE;
		Delay_ms(1000);
		
		LED_RGBOFF;
		Delay_ms(1000);
	}
}

void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/

