#include "ALL_HEAD.H"

static uint8_t u8WaterExpendCnt;	  // 用于计时
static uint32_t u32WaterExpendRec;	  // 用水水量标志（上一次关水时记录）
static uint32_t u32WaterExpendSumRec; // 重置累计
static uint8_t u8GasExpendCnt;		  // 用于计时
static uint32_t u32GasExpend;		  // 用水水量（从上电开始算）
static uint32_t u32GasExpendRec;	  // 用水水量标志（上一次关水时记录）
static uint32_t u32GasExpendSumRec;	  // 重置累计

static ST_FUNCTION_OUTPUTS_T stFunctionOutputs;
#define EXPEND_TIME 10		 // 卫浴管家工作时间（0.1s）
#define EXPEND_WATER_LTR 600 // 1L的累计值 1秒记录一次，每次记录L/min， 60 * 10
#define EXPEND_12T_LTR 44500 // 每1L的累计负荷
#define EXPEND_20Y_LTR 145266

// 智能防护（排气）
static uint16_t u16ProFlow;	  // 用于排气的计算
static uint8_t u8PumpFlowCnt; // 水流量记录时间计时
static uint8_t u8ProAirChk_500ms;
static uint8_t u8ProInitCnt;   // 排气初始化时间
static uint8_t u8ProAirChkNum; // 变化超过1L的次数
#define PRO_AIR_TIME 20		   // 排气累计时间（0.5s）
#define PRO_AIR_LITE 10		   // 排气时，变化水量（0.1L）
#define PRO_PUMP_INI_TIME 50
#define PRO_AIR_NUM 3 // 变化的次数

static uint16_t u16SelfClean_100ms;
#define SLEF_CLEAN_CNT 300

// 智能防护（气压）
static uint8_t u8WindChk_100ms;
#define PRO_WIND_TIME 50 // 气压判断时间

static const ST_GAS_PRO_T astProGas[] =
	{
		// CH4故障，CH4解除，CO故障，CO解除
		{15, 4, 30, 12},
		{20, 4, 40, 12}};

/**********************************************
数据接口
 **********************************************/
const ST_FUNCTION_OUTPUTS_T *GetFunOutputs(void)
{
	return &stFunctionOutputs;
}

/**********************************************
 *卫浴管家
 **********************************************/
// 避免溢出
uint32_t FunctionU32Add(uint32_t _u32Now, uint32_t _u32Add)
{
	uint32_t _u32temp;
	_u32temp = 0xFFFFFFFF - _u32Now;
	if (_u32Add <= _u32temp)
	{
		_u32temp = _u32Now + _u32Add;
	}
	else
	{
		_u32temp = _u32Add - _u32temp;
	}
	return _u32temp;
}

// 用水量计算
void WaterExpedFunction(void)
{
	DEC(u8WaterExpendCnt);
	if (0 == u8WaterExpendCnt)
	{
		u8WaterExpendCnt = EXPEND_TIME;

		// 关机，后循环，防冻，即热不计算累计用水量
		if (((EN_PUMP_UNWORK == GetpPumpParm()->enState)
			 || (EN_PUMP_BURST == GetpPumpParm()->enState)) &&
			GetSystemRunData()->sysSta.BIT.bOnOff)
		{
			stFunctionOutputs.u32WaterExpend = FunctionU32Add(stFunctionOutputs.u32WaterExpend, GetSystemRunData()->Flux);
		}
	}
}

void GasExpedFunction(void)
{
	DEC(u8GasExpendCnt);
	if (0 == u8GasExpendCnt)
	{
		u8GasExpendCnt = EXPEND_TIME;
		// 燃烧状态是进行气量累计
		if (FSM_STATE_STABLE == GetFsmState())
		{
			u32GasExpend = FunctionU32Add(u32GasExpend, GetTmpCtrlData()->u16SetLoad);
		}
	}
}

// 判断升数是否需要增加
uint8_t RecordLiterFunction(uint32_t *_LaterRec, EN_REC_T _enRecordType)
{
	uint8_t u8temp = 0;
	uint32_t _u32temp;
	uint32_t _u32temp2 = *_LaterRec;
	uint32_t _u32RecordTmp;
	if (EN_REC_GAS == _enRecordType)
	{
		if (EN_GAS_20Y == GetMaInfo()->pstFA->enGasType)
		{
			_u32RecordTmp = EXPEND_20Y_LTR;
		}
		else
		{
			_u32RecordTmp = EXPEND_12T_LTR;
		}

		_u32temp = u32GasExpend;
	}
	else
	{
		_u32RecordTmp = EXPEND_WATER_LTR;
		_u32temp = stFunctionOutputs.u32WaterExpend;
	}
	// 计算两次的差值
	if (_u32temp >= _u32temp2)
	{
		_u32temp = _u32temp - _u32temp2;
	}
	else
	{
		_u32temp = 0xFFFFFFFF - _u32temp2 + _u32temp;
	}
	// 判断变化有无超过1L
	if (_u32temp >= _u32RecordTmp)
	{
		u8temp = 1;
		_u32temp = 0xFFFFFFFF - _u32temp2;
		if (_u32temp >= _u32RecordTmp)
		{
			*_LaterRec += _u32RecordTmp;
		}
		else
		{
			*_LaterRec = _u32RecordTmp - _u32temp;
		}
	}
	return u8temp;
}

void WaterGasRecord(void)
{
	uint8_t bFlashReSet = 0;
	// 接收卫浴管家清除指令/刷新Flash
	if (0 < GetCommSet()->u8ClrWSum)
	{
		bFlashReSet = 1;
		u32WaterExpendSumRec = stFunctionOutputs.u32WaterExpend;
		stFunctionOutputs.u32WaterUseSum = 0;
		GetFlashDataSector1()->basicFlashData.u32WaterSum = 0;
		GetCommSet()->u8ClrWSum = 0;
	}
	if (0 < GetCommSet()->u8ClrGSum)
	{
		bFlashReSet = 1;
		stFunctionOutputs.u32GasUseSum = 0;
		u32GasExpendSumRec = u32GasExpend;
		GetFlashDataSector1()->basicFlashData.u32GasSum = 0;
		GetCommSet()->u8ClrGSum = 0;
	}

	if (1 == bFlashReSet)
	{
		UserWrtieSetCD(300);
	}

	// 用水卫浴管家工作处理
	if (0 < RecordLiterFunction(&u32WaterExpendRec, EN_REC_WATER))
	{
		INC_W(stFunctionOutputs.u16WaterUsed);
	}

	if (0 < RecordLiterFunction(&u32WaterExpendSumRec, EN_REC_WATER))
	{
		INC_L(stFunctionOutputs.u32WaterUseSum);
	}
	if (0 < RecordLiterFunction(&u32GasExpendRec, EN_REC_GAS))
	{
		INC_W(stFunctionOutputs.u16GasUsed);
	}
	if (0 < RecordLiterFunction(&u32GasExpendSumRec, EN_REC_GAS))
	{
		INC_L(stFunctionOutputs.u32GasUseSum);
	}
}
/**********************************************
 *双防故障功能
 **********************************************/
void GasErrFunction(void)
{
	// 查看报警器失联？
	if ((0 == GetCommSet()->GasErrorSet) || (EN_DBG_CO_NO <= GetFlashDataSector0()->debugData.u8Co))
	{
		stFunctionOutputs.stSign.bCH4Warm = 0;
		stFunctionOutputs.u8CoSat = EN_CO_SAFE;
	}
	else if (0 == GetCommSet()->au8AddrOnlineCnt[EN_ADDR_ALARM])
	{
		stFunctionOutputs.u8CoSat = EN_CO_UNCOMM;
		GetCommSet()->u8CH4PPM = 0;
		GetCommSet()->u8CoPPM = 0;
		stFunctionOutputs.stSign.bCH4Warm = 0;
	}
	// 判断是否故障
	else if (EN_COCH4_ERR == GetCommSet()->u8AlarmSta)
	{
		stFunctionOutputs.u8CoSat = EN_CO_ERR;
		GetCommSet()->u8CH4PPM = 0;
		GetCommSet()->u8CoPPM = 0;
		stFunctionOutputs.stSign.bCH4Warm = 0;
	}
	else
	{
		if ((EN_CO_UNCOMM == stFunctionOutputs.u8CoSat) || (EN_CO_ERR == stFunctionOutputs.u8CoSat))
		{
			stFunctionOutputs.u8CoSat = EN_CO_SAFE;
			stFunctionOutputs.stSign.bCH4Warm = 0;
		}

		// 判断CO
		if (EN_CO_SAFE == stFunctionOutputs.u8CoSat)
		{
			if (astProGas[GetFlashDataSector0()->debugData.u8Co].u8CoWarm <= GetCommSet()->u8CoPPM)
			{
				stFunctionOutputs.u8CoSat = EN_CO_WARM;
			}
		}
		else
		{
			if (astProGas[GetFlashDataSector0()->debugData.u8Co].u8CoSafe >= GetCommSet()->u8CoPPM)
			{
				stFunctionOutputs.u8CoSat = EN_CO_SAFE;
			}
		}

		// 判断CH4
		if (0 == stFunctionOutputs.stSign.bCH4Warm)
		{
			if (astProGas[GetFlashDataSector0()->debugData.u8Co].u8CH4Warm <= GetCommSet()->u8CH4PPM)
			{
				stFunctionOutputs.stSign.bCH4Warm = 1;
			}
		}
		else
		{
			if (astProGas[GetFlashDataSector0()->debugData.u8Co].u8CH4Safe >= GetCommSet()->u8CH4PPM)
			{
				stFunctionOutputs.stSign.bCH4Warm = 0;
			}
		}

		// 不处于F6和E0故障
	if (((1 == stFunctionOutputs.stSign.bCH4Warm) || (EN_CO_WARM == stFunctionOutputs.u8CoSat)) && ((FSM_STATE_F6 !=GetFsmState()) && (FSM_STATE_ERROR != GetFsmState())))
		{
			SetFsmState(FSM_STATE_F6);
		}
	}
}

/**********************************************
 *智能防护
 **********************************************/
// TODO:智能防护
void ProtcetFunction(void)
{
	int32_t _s32temp;
	uint16_t _u16temp;
	// 排气（水泵工作时才判定）
#if (1 == UNITTEST)
	u8ProInitCnt = 0;
#else
	DEC(u8ProInitCnt);
#endif
	// 水泵开始一段时间内不进行排气判断
	if ((EN_PUMP_COLD != GetpPumpParm()->enState) && (EN_PUMP_HEAT != GetpPumpParm()->enState) && (EN_PUMP_CYCLE != GetpPumpParm()->enState))
	{
		u8ProInitCnt = PRO_PUMP_INI_TIME;
		u8ProAirChk_500ms = 0;
		stFunctionOutputs.stSign.bProAir = 0;
		u8ProAirChkNum = 0;
	}
	// 排气判断
#if (1 == UNITTEST)
	u8PumpFlowCnt = 5;
#else
	INC_B(u8PumpFlowCnt);
#endif
	if (5 <= u8PumpFlowCnt)
	{
		if (0 == u8ProInitCnt)
		{
			_s32temp = (int32_t)u16ProFlow - GetSystemRunData()->Flux;
			if ((_s32temp >= -PRO_AIR_LITE) && (_s32temp <= PRO_AIR_LITE))
			{
#if (1 == UNITTEST)
				u8ProAirChk_500ms += 10;
#else
				INC_B(u8ProAirChk_500ms);
#endif
				if (PRO_AIR_TIME <= u8ProAirChk_500ms)
				{
					u8ProAirChkNum = 0;
				}
			}
			else
			{
				u8ProAirChk_500ms = 0;
				INC_B(u8ProAirChkNum);
				if (PRO_AIR_NUM < u8ProAirChkNum)
				{
					stFunctionOutputs.stSign.bProAir = 1;
				}
			}
		}
		// 刷新水量
		u16ProFlow = GetSystemRunData()->Flux;
		u8PumpFlowCnt = 0;
	}

	// 气压和风压（正常燃烧时判定）
	if (FSM_STATE_STABLE == GetFsmState())
	{
		// 风压
		_u16temp = (uint16_t)((uint32_t)GetDCFanData()->u16FanSpdErr * 92 / 100);
		if (GetSystemRunData()->u16FanSpeed < _u16temp)
		{
			u8WindChk_100ms = 0;
		}
		else
		{
#if (1 == UNITTEST)
			u8WindChk_100ms += u16UTestSysTime;
#else
			INC_B(u8WindChk_100ms);
#endif
		}
		if (PRO_WIND_TIME <= u8WindChk_100ms)
		{
			stFunctionOutputs.stSign.bProWindPress = 1;
		}
		else
		{
			stFunctionOutputs.stSign.bProWindPress = 0;
		}
		// 气压
	}
	else
	{
		stFunctionOutputs.stSign.bProWindPress = 0;
		u8WindChk_100ms = 0;
	}
}

/**********************************************
 *自清洁
 **********************************************/
void SelfCelanFunction(void)
{
	if (0 < GetCommSet()->u8SelfCleanEn)
	{
		INC_W(u16SelfClean_100ms);
		if ((SLEF_CLEAN_CNT <= u16SelfClean_100ms) || (FSM_STATE_IDLE != GetFsmState()) || (0 == GetSystemRunData()->sysSta.BIT.bOnOff))
		{
			GetCommSet()->u8SelfCleanEn = 0;
			u16SelfClean_100ms = 0;
			GetCommSet()->u8Priority = PRI_NONE;
		}
	}
	else
	{
		u16SelfClean_100ms = 0;
	}
}

/**********************************************
 *功能
 **********************************************/
void FunFunction(void) // 10MS
{
	static uint8_t FunFunction_Tick;

	if (++FunFunction_Tick < 10)
		return;
	FunFunction_Tick = 0;
	// 卫浴管家
	WaterExpedFunction(); // 100MS
	GasExpedFunction();

	WaterGasRecord();

	// 双气防护
	GasErrFunction();

	// 智能防护（排气）（气压）
	ProtcetFunction();

	// 自清洁
	SelfCelanFunction();
}

/**********************************************
卫浴管家参数重置
 **********************************************/
void WaterGasInit(void)
{
	u32WaterExpendRec = stFunctionOutputs.u32WaterExpend;
	u32GasExpendRec = u32GasExpend;
	stFunctionOutputs.u16WaterUsed = 0;
	stFunctionOutputs.u16GasUsed = 0;
}
void FunctionInit(void)
{
	memset(&stFunctionOutputs, 0, sizeof(stFunctionOutputs));
	stFunctionOutputs.u32GasUseSum = GetFlashDataSector1()->basicFlashData.u32GasSum;
	stFunctionOutputs.u32WaterUseSum = GetFlashDataSector1()->basicFlashData.u32WaterSum;
}
