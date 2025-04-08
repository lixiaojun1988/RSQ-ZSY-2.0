
#ifndef _DRV_LED_H_
#define _DRV_LED_H_
//led亮度级数
typedef enum
{
	LED_Bright_OFF = 0,
	LED_Bright_half,
	LED_Bright_better,
	LED_Bright_ON,
	LED_Bright_SUM,
} LED_Bright_State;
//IO单独控制的LED
typedef enum
{
	ledAloneIndex_KeyPower,//电源
	ledAloneIndex_Clean,//清洁
	ledAloneIndex_Lamp,//照明
	ledAloneIndex_Blast,//爆炒
	ledAloneIndex_Fan,//风速
	ledAlone_sum
} Led_Alone_Type;
extern void LED_init(void);
extern void LED_ScanDrive(void);
extern void led_Alone_set(Led_Alone_Type led, LED_Bright_State status);
extern void led_hard_dispAllClose(void);
#endif
