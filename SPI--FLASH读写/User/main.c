/**
  ******************************************************************************
  * @file    main.c
  * @author  秦殇
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   SPI读写FLASH。
  * @note 	存储小数的核心代码，核心：将小数化为化为两部分整数，分扇区存储
  * 	读函数：
  * 		SPI_FLASH_BufferRead((void *)double_buffer, SPI_FLASH_PageSize * 1, sizeof(double_buffer));
  * 		SPI_FLASH_BufferRead((void *)int_bufffer, SPI_FLASH_PageSize * 2, sizeof(int_bufffer));
  * 	写函数：
  * 		//写入小数数据到第一页
  * 		SPI_FLASH_BufferWrite((void *)double_buffer, SPI_FLASH_PageSize * 1, sizeof(double_buffer));
  * 		//写入整数数据到第二页
  * 		SPI_FLASH_BufferWrite((void *)int_bufffer, SPI_FLASH_PageSize * 2, sizeof(int_bufffer));
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板
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

/* 获取缓冲区的长度 */
#define TxBufferSize1 (countof(TxBuffer1) - 1)
#define RxBufferSize1 (countof(TxBuffer1) - 1)
#define countof(a) (sizeof(a) / sizeof(*(a)))
#define BufferSize (countof(Tx_Buffer) - 1)

#define FLASH_WriteAddress 0x00000
#define FLASH_ReadAddress FLASH_WriteAddress
#define FLASH_SectorToErase FLASH_WriteAddress

/* 发送缓冲区初始化 */
uint8_t Tx_Buffer[] = "世界第一可爱！";
uint8_t Rx_Buffer[BufferSize];

//读取的ID存储位置
__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;

// 函数原型声明
void Delay(__IO uint32_t nCount);
TestStatus Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength);

/**
  * @brief  主函数
  *	@note 	读写FLASH之前必须进行擦除
			FLASH只能将
  * @param  无
  * @retval 无
  */
int main(void)
{
	uint8_t FLASH_ID = 0;
	/*初始化USART 配置模式为 115200 8-N-1，中断接收*/
	Debug_USART_Config();

	SPI_FLASH_Init();

	printf("这是一个FLASH读写实验实验\n");

	FLASH_ID = SPI_FLASH_ReadID();

	printf("\r\nFlash ID = 0x%X\r\n", FLASH_ID);

	//擦除测试
	printf("\r\n开始擦除");
	SPI_FLASH_SectorErase(0);
	printf("\r\n擦除完成");

	printf("\r\n开始写入");
	printf("\r\n写入的数据：%s", Tx_Buffer);
	SPI_FLASH_BufferWrite(Tx_Buffer, 10, BufferSize);
	printf("\r\n写入完成");

	//读取FLASH数据
	printf("\r\n开始读取");
	SPI_FLASH_BufferRead(Rx_Buffer, 10, BufferSize);
	printf("\r\n读取的数据：%s", Rx_Buffer);
	printf("\r\n读取完成");

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
