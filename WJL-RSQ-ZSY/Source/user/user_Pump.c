#include "ALL_HEAD.H"

typedef struct
{
	uint8_t u3Index : 3;
	uint8_t u5Cnt : 5;
} ST_BURST_T;

static ST_BURST_T stBurst;				// 引索号
#define STUDY_TIME_ERR 3000				// 5min内连续改变天数为无效操作
#define STUDY_TIME_RE 600				// 刷新时间相差较大，整天时间段重新刷新
#define STUDY_RECORD_MAX 3				// 最多记录3天
#define PUMP_ERROR_TIME 150				// 水泵异常计时
#define BURST_FLX_NUM 4					// 水流量记录数量上限
#define BURST_FLX_INDEX stBurst.u3Index // 水流量数组引索号
#define BURST_CNT stBurst.u5Cnt			// 用于次数满足一定时，刷新水流量保存

#define BOOK_SET_MOR 0x01 // 晨洗
#define BOOK_SET_NIG 0x02 // 夜浴
#define BOOK_SET_DAY 0X04 // 全天

#define COLD_TMP 50		  // 防冻触发温度（0.1℃）
#define COLD_TIME 600	  // 防冻工作时间（0.1s）
#define COLD_FORCE_TIME 0 // 强制防冻时间

#define BURST_TIME 60		 // 即热启动时间（0.1s）
#define HEAT_UNWORK_TIME 600 // 水泵工作间隔（0.1s）
#define HEAT_FORCE_TIME 9000 // 两次强制即热时间（0.1s）
#define HEAT_MAX_TIME 18000	 // 即热最长时间（0.1s）

#define HEAT_STOP_TMP_IN 20	  // 进水停止条件温差（0.1℃）
#define STUDY_STOP_TMP_IN 50  // 自学习情况下停止条件温差（0.1℃）
#define HEAT_STOP_TMP_OUT 100 // 出水停止条件温差（0.1℃）

#define BURST_FLU_MAX 100 // 增压启动最大水流量（0.1L）
#define BURST_FLU_STOP 20 // 增压停止水流量降低（0.1L）

#define PREHEAT_MODE_ALL 0
#define PREHEAT_MODE_HALF 1

#define PREHEAT_UNSTUDY 0XFFFF	// 未学习，时间为0
#define PREHEAT_STUDY_LIMIT 300 // 学习失败最短时间（0.1s）
#define PREHEAT_STUDY_MIN 30	// 学习最短时间（1s）

#define CYCLE_STOP_TMP 70  // 退出后循环条件（0.1℃）
#define CYCLE_STOP_TIME 30 // 退出后循环持续判断时间

#define WATER_CTRL_CNT_MAX 50 // 进入即热最长时间（0.1s）
#define WATER_CTRL_CNT_MIN 30

#define BOOK24_ARRAY_MAX 6

#define PUMP_FORCE_TIME 1152000 // 水泵强制执行时间（0.1s）
#define PUMP_FORCE_WORK 50		// 水泵强制执行工作时间（0.1s）

#define PUMP_STOP_LIMIT_TIME 30 // 即热转洗浴（0.1s）

#define PUMP_STUDY_TIME_TIME 10 // 学习默认时间

static ST_PUMP_OUTPUTS_T stPumpOutputs;

static uint16_t u16HeatUnworkCnt; // 即热水泵不允许工作时间
static uint8_t u8BurstCnt;		  // 增压启动时间
static uint16_t u16ColdCnt;		  // 防冻工作时间
static uint16_t u16PumpStudyTime; // 智能学习时间
static uint16_t u16PumpCycleCnt;  // 后循环时间（0.1s）
static uint32_t u32HeatCnt;
static uint16_t u16HeatOnceCnt; // 单次即热时间（0.1s）
static uint8_t u8WaterCtrlCnt;	// 水控情况下温度控制
static uint8_t u8PumpErrCnt;	// 水泵异常判定

static uint32_t u32PumpForceCnt;
static uint8_t u8PumpForceWorkCnt; // 水泵工作时间
static uint8_t u8PumpCycleOffCnt;

static uint8_t u8HeatStop_100ms;

static uint16_t u16StudyUsedTime; // 使用时间
static uint8_t u8StudyTimeBef;	  // 上次记录时间，若存在大幅度时间修改，清除当天数据重新计算
static uint8_t au8StudyTime[3][6];
static uint8_t au8StudyTimeTmp[6];
static uint8_t u8StudyTimeUsed;
static uint8_t u8StudyDay; // 学习天数
static uint8_t u8StudyInit;

static uint16_t au16BurstFlux[BURST_FLX_NUM]; // 用于判断2s内水流变化

uint8_t PumpTmpStop(void);

ST_PUMP_OUTPUTS_T *GetpPumpParm(void)
{
	return &stPumpOutputs;
}
void PumpUnworkFunction(void);
void PumpHeatFunction(void);
void PumpCycleFunction(void);
void PumpColdFunction(void);
void PumpBurstFunction(void);
void PumpErrorFunction(void);
void PumpForceFunction(void);
static const FUCTION apPumpFunction[] =
	{
		PumpUnworkFunction,
		PumpHeatFunction,
		PumpCycleFunction,
		PumpColdFunction,
		PumpBurstFunction,

		PumpForceFunction,
};

/**********************************************
 *处理温度异常
 *异常当做零处理
 **********************************************/
// void TmpErrorDeal(int16_t* Save, int16_t TargetTmp)
// {
// 	if (TMP_ERROR == TargetTmp)
// 	{
// 		*Save = 0;
// 	}
// 	else
// 	{
// 		*Save = TargetTmp;
// 	}
// }
void PumpStudyTimeInit(void)
{
	uint16_t _u16Tmp;
	_u16Tmp = DS1302_TimeRead(EN_DS_HOUR);
	_u16Tmp = _u16Tmp * 60 + DS1302_TimeRead(EN_DS_MIN);
	u8StudyTimeBef = _u16Tmp / 30;
}

/**********************************************
 *智学习 获取时间
 **********************************************/
uint8_t PumpStudyGetTime(void)
{
	uint8_t _u8Tmp;
	_u8Tmp = GetSystemRunData()->u8HourCur * 2;
	_u8Tmp += (GetSystemRunData()->u8MinCur / 30);

	return _u8Tmp;
}
void PumpStudySet(uint8_t _u8On, EN_STUDY_TYPE_T _Type)
{
	uint16_t _u16Tmp;
	uint8_t _i;
	uint8_t _j;
	if (EN_STUDY_NOW == _Type)
	{
		_u16Tmp = PumpStudyGetTime();
		// _u16Tmp = _u16Tmp / 30;
	}
	else if (EN_STUDY_BEF == _Type)
	{
		_u16Tmp = u8StudyTimeBef;
	}

	_i = _u16Tmp / 8;
	_j = _u16Tmp % 8;

	if (0 < _u8On)
	{
		au8StudyTimeTmp[_i] |= (1 << _j);
	}
	else
	{
		au8StudyTimeTmp[_i] &= ~(1 << _j);
	}
}

uint8_t PumpStudyAction(uint8_t _u8Time)
{
	uint8_t _i;
	uint8_t _j;
	uint8_t _z;
	uint8_t _u8Tmp;
	_u8Tmp = 1;
	_i = _u8Time / 8;
	_j = _u8Time % 8;
	for (_z = 0; _z < ARR_NUM(au8StudyTime); _z++)
	{
		if (0 == (au8StudyTime[_z][_i] & (1 << _j)))
		{
			_u8Tmp = 0;
			break;
		}
	}

	return _u8Tmp;
}



void PumpParmReSet(uint8_t *_Pump, uint8_t _Order, uint8_t *_pSign)
{
	if (*_Pump != _Order)
	{
		*_Pump = _Order;
		*_pSign = 1;
	}
}
/**********************************************
 *通讯数据处理（接收指令）
 **********************************************/
void PumpComm(void)
{
	uint8_t _u8i;
	uint8_t _u8FlashRe = 0;
	// 无掉电记忆
	if (GetCommSet()->u8FastHeat != stPumpOutputs.bSign.bFastHeat)
	{
		stPumpOutputs.bSign.bFastHeat = GetCommSet()->u8FastHeat;
		if (0 < GetCommSet()->u8FastHeat)
		{
			stPumpOutputs.bSign.bHeatFirst = 1;
			PumpStudySet(1, EN_STUDY_NOW);
		}
		else
		{
			stPumpOutputs.bSign.bHeatFirst = 0;
			PumpStudySet(0, EN_STUDY_NOW);
		}
	}

	// 有掉电记忆
	PumpParmReSet(&GetFlashDataSector1()->pumpData.u8CycleMode, GetCommSet()->u8CycleMode, &_u8FlashRe);
	PumpParmReSet(&GetFlashDataSector1()->pumpData.u8PreWarmTime, GetCommSet()->u8PreWarmTime, &_u8FlashRe);
	PumpParmReSet(&GetFlashDataSector1()->pumpData.u8PreWarmDif, GetCommSet()->u8PreWarmDif, &_u8FlashRe);

	for (_u8i = 0; _u8i < BOOK24_ARRAY_MAX; _u8i++)
	{
		PumpParmReSet(&GetFlashDataSector1()->pumpData.au8Book24Set[_u8i], GetCommSet()->unBook24Set.Bytes[_u8i], &_u8FlashRe);
	}

	PumpParmReSet(&GetFlashDataSector1()->pumpData.u8MorWashStart, GetCommSet()->u8MorWashStart, &_u8FlashRe);
	PumpParmReSet(&GetFlashDataSector1()->pumpData.u8MorWashEnd, GetCommSet()->u8MorWashEnd, &_u8FlashRe);
	PumpParmReSet(&GetFlashDataSector1()->pumpData.u8NigWashStart, GetCommSet()->u8NigWashStart, &_u8FlashRe);
	PumpParmReSet(&GetFlashDataSector1()->pumpData.u8NigWashEnd, GetCommSet()->u8NigWashEnd, &_u8FlashRe);
	PumpParmReSet(&GetFlashDataSector1()->pumpData.u8WaterUSedTime, GetCommSet()->u8WaterUseTime, &_u8FlashRe);

	if (0 < _u8FlashRe)
	{
		UserWrtieSetCD(500);
	}
}

/**********************************************
 *判断是否在即热工作范围内
 *1 是
 *0 不是
 **********************************************/
uint8_t PumpHeatAble(void)
{
	uint8_t _u8HeatAble = 0; // 临时标志位，最后用于输出
	uint32_t _u32Temp;
	uint16_t _u16Temp;
	uint8_t _u8Temp;
	if (0 == GetSystemRunData()->sysSta.BIT.bOnOff)
	{
		GetCommSet()->u8FastHeat = 0;
		GetCommSet()->u8Priority = PRI_NONE;
		u32HeatCnt = 0;
	}
	else
	{
		// 即热功能判断
		if (GetCommSet()->u8FastHeat)
		{
			// 是否为一次即热
			if (0 == GetFlashDataSector1()->pumpData.u8PreWarmTime) // 保温时长为0，一次即热
			{
				_u8HeatAble = 1;
			}
			else
			{
				_u32Temp = (uint32_t)GetFlashDataSector1()->pumpData.u8PreWarmTime * 600 * 10; // 临时计算保温时长，u8PreWarm单位是10min
				// 若处于后循环过程过程中，不关闭即热
				if (_u32Temp > u32HeatCnt)
				{
					_u8HeatAble = 1;
				}
				else
				{
					if (EN_PUMP_CYCLE != stPumpOutputs.enState)
					{
						stPumpOutputs.bSign.bFastHeat = 0;
                        if (GetCommSet()->u8FastHeat)
                        {
                            GetCommSet()->u8FastHeat = 0;
							GetCommSet()->u8Priority = PRI_NONE;
                        }
						u32HeatCnt = 0;
					}
					else
					{
						_u8HeatAble = 1;
					}
				}
			}
		}
		else
		{
			u32HeatCnt = 0;
		}

		// 预约功能启动，晨洗夜浴
		if ((0 == _u8HeatAble) && (0 < GetCommSet()->u8BookFuncSet))
		{
			if (BOOK_SET_DAY == GetCommSet()->u8BookFuncSet) // 开启全天
			{
				_u8HeatAble = 1;
			}
			else
			{
				_u16Temp = (GetSystemRunData()->u8HourCur * 2 + GetSystemRunData()->u8MinCur / 30); // 将时间转换为min
				if (0 < (BOOK_SET_MOR & GetCommSet()->u8BookFuncSet))								// 开启了晨洗
				{
					// _u16Temp = GetSystemRunData()->u8HourCur * 60 + GetSystemRunData()->u8MinCur;//将时间转换为min
					if (GetFlashDataSector1()->pumpData.u8MorWashStart <= _u16Temp)
					{
						if (GetFlashDataSector1()->pumpData.u8MorWashEnd > _u16Temp)
						{
							_u8HeatAble = 1;
						}
					}
				}
				if (0 < (BOOK_SET_NIG & GetCommSet()->u8BookFuncSet))
				{
					// _u16Temp = GetSystemRunData()->u8HourCur * 60 + GetSystemRunData()->u8MinCur;//将时间转换为min
					if (GetFlashDataSector1()->pumpData.u8NigWashStart <= _u16Temp)
					{
						if (GetFlashDataSector1()->pumpData.u8NigWashEnd > _u16Temp)
						{
							_u8HeatAble = 1;
						}
					}
				}
			}
		}

		// 预约启动
		if ((0 == _u8HeatAble) && (0 < GetCommSet()->u8BookSet))
		{
			_u16Temp = GetSystemRunData()->u8HourCur / 4;
			_u8Temp = GetFlashDataSector1()->pumpData.au8Book24Set[_u16Temp];
			_u16Temp = (GetSystemRunData()->u8HourCur % 4) * 2 + GetSystemRunData()->u8MinCur / 30;
			if (0 < ((0x80 >> _u16Temp) & _u8Temp))
			{
				_u8HeatAble = 1;
			}
		}

		// 智学习
		if ((0 == _u8HeatAble) && (0 < GetCommSet()->u8SmartLearnEn))
		{
			_u8Temp = (GetSystemRunData()->u8HourCur * 2) + GetSystemRunData()->u8MinCur / 30;
			if (0 < PumpStudyAction(_u8Temp))
			{
				_u8HeatAble = 1;
			}
		}
	}
	return _u8HeatAble;
}

// 水控功能
void PumpWaterCtrl(void)
{
	if ((0 < GetCommSet()->u8WaterControl) && (0 < GetSystemRunData()->sysSta.BIT.bOnOff))
	{
		if ((0 < GetSystemRunData()->sysSta.BIT.bWater) && (EN_PUMP_UNWORK == stPumpOutputs.enState))
		{
#if (1 == UNITTEST)
			u8WaterCtrlCnt += u16UTestSysTime;
#else
			INC_B(u8WaterCtrlCnt);
#endif
		}
		else
		{
			if ((WATER_CTRL_CNT_MAX >= u8WaterCtrlCnt) && (WATER_CTRL_CNT_MIN < u8WaterCtrlCnt))
			{
				GetCommSet()->u8FastHeat = 1;
				GetCommSet()->u8Priority = PRI_NONE;
				PumpStudySet(1, EN_STUDY_NOW);
				stPumpOutputs.bSign.bHeatFirst = 1;
				u32HeatCnt = 0;
			}
			u8WaterCtrlCnt = 0;
		}
	}
	else
	{
		u8WaterCtrlCnt = 0;
	}
}

/**********************************************
 *循环工作时，E7判断
 **********************************************/
void PumpErrCheck(void)
{
	if (EN_PUMP_UNWORK != stPumpOutputs.enState)
	{
		if (0 < GetSystemRunData()->sysSta.BIT.bWater)
		{
			u8PumpErrCnt = 0;
		}
		else
		{
			INC_B(u8PumpErrCnt);
			if (PUMP_ERROR_TIME <= u8PumpErrCnt) // 15S
			{
				u8PumpErrCnt = 0;
				GetSystemRunData()->u8ErrorCode = EN_ERR_E7;
				SetFsmState(FSM_STATE_ERROR);
			}
		}
	}
	else
	{
		u8PumpErrCnt = 0;
	}
}

/**********************************************
 *后循环时间
 *单位0.1s
 **********************************************/
uint16_t PumpCycleTime(uint16_t _u16PreHeatOnceTime)
{
	uint16_t _u16CycleTime;
	// if (_u16PreHeatOnceTime < 1800)
	// {
	// 	_u16CycleTime = 600;
	// }
	// else
	// {
	// 	_u16CycleTime = (_u16PreHeatOnceTime - 1800) / 2400 * 600 + 1200;
	// 	_u16CycleTime = GETMIN(_u16CycleTime, 3000);
	// }

	// if (9000 <= _u16PreHeatOnceTime)
	// {
	// 	_u16CycleTime = 3000;
	// }
	// else
	// {
	// 	_u16CycleTime = 1;
	// 	if (1800 <= _u16PreHeatOnceTime)
	// 	{
	// 		_u16CycleTime += (_u16PreHeatOnceTime - 1800) / 2400 + 1;
	// 	}
	// 	_u16CycleTime *= 600;
	// }
	if (_u16PreHeatOnceTime < 1800)
	{
		_u16CycleTime = 600;
	}
	else if (_u16PreHeatOnceTime < 4200)
	{
		_u16CycleTime = 1200;
	}
	else if (_u16PreHeatOnceTime < 6600)
	{
		_u16CycleTime = 1800;
	}
	else if (_u16PreHeatOnceTime < 9000)
	{
		_u16CycleTime = 2400;
	}
	else
	{
		_u16CycleTime = 3000;
	}
	return _u16CycleTime;
}

/**********************************************
 *返回即热时间
 *单位s
 **********************************************/
uint16_t PumpHeatTime(void)
{
	uint16_t _u16Temp;
	if (21 <= GetFlashDataSector1()->pumpData.u8CycleMode)
	{
		_u16Temp = 120;
	}
	else if (20 == GetFlashDataSector1()->pumpData.u8CycleMode)
	{
		_u16Temp = 210;
	}
	else if (19 == GetFlashDataSector1()->pumpData.u8CycleMode)
	{
		_u16Temp = 150;
	}
	else if (16 <= GetFlashDataSector1()->pumpData.u8CycleMode)
	{
		_u16Temp = ((GetFlashDataSector1()->pumpData.u8CycleMode - 15) * 30);
	}
	else if (2 <= GetFlashDataSector1()->pumpData.u8CycleMode)
	{
		_u16Temp = (GetFlashDataSector1()->pumpData.u8CycleMode * 60);
	}
	else if (1 == GetFlashDataSector1()->pumpData.u8CycleMode)
	{
		if (0 == u16PumpStudyTime)
		{
			_u16Temp = PREHEAT_UNSTUDY;
		}

		else
		{
			_u16Temp = u16PumpStudyTime;
		}
	}
	else
	{
		_u16Temp = PREHEAT_UNSTUDY;
	}
	return _u16Temp;
}

/**********************************************
 *判断用户用水关闭即热
 **********************************************/
void PumpHeatStopCnt(void)
{
	if (EN_PUMP_HEAT == stPumpOutputs.enState)
	{
		if ((GetSystemRunData()->u16BlfIRun_Max == GetSystemRunData()->u16SetBlfI) && (GetWorkCon()->u8MaxSeg == GetSegCtrl()->u8Set) && (78 < GetSystemRunData()->Flux))
		{
			INC_B(u8HeatStop_100ms);
		}
		else
		{
			u8HeatStop_100ms = 0;
		}
	}
	else
	{
		u8HeatStop_100ms = 0;
	}
}

/**********************************************
 *水泵不工作时，状态判断及切换
 **********************************************/
void PumpUnworkFunction(void)
{
	int16_t _s16Temp;
	uint8_t _u8temp;
	uint8_t _u8i;
    //出现故障，清除
	if ((EN_ERR_NO != GetSystemRunData()->u8ErrorCodePump) || (EN_ERR_NO != GetSystemRunData()->u8ErrorCode))
	{
		stPumpOutputs.bSign.bFastHeat = 0;
		u8BurstCnt = 0;
		u16HeatOnceCnt = 0;
		u32HeatCnt = 0;
		u8WaterCtrlCnt = 0;
		GetCommSet()->u8FastHeat = 0;
	}
	else
	{
		stPumpOutputs.bSign.bColdErr = 0;
		if ((PUMP_FORCE_TIME <= u32PumpForceCnt) && (FSM_STATE_IDLE == GetFsmState())) // 强制时间到了，强制开启水泵
		{
			stPumpOutputs.enState = EN_PUMP_FORCE;
			u8PumpForceWorkCnt = PUMP_FORCE_WORK;
		}
		// 防冻判定（开机/关机都需要判断）
        // FD探头异常时，判断为0℃
		else if ((0 == GetSystemRunData()->sysSta.BIT.bWater) && ((COLD_TMP > GetSystemRunData()->TmpFD) || (temper_ntc_error_get(TMP_FDTX))))
		{
			stPumpOutputs.enState = EN_PUMP_COLD;
			u16ColdCnt = COLD_TIME;
			// 需要添加燃热启动请求
		}
		else
		{
			// 处于开机状态？
			if (0 < GetSystemRunData()->sysSta.BIT.bOnOff)
			{
				// 检测到有水？
				if (0 < GetSystemRunData()->sysSta.BIT.bWater)
				{
					// 增压启动？
					_u8temp = PumpTmpStop();
					if ((0 < GetCommSet()->u8BurstMode) && (BURST_FLU_MAX > GetSystemRunData()->Flux) && (0 == _u8temp))
					{
#if (1 == UNITTEST)
						u8BurstCnt += u16UTestSysTime;
#else
						u8BurstCnt++;
#endif
						// 启动增压
						if (BURST_TIME <= u8BurstCnt)
						{
							for (_u8i = 0; BURST_FLX_NUM > _u8i; _u8i++)
							{
								au16BurstFlux[_u8i] = 0;
							}
							BURST_FLX_INDEX = 0;
							stPumpOutputs.enState = EN_PUMP_BURST;
							u8BurstCnt = 0;
						}
					}
					else
					{
						u8BurstCnt = 0;
					}
				}
				// 没水
				else
				{
					u8BurstCnt = 0;
					_s16Temp = (int16_t)GetSystemRunData()->TmpSet - GetSystemRunData()->TmpOut; // 计算回差温度
					if (0 < stPumpOutputs.bSign.bPumpWarm)
					{
						if (((HEAT_UNWORK_TIME < u16HeatUnworkCnt) && (_s16Temp >= GetFlashDataSector1()->pumpData.u8PreWarmDif * 10) && (((uint16_t)GetSystemRunData()->TmpSet - HEAT_STOP_TMP_IN) > GetSystemRunData()->TmpIn)) || (0 < stPumpOutputs.bSign.bHeatFirst) || (HEAT_FORCE_TIME < u16HeatUnworkCnt))
						{
							stPumpOutputs.bSign.bHeatFirst = 0;
							stPumpOutputs.enState = EN_PUMP_HEAT;
						}
					}
				}
			}
		}
	}
}

/**********************************************
 *水泵循环工作时，状态判断及切换
 **********************************************/
void PumpHeatFunction(void)
{
	uint16_t _u16Temp;
	uint8_t _u8Temp;
	// 当次循环时间增加
	if ((0 == stPumpOutputs.bSign.bPumpWarm) || (0 < GetSystemRunData()->sysSta.BIT.bError))
	{
		stPumpOutputs.enState = EN_PUMP_UNWORK;
	}

	else
	{
		// 计算单次即热最长时间
		_u16Temp = PumpHeatTime();
		if (PREHEAT_UNSTUDY == _u16Temp)
		{
			_u16Temp = HEAT_MAX_TIME;
		}
		else
		{
			_u16Temp *= PUMP_RUN_FREQ;
		}
		// 单次工作时间超过设定时间
		if ((_u16Temp <= u16HeatOnceCnt) || (PUMP_STOP_LIMIT_TIME <= u8HeatStop_100ms))
		{
			stPumpOutputs.enState = EN_PUMP_UNWORK;
		}
		else
		{
			// 自学习未成功（有后清扫，且停止条件存在差异）
			if (((PREHEAT_MODE_HALF == GetFlashDataSector1()->pumpData.u8CycleMode) && (0 == u16PumpStudyTime)))
			{
				if ((((uint16_t)GetSystemRunData()->TmpSet - STUDY_STOP_TMP_IN) <= GetSystemRunData()->TmpIn) || (((uint16_t)GetSystemRunData()->TmpSet + HEAT_STOP_TMP_OUT) <= GetSystemRunData()->TmpOut))
				{
					stPumpOutputs.enState = EN_PUMP_CYCLE;
					u16PumpCycleCnt = PumpCycleTime(u16HeatOnceCnt);
					u8PumpCycleOffCnt = 0;
					stPumpOutputs.u8PumpForceWorkCnt = PUMP_WORK_TIME;
					if (PREHEAT_STUDY_LIMIT <= u16HeatOnceCnt)
					{
						// 计算最小学习时间（单位s）
						_u16Temp = ((uint32_t)u16HeatOnceCnt * 3) / (PUMP_RUN_FREQ * 5); // 3/5的时间
						if (PREHEAT_STUDY_MIN <= _u16Temp)
						{
							u16PumpStudyTime = _u16Temp;
						}
						else
						{
							u16PumpStudyTime = PREHEAT_STUDY_MIN;
						}
					}
				}
			}
			else
			{
				_u8Temp = PumpTmpStop();
				if (1 == _u8Temp)
				{
					// 判断需要后循环？
					if (PREHEAT_MODE_ALL == GetFlashDataSector1()->pumpData.u8CycleMode)
					{
						stPumpOutputs.enState = EN_PUMP_CYCLE;
						u16PumpCycleCnt = PumpCycleTime(u16HeatOnceCnt);
						u8PumpCycleOffCnt = 0;
						stPumpOutputs.u8PumpForceWorkCnt = PUMP_WORK_TIME;
					}
					else
					{
						stPumpOutputs.enState = EN_PUMP_UNWORK;
					}
				}
			}
		}
	}
	// 完成当次即热
	if (EN_PUMP_HEAT != stPumpOutputs.enState)
	{
		u16HeatOnceCnt = 0;
		// 保温时长为0，直接退出即热
		if (((0 == GetFlashDataSector1()->pumpData.u8PreWarmTime) && (EN_PUMP_CYCLE != stPumpOutputs.enState)) || (0 < GetSystemRunData()->sysSta.BIT.bError))
		{
			stPumpOutputs.bSign.bFastHeat = 0;
			u32HeatCnt = 0;
			if (GetCommSet()->u8FastHeat)
			{
				GetCommSet()->u8FastHeat = 0;
				GetCommSet()->u8Priority = PRI_NONE;
			}
		}
	}
}

/**********************************************
 *水泵后循环时，状态判断及切换
 **********************************************/
void PumpCycleFunction(void)
{
	stPumpOutputs.u8PumpForceWorkCnt = PUMP_WORK_TIME;
	if ((0 == stPumpOutputs.bSign.bPumpWarm) || (0 < GetSystemRunData()->sysSta.BIT.bError))
	{
		stPumpOutputs.enState = EN_PUMP_UNWORK;
	}
	else if (0 < u16PumpCycleCnt)
	{
		// 到达退出后循环条件
		if ((GetSystemRunData()->TmpSet - CYCLE_STOP_TMP) >= (GetSystemRunData()->TmpIn))
		{
#if (1 == UNITTEST)
			u8PumpCycleOffCnt = CYCLE_STOP_TIME;
#else
			INC_B(u8PumpCycleOffCnt);
#endif
			if (CYCLE_STOP_TIME <= u8PumpCycleOffCnt)
			{
				stPumpOutputs.enState = EN_PUMP_UNWORK;
			}
		}
		else
		{
			u8PumpCycleOffCnt = 0;
		}
	}
	else if (0 == u16PumpCycleCnt)
	{
		stPumpOutputs.enState = EN_PUMP_UNWORK;
	}
	if (EN_PUMP_UNWORK == stPumpOutputs.enState)
	{
		if ((0 < GetSystemRunData()->sysSta.BIT.bError) || (0 == GetFlashDataSector1()->pumpData.u8PreWarmTime))
		{
			stPumpOutputs.bSign.bFastHeat = 0;
			if (GetCommSet()->u8FastHeat)
			{
				GetCommSet()->u8FastHeat = 0;
				GetCommSet()->u8Priority = PRI_NONE;	
			}
			u32HeatCnt = 0;
		}
	}
}

/**********************************************
 *水泵防冻时，状态判断及切换
 **********************************************/
void PumpColdFunction(void)
{
	if ((EN_ERR_NO != GetSystemRunData()->u8ErrorCodePump) || (EN_ERR_NO != GetSystemRunData()->u8ErrorCode))
	{
		stPumpOutputs.bSign.bColdErr = 1;
		stPumpOutputs.enState = EN_PUMP_UNWORK;
	}
	else if (0 < u16ColdCnt)
	{
		if (((COLD_TIME - COLD_FORCE_TIME) >= u16ColdCnt) && (((uint16_t)GetSystemRunData()->TmpSet + HEAT_STOP_TMP_OUT) <= GetSystemRunData()->TmpOut))
		{
			u16ColdCnt = 0;
		}
		// else
		//{
		//     //温度停止条件
		//     if((((uint16_t)GetSystemRunData()->stSysRunData.u16TmpRun - HEAT_STOP_TMP_IN)  <= u16TmpIn_PM)
		//     || (((uint16_t)GetSystemRunData()->stSysRunData.u16TmpRun + HEAT_STOP_TMP_OUT) <= u16TmpOut_PM)
		//     )
		//     {
		//         stPumpOutputs.enState = EN_PUMP_UNWORK;
		//         u16ColdCnt = 0;
		//     }
		// }
	}

	if (0 == u16ColdCnt)
	{
		stPumpOutputs.enState = EN_PUMP_UNWORK;
	}
}

/**********************************************
 *水泵增压时，状态判断及切换
 **********************************************/
void PumpBurstFunction(void)
{
	int32_t _s32Temp = 0;
	uint8_t _u8i;
	uint8_t _u8Temp;

	_u8Temp = PumpTmpStop();
	// 非燃烧工作，增压关闭退出增压
	if ((FSM_STATE_STABLE != GetFsmState()) || (0 == GetCommSet()->u8BurstMode))
	{
		stPumpOutputs.enState = EN_PUMP_UNWORK;
	}
	// 温度达到停止条件
	else if (1 == _u8Temp)
	{
		stPumpOutputs.enState = EN_PUMP_UNWORK;
	}
	else
	{
#if (1 == UNITTEST)
		if (1)
#else
		BURST_CNT++;
		if (5 <= BURST_CNT)
#endif
		{
			BURST_CNT = 0;
			for (_u8i = 0; BURST_FLX_NUM > _u8i; _u8i++)
			{
				_s32Temp += au16BurstFlux[_u8i];
			}
			_s32Temp >>= 2; // 求平均，与这次做对比
			_s32Temp = _s32Temp - GetSystemRunData()->Flux;
			if (BURST_FLU_STOP <= _s32Temp)
			{
				stPumpOutputs.enState = EN_PUMP_UNWORK;
			}
			else
			{
				au16BurstFlux[BURST_FLX_INDEX] = GetSystemRunData()->Flux;
				BURST_FLX_INDEX++;
				if (BURST_FLX_NUM <= BURST_FLX_INDEX)
				{
					BURST_FLX_INDEX = 0;
				}
			}
		}
	}
}
/**********************************************
 *水泵强制工作5s
 **********************************************/
void PumpForceFunction(void)
{
	stPumpOutputs.u8PumpForceWorkCnt = PUMP_WORK_TIME;
	if (0 == u8PumpForceWorkCnt)
	{
		stPumpOutputs.enState = EN_PUMP_UNWORK;
	}
}

/**********************************************
 *判断温度停止条件
 **********************************************/
uint8_t PumpTmpStop(void)
{
	uint8_t _u8temp = 0;
	if ((((uint16_t)GetSystemRunData()->TmpSet - HEAT_STOP_TMP_IN) <= GetSystemRunData()->TmpIn) || (((uint16_t)GetSystemRunData()->TmpSet + HEAT_STOP_TMP_OUT) <= GetSystemRunData()->TmpOut))
	{
		_u8temp = 1;
	}
	// else
	// {
	// 	_u8temp = 0;
	// }
	return _u8temp;
}

/**********************************************
 *水泵工作状态判断
 **********************************************/
void Pump_StateFunction(void)
{
	static uint8_t u8PumpInitCnt = 30; // 主要避免初始化时，进入防冻
	if (u8PumpInitCnt)
		u8PumpInitCnt--;
	if (0 == u8PumpInitCnt)
	{
		(*apPumpFunction[stPumpOutputs.enState])();
	}
}

void Pump_Timer(void)
{
	DEC(stPumpOutputs.u8PumpForceWorkCnt);
	DEC(u16PumpCycleCnt);
	DEC(u16ColdCnt);
	DEC(u8PumpForceWorkCnt);
	if (EN_PUMP_UNWORK != stPumpOutputs.enState)
	{
		if (EN_PUMP_BURST != stPumpOutputs.enState)
		{
			stPumpOutputs.u8PumpWorkCnt = PUMP_WORK_TIME;
		}
		u32PumpForceCnt = 0;
	}
	else
	{
		INC_L(u32PumpForceCnt);
		DEC(stPumpOutputs.u8PumpWorkCnt);
	}

	// 没有进行燃烧？
	if (FSM_STATE_STABLE != GetFsmState())
	{
		// 不允许工作时间减少
		if (EN_PUMP_UNWORK != stPumpOutputs.enState)
		{
			u16HeatUnworkCnt = 0;
		}
		else
		{
			INC_W(u16HeatUnworkCnt);
		}
	}
	else
	{
		if (0 < stPumpOutputs.bSign.bPumpWarm)
		{
			u16HeatUnworkCnt = 0;
		}
		else
		{
			u16HeatUnworkCnt = HEAT_UNWORK_TIME;
		}
	}

	if (EN_PUMP_HEAT == stPumpOutputs.enState)
	{
#if (1 == UNITTEST)
		u16HeatOnceCnt += u16UTestSysTime;
#else
		INC_W(u16HeatOnceCnt);
#endif
	}

	if (0 < GetCommSet()->u8FastHeat)
	{
#if (1 == UNITTEST)
		u32HeatCnt += u16UTestSysTime;
#else
		INC_L(u32HeatCnt);
#endif
	}

	PumpHeatStopCnt();
}

/**********************************************
 *水泵工作判断
 **********************************************/
void Pump_Work(void)
{
	static uint8_t u8PumpWorkState = 0;
	static uint8_t u8PumpWorkChange_100ms; // 工作状态改变
    const ST_DBG_INFO_T *_stDbgInfo;
    _stDbgInfo = GetDbgInfo(GetCommSet()->u8DebugIndex);
	uint8_t _u8Temp;
	if ((EN_PUMP_BURST == stPumpOutputs.enState) || (EN_PUMP_COLD == stPumpOutputs.enState) || (EN_PUMP_HEAT == stPumpOutputs.enState) || (EN_PUMP_CYCLE == stPumpOutputs.enState) || (EN_PUMP_FORCE == stPumpOutputs.enState))
	{
		_u8Temp = 1;
	}
	else
	{
		_u8Temp = 0;
	}

	if (_u8Temp == u8PumpWorkState)
	{
		u8PumpWorkChange_100ms = 0;
	}
	else
	{
		if (++u8PumpWorkChange_100ms >= 3)
		{
			u8PumpWorkState = _u8Temp;
		}
	}

	if ((u8PumpWorkState) ||
        (((EN_CODE_P == _stDbgInfo->u8CodeH) && (EN_CODE_U == _stDbgInfo->u8CodeL))))
	{
        Drv_OutPut_Set(EN_OUTPORT_WPump, 1);
	}
	else
	{
		Drv_OutPut_Set(EN_OUTPORT_WPump, 0);
	}
}

/**********************************************
 *智学习 获取时间
 **********************************************/
// uint16_t PumpStudyGetTime(void)
// {
// 	uint16_t _u16Tmp;
// 	_u16Tmp = (uint16_t)GetSystemRunData()->u8HourCur * 60;
// 	_u16Tmp += GetSystemRunData()->u8MinCur;

// 	return _u16Tmp;
// }

void PumpStudyCatch(void)
{
	int16_t s8Tmp;
	uint8_t u8Tmp;
	uint8_t bReInit; // 该位置1后，重置当天数据
	uint8_t _i;
	// u16Tmp = PumpStudyGetTime();
	// 计算出所在时间段
	// u8Tmp = u16Tmp / 30;
	u8Tmp = PumpStudyGetTime();
	if (u8Tmp != u8StudyTimeBef)
	{
		bReInit = 0;
		INC_B(u8StudyTimeUsed);
		if (((uint16_t)GetCommSet()->u8WaterUseTime * 600) <= u16StudyUsedTime)
		{
			PumpStudySet(1, EN_STUDY_BEF);
		}
		// 若一天过去
		if ((0 == u8Tmp) && (47 <= u8StudyTimeBef))
		{
			// 当天数据有效（首次上电，或当天正常计时时间超过10次
			if ((0 < u8StudyInit) || (12 < u8StudyTimeUsed))
			{
				u8StudyInit = 0;
				for (_i = 0; _i < 6; _i++)
				{
					au8StudyTime[u8StudyDay][_i] = au8StudyTimeTmp[_i];
				}
				INC_B(u8StudyDay);
				if (STUDY_RECORD_MAX <= u8StudyDay)
				{
					u8StudyDay = 0;
				}
				bReInit = 1;
			}
			else
			{
				bReInit = 1;
			}
			u8StudyTimeUsed = 0;
		}
		else
		{
			s8Tmp = (int8_t)u8StudyTimeBef - u8Tmp;
			// 两个时间变化超过30min 重置所有时间
			if ((1 < s8Tmp) || ((-1) > s8Tmp))
			{
				bReInit = 1;
			}
		}
		if (0 < bReInit)
		{
			if ((0 != u8Tmp) || (23 != u8StudyTimeBef))
			{
				for (_i = 0; _i < 6; _i++)
				{
					au8StudyTimeTmp[_i] = 0;
				}
			}
			u8StudyTimeUsed = 0;
		}
		u16StudyUsedTime = 0;
		u8StudyTimeBef = u8Tmp;
	}

	// 判断到正在洗浴用水
	if (((EN_PUMP_UNWORK == stPumpOutputs.enState) || (EN_PUMP_BURST == stPumpOutputs.enState)) && (FSM_STATE_STABLE == GetFsmState()))
	{
		INC_W(u16StudyUsedTime);
	}
}

/**********************************************
 *通讯数据
 *[5][9]-[5][10]加热倒计时
 **********************************************/
uint16_t GetPreHeatCD(void)
{
	uint16_t _u16Temp;
	if (PREHEAT_MODE_ALL == GetFlashDataSector1()->pumpData.u8CycleMode)
	{
		_u16Temp = PREHEAT_UNSTUDY;
	}
	else if (PREHEAT_MODE_HALF == GetFlashDataSector1()->pumpData.u8CycleMode)
	{
		if (0 == u16PumpStudyTime)
		{
			_u16Temp = PREHEAT_UNSTUDY;
		}
		else
		{
			_u16Temp = u16PumpStudyTime - (u16HeatOnceCnt / PUMP_RUN_FREQ);
		}
	}
	else
	{
		_u16Temp = PumpHeatTime() - (u16HeatOnceCnt / PUMP_RUN_FREQ);
	}
	return _u16Temp;
}

uint16_t GetPreHeatTime(void)
{
	uint16_t _u16Temp;
	_u16Temp = u32HeatCnt / 600; // HeatCnt单位0.1s，1min为600
	return _u16Temp;
}

uint8_t GetPreHeatStudy(void)
{
	uint8_t _u8Temp;
	if (0 == u16PumpStudyTime)
	{
		_u8Temp = 0xFF;
	}
	else if (60 > u16PumpStudyTime)
	{
		_u8Temp = 1;
	}
	else
	{
		_u8Temp = u16PumpStudyTime / 60;
	}
	return _u8Temp;
}

void SetPumpParm(void)
{
	GetCommSet()->u8CycleMode = GetFlashDataSector1()->pumpData.u8CycleMode;
	GetCommSet()->u8PreWarmTime = GetFlashDataSector1()->pumpData.u8PreWarmTime;
	GetCommSet()->u8PreWarmDif = GetFlashDataSector1()->pumpData.u8PreWarmDif;

	GetCommSet()->u8MorWashStart = GetFlashDataSector1()->pumpData.u8MorWashStart;
	GetCommSet()->u8MorWashEnd = GetFlashDataSector1()->pumpData.u8MorWashEnd;
	GetCommSet()->u8NigWashStart = GetFlashDataSector1()->pumpData.u8NigWashStart;
	GetCommSet()->u8NigWashEnd = GetFlashDataSector1()->pumpData.u8NigWashEnd;
	GetCommSet()->unBook24Set.Bytes[0] = GetFlashDataSector1()->pumpData.au8Book24Set[0];
	GetCommSet()->unBook24Set.Bytes[1] = GetFlashDataSector1()->pumpData.au8Book24Set[1];
	GetCommSet()->unBook24Set.Bytes[2] = GetFlashDataSector1()->pumpData.au8Book24Set[2];
	GetCommSet()->unBook24Set.Bytes[3] = GetFlashDataSector1()->pumpData.au8Book24Set[3];
	GetCommSet()->unBook24Set.Bytes[4] = GetFlashDataSector1()->pumpData.au8Book24Set[4];
	GetCommSet()->unBook24Set.Bytes[5] = GetFlashDataSector1()->pumpData.au8Book24Set[5];
	GetCommSet()->u8WaterUseTime = GetFlashDataSector1()->pumpData.u8WaterUSedTime;
	// 首次上电初始化，智慧模式初始化（为1时，无论工作多长时间都会记录）
	u8StudyInit = 1;
	PumpStudyTimeInit();
}
/**********************************************
 *水泵增压时，状态判断及切换
 **********************************************/
void PumpFunction(void) // 10MS
{
	static uint8_t PumpFunctionTick;

	if (++PumpFunctionTick < 10)
		return;
	PumpFunctionTick = 0;

    // 时间
	Pump_Timer();

	// 通信信息处理
	PumpComm();
	// E7故障判定
	PumpErrCheck();
	// 判断是否在即热时间段内
	stPumpOutputs.bSign.bPumpWarm = PumpHeatAble();
	// 判断水控
	PumpWaterCtrl();

	// 智学习
	PumpStudyCatch();
	//
	Pump_StateFunction();

	Pump_Work();
}
void PumpFunctionInit(void)
{
	memset(&stPumpOutputs, 0, sizeof(stPumpOutputs));
	SetPumpParm();
}
