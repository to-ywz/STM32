/**
  ******************************************************************************
  * @file    main.c
  * @author  ����
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   SPI��дFLASH��
  * @note 	�洢С���ĺ��Ĵ��룬���ģ���С����Ϊ��Ϊ�������������������洢
  * 	��������
  * 		SPI_FLASH_BufferRead((void *)double_buffer, SPI_FLASH_PageSize * 1, sizeof(double_buffer));
  * 		SPI_FLASH_BufferRead((void *)int_bufffer, SPI_FLASH_PageSize * 2, sizeof(int_bufffer));
  * 	д������
  * 		//д��С�����ݵ���һҳ
  * 		SPI_FLASH_BufferWrite((void *)double_buffer, SPI_FLASH_PageSize * 1, sizeof(double_buffer));
  * 		//д���������ݵ��ڶ�ҳ
  * 		SPI_FLASH_BufferWrite((void *)int_bufffer, SPI_FLASH_PageSize * 2, sizeof(int_bufffer));
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������
  *
  ******************************************************************************
  */

#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./SPI_FLASH/bsp_spi_flash.h"

typedef enum
{
	FAILED = 0,
	PASSED = !FAILED
} TestStatus;

/* ��ȡ�������ĳ��� */
#define TxBufferSize1 (countof(TxBuffer1) - 1)
#define RxBufferSize1 (countof(TxBuffer1) - 1)
#define countof(a) (sizeof(a) / sizeof(*(a)))
#define BufferSize (countof(Tx_Buffer) - 1)

#define FLASH_WriteAddress 0x00000
#define FLASH_ReadAddress FLASH_WriteAddress
#define FLASH_SectorToErase FLASH_WriteAddress

/* ���ͻ�������ʼ�� */
uint8_t Tx_Buffer[] = "�����һ�ɰ���";
uint8_t Rx_Buffer[BufferSize];

//��ȡ��ID�洢λ��
__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;

// ����ԭ������
void Delay(__IO uint32_t nCount);
TestStatus Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength);

/**
  * @brief  ������
  *	@note 	��дFLASH֮ǰ������в���
			FLASHֻ�ܽ�
  * @param  ��
  * @retval ��
  */
int main(void)
{
	uint8_t FLASH_ID = 0;
	/*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
	Debug_USART_Config();

	SPI_FLASH_Init();

	printf("����һ��FLASH��дʵ��ʵ��\n");

	FLASH_ID = SPI_FLASH_ReadID();

	printf("\r\nFlash ID = 0x%X\r\n", FLASH_ID);

	//��������
	printf("\r\n��ʼ����");
	SPI_FLASH_SectorErase(0);
	printf("\r\n�������");

	printf("\r\n��ʼд��");
	printf("\r\nд������ݣ�%s", Tx_Buffer);
	SPI_FLASH_BufferWrite(Tx_Buffer, 10, BufferSize);
	printf("\r\nд�����");

	//��ȡFLASH����
	printf("\r\n��ʼ��ȡ");
	SPI_FLASH_BufferRead(Rx_Buffer, 10, BufferSize);
	printf("\r\n��ȡ�����ݣ�%s", Rx_Buffer);
	printf("\r\n��ȡ���");

	//if (PASSED == Buffercmp(0xff, Rx_Buffer, 4096))

	while (1)
	{
	}
}

TestStatus Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength)
{
	while (BufferLength--)
	{
		if (*pBuffer1 != *pBuffer2)
		{
			return FAILED;
		}

		pBuffer1++;
		pBuffer2++;
	}
	return PASSED;
}
/*********************************************END OF FILE**********************/
