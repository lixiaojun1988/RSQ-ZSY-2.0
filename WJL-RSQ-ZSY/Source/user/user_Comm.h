#ifndef USER_COMM_H
#define USER_COMM_H
#define TM_USER_COMM_TIMER 10
#define COMM_HEAD_CODE 0xfa

#define FRAMELENGTH_CTRL 8
#define FRAMELENGTH_MAIN 15
#define DATALENGTH_CTRL 4
#define DATALENGTH_MAIN 11

#define COMMSET_NULL 0xFF
#define SN_CODE_NUM 9

#define PRI_NONE 0x7
#define	SOFT_VERSION	4
typedef enum
{
	EN_RESET_ONOFF,

	// 婵☆垪鈧磭纭€闂佽法鍠愰弸濠氬箯閻戣姤鏅搁柡鍌樺€栫€氾拷
	EN_RESET_TMP_SET,
	EN_RESET_FLU_SET,
	EN_RESET_MODE_SET,
	EN_RESET_WATER_SET,

	// 闂佽法鍠愰弸濠氬箯闁垮宕遍梺璺ㄥ枑濠㈠啰娑甸柨瀣伓
	EN_RESET_CLR_W,
	EN_RESET_CLR_G,

	EN_RESET_CH4,
	EN_RESET_CO,

	// 婵ɑ鎸抽弫鎾绘儗椤愩儺鍤嬮柟椋庡厴閺佹捇寮妶鍡楊伓
	EN_RESET_HEAT,

	// 闂佽法鍠愰弸濠氬箯閻戣姤鏅搁柡鍌樺€栫€氾拷
	EN_RESET_HAND,
	EN_RESET_HAND_EN,

	// 闂佽法鍠愰弸濠氬箯閻戣姤鏅搁柡鍌樺€栫€氬綊宕板▎娆掝吅闁归鍏橀弫鎾绘晸閿燂拷
	EN_RESET_SELF_CLEAN,

	// SUR闂佽法鍠愰弸濠氬箯缁嬵檳O
	EN_RESET_SUR,
	EN_RESET_ECO,

	// 闂佽法鍠愰弸濠氬箯閻戣姤鏅搁柡鍌樺€栫€氬綊鏌ㄩ悢鍛婄伄闁归鍏橀弫鎾诲棘閵堝棗顏堕梺璺ㄥ枑閺嬪骞忛敓锟�
	EN_RESET_CYCLE_MODE,
	EN_RESET_WARM_TIME,
	EN_RESET_WARM_DIF,

	EN_RESET_MOR_START,
	EN_RESET_MOR_END,
	EN_RESET_NIG_START,
	EN_RESET_NIG_END,
	EN_RESET_BOOK24_0,
	EN_RESET_BOOK24_1,
	EN_RESET_BOOK24_2,
	EN_RESET_BOOK24_3,
	EN_RESET_BOOK24_4,
	EN_RESET_BOOK24_5,

	// 閻庡湱鍋炲鍌炲籍閸洘鏅搁柡鍌樺€栫€氾拷
	EN_RESET_HOUR,
	EN_RESET_MIN,

	// 鐎殿喗妞介弫鎾绘偑闄囬幓顏堝箯閻戣姤鏅搁柡鍌樺€栫€氬綊鏌ㄩ悢鍛婄伄闁归鍏橀弫鎾绘晸閿燂拷
	EN_RESET_DEBUG_DATA,

	EN_RESET_PRI, // 闂佽法鍠愰弸濠氬箯閻戣姤鏅搁柡鍌樺€栫€氬綊鏌ㄩ悢鍛婄伄闁归鍏橀弫鎾诲棘閵堝棗顏堕柡澶涙嫹
	EN_RESET_OL,  // 闂佽法鍠庨埀顒傚枎椤︻剟鏌ㄩ悢鍛婄伄闁归鍏橀弫鎾诲棘閵堝棗顏堕柣妯垮煐閳ь剨鎷�

	// 闂佽法鍠愰弸濠氬箯閻戣姤鏅搁柡鍌樺€栫€氬湱鈧冻缂氱弧锟�
	EN_RESET_WATER_USE_TIME,
	EN_RESET_MAX
} EN_COMMRESET_T;
typedef enum
{
	EN_ADDR_WIFI,
	EN_ADDR_REMOTE,
	EN_ADDR_WIRE,
	EN_ADDR_CTRL,
	EN_ADDR_MAIN,
	EN_ADDR_ALARM,
	EN_ADDR_MONITOR,
	EN_ADDR_MAX
} EN_DEVADDR_T;
typedef enum
{
	EN_MAIN_FRAME_0,
	EN_MAIN_FRAME_1,
	EN_MAIN_FRAME_2,
	EN_MAIN_FRAME_3,
	EN_MAIN_FRAME_4,
	EN_MAIN_FRAME_5,
	EN_MAIN_FRAME_6,
	EN_MAIN_FRAME_7,
	EN_MAIN_FRAME_8,
	EN_MAIN_FRAME_9,
	EN_MAIN_FRAME_10,
	EN_MAIN_FRAME_11,
	EN_MAIN_FRAME_12,
	EN_MAIN_FRAME_13,
	EN_MAIN_FRAME_14,
	EN_MAIN_FRAME_MAX = 15
} EN_MAIN_FRAME_T;
typedef enum
{
	EN_ADDRCHK_OFFLINE,
	EN_ADDRCHK_ONLINE,
	EN_ADDRCHK_MAX
} EN_ADDRCHKSTA_T;
typedef struct
{
	uint8_t u4AddrPnt;
	uint8_t u4FlamePnt;
} ST_FLAMESET_T;

typedef struct
{
	uint8_t u8Head;
	uint8_t u3TarDevice : 3;
	uint8_t u3SourAddr : 3;
	uint8_t u8Reserved : 2;
	uint8_t u3PriDevice : 3;
	uint8_t u5DeviceComm : 5;
	uint8_t au8CommData[4];
	uint8_t u8CheckSum;
} ST_FRAME_CONTROLLER_T;
typedef union
{
	uint8_t au8DataBuff[8];
	ST_FRAME_CONTROLLER_T stController;
} UN_FRAME_CONTROLLER_T;

typedef struct
{
	uint8_t u8Head;
	uint8_t u3TarDevice : 3;
	uint8_t u3SourAddr : 3;
	uint8_t u8Reserved : 2;
	uint8_t u3PriDevice : 3;
	uint8_t u5MainComm : 5;
	uint8_t au8CommData[11];
	uint8_t u8CheckSum;
} ST_FRAME_MAINBOARD_T;
typedef union
{
	uint8_t u8DataBuff[15];
	ST_FRAME_MAINBOARD_T stMainBoard;
} UN_FRAME_MAINBOARD_T;
/*
typedef struct
{
	ST_UART_RECV_T* stRecvBuff;
	FN_UARTFIRE_T Fn_Comm_UartFire;
}ST_COMM_INPUT_T;*/
typedef enum
{
	EN_COMM_RECV_HEAD,
	EN_COMM_RECV_DATA,
	EN_COMM_RECV_END,
	EN_COMM_RECV_MAX
} EN_COMM_RECV_T;
typedef struct
{
	uint8_t u8RecvPnt;
	uint8_t u8RecvCnt;
	EN_COMM_RECV_T enRecvSta;
	uint8_t u8RecvByteDly_10ms;
	uint8_t u8SendDly_50ms;
	uint8_t Tick_CommCnt;
	uint8_t u8SendDelay;
} ST_COMM_RUNDATA_T;
typedef struct
{
	uint8_t *apu8Data[4];
} ST_CTRLDATA_T;
typedef union
{
	uint8_t Bytes[6];
	struct
	{
		uint8_t stBook4 : 2;
		uint8_t stBook3 : 2;
		uint8_t stBook2 : 2;
		uint8_t stBook1 : 2;
		uint8_t stBook8 : 2;
		uint8_t stBook7 : 2;
		uint8_t stBook6 : 2;
		uint8_t stBook5 : 2;
		uint8_t stBook12 : 2;
		uint8_t stBook11 : 2;
		uint8_t stBook10 : 2;
		uint8_t stBook9 : 2;
		uint8_t stBook16 : 2;
		uint8_t stBook15 : 2;
		uint8_t stBook14 : 2;
		uint8_t stBook13 : 2;
		uint8_t stBook20 : 2;
		uint8_t stBook19 : 2;
		uint8_t stBook18 : 2;
		uint8_t stBook17 : 2;
		uint8_t stBook24 : 2;
		uint8_t stBook23 : 2;
		uint8_t stBook22 : 2;
		uint8_t stBook21 : 2;
	} BookBit;
} UN_BOOK24_T;
typedef void (*COMM_CONTROLLER_FN)(uint8_t, uint8_t);

typedef enum
{
	CTRLCOMM_STANDY = 0,
	CTRLCOMM_SETONOFF = 1,
	CTRLCOMM_WASHMODE = 4,
	CTRLCOMM_GASALARM = 8,
	CTRLCOMM_SETBOOKTIME = 13,
	CTRLCOMM_SN = 20,
	CTRLCOMM_PRIASK = 24,
	CTRLCOMM_EXCOMM = 29,
} CTRLCOMM_EN;

#define SN_READ 0x55
#define SN_WRITE1 0xa0
#define SN_WRITE2 0xa1
#define SN_WRITE3 0xa2
#define SN_FCT 0xFF
#define SN_FCT_STEP 0xF0

typedef struct
{
	uint8_t u8Reserved;
	uint8_t au8AddrOnlineCnt[EN_ADDR_MAX]; // 0-offline;0x02-online
	uint8_t u8PriDev;					  
	/////////////////////////
	uint8_t u8DeviceSta;  // 闂佽法鍠庨埀顒傚枎椤︻剟鎮╅懜纰樺亾閿燂拷
	uint8_t u8DeviceType; // 闂佽法鍠庨埀顒傚枎椤︻剟鏌ㄩ悢鍛婄伄闁归鍏橀弫鎾诲棘閵堝棗顏�
	/////////////////////////
	uint8_t u8OnOffSet;
	/////////////////////////
	uint8_t u8DebugIndex;
	uint8_t u8DebugAct;
	uint8_t u8DebugData;
	/////////////////////////
	uint8_t u8RemoLink;
	/////////////////////////
	uint8_t u8ModeSet;
	uint8_t u8TmpSet;
	uint8_t u8FluxSet;
	uint8_t u8WaterSet;
	/////////////////////////
	uint8_t u8ClrWSum;
	uint8_t u8ClrGSum;
	/////////////////////////
	uint8_t u8HealthWash;
	/////////////////////////
	uint8_t u8BurstMode;
	/////////////////////////
	uint8_t u8CoPPM;
	uint8_t u8CH4PPM;
	uint8_t u8AlarmSta;
	/////////////////////////
	uint8_t u8PreWarmSet;
	/////////////////////////
	uint8_t u8PreWarmTime;
	/////////////////////////
	uint8_t u8PreWarmDif;
	/////////////////////////
	uint8_t u8HourCur;
	uint8_t u8MinCur;
	/////////////////////////
	UN_BOOK24_T unBook24Set;
	/////////////////////////
	uint8_t u8FastHeat;
	/////////////////////////
	uint8_t u8BookSet;
	/////////////////////////
	uint8_t u8ECOSet;
	/////////////////////////
	uint8_t u8WaterControl;
	/////////////////////////
	uint8_t u8X6F1Burst;
	/////////////////////////
	uint8_t u8SurSet;
	/////////////////////////
	uint8_t u8SN_Comm;
	uint8_t au8SNCode[SN_CODE_NUM];
	/////////////////////////
	uint8_t u8LimitMaxSet;
	/////////////////////////
	uint8_t u8X0LearnSet;
	/////////////////////////
	uint8_t u8PWorkTime;
	/////////////////////////
	uint8_t u8PriAsk;
	/////////////////////////
	uint8_t u8DrySet;
	/////////////////////////
	uint8_t u8CycleMode;
	/////////////////////////
	uint8_t u8BookFuncSet;
	/////////////////////////
	uint8_t u8MorWashStart;
	uint8_t u8MorWashEnd;
	uint8_t u8NigWashStart;
	uint8_t u8NigWashEnd;
	/////////////////////////
	uint8_t u8FastDry;
	/////////////////////////
	uint8_t u8NightDry;
	/////////////////////////
	uint8_t u8KitTimeSet;
	/////////////////////////
	uint8_t u8HeatOnce;
	/////////////////////////
	uint8_t u8SOSCall;
	/////////////////////////
	uint8_t GasErrorSet;
	/////////////////////////
	uint8_t u8OneWayValveTmp;
	uint8_t u8OneWayValveLost;
	/////////////////////////
	uint8_t u8OneWayValveDifTmp;
	/////////////////////////
	uint8_t u8HandMoveCapEn;
	uint8_t u8HandMoveCapMode;
	/////////////////////////
	uint8_t u8SelfCleanEn;
	/////////////////////////
	uint8_t u8SmartLearnEn;
	uint8_t u8WaterUseTime;
	/////////////////////////
	uint8_t u8BubbleWaterSet;
	uint8_t	u8Priority;
} ST_COMM_SET_T;

typedef enum
{
	EN_PRI_KEEP = 0,
	EN_PRI_RESET,
} EN_PRI_SET;

typedef enum
{
	EN_MAIN_STATE_NORMAL,
	EN_MAIN_STATE_DEBUG,	
	EN_MAIN_STATE_CHKCODE,
	EN_MAIN_STATE_UNBIND,
	EN_MAIN_STATE_TMPCTRL,
	EN_MAIN_STATE_HILINK,	
	EN_MAIN_STATE_MAX
}EN_MAIN_STATE_T;
extern void SetCommData(EN_COMMRESET_T _enCommand, uint8_t _u8data);
extern void Comm_Init(void);
extern ST_COMM_SET_T *GetCommSet(void);
extern uint8_t GetMonitorOnline(void);
extern const uint8_t *GetFlashData_SNCode(void);
extern void Comm_ClearRecvByteDly(void);
extern  void Comm_Recv(void);
extern  void Comm_Send(void);
extern  void comm_tick(void);
#endif
