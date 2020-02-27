/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ���ƶ����ˮ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "bsp_systick.h"

void Delay(__IO u32 nCount); 

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();
	SysTick_Init();
	/* ����LED�� */
	while (1)
	{
		LED1( ON );			 // �� 
		Delay_ms(1000);
		LED1( OFF );		  // ��
		Delay_ms(1000);

		LED2( ON );			// �� 
		Delay_ms(1000);
		LED2( OFF );		  // ��

		LED3( ON );			 // �� 
		Delay_ms(1000);
		LED3( OFF );		  // ��	

		/*������ʾ ������������� ��ɫ*/
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

void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/

