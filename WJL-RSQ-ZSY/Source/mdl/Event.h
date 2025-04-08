
#ifndef _EVENT_H_
#define _EVENT_H_
/************************************************************************/
typedef enum
{
	EVENT_KEY_NULL,
	EVENT_KEY_POWER, // 电源
	EVENT_KEY_CLEAN, // 清洁
	EVENT_KEY_BLAST, // 爆炒
	EVENT_KEY_LAMP,	 // 照明
	EVENT_KEY_FAN,	 // 风速
	EVENT_KEY_FAN_LONG,
	EVENT_KEY_CLEAN_LONG, // 清洁长按
//	EVENT_KEY_BT_BIND,	  // 蓝牙绑定模式
	EVENT_KEY_GEST_FUNC,  // 手势开关
	EVENT_KEY_SUM,
} EVENT_KEY;
typedef enum
{
	GESTURE_EVENT_VOID,
	GESTURE_EVENT_WAVE_TO_LEFT,	 // 从右往左挥手
	GESTURE_EVENT_WAVE_TO_RIGHT, // 从左往右挥手
	GESTURE_EVENT_LEFT_HOVER,
	GESTURE_EVENT_RIGHT_HOVER,
	GESTURE_EVENT_SUM
} EVENT_GESTURE;
/************************************************************************/
extern void EventKey_Init(void);
extern void TriggerKeyEvent(EVENT_KEY eEvent);
extern EVENT_KEY GetKeyEvent(void);
/************************************************************************/
extern void EventComm_Init(void);
extern void TriggerCommEvent(uint8_t eEvent); //
extern uint8_t GetCommEvent(void);

extern void EventGesture_Init(void);
extern void TriggerGestureEvent(EVENT_GESTURE Event); //
extern EVENT_GESTURE GetGestureEvent(void);
#endif
