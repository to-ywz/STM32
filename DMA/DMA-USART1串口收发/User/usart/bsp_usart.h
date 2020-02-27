#ifndef __BSP_USART_H
#define	__BSP_USART_H

#include "stm32f4xx.h"
#include <stdio.h>


/*******************************************************/
//************************串口定义**********************//
#define DEBUG_USART                             USART1
#define DEBUG_USART_CLK                         RCC_APB2Periph_USART1
#define DEBUG_USART_BAUDRATE                    115200  //串口波特率

//引脚定义
///接收
#define DEBUG_USART_RX_GPIO_PORT                GPIOA
#define DEBUG_USART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define DEBUG_USART_RX_PIN                      GPIO_Pin_10
#define DEBUG_USART_RX_AF                       GPIO_AF_USART1
#define DEBUG_USART_RX_SOURCE                   GPIO_PinSource10
///发送
#define DEBUG_USART_TX_GPIO_PORT                GPIOA
#define DEBUG_USART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define DEBUG_USART_TX_PIN                      GPIO_Pin_9
#define DEBUG_USART_TX_AF                       GPIO_AF_USART1
#define DEBUG_USART_TX_SOURCE                   GPIO_PinSource9
//中断
#define DEBUG_USART_IRQHandler                  USART1_IRQHandler
#define DEBUG_USART_IRQ                 		USART1_IRQn

//***********************DMA定义**********************//
#define USART_DR_BASE                           (USART1_BASE + 0x04)        //通道4数据流7 DMA to USART(操控数据寄存器DR)
#define SENDBUFF_SIZE                           32                          //发送数据大小
#define USART_DMA_CLK                           RCC_AHB1Periph_DMA2
#define USART_DMA_CHANNEL                       DMA_Channel_4               //通道4
//TX
#define USART_TX_DMA_STREAM                     DMA2_Stream7                //数据流7发送
#define USART_TX_DMA_STREAM_IRQ                 DMA2_Stream7_IRQHandler
#define USART_TX_DMA_STREAM_IRQx                DMA2_Stream7_IRQn
//RX
#define USART_RX_DMA_STREAM                     DMA2_Stream5                //数据流5接收
#define USART_RX_DMA_STREAM_IRQ                 DMA2_Stream5_IRQHandler
/************************************************************/

// void USART_DMA_Config(void);
// void Debug_USART_Config(void);
// void DMA_Send_Enable(DMA_Stream_TypeDef *DMA_Streamx_tx, uint16_t ndtr);
// void USART1_IRQHandler(void);
// void DMA2_Stream7_IRQHandler(void);

void Init_Usart_Config(void);
void DMA_USART_Config(void);
void DMA_Send_Enable(DMA_Stream_TypeDef *DMA_Streamx_tx, u16 ndtr);
void DMA2_Stream7_IRQHandler(void);
void USART1_IRQHandler(void);

void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch);
void Usart_SendString(USART_TypeDef *pUSARTx, char *str);
void Usart_SendHalfWord(USART_TypeDef *pUSARTx, uint16_t ch);

extern uint16_t rx_len;
extern uint8_t flag;
extern uint8_t SendBuf[SENDBUFF_SIZE + 3];
extern uint8_t RecvBuf[SENDBUFF_SIZE + 3];
#endif /* __USART1_H */
