#ifndef __USER_DEBUG_H__
#define __USER_DEBUG_H__

typedef struct
{
	uint8_t u8Pl;
	uint8_t u8Dh;
	uint8_t u8Ph;

	uint8_t u8Fh;
	uint8_t u8Fl;
	uint8_t u8Hc;
	uint8_t u8Lc;
} DBG_MACH_DEF;

typedef struct
{
	EN_FAN_TYPE_T enFanType;
	uint16_t u16Max;
	uint16_t u16Min;
} ST_FAN_TYPE_FREP_T;
extern const ST_DBG_INFO_T *GetDbgInfo(uint8_t _DbgIndex);
extern uint8_t GetDebugIndexMax(void);
extern void GetCommDebugData(ST_COMM_SET_T *pGetCommRevData);
extern void FanTypeJudge(void);
#endif
