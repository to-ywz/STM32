/**
 * @brief  
 * @note   
 * @param  nCount: 
 * @retval None
 */
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"
#include "./KEY/psb_key.h"
void Delay(__IO u32 nCount);
void LED(int n);

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	//char input;
	//int count = 0;
	/* LED 端口初始化 */
	LED_GPIO_Config();
	KEY_GPIO_Config();
	Debug_USART_Config();
	/* 控制LED灯 */
	Usart_SendString(DEBUG_USART, "祖张依，我……想你\n");
	while (1)
	{
		if (KEY_ON == KEY_Scan(KEY1_GPIO_PORT, KEY1_PIN))
		{
			Usart_SendByte(DEBUG_USART, 'A');
			LED('A' - 64);
		}
		if (KEY_ON == KEY_Scan(KEY2_GPIO_PORT, KEY2_PIN))
		{
			Usart_SendByte(DEBUG_USART, 'B');
			LED('B' - 64);
		}
	}
}


void LED(int n)
{
	switch (n)
	{
	case 1:
		LED_RED;
		;
		break;
	case 2:
		LED_RGBOFF;
		;
		break;
	}
}
/*********************************************END OF FILE**********************/
