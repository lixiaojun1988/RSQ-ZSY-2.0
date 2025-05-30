#ifndef USER_PUMP_H
#define USER_PUMP_H

#define PUMP_RUN_FREQ 10  // 1s内刷新次数
#define PUMP_WORK_TIME 20 // 水泵停止后，仍有水流时间（0.1s）

typedef enum
{
	EN_PUMP_UNWORK, // 水泵不工作
	EN_PUMP_HEAT,	// 即热
	EN_PUMP_CYCLE,	// 后循环
	EN_PUMP_COLD,	// 防冻
	EN_PUMP_BURST,	// 增压
	EN_PUMP_FORCE,
	EN_PUMP_MAX
} EN_PUMP_STA_T;

typedef enum
{
    EN_BURST_DBG,
    EN_BURST_LOW,
    EN_BURST_HIGH,
    EN_BURST_5L,
    EN_BURST_6L,
    EN_BURST_7L,
    EN_BURST_8L,
    EN_BURST_9L,
    EN_BURST_10L,
    EN_BURST_AI,
    EN_BURST_MODE_MAX,
}EN_BURST_MODE_T;

typedef struct
{
	uint8_t bPumpWarm : 1; // 保温时间段
    uint8_t bColdErr : 1;  // 防冻故障
    uint8_t bFastHeat : 1; // 手动触发即热，该标志位置1
    uint8_t bHeatFirst : 1;
    uint8_t bBookHeat : 1;//预约时间段内
    uint8_t bPumpWork : 1;//预约时间段内
    uint8_t u2Res : 2;
} ST_PUMP_SIGN_T;

typedef enum
{
	EN_STUDY_NOW,
	EN_STUDY_BEF,
} EN_STUDY_TYPE_T;

typedef struct
{
	EN_PUMP_STA_T enState; // 状态
	uint8_t u8PumpWorkCnt;
	uint8_t u8PumpForceWorkCnt; // 不能判断工作时间（用于循环停止不能进行工作）
	ST_PUMP_SIGN_T bSign; // 信号
} ST_PUMP_OUTPUTS_T;

#define BOOK_SET_MOR    0x01//晨洗
#define BOOK_SET_NIG    0x02//夜浴
#define BOOK_SET_DAY    0X04//全天

extern ST_PUMP_OUTPUTS_T *GetpPumpParm(void);
extern uint16_t GetPreHeatCD(void);	  // 加热倒计时
extern uint16_t GetPreHeatTime(void); // 即热工作时间
extern uint8_t GetPreHeatStudy(void);
extern void PumpFunction(void);
extern void PumpFunctionInit(void);
#endif
