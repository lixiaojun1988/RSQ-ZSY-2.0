#include "ALL_HEAD.H"

static void DCFan_Timer(void);
void DCFan_Update(void);
uint16_t DCFan_GetStkSta(uint16_t _u16SetFanU);
static uint16_t GetNonStkSpd(uint16_t _u16SetFan, const ST_FANRAW_T *_pstRaw);

uint16_t ConvertDbgToCtrl_DCFan(uint8_t _u8dbgset);

uint16_t GetFanUFromBlf(uint16_t _u16SetBlfi);
uint8_t GetPercentFromFanU(uint16_t _u16Fanset);

ST_DCFAN_T stFanData;
static uint8_t u8LmtStkPercent_100ms;

/**********************************************
 *
 *
 **********************************************/
 ST_DCFAN_T *GetDCFanData(void)
{
	return &stFanData;
}

/**********************************************
 *
 *
 **********************************************/
 uint16_t DCFan_NonStkSet(uint16_t _u16SetBlf)
{
	if (0 < stFanData.u8FanDown_100ms)
	{
		_u16SetBlf = _u16SetBlf - GetBlfIFromPercent(10) + GetSystemRunData()->u16BlfIRun_Min;
	}
	return GetFanUFromBlf(_u16SetBlf);
}
/**********************************************
 * 计算堵风之后的最后电流
 *
 **********************************************/
uint16_t DCFan_GetStkSta(uint16_t _u16SetFanU)
{
	static uint16_t _u16nonstkU;//上一次的风机转速（不加风前）
	uint16_t _16nonstkS;
	uint16_t _u16maxstkU, _16maxstkS;
	uint16_t _u16ret = _u16SetFanU;
	uint16_t _u16temp;
	uint8_t _u8temp;
	int16_t _s16FanUE;
	_s16FanUE = _u16SetFanU;
	_s16FanUE -= _u16nonstkU;
	_u16temp = stFanData.u16FanRunMax - stFanData.u16FanRunMin;
	_u16temp = _u16temp * 1 / 10;
	//若风机电流的变化比变化区间10%要大，则预留一定时间，不对风机状态做判断
	if ((_u16temp <= abs(_s16FanUE)) && (0 < _u16nonstkU) && (0 < _u16SetFanU))
	{
		u8LmtStkPercent_100ms = 30;
	}
	_u16nonstkU = _u16SetFanU;

	//生成最大增加占空比_u16maxstkU和当前比例阀开度下的风机最大占空比
	if (_u16nonstkU >= stFanData.u16FanRunMax)
	{
		_u16maxstkU = (uint32_t)stFanData.u16FanRunMax * stFanData.u8FanMaxMux / 10;
		_16maxstkS = stFanData.u16FanSpdErrMax;
	}
	else if (_u16nonstkU <= stFanData.u16FanRunMin)
	{
		_u16maxstkU = (uint32_t)stFanData.u16FanRunMin * stFanData.u8FanMinMux / 10;
		_16maxstkS = stFanData.u16FanSpdErrMin;
	}
	else
	{
		_u8temp = (uint8_t)((_u16nonstkU - stFanData.u16FanRunMin) * (stFanData.u8FanMaxMux - stFanData.u8FanMinMux) / (stFanData.u16FanRunMax - stFanData.u16FanRunMin));
		_u8temp += stFanData.u8FanMinMux;
		_u16maxstkU = _u16nonstkU * _u8temp / 10;

		_16maxstkS = (uint16_t)(((uint32_t)_u16nonstkU - stFanData.u16FanRunMin) * (stFanData.u16FanSpdErrMax - stFanData.u16FanSpdErrMin) / (stFanData.u16FanRunMax - stFanData.u16FanRunMin));
		_16maxstkS += stFanData.u16FanSpdErrMin;
	}

	//获取不堵塞情况下（通过查表，表格记录占空比和转速关系）
	_16nonstkS = GetNonStkSpd(_u16nonstkU, GetWorkCon()->pstFanRawData);

	stFanData.u16FanSpdRaw = _16nonstkS;
	stFanData.u16FanSpdErr = _16maxstkS;
	if (0 == stFanData.u8NonStk_100ms)
	{
		//大于不堵塞时候的转速
		_u16ret = (uint16_t)((uint32_t)_16nonstkS * 103 / 100);
		if (GetSystemRunData()->u16FanSpeed >= _u16ret)
		{
			if (_u16maxstkU < _u16nonstkU)
			{
				_u16maxstkU = _u16nonstkU;
			}
			//避免整除0导致程序异常
			if (_16maxstkS <= _16nonstkS)
			{
				_16maxstkS = _16nonstkS + 1;
			}
			_u16temp = RANGLMT(GetSystemRunData()->u16FanSpeed, _16nonstkS, _16maxstkS);
			//计算出，理论堵塞情况（同时也是目标加风情况）
			_u8temp = (uint8_t)(((uint32_t)_u16temp - _16nonstkS) * 100 / (_16maxstkS - _16nonstkS));
			if (0 == u8LmtStkPercent_100ms)
			{
				stFanData.u8StkPercent = _u8temp;
				_u16ret = (uint16_t)(((uint32_t)_u16temp - _16nonstkS) * (_u16maxstkU - _u16nonstkU) / (_16maxstkS - _16nonstkS));
				_u16ret += _u16nonstkU;
			}
			else
			{
				stFanData.u8StkPercent = GETMIN(_u8temp, stFanData.u8StkPercent);
				_u16ret = (_u16maxstkU - _u16nonstkU) * stFanData.u8StkPercent / 100;
				_u16ret += _u16nonstkU;
			}
		}
		else
		{
			_u16ret = _u16nonstkU;
			stFanData.u8StkPercent = 0;
		}
	}
	else
	{
		_u16ret = _u16nonstkU;
	}
	return _u16ret;
}

static uint16_t GetNonStkSpd(uint16_t _u16SetFan, const ST_FANRAW_T *_pstRaw)
{
	uint16_t _u16ret = 0, _u16FanUE, _u16temp;
	uint8_t _u8FanUSeg, _u8Flg;

	_u16FanUE = ConvertDbgToCtrl_DCFan(GetFlashDataSector0()->debugData.u8Fh - GetFlashDataSector0()->debugData.u8Fl);
	_u16FanUE /= 9;
	_u16temp = ConvertDbgToCtrl_DCFan(GetFlashDataSector0()->debugData.u8Fl);
	_u8Flg = 0;
	_u8FanUSeg = 0;
	while (9999 > (_pstRaw + _u8FanUSeg)->u16FanSpdRaw)
	{
		if (_u16temp == _u16SetFan)
		{
			_u8Flg = 1;
			break;
		}
		if (_u16temp > _u16SetFan)
		{
			break;
		}
		_u16temp += _u16FanUE;
		_u8FanUSeg += 1;
	}
	_u16ret = (_pstRaw + _u8FanUSeg)->u16FanSpdRaw;
	if (9999 <= _u16ret)
	{
		_u16ret = (_pstRaw + _u8FanUSeg - 1)->u16FanSpdRaw;
	}
	else
	{
		if (0 < _u8Flg)
		{
			_u16ret = (_pstRaw + _u8FanUSeg)->u16FanSpdRaw;
		}
		else
		{
			_u16ret = ((_pstRaw + _u8FanUSeg)->u16FanSpdRaw - (_pstRaw + _u8FanUSeg - 1)->u16FanSpdRaw) * (_u16SetFan - (_u16temp - _u16FanUE)) / _u16FanUE;
			_u16ret += (_pstRaw + _u8FanUSeg - 1)->u16FanSpdRaw;
		}
	}

	return _u16ret;
}
/**********************************************
 *
 *
 **********************************************/
uint16_t GetFanUFromBlf(uint16_t _u16SetBlfi)
{
	uint8_t _u8Temp = 0;	
	uint16_t _u16res = 0;
	_u8Temp = GetPercentFromBlfI(_u16SetBlfi);
	_u16res = (uint16_t)(((uint32_t)stFanData.u16FanRunMax - stFanData.u16FanRunMin) * _u8Temp / 100 + stFanData.u16FanRunMin);
	return _u16res;
}
/**********************************************
 *
 *
 **********************************************/
uint8_t GetPercentFromFanU(uint16_t _u16Fanset)
{
	uint8_t _u8res;
	_u16Fanset = RANGLMT(_u16Fanset, stFanData.u16FanRunMin, stFanData.u16FanRunMax);
	_u8res = (uint8_t)(((uint32_t)_u16Fanset - stFanData.u16FanRunMin) * 100 / (stFanData.u16FanRunMax - stFanData.u16FanRunMin));
	return _u8res;
}
/**********************************************
 *
 *
 **********************************************/
uint16_t GetBlfIFromFanSet(uint8_t _u8FanSet)
{
	uint8_t _u8temp;
	uint16_t _ret;
	_u8temp = GetPercentFromFanU(ConvertDbgToCtrl_DCFan(_u8FanSet));
	_ret = GetBlfIFromPercent(_u8temp);
	return _ret;
}
/**********************************************
 *
 *
 **********************************************/
uint16_t ConvertDbgToCtrl_DCFan(uint8_t _u8dbgset)
{
	uint16_t _u16res;
	_u16res = _u8dbgset;
	_u16res = (uint16_t)((uint32_t)_u16res * FANU_MAX / 0xff);
	return _u16res;
}
/**********************************************
 *
 *
 **********************************************/
void SetFanDownTime(uint8_t _u8set)
{
	stFanData.u8FanDown_100ms = _u8set;
}
/**********************************************
 *
 *
 **********************************************/
void SetLimtStkTime(uint8_t _u8set)
{
	u8LmtStkPercent_100ms = _u8set;
}
/**********************************************
 *
 *
 **********************************************/
void SetNonStkTime(uint8_t _u8set)
{
	stFanData.u8NonStk_100ms = _u8set;
}
/**********************************************
 *
 *
 **********************************************/
static void DCFan_Timer(void) // 10MS
{
	static uint16_t u16_DiffTimer = 0;
	if (++u16_DiffTimer >= 10)
	{
		u16_DiffTimer = 0;
		DEC(stFanData.u8FanDown_100ms);
		DEC(stFanData.u8NonStk_100ms);
		DEC(u8LmtStkPercent_100ms);
	}
}

/**********************************************
 * 更新风机状态
 *
 **********************************************/
void DCFanProcess(void)
{
	uint32_t _u32temp;
	uint16_t _u16temp;
	uint8_t _u8Sur;
	_u8Sur = GetSurState();
	DCFan_Timer();
	//最终风机基本工作最大电流
	_u16temp = GetFlashDataSector0()->debugData.u8Fh + getAdapData()->Fh_add;
	if (_u8Sur)
		_u16temp += GetFlashDataSector0()->debugData.u8Sl;
	_u16temp = GETMIN(_u16temp, 0xFF);
	stFanData.u16FanRunMax = ConvertDbgToCtrl_DCFan((uint8_t)_u16temp);
	// stFanData.u16FanRunMax = ConvertDbgToCtrl_DCFan(GetFlashDataSector0()->debugData.u8Fh+getAdapData()->Fh_add);

	//最终风机基本工作最小电流
	_u16temp = GetFlashDataSector0()->debugData.u8Fl + getAdapData()->Fl_add;
	
	_u16temp = GETMIN(_u16temp, 0xFF);
	stFanData.u16FanRunMin = ConvertDbgToCtrl_DCFan((uint8_t)_u16temp);
	// stFanData.u16FanRunMin = ConvertDbgToCtrl_DCFan(GetFlashDataSector0()->debugData.u8Fl+getAdapData()->Fl_add);

	//比例阀大开度下最大转速限制
	_u16temp = (uint16_t)GetFlashDataSector0()->debugData.u8Hc + getAdapData()->Hc_add;
	_u16temp = GETMIN(_u16temp, 99);// 转速的最大限制为99
	stFanData.u16FanSpdErrMax = _u16temp * 60;

	//比例阀小开度下最大转速限制
	stFanData.u16FanSpdErrMin = (uint16_t)GetFlashDataSector0()->debugData.u8Lc + getAdapData()->Lc_add;
	_u16temp = GETMIN(_u16temp, 99);// 转速的最大限制为99
	stFanData.u16FanSpdErrMin = _u16temp * 60;

	//最大开度下，加风比例
	stFanData.u8FanMaxMux = GetWorkCon()->stFanErrData.u8FanMaxMux;

	//最小开度下，加风比例
	stFanData.u8FanMinMux = GetWorkCon()->stFanErrData.u8FanMinMux;

	//后清扫使用转速
	stFanData.u16FanCleanU = (uint16_t)(((uint32_t)stFanData.u16FanRunMax - stFanData.u16FanRunMin) * 3 / 5 + stFanData.u16FanRunMin);

    //处于换挡的时候
    if (GetSegCtrl()->u8ChgStep)
	{
		// _u32temp = GetSystemRunData()->u16SetDCFanI;
		// _u32temp = _u32temp * 80 / 100;
		Drv_SetDCFanPWM(ConvertDbgToCtrl_DCFan(GetFlashDataSector0()->debugData.u8Fl));
	}
	else
		Drv_SetDCFanPWM(GetSystemRunData()->u16SetDCFanI);
}

/**********************************************
 * 风机堵塞判定处理
 *
 **********************************************/
void SetDcFanUfromBlf(uint16_t u16SetBlfI)
{
    GetDCFanData()->u16FanUGet = DCFan_NonStkSet(u16SetBlfI);
	GetDCFanData()->u16FanUGetNonStk = GetDCFanData()->u16FanUGet;//获取不堵塞时候，风机转速
	//GetSystemRunData()->u16SetDCFanI = DCFan_GetStkSta(GetDCFanData()->u16FanUGet);
	GetSystemRunData()->u16SetDCFanI = GetDCFanData()->u16FanUGet;

}

