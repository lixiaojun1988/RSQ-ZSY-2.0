
/*************************************************************************
 * Copyright:
 * File:         led.c
 * Author:       xjli
 * Version:      v1.0
 * Brief:        单板LED驱动程序
 * Date:         2023-6-14
 *************************************************************************/
#include "all_head.h"

#define LED_ScanCntlimit 10
// 按键灯亮度参数设置
code unsigned char LedAloneBrightRemapTab[LED_Bright_SUM] =
	{
		0,
		2,
		3,
		10,
};
typedef union
{
	unsigned char ucDatas[ledAlone_sum];
	struct
	{
		unsigned char com1;
		unsigned char com2;
		unsigned char com3;
		unsigned char com4;
		unsigned char com5;
		unsigned char com6;
		unsigned char com7;
		unsigned char com8;
		unsigned char com9;
	} LedCom;
} LedAlone; // 独立LED
LedAlone LedAlone_t;
// 单LED扫描函数
void ledAloneScan(void)
{
	uint8_t i;
	static unsigned char ledkeyBrightnessCnt = 0;
	if (++ledkeyBrightnessCnt >= LED_ScanCntlimit)
		ledkeyBrightnessCnt = 0;
	PORT_LED_3 = 0;
	PORT_LED_5 = 0;
	PORT_LED_1 = 0;
	PORT_LED_2 = 0;
	PORT_LED_4 = 0;
	for (i = 0; i < ledAlone_sum; i++)
	{
		if (ledkeyBrightnessCnt < LedAlone_t.ucDatas[i])
		{
			if (0 == i)
				PORT_LED_1 = 1;
			else if (1 == i)
				PORT_LED_4 = 1;
			else if (2 == i)
				PORT_LED_5 = 1;
			else if (3 == i)
				PORT_LED_2 = 1;
			else if (4 == i)
				PORT_LED_3 = 1;
		}
	}
}
// LED 扫描函数
void LED_ScanDrive(void)
{
	ledAloneScan();
}
void led_Alone_set(Led_Alone_Type led, LED_Bright_State status)
{
	LedAlone_t.ucDatas[led] = LedAloneBrightRemapTab[status];
}
void LED_init(void)
{
	memset(&LedAlone_t, 0, sizeof(LedAlone_t));
}
void led_hard_dispAllClose(void)
{
	PORT_LED_3 = 0;
	PORT_LED_5 = 0;
	PORT_LED_1 = 0;
	PORT_LED_2 = 0;
	PORT_LED_4 = 0;
}