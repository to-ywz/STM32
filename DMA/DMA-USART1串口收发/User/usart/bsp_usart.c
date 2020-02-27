
#include "./usart/bsp_usart.h"
uint16_t rx_len = 0;
uint8_t flag = 0;
uint8_t SendBuf[SENDBUFF_SIZE + 3] = {0};
uint8_t RecvBuf[SENDBUFF_SIZE + 3] = {0};

GPIO_InitTypeDef GPIO_InitStructure;   //����IO��ʼ���ṹ��
USART_InitTypeDef USART_InitStructure; //���崮�ڽṹ��
DMA_InitTypeDef DMA_InitStructure;	 //����DMA�ṹ��
NVIC_InitTypeDef NVIC_InitStructure;   //�����жϽṹ��

static void Init_Gpio_Config(void);
static void Init_Usart_Nvic_Config(void);
static void Init_Dma_Tx_Config(void);
static void Init_Dma_Rx_Config(void);

/**
 * @brief  GPIO��ʼ������
 * @note   PA9 => USART1_TX  
 * 			PA10 => USART1_RX
 * @retval None
 */
static void Init_Gpio_Config(void)
{
	//ʹ��GPIOAʱ��
	RCC_AHB1PeriphClockCmd(DEBUG_USART_RX_GPIO_CLK | DEBUG_USART_TX_GPIO_CLK, ENABLE); //ʹ��GPIOAʱ��

	//GPIO���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);  //���Ÿ���ӳ�� PA9 => USART1_TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); //���Ÿ���ӳ�� PA10 => USART1_RX

	//GPIO�ṹ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //�ܽ�ָ��  PA9  PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			//���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//�ٶ�50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//��ʼ��
}

/**
 * @brief  ���ڵĳ�ʼ������
 * @note   None
 * @retval None
 */
void Init_Usart_Config(void)
{
	//��ʼ��GPIO
	Init_Gpio_Config();

	//ʹ��USART1ʱ��s
	RCC_APB2PeriphClockCmd(DEBUG_USART_CLK, ENABLE);

	//���ڽṹ������
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;						//���������ã�һ������Ϊ9600
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ
	USART_Init(DEBUG_USART, &USART_InitStructure);									//��ʼ������

	//ʹ�ܴ���
	USART_Cmd(DEBUG_USART, ENABLE);

	//�������ڿ����ж�
	USART_ITConfig(DEBUG_USART, USART_IT_IDLE, ENABLE);
	Init_Usart_Nvic_Config();
}

/**
 * @brief  ���ڵ��жϳ�ʼ������
 * @note   None
 * @retval None
 */
static void Init_Usart_Nvic_Config(void)
{
	//Usart�� NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;	 //�����ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							  //����ָ���Ĳ�����ʼ��NVIC�Ĵ���
}

/**
 * @brief  ����DMA����USART
 * @note   ���������շ��ļ������������
 * @retval None
 */
void DMA_USART_Config(void)
{
	RCC_AHB1PeriphClockCmd(USART_DMA_CLK, ENABLE); //DMA2ʱ��ʹ��
	Init_Dma_Tx_Config();
	Init_Dma_Rx_Config();
}

/**
 * @brief  DMA�ķ��ͳ�ʼ������
 * @note   
 * @retval None
 */
static void Init_Dma_Tx_Config(void)
{
	DMA_DeInit(USART_TX_DMA_STREAM); //��DMA�ķ�������������Ϊȱʡֵ
	while (DMA_GetCmdStatus(USART_TX_DMA_STREAM) != DISABLE)
		; //�ȴ�DMA�ķ���������������

	//DMA�Ľṹ������
	DMA_InitStructure.DMA_Channel = USART_DMA_CHANNEL;						//ͨ��ѡ��
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&DEBUG_USART->DR;		//DMA�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)SendBuf;					//DMA �洢��0��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;					//�洢��������ģʽ
	DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;						//���ݴ�����
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//���������ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//�洢������ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //�������ݳ���:8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//�洢�����ݳ���:8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							//ʹ����ͨģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;					//�е����ȼ�
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;					//��ʼ��DMA_FIFOMode
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;			//FIFO ��ֵѡ��
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;				//�洢��ͻ�����δ���
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;		//����ͻ�����δ���
	DMA_Init(USART_TX_DMA_STREAM, &DMA_InitStructure);						//��ʼ��DMA Stream

	//DMA �ж�����
	NVIC_InitStructure.NVIC_IRQChannel = USART_TX_DMA_STREAM_IRQx; //DMA�����������ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	  //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			   //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				   //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);								   //����ָ���Ĳ�����ʼ��NVIC�Ĵ���

	USART_DMACmd(DEBUG_USART, USART_DMAReq_Tx, ENABLE);   //ʹ�ܴ���1��DMA����
	DMA_Cmd(USART_TX_DMA_STREAM, DISABLE);				  //�ر�DMA����
	DMA_ITConfig(USART_TX_DMA_STREAM, DMA_IT_TC, ENABLE); //ʹ��DMA�ķ����������ж�
}

/**
 * @brief  DMA�Ľ��ճ�ʼ������
 * @note   
 * @retval None
 */
static void Init_Dma_Rx_Config(void)
{
	DMA_DeInit(USART_RX_DMA_STREAM); //��DMA�Ľ�������������Ϊȱʡֵ
	while (DMA_GetCmdStatus(USART_RX_DMA_STREAM) != DISABLE)
		; //�ȴ�DMA�Ľ���������������

	//DMA�Ľṹ������
	DMA_InitStructure.DMA_Channel = USART_DMA_CHANNEL;						//ͨ��ѡ��
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&DEBUG_USART->DR;		//DMA�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)RecvBuf;					//DMA �洢��0��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;					//���赽�洢��ģʽ
	DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;						//���ݴ�����
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//���������ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//�洢������ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //�������ݳ���:8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//�洢�����ݳ���:8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							// ʹ����ͨģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;					//�е����ȼ�
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;					//��ʼ��DMA_FIFOMode
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;			//FIFO ��ֵѡ��
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;				//�洢��ͻ�����δ���
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;		//����ͻ�����δ���
	DMA_Init(USART_RX_DMA_STREAM, &DMA_InitStructure);						//��ʼ��DMA Stream

	USART_DMACmd(DEBUG_USART, USART_DMAReq_Rx, ENABLE); //DEBUG_USART
	DMA_Cmd(USART_RX_DMA_STREAM, ENABLE);				//����DMA����
}

/**
 * @brief  DMA�ķ���ʹ��
 * @note   
 * @param  DMA_Streamx_tx: ����DMA�ķ�����������DMA1_Stream0~DMA1_Stream7 / DMA2_Stream0~DMA2_Stream7
 * @param  ndtr: ����DMA�ķ������ݸ���
 * @retval None
 */
void DMA_Send_Enable(DMA_Stream_TypeDef *DMA_Streamx_tx, u16 ndtr)
{
	DMA_Cmd(DMA_Streamx_tx, DISABLE); //�ر�DMA����
	while (DMA_GetCmdStatus(DMA_Streamx_tx) != DISABLE)
		;										  //ȷ��DMA���Ա�����
	DMA_SetCurrDataCounter(DMA_Streamx_tx, ndtr); //���ݴ�����
	DMA_Cmd(DMA_Streamx_tx, ENABLE);			  //����DMA����
}

/**
 * @brief  DMA�ķ�������жϺ���
 * @note   
 * @retval None
 */
void DMA2_Stream7_IRQHandler(void)
{
	if (DMA_GetFlagStatus(DMA2_Stream7, DMA_FLAG_TCIF7) != RESET) //�ȴ�DMA2_Steam7�������
	{
		DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);	   //���DMA2_Steam7������ɱ�־
		DMA_Cmd(DMA2_Stream7, DISABLE);					   //�ر�DMA
		DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7); //����ж�����λ
	}
}

/**
 * @brief  ����1�Ŀ����жϺ���
 * @note   
 * @retval None
 */
void USART1_IRQHandler(void)
{
	u16 i;
	if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) //����ӵ�һ֡����
	{
		DMA_Cmd(DMA2_Stream5, DISABLE); //�ر�DMA,��ֹ�������������

		//�ȶ�SR���ٶ�DR����Ϊ�����IDLE�ж�
		i = USART1->SR; //��SR�Ĵ���
		i = USART1->DR; //��DR�Ĵ���

		rx_len = SENDBUFF_SIZE - DMA_GetCurrDataCounter(DMA2_Stream5); //�����һ֡���ݵĳ���
		if (rx_len != 0)											   //��һ֡���ݳ��Ȳ�Ϊ0
		{
			flag = 1; //����֡���ձ�־��1
			for (i = 0; i < rx_len; i++)
			{
				SendBuf[i] = RecvBuf[i]; //�����յ����ݱ���
			}
			for (i = 0; i < rx_len; i++) //�����ջ���������
			{
				RecvBuf[i] = 0;
			}
		}

		//���DMA2_Steam5������ɱ�־
		DMA_ClearFlag(DMA2_Stream5, DMA_FLAG_TCIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5);
		DMA_SetCurrDataCounter(DMA2_Stream5, SENDBUFF_SIZE); //����DMA���յ�Ԫ�ĳ���
		DMA_Cmd(DMA2_Stream5, ENABLE);						 //��DMA
	}
}

/*****************  ����һ���ַ� **********************/
void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx, ch);

	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET)
		;
}

/*****************  �����ַ��� **********************/
void Usart_SendString(USART_TypeDef *pUSARTx, char *str)
{
	unsigned int k = 0;
	do
	{
		Usart_SendByte(pUSARTx, *(str + k));
		k++;
	} while (*(str + k) != '\0');

	/* �ȴ�������� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET)
	{
	}
}

/*****************  ����һ��16λ�� **********************/
void Usart_SendHalfWord(USART_TypeDef *pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;

	/* ȡ���߰�λ */
	temp_h = (ch & 0XFF00) >> 8;
	/* ȡ���Ͱ�λ */
	temp_l = ch & 0XFF;

	/* ���͸߰�λ */
	USART_SendData(pUSARTx, temp_h);
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET)
		;

	/* ���͵Ͱ�λ */
	USART_SendData(pUSARTx, temp_l);
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET)
		;
}

///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ����� */
	USART_SendData(DEBUG_USART, (uint8_t)ch);

	/* �ȴ�������� */
	while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET)
		;

	return (ch);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
	/* �ȴ������������� */
	while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET)
		;

	return (int)USART_ReceiveData(DEBUG_USART);
}
/*********************************************END OF FILE**********************/
