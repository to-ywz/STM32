/**
  ******************************************************************************
  * @file    main.c
  * @author  秦殇
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   串口接发测试，串口接收到数据后马上回传。
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板
  *
  ******************************************************************************
  */

#include "stm32f4xx.h"
#include "string.h"
#include "./usart/bsp_debug_usart.h"
#include "./usart/rx_data_queue.h"
#include "./KEY/psb_key.h"
#include "./led/bsp_led.h"

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	int count = 7;
	char *str = "祖张依世界第一可爱A！\n";
	/*初始化USART 配置模式为 115200 8-N-1，中断接收*/
	Debug_USART_Config();
	KEY_GPIO_Config();
	rx_queue_init();
	LED_GPIO_Config();
	
	/* 发送一个字符串 */
	Usart_SendString(DEBUG_USART, "这是一个串口中断接收回显实验\n");
	printf("欢迎使用野火STM32开发板\n\n\n\n");

	while (1)
	{
		//获取数据并输出
		//实际应用中可参考pull data的方式获取数据进行处理
		if (KEY_ON == KEY_Scan(KEY1_GPIO_PORT, KEY1_PIN))
		{
			pull_data_from_queue();
			printf("ACounter: %d\nBCounter: %d\n", ACount, BCount);
			count = (count+MAX_COLOR+ACount-BCount)%MAX_COLOR;
			printf("count = %d\n", count);
			_7_Color_LED(count);
			ACount = BCount = 0;
		}
		if (KEY_ON == KEY_Scan(KEY2_GPIO_PORT, KEY2_PIN))
		{
			push_data_to_queue(str, strlen(str));
		}
	}
}



/*********************************************END OF FILE**********************/
