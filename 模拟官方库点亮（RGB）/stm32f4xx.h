
/*Ƭ���������ַ  */
#define PERIPH_BASE           ((unsigned int)0x40000000)                          

/*���߻���ַ */
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000)	

/*GPIO�������ַ*/
#define GPIOA_BASE            (AHB1PERIPH_BASE + 0x0000)
#define GPIOB_BASE            (AHB1PERIPH_BASE + 0x0400)
#define GPIOC_BASE            (AHB1PERIPH_BASE + 0x0800)
#define GPIOD_BASE            (AHB1PERIPH_BASE + 0x0C00)
#define GPIOE_BASE            (AHB1PERIPH_BASE + 0x1000)
#define GPIOF_BASE            (AHB1PERIPH_BASE + 0x1400)
#define GPIOG_BASE            (AHB1PERIPH_BASE + 0x1800)
#define GPIOH_BASE            (AHB1PERIPH_BASE + 0x1C00)

/*RCC�������ַ*/
#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800)


/* GPIOF�Ĵ�����ַ,ǿ��ת����ָ�� */
//#define GPIOF_MODER				*(unsigned int*)(GPIOF_BASE+0x00)
//#define GPIOF_OTYPER			*(unsigned int*)(GPIOF_BASE+0x04)
//#define GPIOF_OSPEEDR			*(unsigned int*)(GPIOF_BASE+0x08)
//#define GPIOF_PUPDR				*(unsigned int*)(GPIOF_BASE+0x0C)
//#define GPIOF_IDR					*(unsigned int*)(GPIOF_BASE+0x10)
//#define GPIOF_ODR					*(unsigned int*)(GPIOF_BASE+0x14)
//#define GPIOF_BSRR					*(unsigned int*)(GPIOF_BASE+0x18)
//#define GPIOF_LCKR					*(unsigned int*)(GPIOF_BASE+0x1C)
//#define GPIOF_AFRL					*(unsigned int*)(GPIOF_BASE+0x20)
//#define GPIOF_AFRH					*(unsigned int*)(GPIOF_BASE+0x24)

/*RCC��AHB1ʱ��ʹ�ܼĴ�����ַ,ǿ��ת����ָ��*/
//�˴���ʹ�ã��ýṹ��������
//#define RCC_AHB1ENR				*(unsigned int*)(RCC_BASE+0x30)


//�Ĵ�����ֵ������оƬ�����Զ����ĵģ���ʹCPUû��ִ�г���Ҳ�п��ܷ����仯
//�������п��ܻ��û��ִ�г���ı��������Ż�

//volatile��ʾ�ױ�ı�������ֹ�������Ż���
#define     __IO    volatile
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;

/* GPIO�Ĵ����б� */
typedef struct
{
	__IO	uint32_t MODER;    /*GPIOģʽ�Ĵ���						��ַƫ��: 0x00      */
	__IO	uint32_t OTYPER;   /*GPIO������ͼĴ���				��ַƫ��: 0x04      */
	__IO	uint32_t OSPEEDR;  /*GPIO����ٶȼĴ���				��ַƫ��: 0x08      */
	__IO	uint32_t PUPDR;    /*GPIO����/�����Ĵ���			��ַƫ��: 0x0C      		*/
	__IO	uint32_t IDR;      /*GPIO�������ݼĴ���				��ַƫ��: 0x10      		*/
	__IO	uint32_t ODR;      /*GPIO������ݼĴ���				��ַƫ��: 0x14      		*/
	__IO	uint16_t BSRRL;    /*GPIO��λ/��λ�Ĵ��� ��16λ����	��ַƫ��: 0x18 	*/
	__IO	uint16_t BSRRH;    /*GPIO��λ/��λ�Ĵ��� ��16λ����	��ַƫ��: 0x1A  */
	__IO	uint32_t LCKR;     /*GPIO���������Ĵ���				��ַƫ��: 0x1C      */
	__IO	uint32_t AFR[2];   /*GPIO���ù������üĴ���		��ַƫ��: 0x20-0x24 		*/
} GPIO_TypeDef;

/*RCC�Ĵ����б�*/
typedef struct
{
__IO	uint32_t CR;            /*!< RCC ʱ�ӿ��ƼĴ���,				��ַƫ��: 0x00 */
__IO	uint32_t PLLCFGR;       /*!< RCC PLL���üĴ���, 				��ַƫ��: 0x04 */
__IO	uint32_t CFGR;          /*!< RCC ʱ�����üĴ���,   		��ַƫ��: 0x08 */
__IO	uint32_t CIR;           /*!< RCC ʱ���жϼĴ���,     		��ַƫ��: 0x0C */
__IO	uint32_t AHB1RSTR;      /*!< RCC AHB1 ���踴λ�Ĵ���,	��ַƫ��: 0x10 */
__IO	uint32_t AHB2RSTR;      /*!< RCC AHB2 ���踴λ�Ĵ���, 	��ַƫ��: 0x14 */
__IO	uint32_t AHB3RSTR;      /*!< RCC AHB3 ���踴λ�Ĵ���, 	��ַƫ��: 0x18 */
__IO	uint32_t RESERVED0;     /*!< ����, 										��ַƫ�ƣ�0x1C */
__IO	uint32_t APB1RSTR;      /*!< RCC APB1 ���踴λ�Ĵ���,	��ַƫ��: 0x20 */
__IO	uint32_t APB2RSTR;      /*!< RCC APB2 ���踴λ�Ĵ���,	��ַƫ��: 0x24 */
__IO	uint32_t RESERVED1[2];  /*!< ����, 										��ַƫ�ƣ�0x28-0x2C*/
__IO	uint32_t AHB1ENR;       /*!< RCC AHB1 ����ʱ�ӼĴ���,	��ַƫ��: 0x30 */
__IO	uint32_t AHB2ENR;       /*!< RCC AHB2 ����ʱ�ӼĴ���,	��ַƫ��: 0x34 */
__IO	uint32_t AHB3ENR;       /*!< RCC AHB3 ����ʱ�ӼĴ���,	��ַƫ��: 0x38 */
	/*RCC���滹�кܶ�Ĵ������˴�ʡ��*/
} RCC_TypeDef;


/*����GPIOA-H �Ĵ����ṹ��ָ��*/
#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC               ((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD               ((GPIO_TypeDef *) GPIOD_BASE)
#define GPIOE               ((GPIO_TypeDef *) GPIOE_BASE)
#define GPIOF               ((GPIO_TypeDef *) GPIOF_BASE)
#define GPIOG               ((GPIO_TypeDef *) GPIOG_BASE)
#define GPIOH               ((GPIO_TypeDef *) GPIOH_BASE)

/*����RCC���� �Ĵ����ṹ��ָ��*/
#define RCC                 ((RCC_TypeDef *) RCC_BASE)

