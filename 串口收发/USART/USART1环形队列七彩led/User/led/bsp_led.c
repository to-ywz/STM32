#include "./led/bsp_led.h"

/**
  * @brief  初始化控制LED的IO
  * @param  无
  * @retval 无
  */
void LED_GPIO_Config(void)
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启LED相关的GPIO外设时钟*/
	RCC_AHB1PeriphClockCmd(LED1_GPIO_CLK |
							   LED2_GPIO_CLK |
							   LED3_GPIO_CLK,
						   ENABLE);

	/*选择要控制的GPIO引脚*/
	GPIO_InitStructure.GPIO_Pin = LED1_PIN;

	/*设置引脚模式为输出模式*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	/*设置引脚的输出类型为推挽输出*/
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	/*设置引脚为上拉模式*/
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	/*设置引脚速率为2MHz */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

	/*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
	GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);

	/*选择要控制的GPIO引脚*/
	GPIO_InitStructure.GPIO_Pin = LED2_PIN;
	GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);

	/*选择要控制的GPIO引脚*/
	GPIO_InitStructure.GPIO_Pin = LED3_PIN;
	GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);

	/*关闭RGB灯*/
	LED_RGBOFF;
}

void _7_Color_LED(int n)
{
	switch (n)
	{
	case 1:
		LED_RED;
		//Delay_ms(1000);
		;
		break;
	case 2:
		LED_GREEN;
		//Delay_ms(1000);
		;
		break;
	case 3:
		LED_BLUE;
		//Delay_ms(1000);
		;
		break;
	case 4:
		LED_YELLOW;
		//Delay_ms(1000);
		;
		break;
	case 5:
		LED_PURPLE;
		//Delay_ms(1000);
		;
		break;
	case 6:
		LED_CYAN;
		//Delay_ms(1000);
		break;
	case 7:
		LED_WHITE;
		//Delay_ms(1000);
		break;
	case 0:
		LED_RGBOFF;
		break;
	}
}

/*********************************************END OF FILE**********************/
