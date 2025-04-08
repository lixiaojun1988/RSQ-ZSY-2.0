#include "ALL_HEAD.H"

static uint8_t u8WaterExpendCnt;	  // ���ڼ�ʱ
static uint32_t u32WaterExpendRec;	  // ��ˮˮ����־����һ�ι�ˮʱ��¼��
static uint32_t u32WaterExpendSumRec; // �����ۼ�
static uint8_t u8GasExpendCnt;		  // ���ڼ�ʱ
static uint32_t u32GasExpend;		  // ��ˮˮ�������ϵ翪ʼ�㣩
static uint32_t u32GasExpendRec;	  // ��ˮˮ����־����һ�ι�ˮʱ��¼��
static uint32_t u32GasExpendSumRec;	  // �����ۼ�

static ST_FUNCTION_OUTPUTS_T stFunctionOutputs;
#define EXPEND_TIME 10		 // ��ԡ�ܼҹ���ʱ�䣨0.1s��
#define EXPEND_WATER_LTR 600 // 1L���ۼ�ֵ 1���¼һ�Σ�ÿ�μ�¼L/min�� 60 * 10
#define EXPEND_12T_LTR 44500 // ÿ1L���ۼƸ���
#define EXPEND_20Y_LTR 145266

// ���ܷ�����������
static uint16_t u16ProFlow;	  // ���������ļ���
static uint8_t u8PumpFlowCnt; // ˮ������¼ʱ���ʱ
static uint8_t u8ProAirChk_500ms;
static uint8_t u8ProInitCnt;   // ������ʼ��ʱ��
static uint8_t u8ProAirChkNum; // �仯����1L�Ĵ���
#define PRO_AIR_TIME 20		   // �����ۼ�ʱ�䣨0.5s��
#define PRO_AIR_LITE 10		   // ����ʱ���仯ˮ����0.1L��
#define PRO_PUMP_INI_TIME 50
#define PRO_AIR_NUM 3 // �仯�Ĵ���

static uint16_t u16SelfClean_100ms;
#define SLEF_CLEAN_CNT 300

// ���ܷ�������ѹ��
static uint8_t u8WindChk_100ms;
#define PRO_WIND_TIME 50 // ��ѹ�ж�ʱ��

static const ST_GAS_PRO_T astProGas[] =
	{
		// CH4���ϣ�CH4�����CO���ϣ�CO���
		{15, 4, 30, 12},
		{20, 4, 40, 12}};

/**********************************************
���ݽӿ�
 **********************************************/
const ST_FUNCTION_OUTPUTS_T *GetFunOutputs(void)
{
	return &stFunctionOutputs;
}

/**********************************************
 *��ԡ�ܼ�
 **********************************************/
// �������
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

// ��ˮ������
void WaterExpedFunction(void)
{
	DEC(u8WaterExpendCnt);
	if (0 == u8WaterExpendCnt)
	{
		u8WaterExpendCnt = EXPEND_TIME;

		// �ػ�����ѭ�������������Ȳ������ۼ���ˮ��
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
		// ȼ��״̬�ǽ��������ۼ�
		if (FSM_STATE_STABLE == GetFsmState())
		{
			u32GasExpend = FunctionU32Add(u32GasExpend, GetTmpCtrlData()->u16SetLoad);
		}
	}
}

// �ж������Ƿ���Ҫ����
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
	// �������εĲ�ֵ
	if (_u32temp >= _u32temp2)
	{
		_u32temp = _u32temp - _u32temp2;
	}
	else
	{
		_u32temp = 0xFFFFFFFF - _u32temp2 + _u32temp;
	}
	// �жϱ仯���޳���1L
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
	// ������ԡ�ܼ����ָ��/ˢ��Flash
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

	// ��ˮ��ԡ�ܼҹ�������
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
 *˫�����Ϲ���
 **********************************************/
void GasErrFunction(void)
{
	// �鿴������ʧ����
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
	// �ж��Ƿ����
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

		// �ж�CO
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

		// �ж�CH4
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

		// ������F6��E0����
	if (((1 == stFunctionOutputs.stSign.bCH4Warm) || (EN_CO_WARM == stFunctionOutputs.u8CoSat)) && ((FSM_STATE_F6 !=GetFsmState()) && (FSM_STATE_ERROR != GetFsmState())))
		{
			SetFsmState(FSM_STATE_F6);
		}
	}
}

/**********************************************
 *���ܷ���
 **********************************************/
// TODO:���ܷ���
void ProtcetFunction(void)
{
	int32_t _s32temp;
	uint16_t _u16temp;
	// ������ˮ�ù���ʱ���ж���
#if (1 == UNITTEST)
	u8ProInitCnt = 0;
#else
	DEC(u8ProInitCnt);
#endif
	// ˮ�ÿ�ʼһ��ʱ���ڲ����������ж�
	if ((EN_PUMP_COLD != GetpPumpParm()->enState) && (EN_PUMP_HEAT != GetpPumpParm()->enState) && (EN_PUMP_CYCLE != GetpPumpParm()->enState))
	{
		u8ProInitCnt = PRO_PUMP_INI_TIME;
		u8ProAirChk_500ms = 0;
		stFunctionOutputs.stSign.bProAir = 0;
		u8ProAirChkNum = 0;
	}
	// �����ж�
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
		// ˢ��ˮ��
		u16ProFlow = GetSystemRunData()->Flux;
		u8PumpFlowCnt = 0;
	}

	// ��ѹ�ͷ�ѹ������ȼ��ʱ�ж���
	if (FSM_STATE_STABLE == GetFsmState())
	{
		// ��ѹ
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
		// ��ѹ
	}
	else
	{
		stFunctionOutputs.stSign.bProWindPress = 0;
		u8WindChk_100ms = 0;
	}
}

/**********************************************
 *�����
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
 *����
 **********************************************/
void FunFunction(void) // 10MS
{
	static uint8_t FunFunction_Tick;

	if (++FunFunction_Tick < 10)
		return;
	FunFunction_Tick = 0;
	// ��ԡ�ܼ�
	WaterExpedFunction(); // 100MS
	GasExpedFunction();

	WaterGasRecord();

	// ˫������
	GasErrFunction();

	// ���ܷ���������������ѹ��
	ProtcetFunction();

	// �����
	SelfCelanFunction();
}

/**********************************************
��ԡ�ܼҲ�������
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
