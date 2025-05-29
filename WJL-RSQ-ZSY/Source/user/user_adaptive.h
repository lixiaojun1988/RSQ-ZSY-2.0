#ifndef __USER_ADAPTIVE_H__
#define __USER_ADAPTIVE_H__
typedef enum
{
	EN_AIR_PRESS_H0, // 海拔状态
	EN_AIR_PRESS_H1,
	EN_AIR_PRESS_H2,
	EN_AIR_PRESS_H3,
	EN_AIR_PRESS_MAX
} EN_AIR_PRESS_TYPE;

typedef enum
{
    EN_LOW_TEMPER_T0, // 非低W温
    EN_LOW_TEMPER_T1,
    EN_LOW_TEMPER_T2,
    EN_LOW_TEMPER_MAX
} EN_LOW_TEMPER_TYPE;
typedef enum
{
    EN_EE_AUTO, // 自动识别
    EN_EE_HAND, // 手动设置
} EN_EE_TYPE;

typedef struct 
{
	int8_t s8HcLc_Add;
	int8_t s8Fh_Add;
	int8_t s8Ph_Add;
}ST_HIGH_ALTITUDE_TYPE;


typedef struct
{
    uint8_t EE_value;
    uint16_t airPressValue;            // 气压值（单位0.1hPa）
    EN_AIR_PRESS_TYPE airPressGrade;   // 海拔分档
    EN_LOW_TEMPER_TYPE lowTemperGrade; // 低温冷态分档
    int8_t Fh_add;                     // 额外增加的代码
    int8_t Fl_add;
    uint8_t AddGasSpedPer;
    int8_t Lc_add;
    int8_t Hc_add;
    int8_t Ph_add;
    uint8_t finalGrade;      // 最终分档;
    uint16_t delayAddGasTime_1s; // 延时加气时间;
    
	  float fSplTemp;//室温
	 float fSplPressure;
} adaptive_typedef;

extern void user_adaptive_init(void);
extern void user_adaptive(void); // 10MS
extern adaptive_typedef *getAdapData(void);
#endif
