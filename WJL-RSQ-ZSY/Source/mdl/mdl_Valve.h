#ifndef MDL_VALVE_H
#define MDL_VALVE_H

#define MDL_VALVE_FORCE_TIME 4

typedef enum
{
	EN_VALVE_MAIN,
	EN_VALVE_1,
	EN_VALVE_2,
	EN_VALVE_3,
	VALVE_NUM_MAX
} EN_VALVE_T;

typedef union
{
	uint8_t BYTE;
	struct
	{
		uint8_t fgCheckShort : 1;
		uint8_t fgCheckOpen : 1;
		uint8_t fgWorkOpen : 1;
		uint8_t Res : 5;
	} Bits;
} UN_VALVE_ERROR_T;

typedef struct
{
	UN_VALVE_ERROR_T unError;
	uint8_t u8ValveErrCnt;
	uint8_t u8ValveErrOpenCnt;
	uint8_t u8ValveErrorWorkCnt;
	uint8_t u8ValveOpenCnt;
} MDL_VALVE_OUTPUT_T;

typedef union
{
	uint8_t BYTE;
	struct
	{
		uint8_t fgMainON : 1;
		uint8_t fg1ON : 1;
		uint8_t fg2ON : 1;
		uint8_t fg3ON : 1;
		uint8_t fg4ON : 1;
		uint8_t res : 3;
	} Bits;
} UN_VALVE_ON_T;

typedef union
{
	uint8_t BYTE;
	struct
	{
		uint8_t fgMainForce : 1;
		uint8_t fg1Force : 1;
		uint8_t fg2Force : 1;
		uint8_t fg3Force : 1;
		uint8_t fg4Force : 1;
		uint8_t Res : 3;
	} Bits;
} UN_VLALVE_FORCE_T;

extern UN_VALVE_ERROR_T GetValveError(void);
extern uint8_t ValveCheckFunction(void);
extern void ValveWorkFunction(void);
extern void ValveErrorReset(void);
extern void ValveWorkCtrl(void);
extern void SetValveOnOff(EN_VALVE_T index, uint8_t state);
extern uint8_t GetValveOnOffState(EN_VALVE_T index);
extern UN_VALVE_ON_T  *pGetValveState(void);
#endif
