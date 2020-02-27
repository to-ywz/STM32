/**
  ******************************************************************************
  * @file    main.c
  * @author  ����
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ���ڽӷ����ԣ����ڽ��յ����ݺ����ϻش���
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������
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
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	int count = 7;
	char *str = "�����������һ�ɰ�A��\n";
	/*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
	Debug_USART_Config();
	KEY_GPIO_Config();
	rx_queue_init();
	LED_GPIO_Config();
	
	/* ����һ���ַ��� */
	Usart_SendString(DEBUG_USART, "����һ�������жϽ��ջ���ʵ��\n");
	printf("��ӭʹ��Ұ��STM32������\n\n\n\n");

	while (1)
	{
		//��ȡ���ݲ����
		//ʵ��Ӧ���пɲο�pull data�ķ�ʽ��ȡ���ݽ��д���
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
