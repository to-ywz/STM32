#include "string.h"
#include "bsp_iic_ee.h"
#include "bsp_iic_gpio.h"
#include "bsp_debug_usart.h"

/**
 * @brief  判断串行EERPOM是否正常
 * @note   一定要发送停止信号，否则IIC会一直控制总线
 * @retval 1 表示正常， 0 表示不正常
 */
uint8_t ee_CheckOk(void)
{
    if (0 == I2C_CheackDevice(EEPROM_DEV_ADDR))
    {
        return 1;
    }
    else
    {
        I2C_Stop(); //一定要发送停止信号，否则IIC会一直控制总线
        return 0;
    }
}

/**
 * @brief  从串行EEPROM指定地址处开始读取若干数据
 * @note   
 * @param  *_pReadBuf: 存放读到的数据的缓冲区指针
 * @param  _usAddress: 起始地址
 * @param  _usSize: 数据长度，单位为字节
 * @retval 表示失败，1表示成功
 */
uint8_t ee_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize)
{
    uint16_t i;
    /* 采用串行EEPROM随即读取指令序列，连续读取若干字节 */

    //第1步：发起I2C总线启动信号
    I2C_Start();

    //第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读
    I2C_SendByte(EEPROM_DEV_ADDR | EEPROM_I2C_WR);

    //第3步：等待ACK
    if (0 != I2C_WaitAck())
    {
        printf("Send MSB+Add+W/R No ACK\n");
        goto cmd_fail;
    }

    //第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址
    I2C_SendByte((uint8_t)_usAddress);

    //第5步：等待ACK
    if (0 != I2C_WaitAck())
    {
        printf("Send Page Add No Ack\n");
        goto cmd_fail;
    }

    //第6步：重新启动I2C总线。前面的代码的目的向EEPROM传送地址，下面开始读取数据
    I2C_Start();

    //第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读
    I2C_SendByte(EEPROM_DEV_ADDR | EEPROM_I2C_RD);

    //第8步：发送ACK
    if (0 != I2C_WaitAck())
    {
        printf("Send Menory Add No Ack\n");
        goto cmd_fail;
    }

    //第9步：循环读取数据
    for (i = 0; i < _usSize; i++)
    {
        _pReadBuf[i] = I2C_ReadByte();

        if (i != _usSize - 1)
        {
            I2C_Ack(); //中间字节读完后，CPU产生ACK信号
        }
        else
        {
            I2C_NAck(); //最后1个字节读完后，CPU产生NACK信号
        }
    }

    //发送I2C总线停止信号
    I2C_Stop();
    return 1;

cmd_fail: //命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备
    //发送I2C总线停止信号
    I2C_Stop();
    return 0;
}

/**
 * @brief  向串行EEPROM指定地址写入若干数据，采用页写操作提高写入效率
 * @note   
 * @param  *_pWriteBuf: 存放读到的数据的缓冲区指针
 * @param  _usAddress: 起始地址
 * @param  _usSize: 数据长度，单位为字节
 * @retval 表示失败，1表示成功
 */
uint8_t ee_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize)
{
    uint16_t i, m;
    uint16_t usAddr;

    /* 
		写串行EEPROM不像读操作可以连续读取很多字节，每次写操作只能在同一个page。
		对于24xx02，page size = 8
		简单的处理方法为：按字节写操作模式，，每写1个字节，都发送地址
		为了提高连续写的效率: 本函数采用page wirte操作。
	*/

    usAddr = _usAddress;
    for (i = 0; i < _usSize; i++)
    {
        /* 当发送第1个字节或是页面首地址时，需要重新发起启动信号和地址 */
        if ((i == 0) || (usAddr & (EEPROM_PAGE_SIZE - 1)) == 0)
        {
            /*　第０步：发停止信号，启动内部写操作　*/
            I2C_Stop();

            /* 通过检查器件应答的方式，判断内部写操作是否完成, 一般小于 10ms 			
				CLK频率为200KHz时，查询次数为30次左右
			*/
            for (m = 0; m < 1000; m++)
            {
                /* 第1步：发起I2C总线启动信号 */
                I2C_Start();

                /* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
                I2C_SendByte(EEPROM_DEV_ADDR | EEPROM_I2C_WR); /* 此处是写指令 */

                /* 第3步：发送一个时钟，判断器件是否正确应答 */
                if (I2C_WaitAck() == 0)
                {
                    break;
                }
            }
            if (m == 1000)
            {
                printf("Time Out!\n");
                goto cmd_fail; /* EEPROM器件写超时 */
            }

            /* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
            I2C_SendByte((uint8_t)usAddr);

            /* 第5步：等待ACK */
            if (I2C_WaitAck() != 0)
            {
                printf("Send Add No Ack\n");
                goto cmd_fail; /* EEPROM器件无应答 */
            }
        }

        /* 第6步：开始写入数据 */
        I2C_SendByte(_pWriteBuf[i]);

        /* 第7步：发送ACK */
        if (I2C_WaitAck() != 0)
        {
            printf("Send Data No Ack\n");
            goto cmd_fail; /* EEPROM器件无应答 */
        }

        usAddr++; /* 地址增1 */
    }

    /* 命令执行成功，发送I2C总线停止信号 */
    I2C_Stop();
    return 1;

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
    /* 发送I2C总线停止信号 */
    I2C_Stop();
    return 0;
}

/**
 * @brief  擦除eeprom
 * @note   
 * @retval None
 */
void ee_Erase(void)
{
    //char SaveStr[] = "祖张依世界第一可爱!";
    uint16_t i;
    uint8_t buf[EEPROM_SIZE];

    /* 填充缓冲区 */
    for (i = 0; i < EEPROM_SIZE; i++)
    {
        buf[i] = 0xFF;
    }

    /* 写EEPROM, 起始地址 = 0，数据长度为 256 */
    if (ee_WriteBytes(buf, 0, EEPROM_SIZE) == 0)
    {
        printf("擦除eeprom出错！\r\n");
        return;
    }
    else
    {
        printf("擦除eeprom成功！\r\n");
    }
}

/**
 * @brief  eeprom延迟函数
 * @note   简单的延时函数
 * @param  nCount: 延时时间
 * @retval None
 */
static void ee_Delay(__IO uint32_t nCount)
{
    for (; nCount != 0; nCount--)
        ;
}

/**
 * @brief  检测数据正确性
 * @note   
 * @param  read_buf[]: 写入数组
 * @param  write_buf[]: 读出数组
 * @param  len: 数组规模
 * @retval -1：正确 | x：错误
 */
static int ComparedData(uint8_t read_buf[], uint8_t write_buf[], int len)
{
    uint8_t i;
    for (i = 0; i < len; i++)
    {
        if (read_buf[i] != write_buf[i])
        {
            return i;
        }
    }
    return -1;
}

/**
 * @brief  串口打印字符
 * @note   
 * @retval None
 */
static void PrintChar(uint8_t read_buf[], uint8_t write_buf[], int len, int error)
{
    uint8_t i;
    if (-1 == error)
    {
        for (i = 0; i < len; i++)
        {
            printf("%c ", read_buf[i]);

            if ((i & 15) == 15)
            {
                printf("\r\n");
            }
        }
        return;
    }

    printf("原数据：%c \n", write_buf[i]);
    printf("读数据：%c \n", read_buf[i]);
    printf("错误:EEPROM读出与写入的数据不一致");
}

/**
 * @brief  串口打印int数字
 * @note   
 * @retval None
 */
static void PrintIntger(uint8_t read_buf[], uint8_t write_buf[], int len, int error)
{
    uint8_t i;

    if (-1 == error)
    {
        for (i = 0; i < len; i++)
        {
            printf(" %03d", read_buf[i]);

            if ((i & 15) == 15)
            {
                printf("\r\n");
            }
        }
        printf("\r\n");
        return;
    }

    printf("\r\n原数据：%03d\n", write_buf[error]);
    printf("读数据：%03d\n", read_buf[error]);
    printf("错误:EEPROM读出与写入的数据不一致");
}

/**
 * @brief  串口打印中文
 * @note   
 * @retval None
 */
static void PrintChinese(uint8_t read_buf[], uint8_t write_buf[], int len, int error)
{
    if (-1 == error)
    {
        printf("%s\n", read_buf);
        return;
    }
    else
    {
        printf("\r\n原始数据：%s\r\n", write_buf);
        printf("读出数据：%s\r\n", read_buf);
        printf("错误:EEPROM读出与写入的数据不一致");
    }
}

/**
 * @brief  EEPROM测试代码
 * @note    1：字符
 *          2：整型
 *          3：字符串
 * @param  kind: 测试类型
 * @retval 正常返回1，异常返回0
 */
uint8_t ee_Test(int kind, uint8_t Rec[])
{
    uint16_t len = 0;
    //char SaveStr[] = "祖张依世界第一可爱!";
    while(Rec[len] != '\0')
	{
		len++;
	}
    uint16_t i;
    uint8_t write_buf[EEPROM_SIZE];
    uint8_t read_buf[EEPROM_SIZE];
    printf("The length of SaveStr is %d\r\n", len);
    /*-----------------------------------------------------------------------------------*/
    if (ee_CheckOk() == 0)
    {
        /* 没有检测到EEPROM */
        printf("没有检测到串行EEPROM!\r\n");

        return 0;
    }
    /*------------------------------------------------------------------------------------*/
    /* 填充测试缓冲区 */
    for (i = 0; i < len; i++)
    {
        write_buf[i] = Rec[i];
    }
    /*------------------------------------------------------------------------------------*/
    if (0 == ee_WriteBytes(write_buf, 0, len))
    {
        printf("写eeprom出错！\r\n");
        return 0;
    }
    else
    {
        printf("写eeprom成功！\r\n");
    }

    /*写完之后需要适当的延时再去读，不然会出错*/
    ee_Delay(0x0FFFFF);
    /*-----------------------------------------------------------------------------------*/
    if (ee_ReadBytes(read_buf, 0, len) == 0)
    {
        printf("读eeprom出错！\r\n");
        return 0;
    }
    else
    {
        printf("读eeprom成功，数据如下：\r\n");
    }
    /*-----------------------------------------------------------------------------------*/
    //数据打印
    switch (kind)
    {
    case 1:
        PrintChar(read_buf, write_buf, len, ComparedData(read_buf, write_buf, len));
        break;

    case 2:
        PrintChinese(read_buf, write_buf, len, ComparedData(read_buf, write_buf, len));
        break;

    case 3:
        PrintIntger(read_buf, write_buf, len, ComparedData(read_buf, write_buf, len));
        break;
    }

    printf("eeprom读写测试成功\r\n");
    return 1;
}

void ee_Read(uint8_t read_buf[], uint8_t write_buf[], uint16_t len, uint8_t kind)
{
	if (ee_ReadBytes(read_buf, 0, len) == 0)
    {
        printf("读eeprom出错！\r\n");
        return;
    }
    else
    {
        printf("读eeprom成功，数据如下：\r\n");
    }
    /*-----------------------------------------------------------------------------------*/
    //数据打印
    switch (kind)
    {
    case 1:
        PrintChar(read_buf, write_buf, len, ComparedData(read_buf, write_buf, len));
        break;

    case 2:
        PrintChinese(read_buf, write_buf, len, ComparedData(read_buf, write_buf, len));
        break;

    case 3:
        PrintIntger(read_buf, write_buf, len, ComparedData(read_buf, write_buf, len));
        break;
    }
}

void ee_Write(uint8_t write_buf[], uint16_t len)
{
	if (ee_CheckOk() == 0)
	{
		/* 没有检测到EEPROM */
		printf("没有检测到串行EEPROM!\r\n");
	}
	
	if (0 == ee_WriteBytes(write_buf, 0, len))
	{
		printf("写eeprom出错！\r\n");
	}
	else
	{
		printf("写eeprom成功！\r\n");
	}
	ee_Delay(0x0FFFFF);
}
