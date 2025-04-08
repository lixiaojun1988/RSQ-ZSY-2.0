#include "ALL_HEAD.H"
#include "stdlib.h"

#define TM_SLOW_BURST 60
#define PERCENT_MIN 1
#define TMP_BRUST 12
#define SETLOAD_LENGTH 25
#define SETFLUX_LENGTH 50
#define REALLOAD_LENGTH 10

#define PID_RUN_CYCLE (5)
#define PID_BIF_LIMIT_MAX (300)
#define PID_BIF_LIMIT_MIN (-300)

#define PID_NONE_RUN_INIT (120)

#define KEEP_ARRARY_NUM_MAX 20 // 稳定温度分段细粒度
#define KEEP_ARRARY_SEG_MAX 3  // 分段阀数量
#define BLF_ERROR_PERC_MIN 600 // PID偏差千分比限制
#define BLF_ERROR_PERC_MAX 1400
typedef struct
{
	uint8_t clacDelay;
	uint8_t firEnterFlag;
	uint16_t notRunCnt;
} PidCtrl;
PidCtrl PidCtrl_t;

ST_TMPCTRL_T stTmpCtrl;
#if 0
typedef struct
{
	uint8_t cnt;
	uint8_t okFlag[KEEP_ARRARY_SEG_MAX][KEEP_ARRARY_NUM_MAX];
	uint8_t CalcCtrlFlag[KEEP_ARRARY_SEG_MAX][KEEP_ARRARY_NUM_MAX];
	uint16_t errorPercent[KEEP_ARRARY_SEG_MAX][KEEP_ARRARY_NUM_MAX];
	uint16_t BlfErrorPercentTemp[KEEP_ARRARY_SEG_MAX][KEEP_ARRARY_NUM_MAX];
	uint32_t sum;
} temper_keep_typeDef;

temper_keep_typeDef temper_keep_t;
#endif
typedef struct
{
	uint16_t u16TmpE;
	uint16_t u16FluxE;
	int16_t s16Flux;
	int16_t s16ELoad;
} ST_LOADEDATA_T;

#if (CtrlTemp_Deep)
static uint32_t u32SetLoadFiterSum;
#define DEEP_VALVE      8//滤波深度
#else
static uint16_t au16SetLoadList[SETLOAD_LENGTH];

static uint16_t au16SetFluxList[SETFLUX_LENGTH];
#endif
static uint8_t u8OverWaterStop_100ms;

#if 0
/**
 * @brief  热值曲线信息结构体
 */
typedef struct
{
	unsigned short pa_min; /*!< 最小二次压 */
	unsigned short pa_max; /*!< 最大二次压 */
	float k2;			   /*!< 负荷转二次压公式二次项系数 */
	float k1;			   /*!< 负荷转二次压公式一次项系数 */
	float c;			   /*!< 负荷转二次压公式常数 */
} LoadCurveInfo_t;

static LoadCurveInfo_t loadCurves[3] =
	{
		{.pa_min = 183,
		 .pa_max = 1134,
		 .k2 = 0.00001f,
		 .k1 = -0.0056f,
		 .c = 11.739f

		},
		{.pa_min = 157,
		 .pa_max = 1055,
		 .k2 = 0.000003f,
		 .k1 = -0.009f,
		 .c = 56.356f

		},
		{.pa_min = 122,
		 .pa_max = 934,
		 .k2 = 0.0000006f,
		 .k1 = 0.0039f,
		 .c = -39.954f

		},
};

uint16_t load2pwm(uint16_t load, uint8_t subIndex)
{
	uint16_t result;

	uint16_t pa;
	float k2;
	float k1;
	float c;

	k2 = loadCurves[subIndex].k2;
	k1 = loadCurves[subIndex].k1;
	c = loadCurves[subIndex].c;
	pa = (int)(k2 * load * load + k1 * load + c);

	k2 = ((float)(GetSystemRunData()->u16BlfIRun_E)) /
		 (loadCurves[subIndex].pa_max - loadCurves[subIndex].pa_min);
	result = (uint32_t)(k2 * (pa - loadCurves[subIndex].pa_min) + GetSystemRunData()->u16BlfIRun_Min);

	return (uint16_t)result;
}
#endif
const ST_TMPCTRL_T *GetTmpCtrlData(void)
{
	return &stTmpCtrl;
}
#if 0
void temper_keep_var_clear(void)
{
	uint8_t i, j;
	temper_keep_t.cnt = 0;
	temper_keep_t.sum = 0;
	for (i = 0; i < KEEP_ARRARY_SEG_MAX; i++)
	{
		for (j = 0; j < KEEP_ARRARY_NUM_MAX; j++)
		{
			temper_keep_t.CalcCtrlFlag[i][j] = 0;
			temper_keep_t.BlfErrorPercentTemp[i][j] = 1000;
		}
	}
}
#endif

uint8_t GetBasicBlfMax(void)
{
	uint16_t _u16temp;
	uint8_t _u8Sur;
	// 不在强制状态，且开启sur功能/模式
	_u8Sur = GetSurState();
		
	//比例阀最大开度代码（理论最大范围，各分段额外处理不算）
	_u16temp = GetFlashDataSector0()->debugData.u8Ph;
	_u16temp += getAdapData()->Ph_add;
	if (_u8Sur)
		_u16temp += GetFlashDataSector0()->debugData.u8Sp;
	_u16temp = GETMIN(_u16temp, 0xFF);
	return (uint8_t)_u16temp;
}


uint8_t GetBasicBlfMin(void)
{
	uint16_t _u16temp;	
	//比例阀最大开度代码（理论最大范围，各分段额外处理不算）
	_u16temp = GetFlashDataSector0()->debugData.u8Pl;
	_u16temp = GETMIN(_u16temp, 0xFF);
	return (uint8_t)_u16temp;
}

/**********************************************
 *当前比例阀开度，差值百分比（主要用于风机，风机判断工作范围不直接使用最终）
 *
 **********************************************/
uint8_t GetPercentFromBlfI(uint16_t _u16SetBlfI)
{
	uint16_t _u16BlfBasicMax,_u16BlfBasicMin;
	uint8_t _u8res;
	//根据PH，PL范围判断
	_u16BlfBasicMax = ConvertDbgToCtrl_Blf(GetBasicBlfMax());
	_u16BlfBasicMin = ConvertDbgToCtrl_Blf(GetBasicBlfMin());
	_u16SetBlfI = RANGLMT(_u16SetBlfI, _u16BlfBasicMin, _u16BlfBasicMax);
	_u8res = (uint8_t)(((uint32_t)_u16SetBlfI - _u16BlfBasicMin) * 100 / (_u16BlfBasicMax - _u16BlfBasicMin));
	// 此处不使用判断占比原因下，如果出现修正后的比例阀开度降低。由于风机区间不变，会导致同样的比例阀开度，风机转速会偏高
	// _u16SetBlfI = RANGLMT(_u16SetBlfI, GetSystemRunData()->u16BlfIRun_Min, GetSystemRunData()->u16BlfIRun_Max);
	// _u8res = (uint8_t)(((uint32_t)_u16SetBlfI - GetSystemRunData()->u16BlfIRun_Min) * 100 / GetSystemRunData()->u16BlfIRun_E);
	return _u8res;
}
/**********************************************
 *
 *
 **********************************************/
uint16_t ConvertDbgToCtrl_Blf(uint8_t _u8dbgset)
{
	uint16_t _u16res;
	_u16res = _u8dbgset;
	_u16res = (uint16_t)((uint32_t)_u16res * (BLF_MAX - BLF_MIN) / 0xff) + BLF_MIN;
	return _u16res;
}
/**********************************************
 *
 *
 **********************************************/
uint8_t ConvertCtrlToDbg_Blf(uint16_t _u16setblf)
{
	uint8_t _u8res;

	_u8res = (uint8_t)(((uint32_t)_u16setblf - BLF_MIN) * 0xff / (BLF_MAX - BLF_MIN));

	return _u8res;
}

uint16_t GetBlfIFromPercent(uint8_t _u8percent)
{
	uint16_t _u16res;
	_u8percent = GETMIN(_u8percent, 100);
	_u16res = (uint16_t)((uint32_t)GetSystemRunData()->u16BlfIRun_E * _u8percent / 100 + GetSystemRunData()->u16BlfIRun_Min);
	return _u16res;
}

uint16_t GetBlfFromLoad(uint16_t _u16SetLoad, uint8_t _u8SetSeg)
{

#if 1
	uint16_t _u16ret;
	uint16_t _u16LoadE;

	_u8SetSeg = GETMIN(_u8SetSeg, GetWorkCon()->u8MaxSeg);

	_u16LoadE = GetWorkCon()->astSegLoad[_u8SetSeg].u16MaxLoad;
	_u16LoadE -= GetWorkCon()->astSegLoad[_u8SetSeg].u16MinLoad;
	_u16LoadE = (uint16_t)((uint32_t)_u16LoadE * stTmpCtrl.u16CurvePercent / 1000);

	_u16ret = GetWorkCon()->astSegLoad[_u8SetSeg].u16MinLoad;
	_u16ret = (uint16_t)((uint32_t)_u16ret * stTmpCtrl.u16CurvePercent / 1000);

	if (_u16SetLoad > _u16ret)
	{
		_u16ret = _u16SetLoad - _u16ret;
		_u16ret = RANGLMT(_u16ret, 0, _u16LoadE);
		_u16ret = (uint16_t)((uint32_t)_u16ret * GetSystemRunData()->u16BlfIRun_E / _u16LoadE);
	}
	else
	{
		_u16ret = 0;
	}

	_u16ret += GetSystemRunData()->u16BlfIRun_Min;

	return _u16ret;
#else
	uint16_t blf;
	uint16_t setLoad;
	// setLoad=RANGLMT(_u16SetLoad, GetWorkCon()->astSegLoad[_u8SetSeg].u16MinLoad, GetWorkCon()->astSegLoad[_u8SetSeg].u16MaxLoad);
	blf = load2pwm(setLoad, _u8SetSeg);
	blf = RANGLMT(blf, GetSystemRunData()->u16BlfIRun_Min, GetSystemRunData()->u16BlfIRun_Max);
	return blf;
#endif
}

uint16_t FunctionPercentLimit(uint16_t _u16Temp, int16_t _s16Variance)
{
	int16_t _s16Temp;
	_s16Temp = (int16_t)_u16Temp + _s16Variance;
	_s16Temp = GETMAX(PERCENT_MIN, _s16Temp);
	return (uint16_t)_s16Temp;
}


/**********************************************
 * u8BlfIRunBasic与u16BlfIRun的差异
 * u8BlfIRunBasic为修正后的比例阀基本最大开度（不对分段的额外处理）。主要用于判断风机对比比例阀开度使用进行等比。
 * u16BlfIRun在u8BlfIRunBasic上增加每个分段的修正astSegIncrBLF，主要用于实际比例阀的工作范围
 **********************************************/
void RefreshBlfMaxMin(void)
{
	uint16_t _u16temp;

	//比例阀实际最大开度
	_u16temp = (uint16_t)GetBasicBlfMax() + GetWorkCon()->astSegIncrBLF[GetSegCtrl()->u8Set].s8MaxIncr;
	_u16temp = GETMIN(0xFF, _u16temp);
	GetSystemRunData()->u16BlfIRun_Max = ConvertDbgToCtrl_Blf((uint8_t)_u16temp);

	//比例阀最小开度代码（理论最小范围，各分段额外处理不算）
	_u16temp = GetFlashDataSector0()->debugData.u8Pl;

	//比例阀实际最小开度
	_u16temp = (uint16_t)GetBasicBlfMin() + GetWorkCon()->astSegIncrBLF[GetSegCtrl()->u8Set].s8MinIncr;
	_u16temp = GETMIN(0xFF, _u16temp);
	GetSystemRunData()->u16BlfIRun_Min = ConvertDbgToCtrl_Blf((uint8_t)_u16temp);

	GetSystemRunData()->u16BlfIRun_E = GetSystemRunData()->u16BlfIRun_Max - GetSystemRunData()->u16BlfIRun_Min;
}

void Reset_Pid(void)
{
	Pid_Init();
}

static uint16_t GetLoadFromTmpE(uint16_t _u16TmpE)
{
	uint32_t _u32Tmp;
	uint16_t _u16Tmp;
	_u32Tmp = (uint32_t)_u16TmpE * GetSystemRunData()->Flux;
	_u16Tmp = GETMIN(0xFFFF, _u32Tmp);
	return _u16Tmp;
}
static void UpdateLoadAvg(uint16_t _u16Setload)
{
#if (CtrlTemp_Deep)
    u32SetLoadFiterSum = (uint32_t)_u16Setload * (DEEP_VALVE - 1);
#else
	uint8_t _u8i;
	for (_u8i = 0; _u8i < ARR_NUM(au16SetLoadList); _u8i++)
	{
		au16SetLoadList[_u8i] = _u16Setload;
	}
	stTmpCtrl.u16SetLoad = _u16Setload;
#endif
}

static uint8_t GetDeadZone(void)
{
	uint16_t temp;
	uint16_t temp1;

	if (GetSystemRunData()->TmpSet >= GetSystemRunData()->TmpOut)
	{
		temp = GetSystemRunData()->TmpSet - GetSystemRunData()->TmpOut;
		temp /= 10;
		if (temp > 16)
		{
			temp = 16;
		}
		temp1 = (uint16_t)33 * temp * temp + 1131 * temp;
		temp = temp1 / 1500;
	}
	else
	{
		temp = GetSystemRunData()->TmpOut - GetSystemRunData()->TmpSet;
		temp /= 10;
		if (temp > 16)
		{
			temp = 16;
		}
		temp1 = (uint16_t)temp * 65 / 100;
		temp = (uint8_t)temp1;
	}
	if (temp < 2)
		temp = 2;
	if (temp > 8)
		temp = 8;
	return (uint8_t)(temp * 10);
}
void TmpCtrlPrepare(void)
{
	uint32_t _u32Temp;
	uint16_t _u16Temp;
	static uint16_t s16SetTmpSave;
	// static uint8_t u8LoadChgCnt = 0;
    // 设置温度温度发生改变
	///////////////////////////////////u16SetTmp
    //
	if (s16SetTmpSave != GetSystemRunData()->TmpSet)
	{
		s16SetTmpSave = GetSystemRunData()->TmpSet;
		ClrSubChgCnt();
	}
	///////////////////////////////////u16SetLoad
	if (GetSystemRunData()->TmpSet > GetSystemRunData()->TmpIn)
	{
		_u16Temp = GetSystemRunData()->TmpSet - GetSystemRunData()->TmpIn;
	}
	else
	{
		_u16Temp = 0;
	}
	stTmpCtrl.u16CalcLoad = GetLoadFromTmpE(_u16Temp);//计算出需要的负荷变化
	// stTmpCtrl.u16CalcLoad =RANGLMT(stTmpCtrl.u16CalcLoad, GetWorkCon()->astSegLoad[0].u16MinLoad, GetWorkCon()->astSegLoad[GetWorkCon()->u8MaxSeg].u16MaxLoad);

#if (CtrlTemp_Deep)
    stTmpCtrl.u16SetLoad = FilterDeep(stTmpCtrl.u16CalcLoad, &u32SetLoadFiterSum, DEEP_VALVE);

    //若当前目标负荷与理论负荷偏差约2℃，则立即对理论负荷进行覆盖处理
    if (abs(stTmpCtrl.u16SetLoad - stTmpCtrl.u16CalcLoad) > GetLoadFromTmpE(20))
	{
		UpdateLoadAvg(stTmpCtrl.u16CalcLoad);
		Reset_Pid(); //}
	}
#else
	_u32Temp = 0;
	for (_u16Temp = 0; SETLOAD_LENGTH > _u16Temp; _u16Temp++)
	{
		if ((SETLOAD_LENGTH - 1) == _u16Temp)
		{
			au16SetLoadList[_u16Temp] = stTmpCtrl.u16CalcLoad;
		}
		else
		{
			au16SetLoadList[_u16Temp] = au16SetLoadList[_u16Temp + 1];
		}
		_u32Temp += au16SetLoadList[_u16Temp];
	}
	stTmpCtrl.u16SetLoad = (uint16_t)(_u32Temp / SETLOAD_LENGTH);

	//////////////////////////////////////////////////////////////////
	if (abs(au16SetLoadList[0] - stTmpCtrl.u16CalcLoad) > GetLoadFromTmpE(20))
	{
		// if (++u8LoadChgCnt >= 3)
		//{
		//	u8LoadChgCnt = 0;
		UpdateLoadAvg(stTmpCtrl.u16CalcLoad);
		// stTmpCtrl.u8PidPause_100ms = GetDeadZone(); // 负荷突变PID暂停
		Reset_Pid(); //}
		
	}
	else
	{
		// u8LoadChgCnt = 0;
	}

	_u32Temp = 0;
	for (_u16Temp = 0; SETFLUX_LENGTH > _u16Temp; _u16Temp++)
	{
		if ((SETFLUX_LENGTH - 1) == _u16Temp)
		{
			au16SetFluxList[_u16Temp] = GetSystemRunData()->Flux;
		}
		else
		{
			au16SetFluxList[_u16Temp] = au16SetFluxList[_u16Temp + 1];
		}
		_u32Temp += au16SetLoadList[_u16Temp];
	}
	_u16Temp = (uint16_t)(_u32Temp / SETFLUX_LENGTH);
	if (abs(_u16Temp - GetSystemRunData()->Flux) >= 5)
	{
		//	UpdateLoadAvg(stTmpCtrl.u16CalcLoad);
		// Reset_Pid(); //}
	}
#endif
}
static void GetModBlf_New(void)
{
	static uint8_t u8StopSlot;
	static uint8_t u8LastSlot;
	int32_t _s32temp;
	uint16_t u16_OverWaterE, u16_OverLoadE;
	uint8_t _u8temp, _flg;
	int16_t _s16temp, _s16temp1;
	static uint8_t u8OverLoadConfirmTim = 0;
	static ST_LOADEDATA_T astLoadEData[REALLOAD_LENGTH];
	uint16_t _u16Temp;
	uint16_t _u16temp;
	uint16_t _u16TmpESum = 0;
	uint16_t _u16FluxSum = 0;
	static uint16_t u16FluxSave;
	static uint16_t u16PsvSave;
	uint8_t _FlowDown;//判断是否降水

	_FlowDown = 0;
	for (_u16Temp = 0; REALLOAD_LENGTH > _u16Temp; _u16Temp++)
	{
		if ((REALLOAD_LENGTH - 1) == _u16Temp)
		{
			if (GetSystemRunData()->TmpSet > GetSystemRunData()->TmpIn)
			{
				astLoadEData[_u16Temp].u16TmpE = GetSystemRunData()->TmpSet - GetSystemRunData()->TmpIn;
			}
			else
			{
				astLoadEData[_u16Temp].u16TmpE = 0;
			}
			astLoadEData[_u16Temp].u16FluxE = GetSystemRunData()->Flux;
			astLoadEData[_u16Temp].s16Flux = (int16_t)GetSystemRunData()->Flux - u16FluxSave;
			astLoadEData[_u16Temp].s16ELoad = (int16_t)stTmpCtrl.u16CalcLoad - u16PsvSave;
		}
		else
		{
			astLoadEData[_u16Temp] = astLoadEData[_u16Temp + 1];
		}
		_u16TmpESum += astLoadEData[_u16Temp].u16TmpE;
		_u16FluxSum += astLoadEData[_u16Temp].u16FluxE;
	}
	u16FluxSave = GetSystemRunData()->Flux;
	u16PsvSave = stTmpCtrl.u16CalcLoad;
	//	stTmpCtrl.u16OutLoad = (uint16_t)(((uint32_t)_u16TmpESum * _u16FluxSum) / (REALLOAD_LENGTH * REALLOAD_LENGTH));

	///////////////////<0.1s>/////////////////////////
	DEC(u8LastSlot);
	DEC(u8OverWaterStop_100ms);
	if (0 == u8OverWaterStop_100ms)
	{
		if (0 < u8StopSlot)
		{
			u16_OverLoadE = GetLoadFromTmpE(10);
			u16_OverWaterE = 5;
		}
		else
		{
			u16_OverLoadE = GetLoadFromTmpE(0);
			u16_OverWaterE = 5;
		}
		_u8temp = (REALLOAD_LENGTH - 1);
		_flg = 0;
		_s16temp = 0;
		_s16temp1 = 0;
		while ((u8StopSlot <= _u8temp) && (u8LastSlot <= _u8temp))
		{
			_s16temp += astLoadEData[_u8temp].s16ELoad;
			_s16temp1 += astLoadEData[_u8temp].s16Flux;
			if ((u16_OverLoadE <= abs(_s16temp)) && (u16_OverWaterE <= abs(_s16temp1)))
			{
				_flg = 1;
				u8OverLoadConfirmTim = 0;
				u8StopSlot = _u8temp;
				u8LastSlot = _u8temp + 1;
				_u8temp = 0;
				if (0 > _s16temp1)
					_FlowDown = 1;
			}
			if (0 < _u8temp)
			{
				_u8temp -= 1;
			}
			else
			{
				break;
			}
		}
		if (0 < u8StopSlot)
		{
	
			if (abs(_s16temp) > GetLoadFromTmpE(20))
			{
			  		stTmpCtrl.u8PidNoRun_100ms =10; // 负荷突变PID暂停
			//	Reset_Pid();
			//	UpdateLoadAvg(stTmpCtrl.u16CalcLoad);
			}

			ClrSubChgCnt();
			
			if (_FlowDown)
			{
				
			}
			else
			{
				if (0 < _flg)
				{
					_u8temp = u8StopSlot;
					_s16temp = GetBlfFromLoad(stTmpCtrl.u16CalcLoad, GetSegCtrl()->u8Set);
					_s32temp = (int32_t)stTmpCtrl.u16CalcLoad + astLoadEData[_u8temp].s16ELoad;
					_u16temp = GETMIN(0xFFFF, _s32temp);
					_s16temp -= GetBlfFromLoad(_u16temp, GetSegCtrl()->u8Set);
					stTmpCtrl.s16ModBlf -= (_s16temp * 2 / 1);
				}
				else
				{
					_u8temp = REALLOAD_LENGTH - 1;
					_s16temp = GetBlfFromLoad(stTmpCtrl.u16CalcLoad, GetSegCtrl()->u8Set);
					_s32temp = (int32_t)stTmpCtrl.u16CalcLoad + astLoadEData[_u8temp].s16ELoad;
					_u16temp = GETMIN(0xFFFF, _s32temp);
					_s16temp -= GetBlfFromLoad(_u16temp, GetSegCtrl()->u8Set);
					stTmpCtrl.s16ModBlf -= (_s16temp * 1 / 1);
				}
			}
		}
		if (0 < _flg)
		{
			u8OverLoadConfirmTim = 0;
		}
		else
		{
			if (0 < u8StopSlot)
			{
				if ((REALLOAD_LENGTH - u8StopSlot) <= u8OverLoadConfirmTim)
				{
					u8StopSlot = 0;
					stTmpCtrl.s16ModBlf = 0;
				}
			}
			else
			{
				stTmpCtrl.s16ModBlf = 0;
			}
			INC_B(u8OverLoadConfirmTim);
		}
	}
	else
	{
		stTmpCtrl.s16ModBlf = 0;
		u8StopSlot = 0;
	}
}

static void RefreshSegLoad(void) // 原始负荷修正和赋值
{

	uint16_t _u16temp;
	uint8_t _u8i;
	for (_u8i = 0; _u8i <= GetWorkCon()->u8MaxSeg; _u8i++)
	{
		_u16temp = FunctionPercentLimit(stTmpCtrl.u16MaxSegPercent, stTmpCtrl.as16SegPercent[_u8i]);
		stTmpCtrl.astSegLoad[_u8i].u16MinLoad = (uint16_t)((uint32_t)GetWorkCon()->astSegLoad[_u8i].u16MinLoad * (_u16temp) / 1000);
		stTmpCtrl.astSegLoad[_u8i].u16MaxLoad = (uint16_t)((uint32_t)GetWorkCon()->astSegLoad[_u8i].u16MaxLoad * (_u16temp) / 1000);
	}
}

static void GetTheroyBlf(void)
{
	//	uint8_t index;
	uint16_t blf, _u16LoadE, _u16temp;
	// uint32_t temp;

    //温度已经稳定一段时间，且已经燃烧超过一段时间（说明已经稳定）
	if ((15 < stTmpCtrl.u8BurstStable_100ms) && (40 < stTmpCtrl.u8StartBurst_100ms))
	{
		stTmpCtrl.u8StartBurst_100ms = 0xFF;
	}
    //已经接近目标温度
	// else if ((40 < stTmpCtrl.u8StartBurst_100ms) && (abs(GetSystemRunData()->TmpSet - GetSystemRunData()->TmpOut) < 20))
    else if ((40 < stTmpCtrl.u8StartBurst_100ms) && ((GetSystemRunData()->TmpSet - GetSystemRunData()->TmpOut) < 20))
	{
		stTmpCtrl.u8StartBurst_100ms = GETMAX(TM_STARTBURST, stTmpCtrl.u8StartBurst_100ms);
	}
	if (TM_STARTBURST > stTmpCtrl.u8StartBurst_100ms) /*  */
	{
		_u16LoadE = GetWorkCon()->astSegLoad[GetWorkCon()->u8MaxSeg].u16MaxLoad;
		_u16LoadE -= GetWorkCon()->astSegLoad[0].u16MinLoad;
		_u16LoadE = (uint16_t)((uint32_t)_u16LoadE * 15 / 100);
		_u16temp = stTmpCtrl.u16SetLoad - GetWorkCon()->astSegLoad[0].u16MinLoad;
        //负荷较低时，不使用快速加热
		if (_u16temp > _u16LoadE)
		{
			_u16temp = 0;
			if (GetSystemRunData()->TmpSet > GetSystemRunData()->TmpOut)
			{
				_u16temp = GetSystemRunData()->TmpSet - GetSystemRunData()->TmpOut;
				_u16temp = ((uint32_t)_u16temp * 10 / 100);
				_u16temp = GetLoadFromTmpE(_u16temp);
			}
		}
		else
		{
			_u16temp = 0;
		}
	}
	else
	{
		_u16temp = 0;
	}
	_u16temp += stTmpCtrl.u16SetLoad;

	////////////////////////////////////////////////////////////////////////////////////
	blf = GetBlfFromLoad(_u16temp, GetSegCtrl()->u8Set);
	/*
	stTmpCtrl.u16OrgTheroyBlf = blf;
	if (blf >= GetSystemRunData()->u16BlfIRun_Min)
	{
		temp1 = blf - GetSystemRunData()->u16BlfIRun_Min;
		temp = temp1 * KEEP_ARRARY_NUM_MAX;
		temp = (uint16_t)(temp / GetSystemRunData()->u16BlfIRun_E);
		index = (uint8_t)temp;
		if (!temper_keep_t.CalcCtrlFlag[GetSegCtrl()->u8Cur][index])
		{
			temper_keep_t.CalcCtrlFlag[GetSegCtrl()->u8Cur][index] = 1;
			temper_keep_t.BlfErrorPercentTemp[GetSegCtrl()->u8Cur][index] = temper_keep_t.errorPercent[GetSegCtrl()->u8Cur][index];
		}
		temp = (uint32_t)temper_keep_t.BlfErrorPercentTemp[GetSegCtrl()->u8Cur][index] * blf;
		blf = temp / 1000;
	}*/
	// 比例阀限制
	stTmpCtrl.u16TheroyBlf = RANGLMT(blf, GetSystemRunData()->u16BlfIRun_Min, GetSystemRunData()->u16BlfIRun_Max);
}

void pidCtrlHandle(void)
{
	static unsigned int setTempLast;
	uint8_t pidcalcCtrl = 0;

	if (!PidCtrl_t.firEnterFlag)
	{
		PidCtrl_t.firEnterFlag = 1;
		PidCtrl_t.notRunCnt = PID_NONE_RUN_INIT;
		stTmpCtrl.u8PidPause_100ms = 0;
	}

	if (setTempLast != GetSystemRunData()->TmpSet)
	{
		setTempLast = GetSystemRunData()->TmpSet;
		stTmpCtrl.u8BurstStable_100ms = 0;
		PidCtrl_t.notRunCnt = GetDeadZone(); // PID暂停
		stTmpCtrl.u8PidPause_100ms = 0;
	}
	if (++stTmpCtrl.u8PidPause_100ms >= 60)
		stTmpCtrl.u8PidPause_100ms = 60;
	// if (abs(fluxLast - GetSystemRunData()->Flux) >= 5)
	//{
	// fluxLast = GetSystemRunData()->Flux;
	// stTmpCtrl.u8PidPause_100ms = GetDeadZone(); // 负荷突变PID暂停
	//}
	GetSystemRunData()->u16ELoad = GetWorkCon()->astSegLoad[GetSegCtrl()->u8Set].u16MaxLoad - GetWorkCon()->astSegLoad[GetSegCtrl()->u8Set].u16MinLoad;
	//--------------------------------------------------

	if ((abs(GetSystemRunData()->TmpSet - GetSystemRunData()->TmpOut) <= 20) && PidCtrl_t.notRunCnt > 0 && stTmpCtrl.u8PidPause_100ms >= 60)
		PidCtrl_t.notRunCnt = 0;
	// if (stTmpCtrl.u8StartBurst_100ms >= TM_STARTBURST)
	// PidCtrl_t.notRunCnt = 0;
	//-------------------------------------------------
	if (PidCtrl_t.notRunCnt)
		PidCtrl_t.notRunCnt--;
	if (0 == PidCtrl_t.notRunCnt)
	{
		if (abs(GetSystemRunData()->TmpSet - GetSystemRunData()->TmpOut) >= 100)
		{
			if (++PidCtrl_t.clacDelay >= 3 * 2)
			{
				PidCtrl_t.clacDelay = 0;
				pidcalcCtrl = 1;
			}
		}
		else if ((abs(GetSystemRunData()->TmpSet - GetSystemRunData()->TmpOut) < 100) && (abs(GetSystemRunData()->TmpSet - GetSystemRunData()->TmpOut) >= 20))
		{
			if (++PidCtrl_t.clacDelay >= 4)
			{
				PidCtrl_t.clacDelay = 0;
				pidcalcCtrl = 1;
			}
		}
		else
		{
			if (++PidCtrl_t.clacDelay >= 2)
			{
				PidCtrl_t.clacDelay = 0;
				pidcalcCtrl = 1;
			}
		}

		if (pidcalcCtrl)
		{
			pidcalcCtrl = 0;
			stTmpCtrl.s16PIDBlf = PID_realize(GetSystemRunData());
		}
	}
	else
	{
		stTmpCtrl.s16PIDBlf = 0;
		Reset_Pid();
	}
    if(stTmpCtrl.u8PidNoRun_100ms)Reset_Pid();//负荷突变暂停
	// if (PID_BIF_LIMIT_MAX < stTmpCtrl.s16PIDBlf)
	// 	stTmpCtrl.s16PIDBlf = PID_BIF_LIMIT_MAX;
	// if (PID_BIF_LIMIT_MIN > stTmpCtrl.s16PIDBlf)
	// 	stTmpCtrl.s16PIDBlf = PID_BIF_LIMIT_MIN;
}

static void TmpCtrl_Timer(void)
{
	DEC(stTmpCtrl.u8PidNoRun_100ms);
	INC_B(stTmpCtrl.u8PidPause_100ms);
	INC_B(stTmpCtrl.u8StartBurst_100ms);
	INC_B(stTmpCtrl.u8BurstStable_100ms); // 稳定燃烧时间累计
}
#if 0
// 稳定温度比例阀偏差补偿
void user_water_temper_keep(void)
{
	uint8_t j, k, index;
	uint16_t blf;
	uint32_t temp;
	uint16_t temp2;
	static volatile uint16_t keepTemperInLast = 0xffff;
	static volatile uint16_t keepTemperSetLast = 0xffff;
	// static volatile uint16_t keepFluxLast = 0xffff;
	SystemRunDataTypeDef *pSystemData = GetSystemRunData();

	if (GetSystemRunData()->Flux < 30 && GetSystemRunData()->Flux > 200)
	{
		temper_keep_t.cnt = 0;
		temper_keep_t.sum = 0;
		stTmpCtrl.u8BurstStable_100ms = 0;
	}
	/*
		if (abs(keepFluxLast - GetSystemRunData()->Flux) > 10) // 流量不稳定
		{
			keepFluxLast = GetSystemRunData()->Flux;
			temper_keep_t.cnt = 0;
			temper_keep_t.sum = 0;
			stTmpCtrl.u8BurstStable_100ms = 0;
		}*/
	if (pSystemData->TmpSet != keepTemperSetLast) // 设置温度改变
	{
		keepTemperSetLast = pSystemData->TmpSet;
		temper_keep_t.cnt = 0;
		temper_keep_t.sum = 0;
		stTmpCtrl.u8BurstStable_100ms = 0;
	}
	if (abs(pSystemData->TmpIn - keepTemperInLast) > 10) // 进水温度不稳定
	{
		keepTemperInLast = pSystemData->TmpIn;
		temper_keep_t.cnt = 0;
		temper_keep_t.sum = 0;
		stTmpCtrl.u8BurstStable_100ms = 0;
	}

	if (abs(pSystemData->TmpSet - pSystemData->TmpOut) <= 3) // 出水温度达到设定温度
	{
		temper_keep_t.cnt++;
		temper_keep_t.sum += pSystemData->u16SetBlfI; // 比例阀值累加
	}
	else
	{
		stTmpCtrl.u8BurstStable_100ms = 0;
		temper_keep_t.cnt = 0;
		temper_keep_t.sum = 0;
	}

	if (temper_keep_t.cnt >= 50) // 稳定时间5S
	{
		temper_keep_t.cnt = 0;
		blf = (uint16_t)(temper_keep_t.sum / 50);
		if (blf <= GetSystemRunData()->u16BlfIRun_Max && blf >= GetSystemRunData()->u16BlfIRun_Min)
		{
			temp = 1000ul * blf; // 千分比
			temp /= stTmpCtrl.u16OrgTheroyBlf;
			blf = blf - GetSystemRunData()->u16BlfIRun_Min;
			temp2 = blf * KEEP_ARRARY_NUM_MAX;
			temp2 = (uint16_t)(temp2 / pSystemData->u16BlfIRun_E);
			index = (uint8_t)temp2;
			if (index < KEEP_ARRARY_NUM_MAX)
			{
				if (temp >= BLF_ERROR_PERC_MIN && temp <= BLF_ERROR_PERC_MAX)
				{
					temper_keep_t.errorPercent[GetSegCtrl()->u8Cur][index] = (uint16_t)temp;
					temper_keep_t.okFlag[GetSegCtrl()->u8Cur][index] = 1;
				}
				else
				{
					temper_keep_t.errorPercent[GetSegCtrl()->u8Cur][index] = 1000;
					temper_keep_t.okFlag[GetSegCtrl()->u8Cur][index] = 1;
				}
				for (j = index + 1; j < KEEP_ARRARY_NUM_MAX; j++)
				{
					if (temper_keep_t.okFlag[GetSegCtrl()->u8Cur][j])
					{
						for (k = 0; k <= (j - index) / 2; k++)
						{
							if (index + k < KEEP_ARRARY_NUM_MAX)
								temper_keep_t.errorPercent[GetSegCtrl()->u8Cur][index + k] = temper_keep_t.errorPercent[GetSegCtrl()->u8Cur][index];
						}
						break;
					}
				}
				if (j == KEEP_ARRARY_NUM_MAX)
				{
					for (j = index; j < KEEP_ARRARY_NUM_MAX; j++)
						temper_keep_t.errorPercent[GetSegCtrl()->u8Cur][j] = temper_keep_t.errorPercent[GetSegCtrl()->u8Cur][index];
				}
				if (index > 0)
				{
					for (j = 0; j < index; j++)
					{
						if (temper_keep_t.okFlag[GetSegCtrl()->u8Cur][index - j - 1])
						{
							for (k = 0; k <= (j + 1) / 2; k++)
							{
								if (index >= k)
									temper_keep_t.errorPercent[GetSegCtrl()->u8Cur][index - k] = temper_keep_t.errorPercent[GetSegCtrl()->u8Cur][index];
							}
							break;
						}
					}
					if ((j == index) && (0 == temper_keep_t.okFlag[GetSegCtrl()->u8Cur][0]))
					{
						for (j = 0; j < index; j++)
							temper_keep_t.errorPercent[GetSegCtrl()->u8Cur][j] = temper_keep_t.errorPercent[GetSegCtrl()->u8Cur][index];
					}
				}
			}
		}
		temper_keep_t.sum = 0;
	}
}
#endif
static int16_t GetLoadEData(uint16_t _u16Setload)
{
	uint8_t u8_temp1;
	int16_t s16_maxE, s16_minE, s16_result;
	uint16_t u16_PsvMin, u16_ELoadTemp;
	uint16_t _u16SegMax, _u16SegMin;

	u16_PsvMin = GetWorkCon()->astSegLoad[GetSegCtrl()->u8Cur].u16MinLoad;
	u16_PsvMin = (uint16_t)((uint32_t)u16_PsvMin * stTmpCtrl.u16LoadPercent / 1000);

	u16_ELoadTemp = GetWorkCon()->astSegLoad[GetSegCtrl()->u8Cur].u16MaxLoad;
	u16_ELoadTemp -= GetWorkCon()->astSegLoad[GetSegCtrl()->u8Cur].u16MinLoad;
	u16_ELoadTemp = (uint16_t)((uint32_t)u16_ELoadTemp * stTmpCtrl.u16LoadPercent / 1000);
	u16_ELoadTemp /= 9;

	for (u8_temp1 = 1; u8_temp1 < 10; u8_temp1++)
	{
		s16_maxE = GetWorkCon()->as16LoadArray[u8_temp1];
		s16_minE = GetWorkCon()->as16LoadArray[u8_temp1 - 1];
		_u16SegMax = u16_ELoadTemp * u8_temp1 + u16_PsvMin;
		_u16SegMax = (uint16_t)((uint32_t)_u16SegMax * (s16_maxE + 1000) / 1000);
		if (_u16Setload < _u16SegMax)
		{
			_u16SegMin = u16_ELoadTemp * (u8_temp1 - 1) + u16_PsvMin;
			_u16SegMin = (uint16_t)((uint32_t)_u16SegMin * (s16_minE + 1000) / 1000);
			s16_result = (int16_t)(((int32_t)_u16Setload - _u16SegMin) * (s16_maxE - s16_minE) / (_u16SegMax - _u16SegMin));
			s16_result += s16_minE;
			break;
		}
		s16_result = GetWorkCon()->as16LoadArray[u8_temp1];
	}

	return s16_result;
}

void ReSetPercent(void)
{
	uint8_t _u8i;
	stTmpCtrl.u16MaxSegPercent = 1000;
	for (_u8i = 0; _u8i < sizeof(stTmpCtrl.as16SegPercent); _u8i++)
	{
		stTmpCtrl.as16SegPercent[_u8i] = 0;
	}
}
void TmpCtrlVarReset(void)
{
	stTmpCtrl.u8StartBurst_100ms = 0;
	stTmpCtrl.u8BurstStable_100ms = 0;
	stTmpCtrl.s16PIDBlf = 0;
	stTmpCtrl.s16ModBlf = 0;
	stTmpCtrl.u16TheroyBlf = 0;
	PidCtrl_t.firEnterFlag = 0;
	stTmpCtrl.bFirstStable = 0;
	u8OverWaterStop_100ms = 50;
	stTmpCtrl.u8PidPause_100ms = 0;
	stTmpCtrl.u8PidNoRun_100ms=0;
	stTmpCtrl.bFirstSwSeg = 1;
	stTmpCtrl.bHeatStable = 0;//温度稳定标志位
	Reset_Pid();
	// temper_keep_var_clear();
	if (690 > stTmpCtrl.u16MaxSegPercent)
	{
		ReSetPercent();
	}
}


static void RealLoadPercent(void)
{
	static uint8_t u8CurSegReal;
	static uint16_t u16OutTmpSav,u16GetBlfISav;
	static uint16_t au16RealLoadInstant[REALLOAD_LENGTH];
	uint16_t u16_temp, u16_temp1, u16_PsvMin, u16_ELoadTemp, u16_calRealLoadPer;
	uint32_t u32_temp;
	int16_t s16_LoadEData;
	int16_t s16_Temp, s16_Temp2;
    uint16_t _u16BlfMax,_u16BlfMin,_u16BlfTemp;
    uint8_t _i;

    //当前比例阀开度采用平均的方式进行计算
    _u16BlfMin = 0xFFFF;
    _u16BlfMax = 0;
	s16_LoadEData = GetLoadEData(stTmpCtrl.u16SetLoad);

	s16_Temp = GetSystemRunData()->TmpOut - u16OutTmpSav;

    u32_temp = 0;
	for (_i = 0;_i < (ARR_NUM(stTmpCtrl.au16BlfSetRec) - 1);_i++)
	{
		stTmpCtrl.au16BlfSetRec[_i] = stTmpCtrl.au16BlfSetRec[_i + 1];
		u32_temp += stTmpCtrl.au16BlfSetRec[_i];
		_u16BlfMin = GETMIN(stTmpCtrl.au16BlfSetRec[_i],_u16BlfMin);
		_u16BlfMax = GETMAX(stTmpCtrl.au16BlfSetRec[_i],_u16BlfMax);
	}
	u32_temp += GetSystemRunData()->u16SetBlfI;
	stTmpCtrl.au16BlfSetRec[_i] = GetSystemRunData()->u16SetBlfI;
	_u16BlfMax = GETMAX(GetSystemRunData()->u16SetBlfI,_u16BlfMax);
	_u16BlfMin = GETMIN(GetSystemRunData()->u16SetBlfI,_u16BlfMin);
	_u16BlfTemp = (u32_temp - _u16BlfMax - _u16BlfMin) / (ARR_NUM(stTmpCtrl.au16BlfSetRec) - 2);
	s16_Temp2 = (int16_t) _u16BlfTemp - u16GetBlfISav;
	u32_temp = GetSystemRunData()->u16SetBlfI - GetSystemRunData()->u16BlfIRun_Min;
	u32_temp = u32_temp * 100 / GetSystemRunData()->u16BlfIRun_E;
	if (GetSegCtrl()->u8Cur != u8CurSegReal)
	{
		u8CurSegReal = GetSegCtrl()->u8Cur;
		stTmpCtrl.u8BurstStable_100ms = 0;
	}

	if (abs(GetSystemRunData()->TmpSet - GetSystemRunData()->TmpOut) > 3) // 出水温度达到设定温度
	{
		stTmpCtrl.u8BurstStable_100ms = 0;
	}
	if (GetSystemRunData()->Flux < 30 && GetSystemRunData()->Flux > 200)
	{
		stTmpCtrl.u8BurstStable_100ms = 0;
	}


	if (((3 > s16_Temp) && ((-3) < s16_Temp)) && ((20 > s16_Temp2) && ((-20) < s16_Temp2)))
	{
		// if ((stTmpCtrl.u16SetLoad == GetWorkCon()->astSegLoad[0].u16MinLoad)
		// 	|| (35 >= pstSysInput_TC->u16WaterFlux))
		if ((0 == GetSegCtrl()->u8Cur) && (10 >= u32_temp))
		{
			stTmpCtrl.u8BurstStable_100ms = 0;
		}
	}
	else
	{
		u16OutTmpSav = GetSystemRunData()->TmpOut;
		u16GetBlfISav = GetSystemRunData()->u16SetBlfI;
		stTmpCtrl.u8BurstStable_100ms = 0;
	}

	u16_PsvMin = GetWorkCon()->astSegLoad[GetSegCtrl()->u8Cur].u16MinLoad;

	u16_ELoadTemp = GetWorkCon()->astSegLoad[GetSegCtrl()->u8Cur].u16MaxLoad - GetWorkCon()->astSegLoad[GetSegCtrl()->u8Cur].u16MinLoad;
	u32_temp = u16_ELoadTemp;
	u32_temp = (u32_temp * (GetSystemRunData()->u16SetBlfI - GetSystemRunData()->u16BlfIRun_Min)) / GetSystemRunData()->u16BlfIRun_E + u16_PsvMin;
	u16_temp = stTmpCtrl.u16SetLoad;

	u16_temp = (uint16_t)(((uint32_t)u16_temp * 1000) / u32_temp);
	if (PERCENT_MIN >= u16_temp)
		u16_temp = PERCENT_MIN;
	//--------------------
	if (1300 < u16_temp)
		u16_temp = 1300;
	// if (690 > u16_temp)u16_temp = 690;

	u16_calRealLoadPer = u16_temp;
	u16_temp1 = 0;
	for (u16_temp = 0; u16_temp < (REALLOAD_LENGTH - 1); u16_temp++)
	{
		au16RealLoadInstant[u16_temp] = au16RealLoadInstant[u16_temp + 1];
		if ((REALLOAD_LENGTH / 2) > u16_temp)
		{
			u16_temp1 += au16RealLoadInstant[u16_temp];
		}
	}
	au16RealLoadInstant[REALLOAD_LENGTH - 1] = u16_calRealLoadPer;
	u16_temp1 /= (REALLOAD_LENGTH / 2);

	/*此处需要优化
	if ((0 != stTmpCtrl.u8PidPause_100ms)
		|| (0 != pstSegCtrl_TC->u8Switching_100ms)
		|| (100 >= stTmpCtrl.u16Start_100ms))
	{
		stTmpCtrl.u8BurstStable_100ms = 0;
	}
 */
	if (50 <= stTmpCtrl.u8BurstStable_100ms)
	{
		stTmpCtrl.bHeatStable = 1;//温度稳定标志位置1
		if (0 == stTmpCtrl.bFirstStable)
		{
			s16_Temp = (int16_t)u16_temp1 - s16_LoadEData - stTmpCtrl.as16SegPercent[GetSegCtrl()->u8Cur];
			stTmpCtrl.u16MaxSegPercent = GETMAX(PERCENT_MIN, s16_Temp);
			// stTmpCtrl.u16MaxSegPercent = u16_temp1 - s16_LoadEData - stTmpCtrl.as16SegPercent[GetSegCtrl()->u8Cur];
		}
		else if (GetWorkCon()->u8MaxSeg == GetSegCtrl()->u8Cur)
		{
			s16_Temp = (int16_t)u16_temp1 - s16_LoadEData;
			stTmpCtrl.u16MaxSegPercent = GETMAX(PERCENT_MIN, s16_Temp);
			// stTmpCtrl.u16MaxSegPercent = u16_temp1 - s16_LoadEData;
		}
		else
		{
			stTmpCtrl.as16SegPercent[GetSegCtrl()->u8Cur] = (int16_t)u16_temp1 - s16_LoadEData - stTmpCtrl.u16MaxSegPercent;
		}
		stTmpCtrl.u16LoadPercent = FunctionPercentLimit(stTmpCtrl.u16MaxSegPercent, stTmpCtrl.as16SegPercent[GetSegCtrl()->u8Cur]); // zuida+dangqian
		// stTmpCtrl.u16LoadPercent = stTmpCtrl.u16MaxSegPercent + stTmpCtrl.as16SegPercent[GetSegCtrl()->u8Cur];
		stTmpCtrl.u16CurvePercent = u16_temp1;

		stTmpCtrl.u8BurstStable_100ms = 0;
		stTmpCtrl.bFirstStable = 1;
		Reset_Pid();
	}
	else
	{
		stTmpCtrl.u16LoadPercent = FunctionPercentLimit(stTmpCtrl.u16MaxSegPercent, stTmpCtrl.as16SegPercent[GetSegCtrl()->u8Cur]);
		// stTmpCtrl.u16LoadPercent = stTmpCtrl.u16MaxSegPercent + stTmpCtrl.as16SegPercent[GetSegCtrl()->u8Cur];
		stTmpCtrl.u16CurvePercent = stTmpCtrl.u16LoadPercent + s16_LoadEData;
	}
}

void TmpCtrlProcess(void) // 10MS
{
	static uint16_t u16_DiffTimer = 0;
	if (++u16_DiffTimer >= 10) // 100MS计算一次
	{
		u16_DiffTimer = 0;
		TmpCtrl_Timer();
		RealLoadPercent();
		RefreshSegLoad();	// 刷新设置负荷
		RefreshBlfMaxMin(); // 获取比例阀和比例阀差值
		TmpCtrlPrepare();	// 得到理论负荷
		GetModBlf_New();	// 负荷突变
		SegCtrlProcess();	// 设置分段阀
		GetTheroyBlf();		// 得到理论比例阀开度
		pidCtrlHandle();	// pid计算
		GetSystemRunData()->u16SetBlfI = stTmpCtrl.u16TheroyBlf + stTmpCtrl.s16PIDBlf + stTmpCtrl.s16ModBlf;
		//  比例阀限制
		GetSystemRunData()->u16SetBlfI = RANGLMT(GetSystemRunData()->u16SetBlfI, GetSystemRunData()->u16BlfIRun_Min, GetSystemRunData()->u16BlfIRun_Max);
	}
}

void TmpCtrlInit(void)
{
	//	uint8_t i, j;
	memset(&stTmpCtrl, 0, sizeof(stTmpCtrl));
	memset(&PidCtrl_t, 0, sizeof(PidCtrl_t));

	TmpCtrlVarReset();
#if 0
     	memset(&temper_keep_t, 0, sizeof(temper_keep_t));
	for (i = 0; i < KEEP_ARRARY_SEG_MAX; i++)
	{
		for (j = 0; j < KEEP_ARRARY_NUM_MAX; j++)
		{
			temper_keep_t.okFlag[i][j] = 0;
			temper_keep_t.CalcCtrlFlag[i][j] = 0;
			temper_keep_t.errorPercent[i][j] = 1000; // 初始化偏差值
			temper_keep_t.BlfErrorPercentTemp[i][j] = 1000;
		}
	}
#endif
	stTmpCtrl.u16LoadPercent = 1000;
	stTmpCtrl.u16CurvePercent = 1000;
	stTmpCtrl.u16MaxSegPercent = 1000;
	ReSetPercent();
}

/**********************************************
 *获取首次进入燃烧标志位，同时将该标志位清零
 *
 **********************************************/
uint8_t GetFirstSwSeg(void)
{
	uint8_t _u8temp;
	_u8temp = stTmpCtrl.bFirstSwSeg;
	stTmpCtrl.bFirstSwSeg = 0;
	return _u8temp;
}

/**********************************************
 *重置稳定标志位
 *
 **********************************************/
void ResetHeatStable(void)
{
    stTmpCtrl.bHeatStable = 0;
}


