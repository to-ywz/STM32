#include "SPI_FLASH/bsp_spi_flash.h"

/*
1、初始化相关引脚，及SPI工作模式（GIPO引脚的复用，和SPI的寄存器的设置）
2、编写基本读写单个字节的流程函数
3、组成增删查改的命令
4、利用命令来读写FLASH并效验数据
*/
static __IO uint32_t SPITimeout = SPIT_LONG_TIMEOUT;

/**
  * @brief  等待超时回调函数
  * @param  None.
  * @retval None.
  */
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
	/* 等待超时后的处理,输出错误信息 */
	FLASH_ERROR("SPI 等待超时!errorCode = %d", errorCode);
	return 0;
}

/**
 * @brief SPI_FLSH引脚的初始化
 * @param None
 * @retval None
 */
static void SPI_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 使能FLASH_SPI及GPIO时钟*/
	/*!< SPI_FLASH_SPI_CS_GPIO, SPI_FLASH_SPI_MOSI_GPIO, 
       SPI_FLASH_SPI_MISO_GPIO,SPI_FLASH_SPI_SCK_GPIO 时钟使能 */
	RCC_AHB1PeriphClockCmd(FLASH_SPI_SCK_GPIO_CLK | FLASH_SPI_MISO_GPIO_CLK | FLASH_SPI_MOSI_GPIO_CLK | FLASH_CS_GPIO_CLK, ENABLE);

	/*!< SPI_FLASH_SPI 时钟使能*/
	FLASH_SPI_CLK_INIT(FLASH_SPI_CLK, ENABLE);

	//设置引脚复用
	GPIO_PinAFConfig(FLASH_SPI_SCK_GPIO_PORT, FLASH_SPI_SCK_PINSOURCE, FLASH_SPI_SCK_AF);
	GPIO_PinAFConfig(FLASH_SPI_MISO_GPIO_PORT, FLASH_SPI_MISO_PINSOURCE, FLASH_SPI_MISO_AF);
	GPIO_PinAFConfig(FLASH_SPI_MOSI_GPIO_PORT, FLASH_SPI_MOSI_PINSOURCE, FLASH_SPI_MOSI_AF);

	//配置 SPI 的GPIO引脚：SCK
	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_SCK_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(FLASH_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

	//配置 SPI 的GPIO引脚：MISO
	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MISO_PIN;
	GPIO_Init(FLASH_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

	//配置 SPI 的GPIO引脚：MOSI
	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MOSI_PIN;
	GPIO_Init(FLASH_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

	//配置 SPI 的GPIO引脚：CS
	GPIO_InitStructure.GPIO_Pin = FLASH_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(FLASH_CS_GPIO_PORT, &GPIO_InitStructure);
}

/**
 * @brief SPI模式
 * @param None
 * @retval None
 */
static void SPI_Mode_Config(void)
{
	/* 停止信号 FLASH: CS引脚高电平*/
	SPI_FLASH_CS_HIGH();

	SPI_InitTypeDef SPI_InitStructure;

	/* FLASH_SPI 模式配置 */
	// FLASH芯片 支持SPI模式0及模式3，据此设置CPOL CPHA
	//双线全双工
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	//选择作为SPI主从机
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	//数据帧大小
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	//模式0配置
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	//软件控制NSS
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	//MSB先行，这里取决于FLASH数据手册
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	//CRC校验不使用，如果需要使用需要调用函数
	SPI_InitStructure.SPI_CRCPolynomial = 7;

	SPI_Init(FLASH_SPI, &SPI_InitStructure);
	/* 使能 FLASH_SPI  */
	SPI_Cmd(FLASH_SPI, ENABLE);
}

/**
 * @brief SPI_FLASH初始化接口
 * @param None
 * @retval None
 */
void SPI_FLASH_Init(void)
{
	SPI_GPIO_Config();
	SPI_Mode_Config();
}

//----------------------------SPI预处理------------------------------------
/**
 * @brief FLASH唤醒
 * @param None
 * @retval None
 */
void SPI_Flash_WAKEUP(void)
{
	/*选择 FLASH: CS 低 */
	SPI_FLASH_CS_LOW();

	/* 发上 上电 命令 */
	SPI_FLASH_SendByte(W25X_ReleasePowerDown);

	/* 停止信号 FLASH: CS 高 */
	SPI_FLASH_CS_HIGH(); //等待TRES1
}

/**
 * @brief FLASH掉电
 * @param None
 * @retval None
 */
void SPI_Flash_PowerDown(void)
{
	/* 选择 FLASH: CS 低 */
	SPI_FLASH_CS_LOW();

	/* 发送 掉电 命令 */
	SPI_FLASH_SendByte(W25X_PowerDown);

	/* 停止信号  FLASH: CS 高 */
	SPI_FLASH_CS_HIGH();
}

/**
  * @brief  向FLASH发送 写使能 命令
  * @param  none
  * @retval none
  */
void SPI_FLASH_WriteEnable(void)
{
	/* 通讯开始：CS低 */
	SPI_FLASH_CS_LOW();

	/* 发送写使能命令*/
	SPI_FLASH_SendByte(W25X_WriteEnable);

	/*通讯结束：CS高 */
	SPI_FLASH_CS_HIGH();
}

/**
  * @brief  等待WIP(BUSY)标志被置0，即等待到FLASH内部数据写入完毕
  * @param  none
  * @retval none
  */
void SPI_FLASH_WaitForWriteEnd(void)
{
	u8 FLASH_Status = 0;

	/* 选择 FLASH: CS 低 */
	SPI_FLASH_CS_LOW();

	/* 发送 读状态寄存器 命令 */
	SPI_FLASH_SendByte(W25X_ReadStatusReg);

	SPITimeout = SPIT_FLAG_TIMEOUT;
	/* 若FLASH忙碌，则等待 */
	do
	{
		/* 读取FLASH芯片的状态寄存器 */
		FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);

		{
			if ((SPITimeout--) == 0)
			{ //超时检测
				SPI_TIMEOUT_UserCallback(4);
				return;
			}
		}
	} while (SET == (FLASH_Status & WIP_Flag)); /* 正在写入标志 */

	/* 停止信号  FLASH: CS 高 */
	SPI_FLASH_CS_HIGH();
}
//--------------------------------------------------------------------

//-----------------------------读取FLASH基本属性------------------------------------
/**
  * @brief  读取FLASH ID
  * @param 	无
  * @retval FLASH ID
  */
u32 SPI_FLASH_ReadID(void)
{
	u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

	/* 开始通讯：CS低电平 */
	SPI_FLASH_CS_LOW();

	/* 发送JEDEC指令，读取ID */
	SPI_FLASH_SendByte(W25X_JedecDeviceID);

	/* 读取一个字节数据 */
	Temp0 = SPI_FLASH_SendByte(Dummy_Byte);

	/* 读取一个字节数据 */
	Temp1 = SPI_FLASH_SendByte(Dummy_Byte);

	/* 读取一个字节数据 */
	Temp2 = SPI_FLASH_SendByte(Dummy_Byte);

	/* 停止通讯：CS高电平 */
	SPI_FLASH_CS_HIGH();

	/*把数据组合起来，作为函数的返回值*/
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

	return Temp;
}

/**
  * @brief  读取FLASH Device ID
  * @param 	无
  * @retval FLASH Device ID
  */
u32 SPI_FLASH_ReadDeviceID(void)
{
	u32 Temp = 0;

	/* Select the FLASH: Chip Select low */
	SPI_FLASH_CS_LOW();

	/* Send "RDID " instruction */
	SPI_FLASH_SendByte(W25X_DeviceID);
	SPI_FLASH_SendByte(Dummy_Byte);
	SPI_FLASH_SendByte(Dummy_Byte);
	SPI_FLASH_SendByte(Dummy_Byte);

	/* Read a byte from the FLASH */
	Temp = SPI_FLASH_SendByte(Dummy_Byte);

	/* Deselect the FLASH: Chip Select high */
	SPI_FLASH_CS_HIGH();

	return Temp;
}

// /**
//  * @brief 读取ID0 ~ ID7
//  * @param 	无
//  * @retval FLASH Device ID、
//  * @note    返回的ID取决于FLASH
//  */
// uint32_t SPI_FLASH_ReadID(void)
// {
// 	uint32_t Temp = 0;

// 	//选择FLASH，将FLAHS片选信号拉低
// 	/* Select the FLASH: Chip Select low */
// 	SPI_FLASH_CS_LOW();

// 	//发送SPI指令，读取ID
// 	/* Send "RDID " instruction */
// 	SPI_FLASH_SendByte(W25X_DeviceID);
// 	SPI_FLASH_SendByte(Dummy_Byte);
// 	SPI_FLASH_SendByte(Dummy_Byte);
// 	SPI_FLASH_SendByte(Dummy_Byte);

// 	//接收读取到的内容
// 	/* Read a byte from the FLASH */
// 	//SPI是全双工的由于SPI的从机是FLASH，不能主动发送信号，
// 	//通过STM32发送信号，获取返回值
// 	Temp = SPI_FLASH_SendByte(Dummy_Byte);

// 	//拉高所选择的FLASH，取消片选
// 	/* Deselect the FLASH: Chip Select high */
// 	SPI_FLASH_CS_HIGH();

// 	//接收读取到的内容
// 	return Temp;
// }

//---------------------擦除函数-----------------------------
/**
  * @brief  擦除FLASH扇区,最小单位Sector
  * @param  SectorAddr：要擦除的扇区地址
  * @note	add：必须对齐到要擦除的扇区
  * @retval 无
  */
void SPI_FLASH_SectorErase(u32 SectorAddr)
{
	/* 发送FLASH写使能命令 */
	SPI_FLASH_WriteEnable();
	SPI_FLASH_WaitForWriteEnd();
	/* 擦除扇区 */
	/* 选择FLASH: CS低电平 */
	SPI_FLASH_CS_LOW();
	/* 发送扇区擦除指令*/
	SPI_FLASH_SendByte(W25X_SectorErase);
	/*发送擦除扇区地址的高位*/
	SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
	/* 发送擦除扇区地址的中位 */
	SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
	/* 发送擦除扇区地址的低位 */
	SPI_FLASH_SendByte(SectorAddr & 0xFF);
	/* 停止信号 FLASH: CS 高电平 */
	SPI_FLASH_CS_HIGH();
	/* 等待擦除完毕*/
	SPI_FLASH_WaitForWriteEnd();
}

/**
  * @brief  擦除FLASH扇区，整片擦除
  * @param  无
  * @retval 无
  */
void SPI_FLASH_BulkErase(void)
{
	/* 发送FLASH写使能命令 */
	SPI_FLASH_WriteEnable();

	/* 整块 Erase */
	/* 选择FLASH: CS低电平 */
	SPI_FLASH_CS_LOW();
	/* 发送整块擦除指令*/
	SPI_FLASH_SendByte(W25X_ChipErase);
	/* 停止信号 FLASH: CS 高电平 */
	SPI_FLASH_CS_HIGH();

	/* 等待擦除完毕*/
	SPI_FLASH_WaitForWriteEnd();
}
//----------------------------------------------------------------------------

//-------------------------------------SPI读函数------------------------------------
/**
  * @brief  使用SPI读取一个字节的数据
  * @param  无
  * @retval 返回接收到的数据
  */
uint8_t SPI_FLASH_ReadByte(void)
{
	//SPI是全双工的，收发同步的，由于SPI的从机是FLASH，不能主动发送信号，
	//通过STM32发送信号，获取返回值
	return (SPI_FLASH_SendByte(Dummy_Byte));
}

/**
  * @brief  读取FLASH数据
  * @param 	pBuffer：		存储读出数据的指针
  * @param  ReadAddr：		读取地址
  * @param  NumByteToRead：	读取数据长度
  * @retval 无
  */
void SPI_FLASH_BufferRead(u8 *pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
	/* 选择FLASH: CS低电平 */
	SPI_FLASH_CS_LOW();

	/* 发送 读 指令 */
	SPI_FLASH_SendByte(W25X_ReadData);
	//地址格式 0xfabc
	/* 发送 读 地址高位 */
	SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
	/* 发送 读 地址中位 */
	SPI_FLASH_SendByte((ReadAddr & 0xFF00) >> 8);
	/* 发送 读 地址低位 */
	SPI_FLASH_SendByte(ReadAddr & 0xFF);

	/* 读取数据 */
	while (NumByteToRead--)
	{
		/* 读取一个字节*/
		*pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
		/* 指向下一个字节缓冲区 */
		pBuffer++;
	}

	/* 停止信号 FLASH: CS 高电平 */
	SPI_FLASH_CS_HIGH();
}

//-----------------------------------------------------------------------------------

//------------------------------------SPI写函数---------------------------------------
/**
 * @brief SPI发一个字节
 * @param byte：需要发送的数据/指令
 * @retval 返回MOIS得到的值
 */
uint8_t SPI_FLASH_SendByte(u8 byte)
{
	SPITimeout = SPIT_FLAG_TIMEOUT;

	//等待TXE标志，发送标志
	/* 等待发送缓冲区为空，TXE事件 */
	while (RESET == SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_TXE))
	{
		if ((SPITimeout--) == 0)
		{
			return SPI_TIMEOUT_UserCallback(0);
		}
	}

	/* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
	SPI_I2S_SendData(FLASH_SPI, byte);

	SPITimeout = SPIT_FLAG_TIMEOUT;

	//等待RXNE标志，接收标志，用于确认发送完成，以及准备接收数据
	/* 等待接收缓冲区非空，RXNE事件 */
	while (RESET == SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_RXNE))
	{
		if ((SPITimeout--) == 0)
		{
			return SPI_TIMEOUT_UserCallback(1);
		}
	}

	/* 读取数据寄存器，获取接收缓冲区数据 */
	return SPI_I2S_ReceiveData(FLASH_SPI);
}

/**
	 * @brief  对FLASH写入数据，调用本函数写入数据前需要先擦除扇区
	 * @param	pBuffer，要写入数据的指针
	 * @param  WriteAddr，写入地址
	 * @param  NumByteToWrite，写入数据长度
	 * @retval 无
	 */
void SPI_FLASH_BufferWrite(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

	/*mod运算求余，若writeAddr是SPI_FLASH_PageSize整数倍，运算结果Addr值为0*/
	Addr = WriteAddr % SPI_FLASH_PageSize;

	/*差count个数据值，刚好可以对齐到页地址*/
	count = SPI_FLASH_PageSize - Addr;
	/*计算出要写多少整数页*/
	NumOfPage = NumByteToWrite / SPI_FLASH_PageSize;
	/*mod运算求余，计算出剩余不满一页的字节数*/
	NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

	/* Addr=0,则WriteAddr 刚好按页对齐 aligned  */
	if (Addr == 0)
	{
		/* NumByteToWrite < SPI_FLASH_PageSize */
		if (NumOfPage == 0)
		{
			SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
		}
		else /* NumByteToWrite > SPI_FLASH_PageSize */
		{
			/*先把整数页都写了*/
			while (NumOfPage--)
			{
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
				WriteAddr += SPI_FLASH_PageSize;
				pBuffer += SPI_FLASH_PageSize;
			}

			/*若有多余的不满一页的数据，把它写完*/
			SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
		}
	}
	/* 若地址与 SPI_FLASH_PageSize 不对齐  */
	else
	{
		/* NumByteToWrite < SPI_FLASH_PageSize */
		if (NumOfPage == 0)
		{
			/*当前页剩余的count个位置比NumOfSingle小，写不完*/
			if (NumOfSingle > count)
			{
				temp = NumOfSingle - count;

				/*先写满当前页*/
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
				WriteAddr += count;
				pBuffer += count;

				/*再写剩余的数据*/
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
			}
			else /*当前页剩余的count个位置能写完NumOfSingle个数据*/
			{
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
			}
		}
		else /* NumByteToWrite > SPI_FLASH_PageSize */
		{
			/*地址不对齐多出的count分开处理，不加入这个运算*/
			NumByteToWrite -= count;
			NumOfPage = NumByteToWrite / SPI_FLASH_PageSize;
			NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

			SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
			WriteAddr += count;
			pBuffer += count;

			/*把整数页都写了*/
			while (NumOfPage--)
			{
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
				WriteAddr += SPI_FLASH_PageSize;
				pBuffer += SPI_FLASH_PageSize;
			}
			/*若有多余的不满一页的数据，把它写完*/
			if (NumOfSingle != 0)
			{
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
			}
		}
	}
}

/**
	 * @brief  对FLASH按页写入数据，调用本函数写入数据前需要先擦除扇区
	 * @note	想要想跨区写入数据，需要检测边界值
	 * 		超过边界值须重新发送起始信号
	 * @param	pBuffer，要写入数据的指针
	 * @param  WriteAddr，写入地址
	 * @param  NumByteToWrite，写入数据长度，必须小于等于SPI_FLASH_PerWritePageSize
	 * @retval 无
	 */
void SPI_FLASH_PageWrite(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	/* 发送FLASH写使能命令 */
	SPI_FLASH_WriteEnable();

	/* 选择FLASH: CS低电平 */
	SPI_FLASH_CS_LOW();
	/* 发送页写指令*/
	SPI_FLASH_SendByte(W25X_PageProgram);

	/*发送写地址的高位*/
	SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
	/*发送写地址的中位*/
	SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
	/*发送写地址的低位*/
	SPI_FLASH_SendByte(WriteAddr & 0xFF);

	if (NumByteToWrite > SPI_FLASH_PerWritePageSize)
	{
		NumByteToWrite = SPI_FLASH_PerWritePageSize;
		FLASH_ERROR("SPI_FLASH_PageWrite too large!");
	}

	/* 写入数据*/
	while (NumByteToWrite--)
	{
		/* 发送当前要写入的字节数据 */
		SPI_FLASH_SendByte(*pBuffer);
		/* 指向下一字节数据 */
		pBuffer++;
	}

	/* 停止信号 FLASH: CS 高电平 */
	SPI_FLASH_CS_HIGH();

	/* 等待写入完毕*/
	SPI_FLASH_WaitForWriteEnd();
}

/*********************************************END OF FILE**********************/
