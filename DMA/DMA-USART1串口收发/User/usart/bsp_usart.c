
#include "./usart/bsp_usart.h"
uint16_t rx_len = 0;
uint8_t flag = 0;
uint8_t SendBuf[SENDBUFF_SIZE + 3] = {0};
uint8_t RecvBuf[SENDBUFF_SIZE + 3] = {0};

GPIO_InitTypeDef GPIO_InitStructure;   //定义IO初始化结构体
USART_InitTypeDef USART_InitStructure; //定义串口结构体
DMA_InitTypeDef DMA_InitStructure;	 //定义DMA结构体
NVIC_InitTypeDef NVIC_InitStructure;   //定义中断结构体

static void Init_Gpio_Config(void);
static void Init_Usart_Nvic_Config(void);
static void Init_Dma_Tx_Config(void);
static void Init_Dma_Rx_Config(void);

/**
 * @brief  GPIO初始化配置
 * @note   PA9 => USART1_TX  
 * 			PA10 => USART1_RX
 * @retval None
 */
static void Init_Gpio_Config(void)
{
	//使能GPIOA时钟
	RCC_AHB1PeriphClockCmd(DEBUG_USART_RX_GPIO_CLK | DEBUG_USART_TX_GPIO_CLK, ENABLE); //使能GPIOA时钟

	//GPIO引脚复用映射
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);  //引脚复用映射 PA9 => USART1_TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); //引脚复用映射 PA10 => USART1_RX

	//GPIO结构体设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //管脚指定  PA9  PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//速度50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//初始化
}

/**
 * @brief  串口的初始化配置
 * @note   None
 * @retval None
 */
void Init_Usart_Config(void)
{
	//初始化GPIO
	Init_Gpio_Config();

	//使能USART1时钟s
	RCC_APB2PeriphClockCmd(DEBUG_USART_CLK, ENABLE);

	//串口结构体设置
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;						//波特率设置，一般设置为9600
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式
	USART_Init(DEBUG_USART, &USART_InitStructure);									//初始化串口

	//使能串口
	USART_Cmd(DEBUG_USART, ENABLE);

	//开启串口空闲中断
	USART_ITConfig(DEBUG_USART, USART_IT_IDLE, ENABLE);
	Init_Usart_Nvic_Config();
}

/**
 * @brief  串口的中断初始化配置
 * @note   None
 * @retval None
 */
static void Init_Usart_Nvic_Config(void)
{
	//Usart的 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;	 //串口中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							  //根据指定的参数初始化NVIC寄存器
}

/**
 * @brief  配置DMA——USART
 * @note   自行配置收发文件作用域的数组
 * @retval None
 */
void DMA_USART_Config(void)
{
	RCC_AHB1PeriphClockCmd(USART_DMA_CLK, ENABLE); //DMA2时钟使能
	Init_Dma_Tx_Config();
	Init_Dma_Rx_Config();
}

/**
 * @brief  DMA的发送初始化配置
 * @note   
 * @retval None
 */
static void Init_Dma_Tx_Config(void)
{
	DMA_DeInit(USART_TX_DMA_STREAM); //将DMA的发送数据流设置为缺省值
	while (DMA_GetCmdStatus(USART_TX_DMA_STREAM) != DISABLE)
		; //等待DMA的发送数据流可配置

	//DMA的结构体设置
	DMA_InitStructure.DMA_Channel = USART_DMA_CHANNEL;						//通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&DEBUG_USART->DR;		//DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)SendBuf;					//DMA 存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;					//存储器到外设模式
	DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;						//数据传输量
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//存储器数据长度:8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							//使用普通模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;					//中等优先级
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;					//初始化DMA_FIFOMode
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;			//FIFO 阈值选择。
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;				//存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;		//外设突发单次传输
	DMA_Init(USART_TX_DMA_STREAM, &DMA_InitStructure);						//初始化DMA Stream

	//DMA 中断设置
	NVIC_InitStructure.NVIC_IRQChannel = USART_TX_DMA_STREAM_IRQx; //DMA发送数据流中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	  //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			   //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				   //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);								   //根据指定的参数初始化NVIC寄存器

	USART_DMACmd(DEBUG_USART, USART_DMAReq_Tx, ENABLE);   //使能串口1的DMA发送
	DMA_Cmd(USART_TX_DMA_STREAM, DISABLE);				  //关闭DMA传输
	DMA_ITConfig(USART_TX_DMA_STREAM, DMA_IT_TC, ENABLE); //使能DMA的发送数据流中断
}

/**
 * @brief  DMA的接收初始化配置
 * @note   
 * @retval None
 */
static void Init_Dma_Rx_Config(void)
{
	DMA_DeInit(USART_RX_DMA_STREAM); //将DMA的接收数据流设置为缺省值
	while (DMA_GetCmdStatus(USART_RX_DMA_STREAM) != DISABLE)
		; //等待DMA的接收数据流可配置

	//DMA的结构体设置
	DMA_InitStructure.DMA_Channel = USART_DMA_CHANNEL;						//通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&DEBUG_USART->DR;		//DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)RecvBuf;					//DMA 存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;					//外设到存储器模式
	DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;						//数据传输量
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//存储器数据长度:8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							// 使用普通模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;					//中等优先级
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;					//初始化DMA_FIFOMode
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;			//FIFO 阈值选择。
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;				//存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;		//外设突发单次传输
	DMA_Init(USART_RX_DMA_STREAM, &DMA_InitStructure);						//初始化DMA Stream

	USART_DMACmd(DEBUG_USART, USART_DMAReq_Rx, ENABLE); //DEBUG_USART
	DMA_Cmd(USART_RX_DMA_STREAM, ENABLE);				//开启DMA传输
}

/**
 * @brief  DMA的发送使能
 * @note   
 * @param  DMA_Streamx_tx: 代表DMA的发送数据流，DMA1_Stream0~DMA1_Stream7 / DMA2_Stream0~DMA2_Stream7
 * @param  ndtr: 代表DMA的发送数据个数
 * @retval None
 */
void DMA_Send_Enable(DMA_Stream_TypeDef *DMA_Streamx_tx, u16 ndtr)
{
	DMA_Cmd(DMA_Streamx_tx, DISABLE); //关闭DMA传输
	while (DMA_GetCmdStatus(DMA_Streamx_tx) != DISABLE)
		;										  //确保DMA可以被设置
	DMA_SetCurrDataCounter(DMA_Streamx_tx, ndtr); //数据传输量
	DMA_Cmd(DMA_Streamx_tx, ENABLE);			  //开启DMA传输
}

/**
 * @brief  DMA的发送完成中断函数
 * @note   
 * @retval None
 */
void DMA2_Stream7_IRQHandler(void)
{
	if (DMA_GetFlagStatus(DMA2_Stream7, DMA_FLAG_TCIF7) != RESET) //等待DMA2_Steam7传输完成
	{
		DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);	   //清除DMA2_Steam7传输完成标志
		DMA_Cmd(DMA2_Stream7, DISABLE);					   //关闭DMA
		DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7); //清除中断屏蔽位
	}
}

/**
 * @brief  串口1的空闲中断函数
 * @note   
 * @retval None
 */
void USART1_IRQHandler(void)
{
	u16 i;
	if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) //如果接到一帧数据
	{
		DMA_Cmd(DMA2_Stream5, DISABLE); //关闭DMA,防止处理其间有数据

		//先读SR，再读DR，是为了清除IDLE中断
		i = USART1->SR; //读SR寄存器
		i = USART1->DR; //读DR寄存器

		rx_len = SENDBUFF_SIZE - DMA_GetCurrDataCounter(DMA2_Stream5); //获得这一帧数据的长度
		if (rx_len != 0)											   //这一帧数据长度不为0
		{
			flag = 1; //数据帧接收标志置1
			for (i = 0; i < rx_len; i++)
			{
				SendBuf[i] = RecvBuf[i]; //将接收的数据保存
			}
			for (i = 0; i < rx_len; i++) //将接收缓冲区清零
			{
				RecvBuf[i] = 0;
			}
		}

		//清除DMA2_Steam5传输完成标志
		DMA_ClearFlag(DMA2_Stream5, DMA_FLAG_TCIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5);
		DMA_SetCurrDataCounter(DMA2_Stream5, SENDBUFF_SIZE); //设置DMA接收单元的长度
		DMA_Cmd(DMA2_Stream5, ENABLE);						 //打开DMA
	}
}

/*****************  发送一个字符 **********************/
void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx, ch);

	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET)
		;
}

/*****************  发送字符串 **********************/
void Usart_SendString(USART_TypeDef *pUSARTx, char *str)
{
	unsigned int k = 0;
	do
	{
		Usart_SendByte(pUSARTx, *(str + k));
		k++;
	} while (*(str + k) != '\0');

	/* 等待发送完成 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET)
	{
	}
}

/*****************  发送一个16位数 **********************/
void Usart_SendHalfWord(USART_TypeDef *pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;

	/* 取出高八位 */
	temp_h = (ch & 0XFF00) >> 8;
	/* 取出低八位 */
	temp_l = ch & 0XFF;

	/* 发送高八位 */
	USART_SendData(pUSARTx, temp_h);
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET)
		;

	/* 发送低八位 */
	USART_SendData(pUSARTx, temp_l);
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET)
		;
}

///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到串口 */
	USART_SendData(DEBUG_USART, (uint8_t)ch);

	/* 等待发送完毕 */
	while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET)
		;

	return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
	/* 等待串口输入数据 */
	while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET)
		;

	return (int)USART_ReceiveData(DEBUG_USART);
}
/*********************************************END OF FILE**********************/
