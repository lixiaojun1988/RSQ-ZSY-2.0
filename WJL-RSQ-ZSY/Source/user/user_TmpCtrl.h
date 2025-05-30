#ifndef USER_TMPCTRL_H
#define USER_TMPCTRL_H

#define DATAFLOW_LENGTH 10
#define REALLOAD_LENGTH 10
#define SETLOAD_LENGTH 25

#define TM_STARTBURST 120

typedef struct
{
	ST_SEG_LOAD_T astSegLoad[SUBSEG_MAX];
    uint16_t au16BlfSetRec[5];
	int16_t as16SegPercent[SUBSEG_MAX];//各分段偏差百分比
	uint16_t u16TheroyBlf;	  // 理论比例阀
	uint16_t u16OrgTheroyBlf; // 原始理论比例阀
	int16_t s16PIDBlf;		  // PID比例阀
	int16_t s16ModBlf;
	uint16_t u16CalcLoad; // 计算的理论负荷
	uint16_t u16SetLoad;  // 设置负荷
    uint16_t u16BlfIStart;
    uint16_t u16BurstLimit_100ms;
	//uint16_t u16OutLoad;
	uint8_t u8StartBurst_100ms;
	uint8_t u8BurstStable_100ms;
    
    uint8_t u8AddGasDelay_100ms;
	uint8_t u8PidPause_100ms;
	uint8_t u8PidNoRun_100ms;
	uint16_t u16CurvePercent;
	uint16_t u16LoadPercent;
	uint16_t u16MaxSegPercent;
	uint8_t bFirstStable;
	uint8_t bHeatStable;//温度稳定标志位（避免负荷修正后，温度能烧到目标温度但理论负荷不在而换挡）
	uint8_t bFirstSwSeg;//进入燃烧后正常换挡

} ST_TMPCTRL_T;
extern void TmpCtrlInit(void);
extern void TmpCtrlProcess(void);
extern ST_TMPCTRL_T *GetTmpCtrlData(void);
extern void Reset_Pid(void);
extern uint16_t ConvertDbgToCtrl_Blf(uint8_t _u8dbgset);
extern uint16_t GetBlfIFromPercent(uint8_t _u8percent);

extern uint8_t GetPercentFromBlfI(uint16_t _u16SetBlfI);
extern uint16_t GetBlfFromLoad(uint16_t _u16SetLoad, uint8_t _u8SetSeg);

extern uint8_t ConvertCtrlToDbg_Blf(uint16_t _u16setblf);

/*
extern uint16_t GetBlfIFromSeg(uint8_t _u8seg);
extern void ClrPidCalRes(void);

extern void SetPIDPause(uint8_t _u8set_100ms);
extern void TmpInput_Updata(void);
extern void ReSetPercent(void);*/

extern void TmpCtrlVarReset(void);
extern void RefreshBlfMaxMin(void);
extern void TmpCtrlPrepare(void);
extern void Reset_Pid(void);
extern uint8_t GetFirstSwSeg(void);
extern void ResetHeatStable(void);
extern void ReSetPercent(void);
#endif
