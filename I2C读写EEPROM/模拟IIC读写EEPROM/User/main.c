/**
  ******************************************************************************
  * @file    main.c
  * @author  秦殇
  * @version V1.0
  * @date    2020-01-10
  * @brief   读写EEPROM
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板
  *
  ******************************************************************************
  */

#include "stm32f4xx.h"
#include "psb_key.h"
#include "bsp_led.h"
#include "bsp_iic_ee.h"
#include "./usart/bsp_debug_usart.h"

static void Delay(uint32_t i);

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	uint16_t len;
	uint8_t RecData[256];
	uint8_t ReadBuf[256];
	KEY_GPIO_Config();
	LED_GPIO_Config();
	LED_BLUE;

	/*初始化USART 配置模式为 115200 8-N-1，中断接收*/
	Debug_USART_Config();

	printf("\r\n欢迎使用野火  STM32 F407 开发板。\r\n");
	printf("\r\n这是一个I2C外设(AT24C02)读写测试例程 \r\n");

	while (1)
	{	
		if (1 == KEY_Scan(KEY1_GPIO_PORT, KEY1_PIN))
		{
			MainFuncationGetData(RecData, &len);
			ee_Write(RecData, len);
			printf("\r\nlen = %d\r\n",len);
		}

		if (1 == KEY_Scan(KEY2_GPIO_PORT, KEY2_PIN))
		{
			ee_Read(ReadBuf, RecData, len, 2);
		}
			
	}
}

static void Delay(uint32_t i)
{
	while(i--);
}
/*********************************************END OF FILE**********************/
