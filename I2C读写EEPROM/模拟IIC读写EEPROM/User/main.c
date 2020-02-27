/**
  ******************************************************************************
  * @file    main.c
  * @author  ����
  * @version V1.0
  * @date    2020-01-10
  * @brief   ��дEEPROM
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������
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
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	uint16_t len;
	uint8_t RecData[256];
	uint8_t ReadBuf[256];
	KEY_GPIO_Config();
	LED_GPIO_Config();
	LED_BLUE;

	/*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
	Debug_USART_Config();

	printf("\r\n��ӭʹ��Ұ��  STM32 F407 �����塣\r\n");
	printf("\r\n����һ��I2C����(AT24C02)��д�������� \r\n");

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
