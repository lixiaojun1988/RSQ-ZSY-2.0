#ifndef USER_SYSTEM_RUN_H
#define USER_SYSTEM_RUN_H
typedef enum
{
	EN_CODE_0,
	EN_CODE_1,
	EN_CODE_2,
	EN_CODE_3,
	EN_CODE_4,
	EN_CODE_5,
	EN_CODE_6,
	EN_CODE_7,
	EN_CODE_8,
	EN_CODE_9,
	EN_CODE_A,
	EN_CODE_b,
	EN_CODE_C,
	EN_CODE_d,
	EN_CODE_E,
	EN_CODE_F,
	EN_CODE_H,
	EN_CODE_L,
	EN_CODE_P,
	EN_CODE_q,
	EN_CODE_U,
	EN_CODE_o,
	EN_CODE_n,
	EN_CODE_MAX
} EN_CODE_T;

typedef enum
{
	EN_ADJ_NO,
	EN_ADJ_BOOL,
	EN_ADJ_EN
} EN_ADJSET_T;
typedef enum
{
	EN_FB_NO = 0,
	EN_FB_TMP = 0x08,
	EN_FB_SPD = 0x10
} EN_FBSET_T;
typedef enum
{
	EN_DSP_HEX = 0,
	EN_DSP_DEC = 0x40
} EN_DISPSET_T;

typedef struct
{
	uint8_t bOnOff : 1;
	uint8_t bWater : 1;
	uint8_t bFlame : 1;
	uint8_t bFanOn : 1;
	uint8_t bError : 1;
	uint8_t bMainSta : 3;
} ST_SYS_STA_T;

typedef union
{
	uint8_t Byte;
	ST_SYS_STA_T BIT;
} UN_SYS_STA_T;

typedef struct
{
	uint8_t u8AdjSet : 3;
	uint8_t u8FeedBack : 3;
	uint8_t u8Encoding : 1;
	uint8_t u8Res : 1;
} ST_DBG_ADDINFO_T;

typedef union
{
	uint8_t Byte;
	ST_DBG_ADDINFO_T BIT;
} UN_DBG_ADDINFO_T;
typedef struct
{
	UN_DBG_ADDINFO_T stAddInfo;
	uint8_t u8CodeH;
	uint8_t u8CodeL;
	uint8_t *pu8Data;
} ST_DBG_INFO_T;

typedef struct
{
	uint8_t u8FanRunning : 1;
	uint8_t WkSw : 1;
	uint8_t bBurnAble : 1;//燃烧请求
	UN_SYS_STA_T sysSta;
	UN_VALVE_ON_T unSetValve; // 设置阀
	EN_FAN_TYPE_T enFanType;  // 风机类型
	uint16_t u16SetBlfI;	  // 设置的比例阀
	uint16_t u16SetDCFanI;	  // 设置直流风机电流
	uint16_t u16FanSpeed;	  // 风速
	uint8_t u8ErrorCode;	  // 错误码
	uint8_t u8ErrorCodePump;   
	uint8_t u8ReIgniTimes;	  // 点火次数
	uint8_t u8ValveError;
	uint8_t Flux;			 // 流量
	uint16_t TmpSet;		 // 设置温度
	uint16_t TmpOut;		 // 出水温度
	uint16_t TmpIn;			 // 进水温度
	uint16_t TmpRoom;		 // 室温
	uint16_t TmpFD;			 // 防冻
	uint16_t u16BlfIRun_Max; // 在u8BlfIRunBasic_Max基础上，根据实际情况下进行调整（如各分段额外加减耗气）
	uint16_t u16BlfIRun_Min; // 在u8BlfIRunBasic_Min基础上，根据实际情况下进行调整（如各分段额外加减耗气）
    uint16_t u16BlfILimit;//刚启动时，比例阀的开度限制，避免短时间内加气太多导致的振动问题
	uint16_t u16BlfIRun_E;
	uint16_t u16ELoad;//当前分段负荷差
	uint8_t bWaitInTmp;
	uint8_t u8ManaulSetSeg;
	uint8_t u8ManaulSetFan;
	uint8_t u8ManaulSetPer;

	// uint8_t u8BlfIRunBasic_Max; // 根据模式，强制代码修正后的最大电流参数（强制代码，后续取代PH使用，影响风速）
	// uint8_t u8BlfIRunBasic_Min; // 根据模式，强制代码修正后的最大电流参数（强制代码，后续取代PL使用，影响风速）
	uint8_t u8FanSpdBasicErrMax; // 根据模式，强制代码修正后的最大电流参数（强制代码，后续取代HC使用）
	uint8_t u8FanSpdBasicErrMin; // 根据模式，强制代码修正后的最大电流参数（强制代码，后续取代LC使用）
	uint8_t u8FanMaxMuxBasic; // 根据模式，强制代码修正后的风机电流变化（最大）
	uint8_t u8FanMinMuxBasic; // 根据模式，强制代码修正后的风机电流变化（最小）


	uint8_t u8HourCur;
	uint8_t u8MinCur;
	uint8_t u8SecCur;
	uint16_t u16HeatLast_1s;

	uint16_t u16EndClean_10ms; //后清扫工作时间
	uint16_t u16E0Check_10ms; //E0判定时间（避免状态切换导致E0时间清零）
	uint16_t u16E6Check_10ms; //E6判定时间
	uint16_t u16E5Check_10ms; //E5故障（强制模式以及用户都是用到）

    //开挂机掉电记忆
    uint16_t u16PowerOnline_10ms;//上电判断离线需要时间
    uint16_t u16PowerIgnoreSLL_10ms;//上电忽略水流量时间
    uint8_t u8PowerProtect;//有水不工作标志位
} SystemRunDataTypeDef;

typedef struct
{
	uint16_t var1;
	uint16_t var2;
	uint16_t var3;
	uint16_t var4;
} fsmVarTypedef; // 状态机计数变量

extern SystemRunDataTypeDef *GetSystemRunData(void);
extern void sysTemRunInit(void);
extern void systemRun(void);
extern void SNReSet(void);
extern void UserWrite(void);
extern void UserWrtieSetCD(uint16_t _Val);
//配置后清扫时间
extern void SetEndCleanTimer(void);
//获取是否处于SUR功能或模式
extern uint8_t GetSurState(void);
#endif
