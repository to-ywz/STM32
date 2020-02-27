

#include "stm32f4xx.h"


/*GPIO���źŶ���*/
#define GPIO_Pin_0                 ((uint16_t)0x0001)  /*!< ѡ��Pin0 (1<<0) */
#define GPIO_Pin_1                 ((uint16_t)0x0002)  /*!< ѡ��Pin1 (1<<1)*/
#define GPIO_Pin_2                 ((uint16_t)0x0004)  /*!< ѡ��Pin2 (1<<2)*/
#define GPIO_Pin_3                 ((uint16_t)0x0008)  /*!< ѡ��Pin3 (1<<3)*/
#define GPIO_Pin_4                 ((uint16_t)0x0010)  /*!< ѡ��Pin4 */
#define GPIO_Pin_5                 ((uint16_t)0x0020)  /*!< ѡ��Pin5 */
#define GPIO_Pin_6                 ((uint16_t)0x0040)  /*!< ѡ��Pin6 */
#define GPIO_Pin_7                 ((uint16_t)0x0080)  /*!< ѡ��Pin7 */
#define GPIO_Pin_8                 ((uint16_t)0x0100)  /*!< ѡ��Pin8 */
#define GPIO_Pin_9                 ((uint16_t)0x0200)  /*!< ѡ��Pin9 */
#define GPIO_Pin_10                ((uint16_t)0x0400)  /*!< ѡ��Pin10 */
#define GPIO_Pin_11                ((uint16_t)0x0800)  /*!< ѡ��Pin11 */
#define GPIO_Pin_12                ((uint16_t)0x1000)  /*!< ѡ��Pin12 */
#define GPIO_Pin_13                ((uint16_t)0x2000)  /*!< ѡ��Pin13 */
#define GPIO_Pin_14                ((uint16_t)0x4000)  /*!< ѡ��Pin14 */
#define GPIO_Pin_15                ((uint16_t)0x8000)  /*!< ѡ��Pin15 */
#define GPIO_Pin_All               ((uint16_t)0xFFFF)  /*!< ѡ��ȫ������ */




/** 
  * GPIO�˿�����ģʽ��ö�ٶ���
  */   
typedef enum
{ 
  GPIO_Mode_IN   = 0x00, /*!< ����ģʽ */
  GPIO_Mode_OUT  = 0x01, /*!< ���ģʽ */
  GPIO_Mode_AF   = 0x02, /*!< ����ģʽ */
  GPIO_Mode_AN   = 0x03  /*!< ģ��ģʽ */
}GPIOMode_TypeDef;

/** 
  * GPIO�������ö�ٶ���
  */  
typedef enum
{ 
  GPIO_OType_PP = 0x00,	/*!< ����ģʽ */
  GPIO_OType_OD = 0x01	/*!< ��©ģʽ */
}GPIOOType_TypeDef;


/** 
  * GPIO�������ö�ٶ���
  */  
typedef enum
{ 
  GPIO_Speed_2MHz   = 0x00, /*!< 2MHz   */
  GPIO_Speed_25MHz  = 0x01, /*!< 25MHz  */
  GPIO_Speed_50MHz  = 0x02, /*!< 50MHz  */
  GPIO_Speed_100MHz = 0x03  /*!<100MHz  */
}GPIOSpeed_TypeDef;


  

/** 
  *GPIO��/��������ö�ٶ���
  */ 
typedef enum
{ 
  GPIO_PuPd_NOPULL = 0x00,/*����*/
  GPIO_PuPd_UP     = 0x01, /*����*/
  GPIO_PuPd_DOWN   = 0x02  /*����*/
}GPIOPuPd_TypeDef;



/** 
  * GPIO��ʼ���ṹ�����Ͷ���
  */ 
typedef struct 
{
  uint32_t GPIO_Pin;              /*!< ѡ��Ҫ���õ�GPIO����
                                        ������ GPIO_Pin_ ����ĺ� */

  GPIOMode_TypeDef GPIO_Mode;     /*!< ѡ��GPIO���ŵĹ���ģʽ
                                       ������ GPIOMode_TypeDef �����ö��ֵ*/

  GPIOSpeed_TypeDef GPIO_Speed;   /*!< ѡ��GPIO���ŵ�����
                                       ������ GPIOSpeed_TypeDef �����ö��ֵ */

  GPIOOType_TypeDef GPIO_OType;   /*!< ѡ��GPIO�����������
                                       ������ GPIOOType_TypeDef �����ö��ֵ*/

  GPIOPuPd_TypeDef GPIO_PuPd;     /*!<ѡ��GPIO���ŵ���/����ģʽ
                                       ������ GPIOPuPd_TypeDef �����ö��ֵ*/
}GPIO_InitTypeDef;



void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);


