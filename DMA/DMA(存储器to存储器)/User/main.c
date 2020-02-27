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
//�궨��
#define DMA_STREAM               DMA2_Stream0			//M->Mֻ��DMA2
#define DMA_CHANNEL              DMA_Channel_0			//DMA2ͨ��0
#define DMA_STREAM_CLOCK         RCC_AHB1Periph_DMA2 	//DMA2ʱ����
#define DMA_FLAG_TCIF            DMA_FLAG_TCIF0			//

#define BUFFER_SIZE              32						//�����������
#define TIMEOUT_MAX              10000 /* Maximum timeout value */

//const�洢ΪFlash����
const uint32_t FlashTemp;
const uint32_t aSRC_Const_Buffer[BUFFER_SIZE]= {
                                    0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
                                    0x11121314,0x15161718,0x191A1B1C,0x1D1E1F20,
                                    0x21222324,0x25262728,0x292A2B2C,0x2D2E2F30,
                                    0x31323334,0x35363738,0x393A3B3C,0x3D3E3F40,
                                    0x41424344,0x45464748,0x494A4B4C,0x4D4E4F50,
                                    0x51525354,0x55565758,0x595A5B5C,0x5D5E5F60,
                                    0x61626364,0x65666768,0x696A6B6C,0x6D6E6F70,
                                    0x71727374,0x75767778,0x797A7B7C,0x7D7E7F80};

//SRAM����
uint32_t SRAMTemp;
uint32_t aDST_Buffer[BUFFER_SIZE];
									
static void Delay(__IO uint32_t nCount);
static void DMA_Config(void);
uint8_t Buffercmp(const uint32_t* pBuffer, uint32_t* pBuffer1, uint16_t BufferLength);
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	/* �����űȽϽ������ */
	uint8_t TransferStatus;

	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();

	/* ����RGB��ɫ��Ϊ��ɫ */
	LED_GREEN;  

	/* ����ʱ���� */
	Delay(0xFFFFFF);  

	/* DMA�������� */
	DMA_Config(); 

	/* �ȴ�DMA������� */
	while(DMA_GetFlagStatus(DMA_STREAM,DMA_FLAG_TCIF)==DISABLE); 

	/* �Ƚ�Դ�����봫������� */
	//��� DMA_LISR �� DMA_FLAG_TCIF0 ״̬��ͨ��x DMA_FLAG_TCIFx
	TransferStatus=Buffercmp(aSRC_Const_Buffer, aDST_Buffer, BUFFER_SIZE);

	/* �ж�Դ�����봫������ݱȽϽ��*/
	if(TransferStatus==0)  
	{
		/* Դ�����봫������ݲ����ʱRGB��ɫ����ʾ��ɫ */
		LED_RED;
	}
	else
	{ 
		/* Դ�����봫����������ʱRGB��ɫ����ʾ��ɫ */
		LED_BLUE;
	}

	while (1)
	{		
	}
}

/* �򵥵���ʱ���� */
static void Delay(__IO uint32_t nCount)
{
	for(; nCount != 0; nCount--);
}

static void DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	__IO uint32_t	Timeout = TIMEOUT_MAX;
	
	//ʹ��DMAʱ��
	RCC_AHB1PeriphClockCmd(DMA_STREAM_CLOCK, ENABLE);
	
	//��λ��ʼ��DMA������
	DMA_DeInit(DMA_STREAM);
	
	//�ȴ�DMA��������λ���
	while (DISABLE != DMA_GetCmdStatus(DMA_STREAM));
	
	/* DMA������ͨ��ѡ�� */
	DMA_InitStructure.DMA_Channel = DMA_CHANNEL;  
	/* Դ���ݵ�ַ */
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)aSRC_Const_Buffer;
	/* Ŀ���ַ */
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)aDST_Buffer;
	/* �洢�����洢��ģʽ */
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToMemory;
	/* ������Ŀ */
	DMA_InitStructure.DMA_BufferSize = (uint32_t)BUFFER_SIZE;
	/* ʹ���Զ��������� */
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	/* ʹ���Զ��������� */
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	/* Դ�������ִ�С(32λ) */
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	/* Ŀ������Ҳ���ִ�С(32λ) */
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	/* һ�δ���ģʽ���洢�����洢��ģʽ����ʹ��ѭ������ */
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	/* DMA���������ȼ�Ϊ�� */
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
	/**
	  * @note ��DMA������ M -> M ģʽʱ��FIFOĬ����Ӳ����1
	  * 		��ǰ��ֵΪ FULL �Ŵ�������
	  *			�ó�����DMA����ǿ��FIFOģʽ (M->Mģʽ) 
	  **/
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable; 
	//��ֵ�趨
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	
	/* ����ģʽ */
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//DMA_MemoryBurst_INC4;//(Ҳ����)
	/* ����ģʽ */
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	/* ���DMA�������������� */
	DMA_Init(DMA_STREAM, &DMA_InitStructure);

	/**
	  * @note
	  * @{
	  * ��� DMA_SxCR �� EN λ
	  *
	  **/
	/* ���DMA������������ɱ�־λ */
	DMA_ClearFlag(DMA_STREAM,DMA_FLAG_TCIF);

	/* ʹ��DMA����������ʼDMA���ݴ��� */
	DMA_Cmd(DMA_STREAM, ENABLE);
	
	//���DMA�������Ƿ���Ч && ��ʱ���
	Timeout = TIMEOUT_MAX;
	while ((DMA_GetCmdStatus(DMA_STREAM) != ENABLE) && (Timeout-- > 0));
	/**
	  * @note
	  * @}
	  **/
	if (0 == Timeout)
	{
		LED_RED;
		Delay(0xffffff);
		LED_RGBOFF;
		Delay(0xffffff);
	}
}
/*********************************************END OF FILE**********************/

