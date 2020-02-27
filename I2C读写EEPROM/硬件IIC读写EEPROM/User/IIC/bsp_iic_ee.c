#include "bsp_iic_ee.h"
#include "bsp_usart.h"

uint16_t EEPROM_ADDRESS;

static __IO uint32_t I2CTimeout = I2CT_LONG_TIMEOUT;
static uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode);

static void I2C_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //使能IIC 时钟
    EEPROM_I2C_CLK_INIT(EEPROM_I2C_CLK, ENABLE);

    //使能GPIOB时钟（SDA|SCL时钟线）
    RCC_AHB1PeriphClockCmd(EEPROM_I2C_SCL_GPIO_CLK | EEPROM_I2C_SDA_GPIO_CLK, ENABLE);

    //配置GPIO

    //将PXx连接到I2X——SCL上
    GPIO_PinAFConfig(EEPROM_I2C_SCL_GPIO_PORT, EEPROM_I2C_SCL_GPIO_SOURCE, EEPROM_I2C_SCL_GPIO_AF);
    //将PXx连接到I2X——SDA上
    GPIO_PinAFConfig(EEPROM_I2C_SDA_GPIO_PORT, EEPROM_I2C_SDA_GPIO_SOURCE, EEPROM_I2C_SDA_GPIO_AF);

    //GPIO设置配置
    //SCL配置
    GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SCL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

    //SDA配置,继承SCL配置
    GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SDA_PIN;
    GPIO_Init(EEPROM_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
}

/**
 * @brief  I2C 工作模式
 * @note   
 * @retval None
 */
static void I2C_Mode_Config(void)
{
    I2C_InitTypeDef I2C_InitStructure;

    //I2C配置
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; /* 高电平数据稳定，低电平数据变化 SCL 时钟线的占空比 二分*/
    I2C_InitStructure.I2C_OwnAddress1 = I2C_OWN_ADDRESS7;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; /* I2C的寻址模式 */
    I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;                             /* 通信速率 */
    I2C_Init(EEPROM_I2C, &I2C_InitStructure);                                 /* I2C1 初始化 */
    I2C_Cmd(EEPROM_I2C, ENABLE);

    I2C_AcknowledgeConfig(EEPROM_I2C, ENABLE);
}

/**
  * @brief  I2C 外设(EEPROM)初始化
  * @param  无
  * @retval 无
  */
void I2C_EE_Init(void)
{
    I2C_GPIO_Config();

    I2C_Mode_Config();
    /* 根据头文件i2c_ee.h中的定义来选择EEPROM要写入的地址 */
#ifdef EEPROM_Block0_ADDRESS
    /* 选择 EEPROM Block0 来写入 */
    EEPROM_ADDRESS = EEPROM_Block0_ADDRESS;
#endif

#ifdef EEPROM_Block1_ADDRESS
    /* 选择 EEPROM Block1 来写入 */
    EEPROM_ADDRESS = EEPROM_Block1_ADDRESS;
#endif

#ifdef EEPROM_Block2_ADDRESS
    /* 选择 EEPROM Block2 来写入 */
    EEPROM_ADDRESS = EEPROM_Block2_ADDRESS;
#endif

#ifdef EEPROM_Block3_ADDRESS
    /* 选择 EEPROM Block3 来写入 */
    EEPROM_ADDRESS = EEPROM_Block3_ADDRESS;
#endif
}

/**
  * @brief   将缓冲区中的数据写到I2C EEPROM中(按页写)
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址
  *     @arg NumByteToWrite:写的字节数
  * @retval  无
  */
void I2C_EE_BufferWrite(u8 *pBuffer, u8 WriteAddr, u16 NumByteToWrite)
{
    uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;

    //mod 运算求余，若 writeAddr 是 I2C_PageSize 整数倍，运算结果 Addr 值为 0
    Addr = WriteAddr % I2C_PageSize;
    count = I2C_PageSize - Addr;                 //差 count 个数据，刚好可以对齐到页地址
    NumOfPage = NumByteToWrite / I2C_PageSize;   //数据需要写几页
    NumOfSingle = NumByteToWrite % I2C_PageSize; //无法按页写的数据

    //Addr=0,则 WriteAddr 刚好按页对齐
    if (0 == Addr)
    {
        //如果 NumByteToWrite < I2C_PageSize
        if (0 == NumOfPage)
        { //走到这里下次首地址也必然不对齐
            I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
            I2C_EE_WaitEepromStandbyState();
        }
        else
        { //如果 NumByteToWrite > I2C_PageSize
            while (NumOfPage--)
            { //先写完整页的
                I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize);
                I2C_EE_WaitEepromStandbyState();
                WriteAddr += I2C_PageSize;
                pBuffer += I2C_PageSize;
            }
            //如果走进这一步，下一次写如数据首地址无法对其
            if (0 != NumOfSingle)
            { //若有多余的不满一页的数据，把它写完
                I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
                I2C_EE_WaitEepromStandbyState();
            }
        }
    }
    else
    { //如果 WriteAddr 不是按 I2C_PageSize 对齐
        /**
         * 所谓的地址不对齐就是我上一次写到n*PageSize+x(x < PageSize)。
         * 这次开始写的时候发现这个这个地址不是这一页的首地址了。
         * 这时候我们通过 当前首地址 % 页存储大小 (WriteAddr % I2C_PageSize)
         * 来取得剩余的存储空间,@1处 的目的就是先把当前页填满
         * 方便@2, @3处进行正常的页写入
         **/

        if (0 == NumOfPage)
        { //如果 NumByteToWrite < I2C_PageSize，走到这里下次首地址也必然不对齐
            I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
            I2C_EE_WaitEepromStandbyState();
        }
        else
        { // 如果 NumByteToWrite < I2C_PageSize
            /* 地址不对齐多出的 count 分开处理，不加入这个运算 */
            NumByteToWrite -= count;
            NumOfPage = NumByteToWrite / I2C_PageSize;
            NumOfSingle = NumByteToWrite % I2C_PageSize;

            //@1先把 WriteAddr 所在页的剩余字节写了
            if (0 != count)
            {
                I2C_EE_PageWrite(pBuffer, WriteAddr, count);
                I2C_EE_WaitEepromStandbyState();

                /*WriteAddr 加上 count 后，地址就对齐到页了*/
                WriteAddr += count;
                pBuffer += count;
            }
            //@2
            while (NumOfPage--)
            { //把整数页都写了
                I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize);
                I2C_EE_WaitEepromStandbyState();
                WriteAddr += I2C_PageSize;
                pBuffer += I2C_PageSize;
            }
            //@3如果走进这一步，下一次写如数据首地址无法对其
            if (NumOfSingle != 0)
            { //若有多余的不满一页的数据，把它写完
                I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
                I2C_EE_WaitEepromStandbyState();
            }
        }
    }
}

/**
  * @brief   写一个字节到I2C EEPROM中
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址 
  * @retval  无
  */
uint32_t I2C_EE_ByteWrite(u8 *pBuffer, u8 WriteAddr)
{
    //发送起始信号
    I2C_GenerateSTART(EEPROM_I2C, ENABLE);

    //超时检测
    I2CTimeout = I2CT_FLAG_TIMEOUT;

    //检测EV5(1 == SB(Start bit)) 事件并清除标志
    while (0 == I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if (0 == (I2CTimeout--))
        {
            return I2C_TIMEOUT_UserCallback(0);
        }
    }

    //发送EEPROM设备地址
    I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDRESS, I2C_Direction_Transmitter);

    //超时检测
    I2CTimeout = I2CT_FLAG_TIMEOUT;

    //检测 EV6 (1 == ADDR(地址已发送（主模式） /地址匹配（从模式）))事件并清除标志
    while (0 == I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if (0 == (I2CTimeout--))
        {
            return I2C_TIMEOUT_UserCallback(0);
        }
    }

    //发送要写入的 EEPROM 内部地址(即 EEPROM 内部存储器的地址)
    I2C_SendData(EEPROM_I2C, WriteAddr);

    I2CTimeout = I2CT_FLAG_TIMEOUT;
    //检测 EV8 (1 == TxE(Data register empty(send buffer)))事件并清除标志
    while (0 == I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if (0 == (I2CTimeout--))
        {
            return I2C_TIMEOUT_UserCallback(0);
        }
    }

    //I2C停止信号
    I2C_GenerateSTOP(EEPROM_I2C, ENABLE);

    return 1;
}

/**
 * @brief  将缓冲区中的数据写到 I2C EEPROM 中，
 *          采用单字节写入的方式，速度比页写入慢
 * @note   在EEPROM的一个写循环中可以写多个字节，但一次写入的字节数
 *          不能超过EEPROM页的大小，AT24C02每页有8个字节
 *          I2C_EE_BufferWrite函数对该函数进行了优化
 * @param  pBuffer: 缓冲区指针
 * @param  WriteAddr: 写地址
 * @param  NumByteToWrite: 写的字节数
 * @retval 无
 */
uint8_t I2C_EE_ByetsWrite(uint8_t *pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
    uint16_t i;
    uint8_t res;

    //每写一个字节调用一次 I2C_EE_ByteWrite 函数
    for (i = 0; i < NumByteToWrite; i++)
    {
        I2C_EE_WaitEepromStandbyState();
        res = I2C_EE_ByteWrite(pBuffer++, WriteAddr++);
    }
    return res;
}

/**
 * @brief  按页写入
 * @note   写入函数主体跟单字节写入函数是一样的，只是它在发送数据的时候，
 *          使用 for循环控制发送多个数据，发送完多个数据后才产生 I2C 停止信号
 *          只要每次传输的数据小于等于 EEPROM 时序规定的页大小，就能正常传输
 * @param  pBuffer: 缓冲区指针
 * @param  WriteAddr: 写地址
 * @param  NumByteToWrite:写的字节数
 * @retval 
 */
uint32_t I2C_EE_PageWrite(u8 *pBuffer, u8 WriteAddr, u8 NumByteToWrite)
{
    I2CTimeout = I2CT_LONG_TIMEOUT;

    //等待总线空闲
    while (I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_BUSY))
    {
        if ((I2CTimeout--) == 0)
        {
            return I2C_TIMEOUT_UserCallback(4);
        }
    }

    //起始信号
    I2C_GenerateSTART(EEPROM_I2C, ENABLE);

    I2CTimeout = I2CT_FLAG_TIMEOUT;

    /* Test on EV5 and clear it */ // 产生 I2C 起始信号
    while (!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if ((I2CTimeout--) == 0)
        {
            return I2C_TIMEOUT_UserCallback(5);
        }
    }

    /* Send EEPROM address for write */ //发送地址
    I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDRESS, I2C_Direction_Transmitter);

    I2CTimeout = I2CT_FLAG_TIMEOUT;

    /* Test on EV6 and clear it */ //检测 EV6 事件并清除标志
    while (!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if ((I2CTimeout--) == 0)
        {
            return I2C_TIMEOUT_UserCallback(6);
        }
    }
    /* Send the EEPROM's internal address to write to */
    //发送要写入的 EEPROM 内部地址(即 EEPROM 内部存储器的地址)
    I2C_SendData(EEPROM_I2C, WriteAddr);

    I2CTimeout = I2CT_FLAG_TIMEOUT;

    /* Test on EV8 and clear it */ //检测 EV8 事件并清除标志
    while (!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        if ((I2CTimeout--) == 0)
        {
            return I2C_TIMEOUT_UserCallback(7);
        }
    }
    /* While there is data to be written */
    //循环发送 NumByteToWrite 个数据
    while (NumByteToWrite--)
    {
        /* Send the current byte */ // 发送缓冲区中的数据
        I2C_SendData(EEPROM_I2C, *pBuffer);

        /* Point to the next byte to be written */ //指向缓冲区中的下一个数据
        pBuffer++;

        I2CTimeout = I2CT_FLAG_TIMEOUT;

        /* Test on EV8 and clear it */ //检测 EV8 事件并清除标志
        while (!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        {
            if ((I2CTimeout--) == 0)
            {
                return I2C_TIMEOUT_UserCallback(8);
            }
        }
    }

    /* Send STOP condition */ //发送停止信号
    I2C_GenerateSTOP(EEPROM_I2C, ENABLE);

    return 1;
}

/**
 * @brief  从EEPROM里面读取一块数据
 * @note   
 * @param  pBuffer: 存放从EEPROM读取的数据的缓冲区指针
 * @param  ReadAddr: 接收数据的EEPROM的地址
 * @param  NumByteToRead: 要从EEPROM读取的字节数
 * @retval 
 */
uint32_t I2C_EE_BufferRead(u8 *pBuffer, u8 ReadAddr, u16 NumByteToRead)
{
    I2CTimeout = I2CT_LONG_TIMEOUT;

    //*((u8 *)0x4001080c) |=0x80;
    //等待总线空闲
    while (I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_BUSY))
    {
        if ((I2CTimeout--) == 0)
        {
            return I2C_TIMEOUT_UserCallback(9);
        }
    }
    /* Send START condition */ //产生 I2C 起始信号
    I2C_GenerateSTART(EEPROM_I2C, ENABLE);
    //*((u8 *)0x4001080c) &=~0x80;

    I2CTimeout = I2CT_FLAG_TIMEOUT;

    /* Test on EV5 and clear it */ //检测 EV5 事件并清除标志
    while (!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if ((I2CTimeout--) == 0)
        {
            return I2C_TIMEOUT_UserCallback(10);
        }
    }

    /* Send EEPROM address for write */ //发送 EEPROM 设备地址
    I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDRESS, I2C_Direction_Transmitter);

    I2CTimeout = I2CT_FLAG_TIMEOUT;

    /* Test on EV6 and clear it */ //检测 EV6 事件并清除标志
    while (!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if ((I2CTimeout--) == 0)
        {
            return I2C_TIMEOUT_UserCallback(11);
        }
    }
    /* Clear EV6 by setting again the PE bit */ //通过重新设置 PE 位清除 EV6 事件
    I2C_Cmd(EEPROM_I2C, ENABLE);

    /* Send the EEPROM's internal address to write to */
    //发送要读取的 EEPROM 内部地址(即 EEPROM 内部存储器的地址)
    I2C_SendData(EEPROM_I2C, ReadAddr);

    I2CTimeout = I2CT_FLAG_TIMEOUT;

    /* Test on EV8 and clear it */ //检测 EV8 事件并清除标志
    while (!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        if ((I2CTimeout--) == 0)
        {
            return I2C_TIMEOUT_UserCallback(12);
        }
    }
    /* Send STRAT condition a second time */ //产生第二次 I2C 起始信号
    I2C_GenerateSTART(EEPROM_I2C, ENABLE);

    I2CTimeout = I2CT_FLAG_TIMEOUT;

    /* Test on EV5 and clear it */ //检测 EV5 事件并清除标志
    while (!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if ((I2CTimeout--) == 0)
        {
            return I2C_TIMEOUT_UserCallback(13);
        }
    }
    /* Send EEPROM address for read */ //发送 EEPROM 设备地址
    I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDRESS, I2C_Direction_Receiver);

    I2CTimeout = I2CT_FLAG_TIMEOUT;

    /* Test on EV6 and clear it */ //检测 EV6 事件并清除标志
    while (!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
        if ((I2CTimeout--) == 0)
        {
            return I2C_TIMEOUT_UserCallback(14);
        }
    }
    /* While there is data to be read */
    //读取 NumByteToRead 个数据
    while (NumByteToRead)
    {
        if (NumByteToRead == 1)
        {
            /* 若 NumByteToRead=1，表示已经接收到最后一个数据了，
                发送非应答信号，结束传输 */ 
            /* Disable Acknowledgement *///发送非应答信号 
            I2C_AcknowledgeConfig(EEPROM_I2C, DISABLE);

            /* Send STOP Condition *///发送停止信号
            I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
        }

        I2CTimeout = I2CT_LONG_TIMEOUT;
        while (0 == I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))
        {
            if ((I2CTimeout--) == 0)
            {
                return I2C_TIMEOUT_UserCallback(3);
            }
        }
        {
            /* Read a byte from the device *///通过 I2C，从设备中读取一个字节的数据
            *pBuffer = I2C_ReceiveData(EEPROM_I2C);

            /* Point to the next location where the byte read will be saved */
            //存储数据的指针指向下一个地址
            pBuffer++;

            /* Decrement the read bytes counter *///接收数据自减
            NumByteToRead--;
        }
    }

    /* Enable Acknowledgement to be ready for another reception */
    //使能应答，方便下一次 I2C 传输
    I2C_AcknowledgeConfig(EEPROM_I2C, ENABLE);

    return 1;
}

/**
  * @brief  Wait for EEPROM Standby state 
  * @param  无
  * @retval 无
  */
void I2C_EE_WaitEepromStandbyState(void)
{
    vu16 SR1_Tmp = 0;

    do
    {
        /* Send START condition */
        I2C_GenerateSTART(EEPROM_I2C, ENABLE);
        /* Read EEPROM_I2C SR1 register */
        SR1_Tmp = I2C_ReadRegister(EEPROM_I2C, I2C_Register_SR1);
        /* Send EEPROM address for write */
        I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDRESS, I2C_Direction_Transmitter);
    } while (!(I2C_ReadRegister(EEPROM_I2C, I2C_Register_SR1) & 0x0002));

    /* Clear AF flag */
    I2C_ClearFlag(EEPROM_I2C, I2C_FLAG_AF);
    /* STOP condition */
    I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
}

#define MAX_TRIAL_NUMBER 300

//static uint8_t DEBUG_I2C_EE_WaitEepromStandbyState(void)
//{
//    vu16 tmpSR1 = 0;
//    vu32 EETrials = 0;

//    //总线忙等待
//    I2CTimeout = I2CT_FLAG_TIMEOUT;
//    while (1 == I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_BUSY))
//    {
//        if (0 == (I2CTimeout--))
//        {
//            return I2C_TIMEOUT_UserCallback(20);
//        }
//    }

//    //等待从机应答，最多等待300次
//    while (1)
//    {
//        //IIC Start Signal
//        I2C_GenerateSTART(EEPROM_I2C, ENABLE);

//        //检测 EV5 (1 == SB(Start bit))事件并清除标志位
//        I2CTimeout = I2CT_FLAG_TIMEOUT;
//        while (0 == I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
//        {
//            if (0 == (I2CTimeout--))
//            {
//                return I2C_TIMEOUT_UserCallback(21);
//            }
//        }

//        //发送 EEPROM 设备地址
//        I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDRESS, I2C_Direction_Transmitter);

//        //等待 ADDR 标志
//        I2CTimeout = I2CT_LONG_TIMEOUT;

//        do
//        { //一直等待直到 addr 及 af 标志为 1
//            //获取 SR1 寄存器状态
//            tmpSR1 = EEPROM_I2C->SR1;

//            if (0 == (I2CTimeout--))
//            {
//                return I2C_TIMEOUT_UserCallback(22);
//            }
//        } while (0 == ((I2C_SR1_ADDR | I2C_SR1_AF) & tmpSR1));

//        //检查ADDR标志位是否为 1
//        if (tmpSR1 & I2C_SR1_ADDR)
//        {
//            //清除 addr 标志该标志通过读 SR1 及 SR2 清除
//            (void)EEPROM_I2C->SR2;
//            I2C_GenerateSTOP(EEPROM_I2C, ENABLE);

//            return 1;
//        }
//        else
//        {
//            I2C_ClearFlag(EEPROM_I2C, I2C_FLAG_AF);
//        }

//        //超时检查
//        if (EETrials++ == MAX_TRIAL_NUMBER)
//        {
//            return I2C_TIMEOUT_UserCallback(23);
//        }
//    }
//}

/**
  * @brief  Basic management of the timeout situation.
  * @param  errorCode：错误代码，可以用来定位是哪个环节出错.
  * @retval 返回0，表示IIC读取失败.
  */
static uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
    /* Block communication and all processes */
    EEPROM_ERROR("I2C 等待超时!errorCode = %d", errorCode);

    return 0;
}
