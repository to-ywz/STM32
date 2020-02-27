/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ��ʾʹ��DMA���д��ڴ���
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include "stdio.h"
#include "string.h"
#include "stm32f4xx.h"
#include "./USART/DMA_usart.h"
#include "./led/bsp_led.h"

extern uint8_t SendBuff[SENDBUFF_SIZE];
uint8_t SendData[] = "������,���ǳ����ɰ���^+^\"��~\r\n";

static void Delay(__IO u32 nCount);
static int GetStringLength(char *str);

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
    int Len = sizeof(SendData)/sizeof(char);
    /* ��ʼ��USART */
    USART_Config();

    /* ����ʹ��DMAģʽ */
    USART_DMA_Config();

    /* ����RGB��ɫ�� */
    LED_GPIO_Config();

    //printf("\r\n USART1 DMA TX ���� \r\n");

    /*��佫Ҫ���͵�����*/
	//strcpy(SendBuff, SendData);
	for (int i = 1; i < Len; i++)
	{//����DMAת����������������Ӳ�1����һ�����ִ���1~2��
		SendBuff[i] = SendData[i-1];
	}
	
	printf("\r\n");
	
    /*Ϊ��ʾDMA�������ж�CPU���ܴ����������飬����ʹ��DMA�������ݣ����ǳ���
  *��ʱ�����п��ܻᵼ�µ��Զ˴��ڵ������ֻῨ��������ҷɵ������
  *���DMA�����е�ѭ��ģʽ��Ϊ����ģʽ*/

    /* USART1 �� DMA����TX���� */
    USART_DMACmd(USART, USART_DMAReq_Tx, ENABLE);

    /* ��ʱCPU�ǿ��еģ����Ը����������� */
    //����ͬʱ����LED
    while (1)
    {
        LED1_TOGGLE
        Delay(0xFFFFF);
    }
}

static void Delay(__IO uint32_t nCount) //�򵥵���ʱ����
{
    for (; nCount != 0; nCount--)
        ;
}

static int GetStringLength(char *str)
{
    int length = 0;
    while ('\0' != str[length])
    {
        length++;
    }
    return length;
}

/*********************************************END OF FILE**********************/
