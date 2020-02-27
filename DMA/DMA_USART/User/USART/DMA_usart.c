#include "./USART/DMA_usart.h"

char SendBuff[SENDBUFF_SIZE];
void USART_DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;

    //开启DMA时钟
    RCC_AHB1PeriphClockCmd(USART_DMA_CLK, ENABLE);

    //DMA复位初始化数据流
    DMA_DeInit(USART_DMA_STREAM);

    //等待复位完毕
    while (DISABLE != DMA_GetCmdStatus(USART_DMA_STREAM))
        ;

	DMA_InitStructure.DMA_Channel = USART_DMA_CHANNEL;
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART_DR_BASE;
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)SendBuff;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte   ;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte   ;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

    
    

    DMA_Init(USART_DMA_STREAM, &DMA_InitStructure);
    DMA_Cmd(USART_DMA_STREAM, ENABLE);

    while (ENABLE != DMA_GetCmdStatus(USART_DMA_STREAM))
        ;
}

///重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到USART1 */
		USART_SendData(USART, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到USART1
  int fgetc(FILE *f)
{
		/* 等待串口1输入数据 */
		while (USART_GetFlagStatus(USART, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART);
}

 /**
  * @brief  USART GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */
void USART_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd( USART_RX_GPIO_CLK|USART_TX_GPIO_CLK, ENABLE);

  /* Enable UART clock */
  RCC_APB2PeriphClockCmd(USART_CLK, ENABLE);
  
  /* Connect PXx to USARTx_Tx*/
  GPIO_PinAFConfig(USART_RX_GPIO_PORT,USART_RX_SOURCE, USART_RX_AF);

  /* Connect PXx to USARTx_Rx*/
  GPIO_PinAFConfig(USART_TX_GPIO_PORT,USART_TX_SOURCE,USART_TX_AF);

  /* Configure USART Tx as alternate function  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = USART_TX_GPIO_PIN  ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* Configure USART Rx as alternate function  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = USART_RX_GPIO_PIN;
  GPIO_Init(USART_RX_GPIO_PORT, &GPIO_InitStructure);
			
  /* USART mode config */
  USART_InitStructure.USART_BaudRate = USART_BAUDRATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART, &USART_InitStructure); 
  USART_Cmd(USART, ENABLE);
}

