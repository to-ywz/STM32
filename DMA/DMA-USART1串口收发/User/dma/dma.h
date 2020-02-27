#ifndef DMA_H
#define DMA_H      
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"

extern u16 rx_len;
extern u8 SendBuffer[35];
extern u8 ReceiveBuffer[35];
extern u16 BufferLen;
extern u8 rx_flag; 

extern GPIO_InitTypeDef GPIO_InitStructure;                       //定义IO初始化结构体 
extern USART_InitTypeDef USART_InitStructure;                     //定义串口结构体
extern DMA_InitTypeDef DMA_InitStructure;                         //定义DMA结构体
extern NVIC_InitTypeDef NVIC_InitStructure;                       //定义中断结构体

void Init_Clock_Config(void);
void Init_Gpio_Config(void);
void Init_Usart_Config(USART_TypeDef *Usartx, u32 Bound);
void Init_Usart_Nvic_Config(u32 Usartx_Irqn);
void Init_Dma_Tx_Config(USART_TypeDef *Usartx,u32 Par, DMA_Stream_TypeDef *DMA_Streamx_Tx, u32 DMA_Chx,u32 Tx_Buf, u16 Tx_Size, u32 DMA_Streamx_Irqn);
void Init_Dma_Rx_Config(USART_TypeDef *Usartx, u32 Par, DMA_Stream_TypeDef *DMA_Streamx_Rx, u32 DMA_Chx,u32 Rx_Buf, u16 Rx_Size);
void Dma_Send_Enable(DMA_Stream_TypeDef *DMA_Streamx_tx,u16 ndtr);
void DMA2_Stream7_IRQHandler(void);
void USART1_IRQHandler(void);

#endif
