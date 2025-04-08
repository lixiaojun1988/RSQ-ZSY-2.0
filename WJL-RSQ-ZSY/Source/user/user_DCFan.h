#ifndef USER_DCFAN_H
#define USER_DCFAN_H

#define FANU_MAX (0x31f)
#define FANU_MIN (0)



typedef struct
{
	uint16_t u16FanRunMax;
	uint16_t u16FanRunMin;
	uint16_t u16FanUGet;
	uint16_t u16FanUGetNonStk;
	uint16_t u16FanSpdErrMax;
	uint16_t u16FanSpdErrMin;
	uint16_t u16FanCleanU;
	uint16_t u16FanSpdErr;
	uint16_t u16FanSpdRaw;
    uint8_t u8FanMaxMux;
    uint8_t u8FanMinMux;
	uint8_t u8FanDown_100ms;
	uint8_t u8NonStk_100ms;
	uint8_t u8StkPercent;
	uint8_t u8FanSpdOk;
} ST_DCFAN_T;

typedef struct
{
	uint8_t u8FanSetU;
	uint16_t u16FanSpdRaw;
} ST_FANRAW_T;

extern  ST_DCFAN_T *GetDCFanData(void);
extern uint16_t GetFanUFromBlf(uint16_t _u16SetBlfi);
extern void DCFanProcess(void);
extern uint16_t ConvertDbgToCtrl_DCFan(uint8_t _u8dbgset);
extern uint16_t GetBlfIFromFanSet(uint8_t _u8FanSet);
extern uint16_t DCFan_GetStkSta(uint16_t _u16SetFanU);
extern uint16_t DCFan_NonStkSet(uint16_t _u16SetBlf);
extern void SetDcFanUfromBlf(uint16_t u16SetBlfI);
#endif

