#ifndef DRV_INOUTPUT_H
#define DRV_INOUTPUT_H
#define TM_DRV_INOUTPUT_TIMER 1
#define PORT_FILER_CNT (20 / TM_DRV_INOUTPUT_TIMER)
typedef enum
{
	EN_INPORT_OVERTMP,
	EN_INPORT_VALVE,
	EN_INPORT_FANPRESS,
	EN_INPORT_FLUX,
	EN_INPORT_FSPEED,
	EN_INPORT_FIRE,
	EN_INPORT_WBLFRS,
	EN_INPORT_PUMPSPD,
	EN_INPORT_MAX
}EN_INPORTS_T;

typedef enum
{
	EN_OUTPORT_WPump,
	EN_OUTPORT_Valve1,
	EN_OUTPORT_Valve2,
	EN_OUTPORT_Valve3,
	EN_OUTPORT_Valve4,
	EN_OUTPORT_ValveMain,
	EN_OUTPORT_IGINTION,
	EN_OUTPORT_ValveAll,
	EN_OUTPORT_WBLF,
    EN_OUTPORT_FjBrake,
	EN_OUTPORT_MAX
}EN_OUTPORTS_T;

typedef struct
{
	uint8_t	bOverTmpChk:1;
	uint8_t	bValveCheck:1;
	uint8_t	bFanPressSw:1;
	uint8_t	bWaterFluxIO:1;
	uint8_t	bFanSpeedIO:1;
	uint8_t	bFireFeedBack:1;
	uint8_t	bWBLFReset:1;
	uint8_t	bPumpSpeedIO:1;
}ST_BOARD_INPUTS_T;
typedef	union
{
	uint8_t	u8Byte;
	ST_BOARD_INPUTS_T bits;
}UN_BOARD_INPUTS_T;

typedef struct
{
	UN_BOARD_INPUTS_T	unNewSta;
	UN_BOARD_INPUTS_T	unLastSta;
	uint8_t	u8KeepTime[8];
}ST_IN_PORT_REALTIME_T;
extern	const	ST_BOARD_INPUTS_T* GetInportsSta(void);
extern	const	ST_BOARD_INPUTS_T* GetInportNewSta(void);
extern	uint8_t	GetWaveNewSta(void);
extern	void    Drv_InOutput_Init(void);
extern	void	Drv_OutPut_Set(EN_OUTPORTS_T enPort, uint8_t	_u8set);
extern	void	Drv_InOutput_Process(void);
#endif
