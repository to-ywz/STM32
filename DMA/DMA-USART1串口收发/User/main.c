/****************************************************************
 * 
 *	@brief		 		DMA���մ������ݲ�ת��
 *	@author		����
 *  @version	2020/01/23
 *	@attention ����뷢�����ⳤ�ȵ�����,��Ҫ���д������
 * 
******************************************************************/

#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./KEY/psb_key.h"
//#include "dma.h"
#include "./usart/bsp_usart.h"

void LED(int n);
void Delay(__IO u32 nCount);

int main(void)
{
	//LED��ʼ��
	LED_GPIO_Config();
	//������ʼ��
	KEY_GPIO_Config();

	Init_Usart_Config(); //���ڳ�ʼ��
	DMA_USART_Config();  //DMA����USART����

	while (1)
	{
		//���յ�һ֡����
		if (flag)
		{
			//�����յ������ݷ��͵��豸
			DMA_Send_Enable(USART_TX_DMA_STREAM, rx_len);

			//����֡���ձ�־����
			flag = 0;
		}
		//LED����˸����ʾ������������
		LED2_TOGGLE;
		Delay(0x6fffff);
	}
}

void Delay(__IO u32 nCount)
{
	while (nCount--)
		;
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
