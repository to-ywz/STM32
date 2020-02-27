#include "string.h"
#include "bsp_iic_ee.h"
#include "bsp_iic_gpio.h"
#include "bsp_debug_usart.h"

/**
 * @brief  �жϴ���EERPOM�Ƿ�����
 * @note   һ��Ҫ����ֹͣ�źţ�����IIC��һֱ��������
 * @retval 1 ��ʾ������ 0 ��ʾ������
 */
uint8_t ee_CheckOk(void)
{
    if (0 == I2C_CheackDevice(EEPROM_DEV_ADDR))
    {
        return 1;
    }
    else
    {
        I2C_Stop(); //һ��Ҫ����ֹͣ�źţ�����IIC��һֱ��������
        return 0;
    }
}

/**
 * @brief  �Ӵ���EEPROMָ����ַ����ʼ��ȡ��������
 * @note   
 * @param  *_pReadBuf: ��Ŷ��������ݵĻ�����ָ��
 * @param  _usAddress: ��ʼ��ַ
 * @param  _usSize: ���ݳ��ȣ���λΪ�ֽ�
 * @retval ��ʾʧ�ܣ�1��ʾ�ɹ�
 */
uint8_t ee_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize)
{
    uint16_t i;
    /* ���ô���EEPROM�漴��ȡָ�����У�������ȡ�����ֽ� */

    //��1��������I2C���������ź�
    I2C_Start();

    //��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ��
    I2C_SendByte(EEPROM_DEV_ADDR | EEPROM_I2C_WR);

    //��3�����ȴ�ACK
    if (0 != I2C_WaitAck())
    {
        printf("Send MSB+Add+W/R No ACK\n");
        goto cmd_fail;
    }

    //��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ
    I2C_SendByte((uint8_t)_usAddress);

    //��5�����ȴ�ACK
    if (0 != I2C_WaitAck())
    {
        printf("Send Page Add No Ack\n");
        goto cmd_fail;
    }

    //��6������������I2C���ߡ�ǰ��Ĵ����Ŀ����EEPROM���͵�ַ�����濪ʼ��ȡ����
    I2C_Start();

    //��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ��
    I2C_SendByte(EEPROM_DEV_ADDR | EEPROM_I2C_RD);

    //��8��������ACK
    if (0 != I2C_WaitAck())
    {
        printf("Send Menory Add No Ack\n");
        goto cmd_fail;
    }

    //��9����ѭ����ȡ����
    for (i = 0; i < _usSize; i++)
    {
        _pReadBuf[i] = I2C_ReadByte();

        if (i != _usSize - 1)
        {
            I2C_Ack(); //�м��ֽڶ����CPU����ACK�ź�
        }
        else
        {
            I2C_NAck(); //���1���ֽڶ����CPU����NACK�ź�
        }
    }

    //����I2C����ֹͣ�ź�
    I2C_Stop();
    return 1;

cmd_fail: //����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸
    //����I2C����ֹͣ�ź�
    I2C_Stop();
    return 0;
}

/**
 * @brief  ����EEPROMָ����ַд���������ݣ�����ҳд�������д��Ч��
 * @note   
 * @param  *_pWriteBuf: ��Ŷ��������ݵĻ�����ָ��
 * @param  _usAddress: ��ʼ��ַ
 * @param  _usSize: ���ݳ��ȣ���λΪ�ֽ�
 * @retval ��ʾʧ�ܣ�1��ʾ�ɹ�
 */
uint8_t ee_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize)
{
    uint16_t i, m;
    uint16_t usAddr;

    /* 
		д����EEPROM�������������������ȡ�ܶ��ֽڣ�ÿ��д����ֻ����ͬһ��page��
		����24xx02��page size = 8
		�򵥵Ĵ�����Ϊ�����ֽ�д����ģʽ����ÿд1���ֽڣ������͵�ַ
		Ϊ���������д��Ч��: ����������page wirte������
	*/

    usAddr = _usAddress;
    for (i = 0; i < _usSize; i++)
    {
        /* �����͵�1���ֽڻ���ҳ���׵�ַʱ����Ҫ���·��������źź͵�ַ */
        if ((i == 0) || (usAddr & (EEPROM_PAGE_SIZE - 1)) == 0)
        {
            /*���ڣ�������ֹͣ�źţ������ڲ�д������*/
            I2C_Stop();

            /* ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ����, һ��С�� 10ms 			
				CLKƵ��Ϊ200KHzʱ����ѯ����Ϊ30������
			*/
            for (m = 0; m < 1000; m++)
            {
                /* ��1��������I2C���������ź� */
                I2C_Start();

                /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
                I2C_SendByte(EEPROM_DEV_ADDR | EEPROM_I2C_WR); /* �˴���дָ�� */

                /* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
                if (I2C_WaitAck() == 0)
                {
                    break;
                }
            }
            if (m == 1000)
            {
                printf("Time Out!\n");
                goto cmd_fail; /* EEPROM����д��ʱ */
            }

            /* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
            I2C_SendByte((uint8_t)usAddr);

            /* ��5�����ȴ�ACK */
            if (I2C_WaitAck() != 0)
            {
                printf("Send Add No Ack\n");
                goto cmd_fail; /* EEPROM������Ӧ�� */
            }
        }

        /* ��6������ʼд������ */
        I2C_SendByte(_pWriteBuf[i]);

        /* ��7��������ACK */
        if (I2C_WaitAck() != 0)
        {
            printf("Send Data No Ack\n");
            goto cmd_fail; /* EEPROM������Ӧ�� */
        }

        usAddr++; /* ��ַ��1 */
    }

    /* ����ִ�гɹ�������I2C����ֹͣ�ź� */
    I2C_Stop();
    return 1;

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
    /* ����I2C����ֹͣ�ź� */
    I2C_Stop();
    return 0;
}

/**
 * @brief  ����eeprom
 * @note   
 * @retval None
 */
void ee_Erase(void)
{
    //char SaveStr[] = "�����������һ�ɰ�!";
    uint16_t i;
    uint8_t buf[EEPROM_SIZE];

    /* ��仺���� */
    for (i = 0; i < EEPROM_SIZE; i++)
    {
        buf[i] = 0xFF;
    }

    /* дEEPROM, ��ʼ��ַ = 0�����ݳ���Ϊ 256 */
    if (ee_WriteBytes(buf, 0, EEPROM_SIZE) == 0)
    {
        printf("����eeprom����\r\n");
        return;
    }
    else
    {
        printf("����eeprom�ɹ���\r\n");
    }
}

/**
 * @brief  eeprom�ӳٺ���
 * @note   �򵥵���ʱ����
 * @param  nCount: ��ʱʱ��
 * @retval None
 */
static void ee_Delay(__IO uint32_t nCount)
{
    for (; nCount != 0; nCount--)
        ;
}

/**
 * @brief  ���������ȷ��
 * @note   
 * @param  read_buf[]: д������
 * @param  write_buf[]: ��������
 * @param  len: �����ģ
 * @retval -1����ȷ | x������
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
 * @brief  ���ڴ�ӡ�ַ�
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

    printf("ԭ���ݣ�%c \n", write_buf[i]);
    printf("�����ݣ�%c \n", read_buf[i]);
    printf("����:EEPROM������д������ݲ�һ��");
}

/**
 * @brief  ���ڴ�ӡint����
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

    printf("\r\nԭ���ݣ�%03d\n", write_buf[error]);
    printf("�����ݣ�%03d\n", read_buf[error]);
    printf("����:EEPROM������д������ݲ�һ��");
}

/**
 * @brief  ���ڴ�ӡ����
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
        printf("\r\nԭʼ���ݣ�%s\r\n", write_buf);
        printf("�������ݣ�%s\r\n", read_buf);
        printf("����:EEPROM������д������ݲ�һ��");
    }
}

/**
 * @brief  EEPROM���Դ���
 * @note    1���ַ�
 *          2������
 *          3���ַ���
 * @param  kind: ��������
 * @retval ��������1���쳣����0
 */
uint8_t ee_Test(int kind, uint8_t Rec[])
{
    uint16_t len = 0;
    //char SaveStr[] = "�����������һ�ɰ�!";
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
        /* û�м�⵽EEPROM */
        printf("û�м�⵽����EEPROM!\r\n");

        return 0;
    }
    /*------------------------------------------------------------------------------------*/
    /* �����Ի����� */
    for (i = 0; i < len; i++)
    {
        write_buf[i] = Rec[i];
    }
    /*------------------------------------------------------------------------------------*/
    if (0 == ee_WriteBytes(write_buf, 0, len))
    {
        printf("дeeprom����\r\n");
        return 0;
    }
    else
    {
        printf("дeeprom�ɹ���\r\n");
    }

    /*д��֮����Ҫ�ʵ�����ʱ��ȥ������Ȼ�����*/
    ee_Delay(0x0FFFFF);
    /*-----------------------------------------------------------------------------------*/
    if (ee_ReadBytes(read_buf, 0, len) == 0)
    {
        printf("��eeprom����\r\n");
        return 0;
    }
    else
    {
        printf("��eeprom�ɹ����������£�\r\n");
    }
    /*-----------------------------------------------------------------------------------*/
    //���ݴ�ӡ
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

    printf("eeprom��д���Գɹ�\r\n");
    return 1;
}

void ee_Read(uint8_t read_buf[], uint8_t write_buf[], uint16_t len, uint8_t kind)
{
	if (ee_ReadBytes(read_buf, 0, len) == 0)
    {
        printf("��eeprom����\r\n");
        return;
    }
    else
    {
        printf("��eeprom�ɹ����������£�\r\n");
    }
    /*-----------------------------------------------------------------------------------*/
    //���ݴ�ӡ
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
		/* û�м�⵽EEPROM */
		printf("û�м�⵽����EEPROM!\r\n");
	}
	
	if (0 == ee_WriteBytes(write_buf, 0, len))
	{
		printf("дeeprom����\r\n");
	}
	else
	{
		printf("дeeprom�ɹ���\r\n");
	}
	ee_Delay(0x0FFFFF);
}
