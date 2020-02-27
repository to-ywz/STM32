#include "key.h"

int IsPressedKEY1()
{
	unsigned char flag = 0;
	static unsigned char KEY_LOCK = 0; 
	if (KEY_ON == KEY1)
	{
		//DelayMS(5);
		if (UNLOCK == KEY_LOCK)
		{
			flag = 1;
			KEY_LOCK = LOCKED;
		}
	}
	else
	{
		KEY_LOCK = UNLOCK;
	}
	return flag;
}

int IsPressedKEY2()
{
	unsigned char flag = 0;
	static unsigned char KEY_LOCK = 0;
	if (KEY_ON == KEY2)
	{
		//DelayMS(5);
		if (UNLOCK == KEY_LOCK)
		{
			flag = 1;
			KEY_LOCK = LOCKED;
		}
	}
	else
	{
		KEY_LOCK = UNLOCK;
	}
	return flag;
}

int IsPressedKEY3()
{
	unsigned char flag = 0;
	static unsigned char KEY_LOCK = 0;
	if (KEY_ON == KEY3)
	{
		//DelayMS(5);
		if (UNLOCK == KEY_LOCK)
		{
			flag = 1;
			KEY_LOCK = LOCKED;
		}
	}
	else
	{
		KEY_LOCK = UNLOCK;
	}
	return flag;
}

int IsPressedKEY4()
{
	unsigned char flag = 0;
	static unsigned char KEY_LOCK = 0;
	if (KEY_ON == KEY4)
	{
		//DelayMS(5);
		if (UNLOCK == KEY_LOCK)
		{
			flag = 1;
			KEY_LOCK = LOCKED;
		}
	}
	else
	{
		KEY_LOCK = UNLOCK;
	}
	return flag;
}