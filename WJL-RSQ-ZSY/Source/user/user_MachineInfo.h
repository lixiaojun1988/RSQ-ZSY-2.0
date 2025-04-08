#ifndef	USER_MACHINEINFO_H
#define	USER_MACHINEINFO_H
#include "user_DCFan.h"

#define	FA_MAX_UPPERFAN	25
#define	FA_MAX_LOWERFAN	30
#define	SUBSEG_MAX		4
typedef enum
{
	EN_MAIN_BIT = 0x01,
	EN_SUB1_BIT = 0x02,
	EN_SUB2_BIT = 0x04,
	EN_SUB3_BIT = 0x08,
	EN_SUB4_BIT = 0x10,
	EN_RES_BIT = 0x80
}EN_VALVEBIT_T;

typedef struct
{
	uint16_t	u16MinLoad;
	uint16_t	u16MaxLoad;
}ST_SEG_LOAD_T;

typedef struct
{
	int8_t	s8MinIncr;
	int8_t	s8MaxIncr;
}ST_SEG_INCR_T;

typedef struct
{
	uint8_t	u8FanMaxMux;
	uint8_t	u8FanMinMux;
}ST_FANERR_T;
typedef struct
{
	UN_VALVE_ON_T	aunSubSegSet[SUBSEG_MAX];
	ST_SEG_LOAD_T	astSegLoad[SUBSEG_MAX];
	ST_SEG_INCR_T	astSegIncrBLF[SUBSEG_MAX];
	const	ST_FANRAW_T* pstFanRawData;
	ST_FANERR_T	stFanErrData;
	int16_t	as16LoadArray[10];
	uint16_t u16AddGasSpd;//濠殿噯绲鹃弻褏娆㈤妷鈺傗挀闁逞屽墴瀹曟繈鎮╁畷鍥ㄧ樆闂備緡鍋嗛崰搴ｂ偓瑙勫▕閺佸秹宕煎┑鍡欙紟闂佸憡甯掑Λ娆撴儊椤曗偓閺佸秹鏁撻敓锟�
	uint16_t u16DecGasSpd;//濠殿噯绲鹃弻褏娆㈤妷鈺傗挀闁逞屽墴瀹曟瑨绠涢弮鍌涚樆闂備緡鍋嗛崰搴ｂ偓瑙勫▕閺佸秹宕煎┑鍡欙紟闂佸憡甯掑Λ娆撴儊椤曗偓閺佸秹鏁撻敓锟�
	uint8_t	u8GasQucikReachPer;//闂佸憡鐟﹂敋閻庡灚鐓￠獮鎺楀Ψ閵娧咁唴閻庣懓澹婇崰鏍焵椤掑啫浜规繛鍫熷灴閹倿骞嗚閻庤鈽夐弬娆惧剰妞わ箒娉曟禍鎼佸箣濠靛洤鈧敻鏌ㄥ☉妯肩伇闁诲繑鐩濠氬级閹存繄鍘掗柡澶婄墳閹凤拷
	uint8_t	au8DeadZone[3];
	uint8_t	u8FireSegSet;
	uint8_t	u8MaxSeg;
	int8_t	KP_X_ECK_N;
	int8_t	KI_X_EK_N;
	int8_t	KD_X_ECCK_N;
	uint8_t	u8GasBlfPidOutTimeS;
	uint8_t	u8GasBlfPidOutTimeR;
	uint8_t	u8GasBlfPidOutTimeE;
	uint8_t	u8MachineId;
}ST_WORK_CONDITION_T;

typedef enum
{
	EN_GAS_12T,
	EN_GAS_20Y,
	EN_GAS_07R,
	EN_GAS_MAX
}EN_GAS_T;
typedef enum
{
	EN_STR_NOR,		//闂佽法鍠愰弸濠氬箯閻戣В鍋撻敓锟�
	EN_STR_STL,		//闂佽法鍠愰弸濠氬箯閻戣姤鏅搁柡鍌樺€栫€氬綊鏌ㄩ悤鍌涘
	EN_STR_MAX
}EN_STRUCTURE_T;
typedef enum
{
	EN_CODN_DIS,	//闂佽法鍠愰弸濠氬箯閻戣姤鏅搁柡鍌樺€栫€氬綊鏌ㄩ悢鍛婄伄闁瑰嚖鎷�
	EN_CODN_EN,		//闂佽法鍠愰弸濠氬箯閻戣姤鏅搁柡鍌樺€栫€氾拷
	EN_CODN_MAX
}EN_COOLDOWN_T;
typedef enum
{
	EN_MA_UPFAN,	//闂佽法鍠曠欢婵堢驳鐟欏嫬顏堕梺璺ㄥ櫐閹凤拷
	EN_MA_LOWFAN,	//闂佽法鍠栭幗顏呂楅幘鍐叉闁瑰嚖鎷�
	EN_MA_MAX
}EN_MA_T;
typedef	struct
{
	uint16_t				u16Liter;	//闂佽法鍠愰弸濠氬箯閻戣姤鏅搁柡鍌樺€栫€氾拷
	EN_GAS_T		enGasType;	//闂佽法鍠愰弸濠氬箯閻戣姤鏅搁柡鍌樺€栫€氾拷
	EN_COOLDOWN_T	enoolDown;	//闂佽法鍠愰弸濠氬箯閻戣姤鏅搁柡鍌樺€栫€氾拷
	EN_STRUCTURE_T	enStructure;//闂佽法鍠撶划銊╁几閿燂拷
}ST_FA_INFO_T;
typedef enum
{
	EN_FAN_YUXIANG,
	EN_FAN_XIECHUANG,
	EN_FAN_MAX
}EN_FAN_TYPE_T;
typedef enum
{
	EN_MODEL_Z8L,
	EN_MODEL_MAX
}EN_MODEL_T;
typedef struct
{
	const	ST_FA_INFO_T*	pstFA;//闂佽法鍠愰弸濠氬箯閻戣姤鏅搁柡鍌樺€栫€氱闂佽法鍠愰弸濠氬箯閻戣姤鏅搁柡鍌樺€栫€氱闂佽法鍠愰弸濠氬箯閻戣姤鏅搁柡鍌樺€栫€氱闂佽法鍠撶划銊╁几閿燂拷
	EN_FAN_TYPE_T	enFanType;
	EN_MODEL_T	enModel;
}ST_MA_INFO_T;

typedef struct
{
	EN_MODEL_T enType;
	uint8_t	u8IndexMax;
	const	ST_MA_INFO_T* stInfo;
	void (*GetInfo)(void);
}ST_MODEL_T;

extern	void	UpdateMaInfo(void);
extern	const	ST_WORK_CONDITION_T* GetWorkCon(void);
extern	const	ST_MA_INFO_T*	GetMaInfo(void);
#endif
