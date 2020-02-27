#include "bsp_iic_gpio.h"

static void I2C_GPIOConfigure(void);

/**
 * @brief  I2C延迟函数
 * @note   CPU主频168MHz ，MDK编译环境，1级优化
 *          循环次数为20~250时都能通讯正常
 * @retval None
 */
static void I2C_Delay(void)
{
    uint8_t i;
    for (i = 0; i < 70; i++);
}

/**
 * @brief  CPU发起起始信号
 * @note    SDA      SCL
 * 初始状态    0        0
 * 启动状态    1     1->0
 * 当SCL为高时，SDA下跳为起始信号
 * @retval None
 */
void I2C_Start(void)
{
    EEPROM_I2C_SDA_1();
    EEPROM_I2C_SCL_1();
    I2C_Delay();
    EEPROM_I2C_SDA_0();
    I2C_Delay();
    EEPROM_I2C_SCL_0();
    I2C_Delay();
}

/**
 * @brief  CPU发起终止信号
 * @note   SDA      SCL
 * 初始状态    x        x
 * 终止状态    1     0->1
 * 当SCL为高时，SDA上跳为终止信号
 * @retval None
 */
void I2C_Stop(void)
{
    EEPROM_I2C_SDA_0();
    EEPROM_I2C_SCL_1();
    I2C_Delay();
    EEPROM_I2C_SDA_1();
}

/**
 * @brief CPU发送一个字节数据 
 * @note   SDA      SCL
 * 初始状态    x       0
 * 数据变化 x->x       0
 * 发送数据    x    0->1
 * @param  _ucByte: 待发送的字节
 * @retval None
 */
void I2C_SendByte(uint8_t _ucByte)
{
    uint8_t i;

    //发送高七位数据
    for (i = 0; i < 8; i++)
    {
        //准备数据,SDA线变化
        if (_ucByte & 0x80)
        {
            EEPROM_I2C_SDA_1(); // x -> 1
        }
        else
        {
            EEPROM_I2C_SDA_0(); //x -> 0
        }
        I2C_Delay();
        //发送数据,SCL线变化
        EEPROM_I2C_SCL_1(); //x -> 1
        I2C_Delay();

        //拉低SCL，允许SDA变化
        EEPROM_I2C_SCL_0();

        if (7 == i)
        {
            EEPROM_I2C_SDA_1(); //释放总线
        }
        _ucByte <<= 1;
        I2C_Delay();
    }
}

/**
 * @brief  CPU从I2C总线设备读取8bit数据
 * @note   SDA      SCL
 * 初始状态    x       0
 * 接收数据    x    0->1
 * 数据变化 x->x    1->0
 * @retval 读到的数据
 */
uint8_t I2C_ReadByte(void)
{
    uint8_t i, value;

    for (i = 0; i < 8; i++)
    {
        //准备读取地址
        value <<= 1;
        //读取数据，禁止SDA变化
        EEPROM_I2C_SCL_1();
        I2C_Delay();
        if (EEPROM_I2C_SDA_READ())
        {
            value++; //等价于 value | EEPROM_II_SDA_READ();
        }
        //允许数据变化
        EEPROM_I2C_SCL_0();
        I2C_Delay();
    }

    return value;
}

/**
 * @brief  CPU产生一个时钟，并读取器件的ACK应答信号
 * @note   
 * @retval 返回0表示正确应答，1表示无器件响应
 */
uint8_t I2C_WaitAck(void)
{
    uint8_t re;

    EEPROM_I2C_SDA_1(); //CPU释放SDA总线
    I2C_Delay();
    EEPROM_I2C_SCL_1(); //CPU驱动SCL = 1, 此时器件会返回ACK应答
    I2C_Delay();

    if (1 == EEPROM_I2C_SDA_READ())
    {
        re = 1;
    }
    else
    {
        re = 0;
    }
    EEPROM_I2C_SCL_0();
    I2C_Delay();
    return re;
}

/**
 * @brief  CPU产生一个ACK信号
 * @note   
 * @retval 
 */
void I2C_Ack(void)
{
    EEPROM_I2C_SDA_0(); //CPU驱动SDA = 0;
    I2C_Delay();
    EEPROM_I2C_SCL_1(); //CPU产生1个时钟
    I2C_Delay();
    EEPROM_I2C_SCL_0();
    I2C_Delay();
    EEPROM_I2C_SDA_1(); //CPU释放SDA总线
}

/**
 * @brief  CPU产生1个NACK信号
 * @note   
 * @retval None
 */
void I2C_NAck(void)
{
    EEPROM_I2C_SDA_1(); //CPU驱动SDA = 1
    I2C_Delay();
    EEPROM_I2C_SCL_1(); //CPU产生一个时钟
    I2C_Delay();
    EEPROM_I2C_SCL_0();
}

/**
 * @brief  配置I2C总线的GPIO，采用模拟IO的方式实现
 * @note   STM32有硬件I2C,现在的是软件I2C
 * @retval None
 */
static void I2C_GPIOConfigure(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(EEPROM_I2C_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SCL_PIN | EEPROM_I2C_SDA_PIN;
    GPIO_Init(EEPROM_I2C_GPIO_PORT, &GPIO_InitStructure);

    //复位I2C总线上所有设备到待机模式
    I2C_Stop();
}

/**
 * @brief  检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
 * @note   
 * @param  _Address: 设备的I2C总线地址
 * @retval 返回值 0 表示正确， 返回1表示未探测到
 */
uint8_t I2C_CheackDevice(uint8_t _Address)
{
    uint8_t ucAck;
    I2C_GPIOConfigure(); //配置GPIO

    I2C_Start(); //发送启动信号

    //地址+读写位（0 = w , 1 = r）
    I2C_SendByte(_Address | EEPROM_I2C_WR);
    ucAck = I2C_WaitAck();

    I2C_Stop();

    return ucAck;
}
