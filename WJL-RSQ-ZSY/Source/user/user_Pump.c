#include "ALL_HEAD.H"

//无需修改参数
#define PREHEAT_UNSTUDY_DIS 0XFFFF	// 循环模式为1，且未学习，反馈的时间

#define PUMP_ERROR_TIME 150

//防冻参数
#define COLD_TMP 50     //防冻触发温度（0.1℃）
#define COLD_ENTER_TIME 10//进入防冻时间（0.1s）

//水控参数
#define WATER_CTRL_TIME_MAX 50 // 进入即热最长时间（0.1s）
#define WATER_CTRL_TIME_MIN 30 // 进入即热最长时间（0.1s）

//水泵强制工作参数
#define FORCE_UNWORK_TIME 1152000 // 水泵不工作N后，水泵强制工作（0.1s）10*60*60*32 = 1152000
#define FORCE_WORK_TIME 100       // 水泵强制工作时间（0.1s）

//水泵即热工作参数
#define HEAT_STOP_INTMP 20 // 即热停止进水温度
#define HEAT_STOP_OUTTMP 100 // 即热停止出水温度
#define HEAT_STUDY_STOP_INTMP 50 // 智循环的学习，进水温度停止条件
#define HEAT_FORCE_WORK_TIME 9000 // 两次强制即热时间间隔（0.1s）

#define HEAT_INTERVAL_TIME 600 // 即热与最近工作的最短间隔时间
#define HEAT_ONCE_MAX_TIME 18000// 即热最长单次时间（0.1s） 30min = 60*30*10

#define HEAT_STUDY_SUCCESS_TIME 300// 智学习的最短即热工作时间（避免工作时间太短，学习的时间存在问题）
#define HEAT_STUDY_MIN      300// 智学习最短学习成功时间

#define HEAT_LOAD_STOP_TIME 30// 即热因负荷满足退出时间

#define PREHEAT_UNSTUDY 0XFFFF	// 未学习，时间为0
//增压参数
#define BURST_TIME      60 //增压启动判断时间
#define BURST_STABLE_TIME 100//在燃烧系统燃烧稳定（0.1s）后再进行增压
#define BURST_STOP_INTMP 20 // 即热停止进水温度
#define BURST_STOP_OUTTMP 50 // 即热停止出水温度
#define BURST_STOP_TMP_TIME 30 // 即热停止出水温度

#define BURST_STOP_FLU      10//增压停止流量
#define BURST_STOP_FLU_TIME     5//增压停止时间
//后循环
#define CYCLE_TMP_TIME  30// 后循环启动时间
#define CYCLE_STOP_INTMP 70// 后循环时，遇到进水温度过低，会退出后循环


// 防冻
#define COLD_FORCE_TIME     0// 强制防冻时间（单位0.1s）
#define COLD_TIME           600//防冻时间（单位0.1s）

#define COLD_STOP_OUTTMP    100//防冻停止出水温度

// 智学习
#define SMART_DAY       3//智学习记录的天数


static uint8_t u8StateStep;//用于判断判断处于进入/工作状态
static ST_PUMP_OUTPUTS_T stPumpOutputs;

//即热功能变量
static uint32_t u32Heat_100ms;     // 即热功能持续执行时间（0.1s）
static uint16_t u16HeatStudy_1s;   // 循环模式1，学习完成后的时间

//单次即热的总时间
static uint16_t u16HeatOnce_100ms;

//智学习功能
static uint8_t au8SmartTime[SMART_DAY][6];// 记录3天的用水习惯，时间的逻辑与预约相同（记录3天时间）
static uint8_t au8SmartTimeTmp[6];// 记录一天的用水习惯，一天记录接收后再赋值给au8SmartTime

//增压功能常量参数
static const uint8_t au8BurstLiteTable[EN_BURST_MODE_MAX] =
{
    100,100,100,
    50,60,70,80,90,100,
    100
};



uint8_t CheckDataSycn(uint8_t* _Data, uint8_t _Order)
{
    uint8_t _u8Tmp;
    _u8Tmp = 0;
	if (*_Data != _Order)
	{
		*_Data = _Order;
        _u8Tmp = 1;
	}
    return _u8Tmp;
}

/**********************************************
 * 智学习功能学习当天的调整
 * _On开启或关闭时间
 **********************************************/
void SetSmartTime(uint8_t _On,uint8_t _Time)
{
    uint8_t _i,_j;
    _i = _Time / 8;
    _j = _Time % 8;
    if (_On)
        au8SmartTimeTmp[_i] |= (1 << _j);
    else
        au8SmartTimeTmp[_i] &= ~(1 << _j);
}

/**********************************************
 * 获取当前时间段
 * 返回值类似（晨洗，夜浴，智学习的时间段）
 **********************************************/
uint8_t GetTime(void)
{
	uint8_t _u8Tmp;
	_u8Tmp = GetSystemRunData()->u8HourCur * 2;
	_u8Tmp += (GetSystemRunData()->u8MinCur / 30);

	return _u8Tmp;
}

void PumpE7_Function(uint8_t _On)
{
    static uint8_t u8ErrE7_100ms = 0;
    if (_On)
    {
        if (GetSystemRunData()->sysSta.BIT.bWater)
            u8ErrE7_100ms = 0;
        else
        {
            INC_B(u8ErrE7_100ms);
            if (PUMP_ERROR_TIME < u8ErrE7_100ms)
            {
                GetSystemRunData()->u8ErrorCode = EN_ERR_E7;
                SetFsmState(FSM_STATE_ERROR);
            }
        }
    }
    else
        u8ErrE7_100ms = 0;
    
}

/**********************************************
 * 信息同步
 * 
 **********************************************/
void PumpCommSycn(void)
{
    static uint8_t u8FastHeatBef;
    uint8_t _u8FlashRe = 0;
    uint8_t _i;
    // 无掉电记忆部分
    if (u8FastHeatBef != GetCommSet()->u8FastHeat)
    {
        if (GetCommSet()->u8FastHeat)
        {
            stPumpOutputs.bSign.bHeatFirst = 1;
            SetSmartTime(1,GetTime());
        }
        else
        {
            stPumpOutputs.bSign.bHeatFirst = 0;
            SetSmartTime(0,GetTime());
        }
    }
    u8FastHeatBef = GetCommSet()->u8FastHeat;

    _u8FlashRe |= CheckDataSycn(&GetFlashDataSector1()->pumpData.u8CycleMode,GetCommSet()->u8CycleMode);
    _u8FlashRe |= CheckDataSycn(&GetFlashDataSector1()->pumpData.u8PreWarmTime,GetCommSet()->u8PreWarmTime);
    _u8FlashRe |= CheckDataSycn(&GetFlashDataSector1()->pumpData.u8PreWarmDif,GetCommSet()->u8PreWarmDif);

    _u8FlashRe |= CheckDataSycn(&GetFlashDataSector1()->pumpData.u8MorWashStart,GetCommSet()->u8MorWashStart);
    _u8FlashRe |= CheckDataSycn(&GetFlashDataSector1()->pumpData.u8MorWashEnd,GetCommSet()->u8MorWashEnd);
    _u8FlashRe |= CheckDataSycn(&GetFlashDataSector1()->pumpData.u8NigWashStart,GetCommSet()->u8NigWashStart);
    _u8FlashRe |= CheckDataSycn(&GetFlashDataSector1()->pumpData.u8NigWashEnd,GetCommSet()->u8NigWashEnd);

    for (_i = 0; _i < 6; _i++)
        _u8FlashRe |= CheckDataSycn(&GetFlashDataSector1()->pumpData.au8Book24Set[_i],GetCommSet()->unBook24Set.Bytes[_i]);

    _u8FlashRe |= CheckDataSycn(&GetFlashDataSector1()->pumpData.u8MorWashStart,GetCommSet()->u8MorWashStart);

    if (_u8FlashRe)
        UserWrtieSetCD(500);
}
/**********************************************
 * 更改即热功能状态
 * 
 **********************************************/
void PumpChangeFastHeat(uint8_t _On)
{
    if (_On != GetCommSet()->u8FastHeat)
    {
        GetCommSet()->u8FastHeat = _On;
        GetCommSet()->u8Priority = PRI_NONE;
    }
    // 关闭即热后，同时将即热总时长清零
    if (!_On)
        u32Heat_100ms = 0;
}

/**********************************************
 * 水泵工作处理
 * 
 **********************************************/
void SetPumpWork(uint8_t _On)
{
    static uint8_t ChangeCnt_100ms;
    static uint8_t PumpWorkBef;
    if (PumpWorkBef != _On)
    {
        if (++ChangeCnt_100ms >= 3)
        {
            ChangeCnt_100ms = 0;
            PumpWorkBef = _On;
        }
    }
    else
        ChangeCnt_100ms = 0;

    Drv_OutPut_Set(EN_OUTPORT_WPump,PumpWorkBef);
}
/**********************************************
 *后循环时间
 *单位0.1s
 *_u16PreHeatOnceTime为单次即热时间
 **********************************************/
uint16_t PumpCycleTime(uint16_t _u16PreHeatOnceTime)
{
	uint16_t _u16CycleTime;
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
static uint16_t PumpHeatTime(void)
{
	uint16_t _u16Temp;
	if (21 <= GetFlashDataSector1()->pumpData.u8CycleMode)
		_u16Temp = 120;
	else if (20 == GetFlashDataSector1()->pumpData.u8CycleMode)
		_u16Temp = 210;
	else if (19 == GetFlashDataSector1()->pumpData.u8CycleMode)
		_u16Temp = 150;
	else if (16 <= GetFlashDataSector1()->pumpData.u8CycleMode)
		_u16Temp = ((GetFlashDataSector1()->pumpData.u8CycleMode - 15) * 30);
	else if (2 <= GetFlashDataSector1()->pumpData.u8CycleMode)
		_u16Temp = (GetFlashDataSector1()->pumpData.u8CycleMode * 60);
	else if (1 == GetFlashDataSector1()->pumpData.u8CycleMode)
	{
		if (0 == u16HeatStudy_1s)
			_u16Temp = PREHEAT_UNSTUDY;

		else
			_u16Temp = u16HeatStudy_1s;
	}
	else
		_u16Temp = PREHEAT_UNSTUDY;
	return _u16Temp;
}
/**********************************************
 * 水控功能
 * 判断是否开启即热
 * 注：该状态水泵不工作的时候进行判定
 **********************************************/
static void PumpWaterCtrl(void)
{
    static uint8_t u8WaterCtrl_100ms;
    // 处于以下状态，水控不需要判断时间
    // 1、水控功能未开启；
    // 2、处于关机状态；
    // 3、水泵不处于待机状态
    // 4、处于故障状态
    if (!GetCommSet()->u8WaterControl 
        || !GetSystemRunData()->sysSta.BIT.bOnOff 
        || EN_PUMP_UNWORK != stPumpOutputs.enState
        || GetSystemRunData()->sysSta.BIT.bError)
        u8WaterCtrl_100ms = 0;
    else
    {
        // 有水的时候增加时间累计
        if (GetSystemRunData()->sysSta.BIT.bWater)
            INC_B(u8WaterCtrl_100ms);
        else
        {
            if ((WATER_CTRL_TIME_MAX >= u8WaterCtrl_100ms) && (WATER_CTRL_TIME_MIN < u8WaterCtrl_100ms))
            {
                //TODO：需要增加智学习的即热
                stPumpOutputs.bSign.bHeatFirst = 1;
                u32Heat_100ms = 0;
                // 开启即热
                PumpChangeFastHeat(1);
            }
            u8WaterCtrl_100ms = 0;
            
        }
    }
}

/**********************************************
 * 常规即热温度停止条件判断
 * return 1 ：在停止条件范围内
 * return 0 ：不在停止条件范围内
 **********************************************/
static uint8_t GetHeatTmpStop(void)
{
    uint8_t _u8Tmp = 0;
    // 循环模式为学习时，且未学习成功时，首次判断完成时间缩短
    if (1 == GetCommSet()->u8CycleMode && (0 == u16HeatStudy_1s))
    {
        if ((GetSystemRunData()->TmpSet + HEAT_STOP_OUTTMP <= GetSystemRunData()->TmpOut)
            || (GetSystemRunData()->TmpSet - HEAT_STUDY_STOP_INTMP <= GetSystemRunData()->TmpIn))
            _u8Tmp = 1;
    }
    else
    {
        if ((GetSystemRunData()->TmpSet + HEAT_STOP_OUTTMP <= GetSystemRunData()->TmpOut)
            || (GetSystemRunData()->TmpSet - HEAT_STOP_INTMP <= GetSystemRunData()->TmpIn))
            _u8Tmp = 1;
    }
    return _u8Tmp;
}

/**********************************************
 * 常规即热温度停止条件判断
 * return 1 ：在停止条件范围内
 * return 0 ：不在停止条件范围内
 **********************************************/
static uint8_t GetBurstTmpStop(void)
{
    uint8_t _u8Tmp = 0;
    if ((GetSystemRunData()->TmpSet + BURST_STOP_OUTTMP <= GetSystemRunData()->TmpOut)
        || (GetSystemRunData()->TmpSet - BURST_STOP_INTMP <= GetSystemRunData()->TmpIn))
        _u8Tmp = 1;
    return _u8Tmp;
}

/**********************************************
 * 常规即热负荷停止条件判断
 * 当流量满足大于一定数值，且处于较大负荷时，会退出即热（处于最大负荷，且流量大于7.8L/min）
 * return 1 ：在停止条件范围内
 * return 0 ：不在停止条件范围内
 **********************************************/
static uint8_t GetPumpHeatLoadStop(void)
{
    uint8_t _u8Tmp = 0;
    if ((GetSystemRunData()->u16BlfIRun_Max == GetSystemRunData()->u16SetBlfI) && (GetWorkCon()->u8MaxSeg == GetSegCtrl()->u8Set) && (78 < GetSystemRunData()->Flux))
        _u8Tmp = 1;
    return _u8Tmp;
}

/**********************************************
 *单次即热最长时间即热时间
 *单位s
 **********************************************/
static uint16_t PumpHeatOnceTime(void)
{
	uint16_t _u16Temp;
	if (21 <= GetFlashDataSector1()->pumpData.u8CycleMode)
		_u16Temp = 120;
	else if (20 == GetFlashDataSector1()->pumpData.u8CycleMode)
		_u16Temp = 210;
	else if (19 == GetFlashDataSector1()->pumpData.u8CycleMode)
		_u16Temp = 150;
	else if (16 <= GetFlashDataSector1()->pumpData.u8CycleMode)
		_u16Temp = ((GetFlashDataSector1()->pumpData.u8CycleMode - 15) * 30);
	else if (2 <= GetFlashDataSector1()->pumpData.u8CycleMode)
		_u16Temp = (GetFlashDataSector1()->pumpData.u8CycleMode * 60);
	else if (1 == GetFlashDataSector1()->pumpData.u8CycleMode)
    {
		if (0 == u16HeatStudy_1s)
			_u16Temp = PREHEAT_UNSTUDY_DIS;
		else
			_u16Temp = u16HeatStudy_1s;
	}
    else
		_u16Temp = PREHEAT_UNSTUDY_DIS;
    
	return _u16Temp;
}

/**********************************************
 * 水泵状态切换
 * 
 **********************************************/
void PumpStateChange(EN_PUMP_STA_T _enState)
{
    stPumpOutputs.enState = _enState;
    u8StateStep = 0;
}

/**********************************************
 * 增压模式水量判断
 * return 增压启动水量（0.1L
 **********************************************/
uint8_t GetPumpBurstLite(void)
{
    uint8_t _u8Tmp;
#if (PUMP_FREP_EN)
    if (GetCommSet()->u8BurstType >= sizeof(au8BurstLiteTable))
        _u8Tmp = au8BurstLiteTable[0];
    else
        _u8Tmp = au8BurstLiteTable[GetCommSet()->u8BurstType];
#else
    _u8Tmp = au8BurstLiteTable[0];
#endif
    return _u8Tmp;
}






void Pump_SmartFunction(void)
{
    static uint16_t u16StableTime = 0;// 燃烧时间
    static uint8_t SmartInit = 0;
    static uint8_t TimeBef = 0;
    static uint8_t FirstDay = 1;// 第一天记录
    static uint8_t DayIndex = 0;// 天数引索值
    static uint8_t u8TimeCnt = 0;// 记录一天刷新时间的次数
    
    uint8_t _TimeNow;
    uint8_t _ReSet;
    uint8_t _i;
    _TimeNow = GetTime();
    if (!SmartInit)
    {
        TimeBef = _TimeNow;
    }
    // 初始化完成后
    else
    {
        // 处于非即热/防冻导致的燃烧工作，计时燃烧时间
        if ((FSM_STATE_STABLE == GetpPumpParm()->enState) && (EN_PUMP_COLD != GetpPumpParm()->enState) && (EN_PUMP_HEAT != GetpPumpParm()->enState))
            INC_W(u16StableTime);
        
        // 完成一个时间段数据的采集
        if (TimeBef != _TimeNow)
        {
            INC_B(u8TimeCnt);
            _ReSet = 0;
            // 判断是否当前时间段为用户用水时间
            if ((uint16_t)GetCommSet()->u8WaterUseTime * 600 <= u16StableTime)
                SetSmartTime(1,TimeBef);
            u16StableTime = 0;

            // 判断是否一天结束
            if (!_TimeNow && (47 <= TimeBef))
            {
                //首次上电，或当天正常计时时间超过10次（后者主要是避免过了当天时间，突然修正回最后一个时间段，导致又刷新新的数据）
                if (FirstDay || (12 < u8TimeCnt))
                {
                    FirstDay = 0;
                    for (_i = 0; _i < sizeof(au8SmartTimeTmp); _i++)
                        au8SmartTime[DayIndex][_i] = au8SmartTimeTmp[_i];

                    if (++DayIndex >= SMART_DAY)
                        DayIndex = 0;
                }
                _ReSet = 1;
            }
            // 判断
            else
            {
                // 有大幅度的时间调整，将当天时间进行初始化
                if (abs((int8_t)TimeBef - _TimeNow) > 1)
                    _ReSet = 1;
            }

            if (_ReSet)
            {
                for (_i = 0; _i < sizeof(au8SmartTimeTmp); _i++)
                    au8SmartTimeTmp[_i] = 0;
                u16StableTime = 0;
            }
            TimeBef = _TimeNow;
        }
        
    }

}
/**********************************************
 * 单次即热功能判断
 * 返回值为1，说明在即热工作时间段内
 **********************************************/
uint8_t GetHeatAble_Basic(void)
{
    uint32_t _u32Tmp;
    uint8_t _u8Tmp;
    _u8Tmp = 0;
    if (GetCommSet()->u8FastHeat)
    {
        //单次即热在即热流程结束后自行关闭
        if (0 == GetCommSet()->u8PreWarmTime)
            _u8Tmp = 1;
        //判断保温时间内的停止条件
        else
        {
            _u32Tmp = (uint32_t)GetCommSet()->u8PreWarmTime * 6000;//u8PreWarmTime的单位是10min，换算成0.1s为6000
            // 工作时间未满足
            if (_u32Tmp > u32Heat_100ms)
                _u8Tmp = 1;
            // 工作时间已到
            else
            {
                //若在后循环阶段，则等待后循环结束再退出
                if (EN_PUMP_CYCLE == stPumpOutputs.enState)
                    _u8Tmp = 1;
                else
                    PumpChangeFastHeat(0);
            }
        }
    }
    //TODO:应该可以省略
    else
        u32Heat_100ms = 0;

    return _u8Tmp;
}

/**********************************************
 * 晨洗/夜浴/全天保温时间
 * 返回值为1，说明在即热工作时间段内
 **********************************************/
uint8_t GetHeatAble_BookFunc(void)
{
    uint16_t _u16Tmp;
    uint8_t _u8Tmp;
    _u8Tmp = 0;
    //预约功能启动
	if (0 < GetCommSet()->u8BookFuncSet)
	{
		if (BOOK_SET_DAY == GetCommSet()->u8BookFuncSet)//开启全天
		{
			_u8Tmp = 1;
		}
		else
		{
            // 将当前时间转换成30min为单位
			_u16Tmp = GetTime() ;
			if (0 < (BOOK_SET_MOR & GetCommSet()->u8BookFuncSet))//开启了晨洗
			{
				// _u16Temp = pstNomalOutputs_PM->u8HourCur * 60 + pstNomalOutputs_PM->u8MinCur;//将时间转换为min
				if ((GetCommSet()->u8MorWashStart <= _u16Tmp) && (GetCommSet()->u8MorWashEnd > _u16Tmp))
                    _u8Tmp = 1;
			}
			if (0 < (BOOK_SET_NIG & GetCommSet()->u8BookFuncSet))//开启了晨洗
			{
				// _u16Temp = pstNomalOutputs_PM->u8HourCur * 60 + pstNomalOutputs_PM->u8MinCur;//将时间转换为min
				if ((GetCommSet()->u8NigWashStart <= _u16Tmp) && (GetCommSet()->u8NigWashEnd > _u16Tmp))
					_u8Tmp = 1;
			}
		}
	}
    return _u8Tmp;
}


/**********************************************
 * 时间段预约功能
 * 返回值为1，说明在即热工作时间段内
 **********************************************/
uint8_t GetHeatAble_Book(void)
{
    uint8_t _u8Tmp,_u8Tmp2,_u8Tmp3;
    _u8Tmp = 0;
    // 获取在时间在哪一个数组（每4个小时的启动保存在一个数据）
    _u8Tmp2 = GetSystemRunData()->u8HourCur / 4;
	_u8Tmp2 = GetCommSet()->unBook24Set.Bytes[_u8Tmp2];
    // 计算当前时间段具体在哪一个位
	_u8Tmp3 = (GetSystemRunData()->u8HourCur % 4) * 2 + GetSystemRunData()->u8MinCur / 30;
    // 判断目标时间段内是否需要即热（位与的形式）
	if (0 < ((0x80 >> _u8Tmp3) & _u8Tmp2))
		_u8Tmp = 1;

    return _u8Tmp;
}

/**********************************************
 * 智学习
 * 返回值为1，说明在即热工作时间段内
 **********************************************/
uint8_t GetHeatAble_Smart(void)
{
    uint8_t _u8Time,_u8Tmp;
    uint8_t _i,_j,_z;
    _u8Time = (GetSystemRunData()->u8HourCur * 2) + GetSystemRunData()->u8MinCur / 30;
    _u8Tmp = 1;
    _i = _u8Time / 8;
	_j = _u8Time % 8;
    for (_z = 0;_z < ARR_NUM(au8SmartTime);_z++)
	{
		if (0 == (au8SmartTime[_z][_i] & (1 << _j)))
		{
			_u8Tmp = 0;
			break;
		}
	}
	return _u8Tmp;
}

/**********************************************
 * 判断是否在保温时间段内
 * 
 **********************************************/
void GetPumpHeatWarm(void)
{
    stPumpOutputs.bSign.bPumpWarm = 0;
    stPumpOutputs.bSign.bBookHeat = 0;

    // 关机状态，关闭即热，同时清除即热总时间
    if (0 == GetSystemRunData()->sysSta.BIT.bOnOff)
        PumpChangeFastHeat(0);
    else
    {
        if (GetHeatAble_Basic() || GetHeatAble_BookFunc() || GetHeatAble_Book())
            stPumpOutputs.bSign.bPumpWarm = 1;
        if (GetHeatAble_BookFunc() || GetHeatAble_Book())
            stPumpOutputs.bSign.bBookHeat = 1;
    }
}


/**********************************************
 * 水泵处于待机状态的处理
 * 
 * 
 **********************************************/
void PumpIdleDoingHandler(void)
{
    static uint32_t u32ForceWork_100ms = 0;//强制工作时间
    static uint16_t u16HeatUnWork_100ms = 0;
    static uint8_t u8BurstCnt_100ms = 0;//计算增压的启动时间
    static uint8_t u8BurstStable_100ms = 0;//燃烧稳定时间
    static uint8_t u8ColdCnt_100ms = 0;//防冻判定时间
    static uint8_t u8HeatTmpCnt_100ms = 0;//满足即热启动条件时间
    int16_t _s16Tmp;
    uint8_t _Init = 0;
    //存在故障，需要退出故障，并对部分状态进行初始化
    DEC(stPumpOutputs.u8PumpWorkCnt);
    if (!u8StateStep)
    {
        u8StateStep++;
        u32ForceWork_100ms = 0;
        u16HeatUnWork_100ms = 0;
        u8BurstCnt_100ms = 0;
        u8HeatTmpCnt_100ms = 0;
        u8ColdCnt_100ms = 0;
        u16HeatOnce_100ms = 0;
        _Init = 1;
    }
    else
    {
        INC_L(u32ForceWork_100ms);//强制工作时间累加
        INC_B(u8ColdCnt_100ms);//防冻时间增加
        
        // 非即热保温时间段内
        if (!stPumpOutputs.bSign.bPumpWarm)
            u16HeatUnWork_100ms = HEAT_INTERVAL_TIME;
        else
        {
            if (FSM_STATE_STABLE == GetFsmState())
                u16HeatUnWork_100ms = 0;
            else
                INC_W(u16HeatUnWork_100ms);
        }
        

        //检测系统是否正常
        if (GetSystemRunData()->sysSta.BIT.bError)
        {
            PumpChangeFastHeat(0);
            stPumpOutputs.bSign.bFastHeat = 0;
            u8BurstCnt_100ms = 0;//
            _Init = 1;
        }
        else
        {
            stPumpOutputs.bSign.bColdErr = 0;
            if (COLD_TMP <= GetSystemRunData()->TmpFD)
                u8ColdCnt_100ms = 0;
            //计算处回差温度
            _s16Tmp = GetSystemRunData()->TmpSet - GetSystemRunData()->TmpOut;
            if ((GetCommSet()->u8PreWarmDif * 10 >= _s16Tmp)
                && GetHeatTmpStop())
                u8HeatTmpCnt_100ms = 0;
            else
                INC_B(u8HeatTmpCnt_100ms);

            //满足水泵强制工作时间判断
            if ((GetFsmState() == FSM_STATE_IDLE) && (FORCE_UNWORK_TIME <= u32ForceWork_100ms))
            {
                PumpStateChange(EN_PUMP_FORCE);
            }
            //判断防冻处理
            //条件：1、没有水流量；2、低温时间超过设定时间；3、不在开关机上电保护时间内
            else if ((!GetSystemRunData()->sysSta.BIT.bWater) && (COLD_ENTER_TIME <= u8ColdCnt_100ms) && (!GetSystemRunData()->u8PowerProtect))
            {
                PumpStateChange(EN_PUMP_COLD);
            }
            // 剩下增压及预热判断
            else
            {
                if (GetSystemRunData()->sysSta.BIT.bWater)
                {
                    if (FSM_STATE_STABLE == GetFsmState())
                        INC_B(u8BurstStable_100ms);
                    else
                        u8BurstStable_100ms = 0;

                    if ((GetSystemRunData()->Flux < GetPumpBurstLite()) 
                        && (GetCommSet()->u8BurstMode) 
                        && (FSM_STATE_STABLE == GetFsmState())
                        && (!GetBurstTmpStop()))
                        INC_B(u8BurstCnt_100ms);
                    else
                        u8BurstCnt_100ms = 0;

                    if ((BURST_TIME <= u8BurstCnt_100ms) && (BURST_STABLE_TIME <= u8BurstStable_100ms))
                        PumpStateChange(EN_PUMP_BURST);
                }
                else
                {
                    u8BurstCnt_100ms = 0;
                    u8BurstStable_100ms = 0;

                    //即热判断部分
                    //在即热保温时间段内
                    if (stPumpOutputs.bSign.bPumpWarm)
                    {
                        if (((HEAT_INTERVAL_TIME <= u16HeatUnWork_100ms) && (5 < u8HeatTmpCnt_100ms))
                            || stPumpOutputs.bSign.bHeatFirst || (HEAT_FORCE_WORK_TIME < u16HeatUnWork_100ms))
                            {
                                stPumpOutputs.bSign.bHeatFirst = 0;
                                PumpStateChange(EN_PUMP_HEAT);
                            }
                    }
                }
            }
        }
    }

    
    if (_Init)
    {
        u8ColdCnt_100ms = 0;
        u8BurstCnt_100ms = 0;
        u8BurstStable_100ms = 0;
    }

    PumpE7_Function(0);
    // 水泵工作处理
    SetPumpWork(0);
}

/**********************************************
 * 水泵处于即热状态的处理
 * 
 * 
 **********************************************/
void PumpHeatDoingHandler(void)
{
    static uint8_t u8HeatLoadStop_100ms = 0;// 即热停止条件（温度），分钟
    static uint8_t u8HeatTmpStop_100ms = 0;// 即热停止条件（温度）
    uint16_t _u16Tmp;
    //存在故障，需要退出故障，并对部分状态进行初始化
    stPumpOutputs.u8PumpWorkCnt = PUMP_WORK_TIME;
    if (!u8StateStep)
    {
        u8StateStep++;
        u16HeatOnce_100ms = 0; 
        u8HeatLoadStop_100ms = 0;
        u8HeatTmpStop_100ms = 0;
    }
    else
    {
        INC_W(u16HeatOnce_100ms);
        //负荷停止条件
        if (GetPumpHeatLoadStop())
            INC_B(u8HeatLoadStop_100ms);
        else
            u8HeatLoadStop_100ms = 0;
        //温度停止条件
        if (GetHeatTmpStop())
            INC_B(u8HeatTmpStop_100ms);
        else
            u8HeatTmpStop_100ms = 0;

        // 出现故障，或者非即热时间段内，则退出即热状态
        if ((GetSystemRunData()->sysSta.BIT.bError) || (!stPumpOutputs.bSign.bPumpWarm))
            PumpStateChange(EN_PUMP_UNWORK);
        // 剩下的为根据实际工作情况停止工作
        else
        {
            // 计算单次即热的最长时间
            _u16Tmp = PumpHeatOnceTime();
            if (_u16Tmp == PREHEAT_UNSTUDY_DIS)
                _u16Tmp = HEAT_ONCE_MAX_TIME;
            else
                _u16Tmp *= 10; 

            // 若因为时间停止条件，则不需要进入水泵后循环阶段（主要考虑到该情况为用户用水，所以不会进入后循环）
            if ((_u16Tmp <= u16HeatOnce_100ms) || (HEAT_LOAD_STOP_TIME <= u8HeatLoadStop_100ms))
                PumpStateChange(EN_PUMP_UNWORK);
            else
            {
                // 自学习因温度到达而停止时
                if (5 <= u8HeatTmpStop_100ms)
                {
                    //如果属于智学习且未学习成功，或全循环。会进入后循环阶段
                    if (!GetCommSet()->u8CycleMode 
                    || ((1 == GetCommSet()->u8CycleMode) && (0 == u16HeatStudy_1s)))
                        PumpStateChange(EN_PUMP_CYCLE);//进入后循环
                    else
                        PumpStateChange(EN_PUMP_UNWORK);

                    // 循环模式ECO学习成功处理
                    if ((1 == GetCommSet()->u8CycleMode) && (0 == u16HeatStudy_1s))
                    {
                        // 满足最短学习时间
                        if (HEAT_STUDY_MIN <= u16HeatOnce_100ms)
                        {
                            _u16Tmp = (uint32_t)u16HeatOnce_100ms * 3 / 50;//学习时长为当次即热时长的3/5
                            u16HeatStudy_1s = GETMAX(_u16Tmp, HEAT_STUDY_MIN);
                        }
                    }
                }
            }
        }

        // TODO://是否在该位置判断退出即热功能
        if (!u8StateStep)
        {
            // 判断是否需要退出即热模式
            // 单次即热，且未进入后循环。此时退出即热功能
            if ((0 == GetCommSet()->u8PreWarmTime) && (EN_PUMP_CYCLE != stPumpOutputs.enState))
                PumpChangeFastHeat(0);
        }   
    }
    PumpE7_Function(1);
    SetPumpWork(1);
}

/**********************************************
 * 水泵处于后循环状态的处理
 * 
 * 
 **********************************************/
void PumpCycleDoingHandler(void)
{
    static uint16_t u16Cycle_100ms = 0;// 后循环工作时间
    static uint8_t u8CycleTmpStop_100ms = 0;
    //存在故障，需要退出故障，并对部分状态进行初始化
    stPumpOutputs.u8PumpWorkCnt = PUMP_WORK_TIME;
    if (!u8StateStep)
    {
        u8StateStep++;
        u16Cycle_100ms = PumpCycleTime(u16HeatOnce_100ms);
        u16HeatOnce_100ms = 0;
        u8CycleTmpStop_100ms = 0;
        stPumpOutputs.u8PumpForceWorkCnt = PUMP_WORK_TIME;
    }
    else
    {
        stPumpOutputs.u8PumpForceWorkCnt = PUMP_WORK_TIME;
        DEC(u16Cycle_100ms);

        if ((GetSystemRunData()->TmpSet - CYCLE_STOP_INTMP) >= GetSystemRunData()->TmpIn)
            INC_B(u8CycleTmpStop_100ms);
        else
            u8CycleTmpStop_100ms = 0;

        if (!stPumpOutputs.bSign.bPumpWarm)
            PumpStateChange(EN_PUMP_UNWORK);
        else if (u16Cycle_100ms)
        {
            if (CYCLE_TMP_TIME <= u8CycleTmpStop_100ms)
                PumpStateChange(EN_PUMP_UNWORK);
        }
        else
            PumpStateChange(EN_PUMP_UNWORK);

        if (0 == u8StateStep)
        {
            if (0 == GetCommSet()->u8PreWarmTime)
                PumpChangeFastHeat(0);
        }
    }
    PumpE7_Function(1);
    SetPumpWork(1);
}

/**********************************************
 * 水泵处于防冻状态的处理
 * 
 * 
 **********************************************/
void PumpColdDoingHandler(void)
{
    static uint16_t u16Cold_100ms = 0;// 后循环工作时间
    static uint8_t u8ColdTmpStop_100ms = 0;
    stPumpOutputs.u8PumpWorkCnt = PUMP_WORK_TIME;
    if (!u8StateStep)
    {
        u8StateStep++;
        u16Cold_100ms = 0;
        u8ColdTmpStop_100ms = 0;
    }
    else
    {
        INC_W(u16Cold_100ms);

        if ((GetSystemRunData()->TmpSet + COLD_STOP_OUTTMP) <= GetSystemRunData()->TmpOut)
            INC_B(u8ColdTmpStop_100ms);
        else
            u8ColdTmpStop_100ms = 0;

        if (GetSystemRunData()->sysSta.BIT.bError)
        {
            PumpStateChange(EN_PUMP_UNWORK);
            stPumpOutputs.bSign.bColdErr = 1;
        }
        else if (5 <= u8ColdTmpStop_100ms)
            PumpStateChange(EN_PUMP_UNWORK);
        else if (600 <= u16Cold_100ms)
            PumpStateChange(EN_PUMP_UNWORK);
    }
    PumpE7_Function(1);
    SetPumpWork(1);
}

/**********************************************
 * 水泵处于增压状态的处理
 * 
 * 
 **********************************************/
void PumpBurstDoingHandler(void)
{
    static uint32_t u32BurstFluSum = 0;// 用于计算平均流量
    static uint16_t u16BurstFluMax = 0;// 用于计算平均流量
    static uint16_t u16BurstFluMin = 0;// 用于计算平均流量
    static uint16_t u16BurstStopFlu;
    static uint8_t u8BurstTmpStop_100ms = 0;
    static uint8_t u8BurstFluStop_100ms = 0;
    static uint8_t u8BurstCnt_100ms;
    uint16_t _u16Tmp;
    uint8_t _u8Tmp;
    stPumpOutputs.u8PumpWorkCnt = PUMP_WORK_TIME;
    if (!u8StateStep)
    {
        u8StateStep++;
        u8BurstTmpStop_100ms = 0;
        u16BurstStopFlu = GetSystemRunData()->Flux;//获取当前的流量，作为停止流量的判定条件
        u8BurstCnt_100ms = 0;
    }
    else
    {
        // 获取最新的流量停止条件
        INC_B(u8BurstCnt_100ms);
        _u8Tmp = u8BurstCnt_100ms % 30;
        if (25 == _u8Tmp)
        {
            u32BurstFluSum = GetSystemRunData()->Flux;
            u16BurstFluMax = GetSystemRunData()->Flux;
            u16BurstFluMin = GetSystemRunData()->Flux;
        }
        else if (25 < _u8Tmp)
        {
            u32BurstFluSum += GetSystemRunData()->Flux;
            u16BurstFluMax = GETMAX(u16BurstFluMax, GetSystemRunData()->Flux);
            u16BurstFluMin = GETMIN(u16BurstFluMin, GetSystemRunData()->Flux);
        }
        else if (0 == _u8Tmp)
        {
            // 获取平均流量
            _u16Tmp = (u32BurstFluSum - u16BurstFluMax - u16BurstFluMin) / 3;
            u16BurstStopFlu = GETMAX(u16BurstStopFlu, _u16Tmp);
        }
        if (30 <= u8BurstCnt_100ms)
            u8BurstCnt_100ms = 0;

        // 增压停止条件判断
        if (GetBurstTmpStop())
            INC_B(u8BurstTmpStop_100ms);
        else
            u8BurstTmpStop_100ms = 0;
        
        if (u16BurstStopFlu >= GetSystemRunData()->Flux + 10)
            INC_B(u8BurstFluStop_100ms);
        else
            u8BurstFluStop_100ms = 0;

        // 退出燃烧状态，退出增压
        if ((FSM_STATE_STABLE != GetFsmState())
            || (BURST_STOP_FLU_TIME <= u8BurstFluStop_100ms)
            || (BURST_STOP_TMP_TIME <= u8BurstTmpStop_100ms)
            || (0 == GetCommSet()->u8BurstMode))
            PumpStateChange(EN_PUMP_UNWORK);
    }
    PumpE7_Function(0);
    SetPumpWork(1);
}

/**********************************************
 * 水泵处于后循环状态的处理
 * 
 * 
 **********************************************/
void PumpForceDoingHandler(void)
{
    static uint16_t u16Force_100ms = 0;// 后循环工作时间
    stPumpOutputs.u8PumpWorkCnt = PUMP_WORK_TIME;
    if (!u8StateStep)
    {
        u8StateStep++;
        stPumpOutputs.u8PumpForceWorkCnt = PUMP_WORK_TIME;
    }
    else
    {
        stPumpOutputs.u8PumpForceWorkCnt = PUMP_WORK_TIME;
        if (u16Force_100ms)
        {
            u16Force_100ms--;
            PumpStateChange(EN_PUMP_UNWORK);
        }
    }
    PumpE7_Function(0);
    SetPumpWork(1);
}

/**********************************************
 * 水泵计时（主要为全局变量）
 * 
 **********************************************/
void PumpTimer(void)
{
    DEC(stPumpOutputs.u8PumpForceWorkCnt);
    if (GetCommSet()->u8FastHeat)
        INC_L(u32Heat_100ms);
}

/**********************************************
 * Flash和通信信息同步（开关机掉电记忆）
 * 
 **********************************************/
void PumpReadFlashData(void)
{
    uint8_t _i;
    // 即热基本三参数（回差，保温时间，循环模式）
    GetCommSet()->u8CycleMode = GetFlashDataSector1()->pumpData.u8CycleMode;
    GetCommSet()->u8PreWarmTime = GetFlashDataSector1()->pumpData.u8PreWarmTime;
    GetCommSet()->u8PreWarmDif = GetFlashDataSector1()->pumpData.u8PreWarmDif;

    //晨洗、夜浴
    GetCommSet()->u8MorWashStart = GetFlashDataSector1()->pumpData.u8MorWashStart;
    GetCommSet()->u8MorWashEnd = GetFlashDataSector1()->pumpData.u8MorWashEnd;
    GetCommSet()->u8NigWashStart = GetFlashDataSector1()->pumpData.u8NigWashStart;
    GetCommSet()->u8NigWashEnd = GetFlashDataSector1()->pumpData.u8NigWashEnd;

    // 预约
    for (_i = 0; _i < 6; _i++)
        GetCommSet()->unBook24Set.Bytes[_i] = GetFlashDataSector1()->pumpData.au8Book24Set[_i];

    // 智学习
    GetCommSet()->u8WaterUseTime = GetFlashDataSector1()->pumpData.u8WaterUSedTime;
}

/**********************************************
 * 水泵主流程
 * 
 **********************************************/
void PumpFunction(void)
{
    static uint8_t u8SysInit_100ms = 30;
    static uint8_t u8RunCnt_10ms = 0;
    if (++u8RunCnt_10ms < 10)
        return;

    u8RunCnt_10ms = 0;
    DEC(u8SysInit_100ms);
    // 计时
    PumpTimer();
    // 通信数据同步
    PumpCommSycn();
    // 判断是否在即热时间段内
    GetPumpHeatWarm();

    if (!u8SysInit_100ms)
    {
        switch (stPumpOutputs.enState)
        {
        case EN_PUMP_UNWORK:
            PumpIdleDoingHandler();
            break;
        case EN_PUMP_HEAT:
            PumpHeatDoingHandler();
            break;
        case EN_PUMP_CYCLE:
            PumpCycleDoingHandler();
            break;
        case EN_PUMP_BURST:
            PumpBurstDoingHandler();
            break;
        case EN_PUMP_COLD:
            PumpColdDoingHandler();
            break;
        case EN_PUMP_FORCE:
            PumpForceDoingHandler();
            break;
        default:
            break;
        }
    }
    
    // 水控功能
    PumpWaterCtrl();
}



ST_PUMP_OUTPUTS_T *GetpPumpParm(void)
{
    return &stPumpOutputs;
}

/**********************************************
 * 加热时间倒计时
 * 通信输出使用
 **********************************************/
uint16_t GetPreHeatCD(void)	  // 加热倒计时
{
    uint16_t _u16Temp;
	if (0 == GetCommSet()->u8CycleMode)
	{
		_u16Temp = PREHEAT_UNSTUDY;
	}
	else if (1 == GetCommSet()->u8CycleMode)
	{
		if (0 == u16HeatStudy_1s)
		{
			_u16Temp = PREHEAT_UNSTUDY;
		}
		else
		{
			_u16Temp = u16HeatStudy_1s - (u16HeatOnce_100ms / PUMP_RUN_FREQ);
		}
	}
	else
	{
		_u16Temp = PumpHeatTime() - (u16HeatOnce_100ms / PUMP_RUN_FREQ);
	}
	return _u16Temp;
}
uint16_t GetPreHeatTime(void)
{
    return (u32Heat_100ms / 600);
}

uint8_t GetPreHeatStudy(void)
{
    uint8_t _u8Temp;
	if (0 == u16HeatStudy_1s)
		_u8Temp = 0xFF;
	else if (60 > u16HeatStudy_1s)
		_u8Temp = 1;
	else
		_u8Temp = u16HeatStudy_1s / 60;
	return _u8Temp;
}

void PumpFunctionInit(void)
{
	memset(&stPumpOutputs, 0, sizeof(stPumpOutputs));
	PumpReadFlashData();
}
