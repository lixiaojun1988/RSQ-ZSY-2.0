#include "ALL_HEAD.H"
#define SYS_FLUX_OPEN_NUM 7
#define SYS_FLUX_CLOSE 20
#define SYS_FLUX_MAX 250
#define DCFAN_ON_SPD 600
#define DCFAN_OFF_SPD 450
#define ENV_TMP_ADD 3
#define FIRE_EXIST_AD_VAL  20

#define PowerOnIgnoreSLL_Time	300
#define PowerOnIgnoreOl_Time	800

// 需要访问在线的菜单表
static const uint8_t au8AddrOnline[] =
	{
		EN_ADDR_WIFI,
		EN_ADDR_REMOTE,
		EN_ADDR_WIRE,
		EN_ADDR_CTRL,
#if (ADD_DBG_INFO)
		EN_ADDR_MONITOR
#endif
};
typedef struct
{
	uint8_t CheckCnt;
	uint8_t LoseCnt;
} InputCheckFilt_Def;
static const uint8_t au8SysFluxOpen[] =
	{
		25, 30, 35, 40, 45, 50, 60};

InputCheckFilt_Def InputCheckFiltFlame_t, InputCheckFiltWater_t;
SystemRunDataTypeDef SystemRunData_t;

SystemRunDataTypeDef *GetSystemRunData(void)
{
	return &SystemRunData_t;
}

static uint16_t UserWrite_10ms = 0;
static uint8_t SnReSetCnt=0;

#if (FAKE_WATER)
static uint16_t u16FackFlu = 80;
#endif
uint8_t Flame_Check(uint16_t ADC_VAL) // 10MS
{
	static uint8_t b_flameExist = 0;
	if (ADC_VAL<FIRE_EXIST_AD_VAL) // 有火
	{
		if (InputCheckFiltFlame_t.CheckCnt < 255)
		{
			InputCheckFiltFlame_t.CheckCnt++;
		}
		InputCheckFiltFlame_t.LoseCnt = 0;
	}
	else
	{
		if (InputCheckFiltFlame_t.LoseCnt < 255)
		{
			InputCheckFiltFlame_t.LoseCnt++;
		}
		InputCheckFiltFlame_t.CheckCnt = 0;
	}

	if (InputCheckFiltFlame_t.CheckCnt >= 6) // 60MS
	{
		b_flameExist = 1;
	}
	if (InputCheckFiltFlame_t.LoseCnt >= 25) // 200MS
	{
		b_flameExist = 0;
	}
	return b_flameExist;
}

uint8_t Water_Check(void)
{
	static uint8_t b_waterExist = 0;
	uint8_t _u8Temp;
	if ((EN_PUMP_COLD == GetpPumpParm()->enState) || (EN_PUMP_CYCLE == GetpPumpParm()->enState) || (EN_PUMP_HEAT == GetpPumpParm()->enState))
	{
		_u8Temp = au8SysFluxOpen[0];
	}
	else
	{
		if (SYS_FLUX_OPEN_NUM > GetFlashDataSector0()->debugData.u8Ls)
		{
			_u8Temp = GetFlashDataSector0()->debugData.u8Ls;
		}
		else
		{
			_u8Temp = SYS_FLUX_OPEN_NUM - 1;
		}
		_u8Temp = au8SysFluxOpen[_u8Temp];
	}

	if (!b_waterExist)
	{
		if (GetSystemRunData()->Flux >=_u8Temp) // 有水
		{
			if (InputCheckFiltWater_t.CheckCnt < 255)
				InputCheckFiltWater_t.CheckCnt++;
		}
		else
			InputCheckFiltWater_t.CheckCnt = 0;

		InputCheckFiltWater_t.LoseCnt = 0;
	}
	else
	{
		if (GetSystemRunData()->Flux <= SYS_FLUX_CLOSE) // 有水
		{
			if (InputCheckFiltWater_t.LoseCnt < 255)
				InputCheckFiltWater_t.LoseCnt++;
		}
		else
			InputCheckFiltWater_t.LoseCnt = 0;

		InputCheckFiltWater_t.CheckCnt = 0;
	}

	if (InputCheckFiltWater_t.CheckCnt >= 5) // 50MS
	{
		if(!b_waterExist)
		{
			WaterGasInit();
			//若前期水流量保护时间段内有水，则不进行燃烧
			if (GetSystemRunData()->u16PowerIgnoreSLL_10ms)
			{
				GetSystemRunData()->u8PowerProtect = 1;
				GetSystemRunData()->u16PowerIgnoreSLL_10ms = 0;
			}
				
		}
		b_waterExist = 1;
	}
	if (InputCheckFiltWater_t.LoseCnt >= 5) // 50MS
	{
		b_waterExist = 0;
		GetSystemRunData()->u8PowerProtect = 0;
	}
	return b_waterExist;
}

void getOnOffDataFromComm(void)
{
	uint8_t _u8Temp;
	uint8_t _u8i;
	DEC(SystemRunData_t.u16PowerIgnoreSLL_10ms);
	DEC(SystemRunData_t.u16PowerOnline_10ms);
	// 判断所有设备是否离线，若均离线，则切换到关机
	if (GetSystemRunData()->sysSta.BIT.bOnOff == GetCommSet()->u8OnOffSet) 
	{
		_u8Temp = 0;
		for (_u8i = 0; sizeof(au8AddrOnline) > _u8i; _u8i++)
		{
			if (0 < GetCommSet()->au8AddrOnlineCnt[au8AddrOnline[_u8i]])
			{
				_u8Temp = 1;
				break;
			}
		}
		if ((0 == _u8Temp) && (0 == SystemRunData_t.u16PowerOnline_10ms))
		{
			if (0 < GetCommSet()->u8OnOffSet)
			{
				GetCommSet()->u8OnOffSet = 0;
			}	
		}
	}
	if (GetSystemRunData()->sysSta.BIT.bOnOff != GetCommSet()->u8OnOffSet) // 以接收到指令为优先判断
	{
		WaterGasInit();
		//开关机指令可以清除故障代码
		GetSystemRunData()->u8ErrorCode = EN_ERR_NO;
		GetSystemRunData()->u8ErrorCodePump = EN_ERR_NO;
		GetSystemRunData()->sysSta.BIT.bOnOff = GetCommSet()->u8OnOffSet;

		//接收到新的开关机状态，不分状态清零
		GetSystemRunData()->u16PowerIgnoreSLL_10ms = 0;
		GetSystemRunData()->u8PowerProtect = 0;
		// 如果为关机，则关闭所有在线状态
		if (0 == GetCommSet()->u8OnOffSet)
		{
			for (_u8i = 0; sizeof(au8AddrOnline) > _u8i; _u8i++)
			{
				GetCommSet()->au8AddrOnlineCnt[au8AddrOnline[_u8i]] = 0;
			}
			GetCommSet()->u8Priority = PRI_NONE;
		}
	}
	// 最后判断强制状态，强制状态下切换到开机
	if (0 < GetCommSet()->u8DebugIndex)
	{
		GetSystemRunData()->sysSta.BIT.bOnOff = 1;
		GetCommSet()->u8OnOffSet = 1;
	}

	if (GetFlashDataSector1()->basicFlashData.u8PowerOn != GetCommSet()->u8OnOffSet)
	{
		GetFlashDataSector1()->basicFlashData.u8PowerOn = GetCommSet()->u8OnOffSet;
		UserWrtieSetCD(500);
	}
}
/**********************************************
 *设置温度处理
 **********************************************/
void getSetTmpFromComm(void)
{
	uint16_t _u16Temp;
	// 智温感意外模式，温度无特殊处理
	if (EN_WASH_ENV != GetCommSet()->u8ModeSet)
	{
		_u16Temp = GetCommSet()->u8TmpSet;
	}
	else
	{
		// 智温感附加温度处理
		_u16Temp = (GetCommSet()->u8TmpSet + ENV_TMP_ADD - GetWashModeData()->u8EnvSta);
	}
	// 48℃温度上限
	if ((EN_WASH_ENV == GetCommSet()->u8ModeSet) || (EN_WASH_ECO == GetCommSet()->u8ModeSet) || (EN_WASH_ECO1 == GetCommSet()->u8ModeSet))
	{
		if (WASH_SPE_TMP_LIMIT < _u16Temp)
		{
			_u16Temp = WASH_SPE_TMP_LIMIT;
		}
	}
	// 60℃温度上限
	else
	{
		if (WASH_NOMAL_TMP_LIMIT < _u16Temp)
		{
			_u16Temp = WASH_NOMAL_TMP_LIMIT;
		}
	}

	GetSystemRunData()->TmpSet = _u16Temp * 10;
}

void systemSignalInput(void)
{
	// 火焰检测
#if (FAKE_FLAME)
	if ((FSM_STATE_IGINTION == GetFsmState())
		|| (FSM_STATE_STABLE == GetFsmState())
		|| (FSM_STATE_DEBUG == GetFsmState()))
	{
		GetSystemRunData()->sysSta.BIT.bFlame = 1;
	}
	else
	{
		GetSystemRunData()->sysSta.BIT.bFlame = 0;
	}
#else
	GetSystemRunData()->sysSta.BIT.bFlame = Flame_Check(adc_result_get(ADC_TMP_HY));
#endif

	// 温度获取
#if (DIS_TMP_ERR)
	GetSystemRunData()->TmpOut = 200;
	GetSystemRunData()->TmpIn = 200;
	GetSystemRunData()->TmpRoom = 200;
	GetSystemRunData()->TmpFD = 200;
#else
	GetSystemRunData()->TmpOut = temper_result_get(TMP_OUT);
	GetSystemRunData()->TmpIn = temper_result_get(TMP_IN);
	GetSystemRunData()->TmpRoom = temper_result_get(TMP_ROOM);
	GetSystemRunData()->TmpFD = temper_result_get(TMP_FDTX);
#endif
	// 流量数据
#if (FAKE_WATER)
	if (GetSystemRunData()->sysSta.BIT.bOnOff)
		GetSystemRunData()->Flux = u16FackFlu;
#else
	GetSystemRunData()->Flux = GetWaveSensorValue(GET_WAVE_FLUX);
#endif
	// 水检测
	GetSystemRunData()->sysSta.BIT.bWater = Water_Check();
	
	// 风速检测
#if (FAKE_FSPD)
	GetSystemRunData()->u16FanSpeed = 1500;
	GetSystemRunData()->u8FanRunning = 1;
#else
	GetSystemRunData()->u16FanSpeed = GetWaveSensorValue(GET_WAVE_FAN);
	if (DCFAN_ON_SPD < GetSystemRunData()->u16FanSpeed)
	{
		GetSystemRunData()->u8FanRunning = 1;
	}
	else if (DCFAN_OFF_SPD > GetSystemRunData()->u16FanSpeed)
	{
		GetSystemRunData()->u8FanRunning = 0;
	}
#endif
	// 温控开关检测
#if (DIS_VALVE_ERR)
	GetSystemRunData()->WkSw=0;
#else
	GetSystemRunData()->WkSw = GetInportsSta()->bOverTmpChk;
#endif
}
//条形码重新保存
void SNReSet(void)
{
	SnReSetCnt = 200;//2S

}

void UserWrtieSetCD(uint16_t _Val)
{
    UserWrite_10ms = _Val;
}

void UserWrite(void)
{
    if(UserWrite_10ms)
    {
        UserWrite_10ms--;
        if(0 == UserWrite_10ms)
        {
            writeFlashSector1();
        }
    }
}

void SNWrite(void)
{
	if(SnReSetCnt)
	{
		SnReSetCnt--;
		if(0 == SnReSetCnt)
		{
			memcpy(&GetFlashDataSector0()->snCodebuff,&GetCommSet()->au8SNCode,sizeof(GetCommSet()->au8SNCode));
			writeFlashSector0();
		}
	}
}
void systemSignalOutPut(void)
{
	// 风机图标
	if (SET_DCFAN_OFF < GetSystemRunData()->u16SetDCFanI)
	{
		GetSystemRunData()->sysSta.BIT.bFanOn = 1;
	}
	else
	{
		GetSystemRunData()->sysSta.BIT.bFanOn = 0;
	}

	// 故障信号
	if (EN_ERR_NO != GetSystemRunData()->u8ErrorCode || EN_ERR_NO != GetSystemRunData()->u8ErrorCodePump)
	{
		GetSystemRunData()->sysSta.BIT.bError = 1;
		GetCommSet()->u8Priority = PRI_NONE;
	}
	else
	{
		GetSystemRunData()->sysSta.BIT.bError = 0;
	}
}
void getDataFromComm(void)
{
	getSetTmpFromComm();
	// 开关机状态
	getOnOffDataFromComm();
	// 调试PP
	GetSystemRunData()->u8ManaulSetSeg = GetSegCtrl()->u8Set;
	GetSystemRunData()->u8ManaulSetFan = GetCommSet()->u8DebugData;
	GetSystemRunData()->u8ManaulSetPer = GetCommSet()->u8DebugData;
}

/**********************************************
 * 
 *
 **********************************************/
void sysBurnAbleCheck(void)
{
	GetSystemRunData()->bBurnAble = 0;
	// 两个状态可以接收燃烧请求：
	// 1、开机状态（非水泵强制工作时间）
	// 2、防冻工作时间
	if (((GetSystemRunData()->sysSta.BIT.bOnOff) && (!GetpPumpParm()->u8PumpForceWorkCnt))
		|| (EN_PUMP_COLD == GetpPumpParm()->enState))
	{
		if (0 == GetSystemRunData()->u8PowerProtect)
			GetSystemRunData()->bBurnAble = 1;
	}
}


void systemRun(void) // 10MS
{
	systemSignalInput();	// 系统信号输入处理
	systemSignalOutPut();	// 系统信号输出处理
	sysBurnAbleCheck();		// 燃烧请求判断
	getDataFromComm();		// 获取通讯信息
	RefreshBlfMaxMin();		// 获取比例阀和比例阀差值
	SNWrite();//sn码写入
    UserWrite();
}

void sysTemRunInit(void)
{
	memset(&SystemRunData_t, 0, sizeof(SystemRunData_t));
	GetSystemRunData()->u8ErrorCode = EN_ERR_NO;
	GetSystemRunData()->u8ErrorCodePump = EN_ERR_NO;
	memset(&InputCheckFiltFlame_t, 0, sizeof(InputCheckFiltFlame_t));
	memset(&InputCheckFiltWater_t, 0, sizeof(InputCheckFiltWater_t));
	GetSystemRunData()->sysSta.BIT.bOnOff = GetCommSet()->u8OnOffSet;
	if (GetCommSet()->u8OnOffSet)
	{
		
		GetSystemRunData()->u16PowerIgnoreSLL_10ms = PowerOnIgnoreSLL_Time;
		GetSystemRunData()->u16PowerOnline_10ms = PowerOnIgnoreOl_Time;
	}
	else
	{
		GetSystemRunData()->u16PowerIgnoreSLL_10ms = 0;
		GetSystemRunData()->u16PowerOnline_10ms = 0;
	}
}

/**********************************************
 *配置后清扫时间
 *
 **********************************************/
void SetEndCleanTimer(void)
{
	if (1 == GetFlashDataSector0()->debugData.u8Nh)
		GetSystemRunData()->u16EndClean_10ms = 6000;
	else
		GetSystemRunData()->u16EndClean_10ms = 3000;
	
	//E2故障不需要后清扫
	if ((EN_ERR_E2 == GetSystemRunData()->u8ErrorCode) || (EN_ERR_E2 == GetSystemRunData()->u8ErrorCodePump))
		GetSystemRunData()->u16EndClean_10ms = 0;
	//没有故障且在即热保温时间范围内，后清扫时间缩短 
	else if ((EN_ERR_NO == GetSystemRunData()->u8ErrorCode) && (EN_ERR_NO == GetSystemRunData()->u8ErrorCodePump) && (GetpPumpParm()->bSign.bPumpWarm))
		GetSystemRunData()->u16EndClean_10ms = 300;
}

/**********************************************
 * 查看是否处于超负荷状态
 *
 **********************************************/
uint8_t GetSurState(void)
{
    uint8_t _u8temp;
	_u8temp = 0;
    //处于非强制模式下，SUR模式或者SUR功能启动
	if ((FSM_STATE_DEBUG != GetFsmState()) && ((EN_WASH_SUR == GetCommSet()->u8ModeSet) || (GetCommSet()->u8SurSet)))
		_u8temp = 1;
	return _u8temp;
}
