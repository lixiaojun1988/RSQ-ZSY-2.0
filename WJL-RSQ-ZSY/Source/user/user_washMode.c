/*************************************************************************
 * Copyright:    wjl Kitchenware.
 * File:
 * Author:       xjli
 * Version:      v1.0
 * Brief:
 * Date:
 *************************************************************************/

#include "all_head.h"

ST_WASHMODE_OUTPUTS_T WashMode_t;

ST_WASHMODE_OUTPUTS_T *GetWashModeData(void)
{
    return &WashMode_t;
}

// 厨房模式
static uint32_t u32KitchenCnt; // 厨房功能计时
static uint8_t u8KitTime;      // 现在厨房时间
// 浴缸
static uint32_t u32WaterBath; // 浴缸模式用水计算
// static	uint16_t	u16BathLite;//浴缸模式已用水量
//  厨房模式时间
static const uint32_t au32KitchenTime[] =
    {
        // 30min,60min,120min，180min
        18000, 36000, 72000, 108000};
/**********************************************
 *刷新浴缸记录水量
 **********************************************/
void BathRecordReset(void)
{
    WashMode_t.u16BathLite = 0;
    //  u32WaterBath = pstFun_WM->u32WaterExpend;
}
void BathInit(void)
{
    BathRecordReset();
    WashMode_t.stWashSign.bBathFin = 0;
}
/**********************************************
模式判断
 **********************************************/
void WashModeJdg(void)
{
    static uint8_t modeLast = COMMSET_NULL;
    static uint8_t tempLast = COMMSET_NULL;
    static uint8_t fluxLast = COMMSET_NULL;
    static uint8_t waterLast = COMMSET_NULL;
    if ((modeLast != GetCommSet()->u8ModeSet) || (tempLast != GetCommSet()->u8TmpSet)
        || (fluxLast != GetCommSet()->u8FluxSet) || (waterLast != GetCommSet()->u8WaterSet))
    {
		if (EN_WASH_BATH != GetCommSet()->u8ModeSet)
		{
			BathInit();
		}
		if (EN_WASH_KITCHEN != GetCommSet()->u8ModeSet)
		{
			u8KitTime = GetCommSet()->u8KitTimeSet;
			u32KitchenCnt = au32KitchenTime[u8KitTime];
		}
		//TODO:考虑是否增加额外标志位给操作器，用于接收到新的浴缸模式指令清除浴缸完成标志位
		if ((EN_WASH_BATH == GetCommSet()->u8ModeSet) && WashMode_t.stWashSign.bBathFin)
		{
			BathInit();
		}
		//厨房模式不触掉电记忆保存
		if (EN_WASH_KITCHEN != GetCommSet()->u8ModeSet)
		{
			GetFlashDataSector1()->basicFlashData.u8ModeSet = GetCommSet()->u8ModeSet;
			GetFlashDataSector1()->basicFlashData.u8TmpSet = GetCommSet()->u8TmpSet;
			GetFlashDataSector1()->basicFlashData.u8FluxSet = GetCommSet()->u8FluxSet;
			GetFlashDataSector1()->basicFlashData.u8WaterSet = GetCommSet()->u8WaterSet;
			UserWrtieSetCD(500);
        }
        modeLast = GetCommSet()->u8ModeSet;
        tempLast = GetCommSet()->u8TmpSet;
        fluxLast = GetCommSet()->u8FluxSet;
        waterLast = GetCommSet()->u8WaterSet;
    }

    // 如果厨房工作时间改变重置时间
    if (u8KitTime != GetCommSet()->u8KitTimeSet)
    {
        u8KitTime = GetCommSet()->u8KitTimeSet;
        u32KitchenCnt = au32KitchenTime[u8KitTime];
    }
}

/**********************************************
智温感功能
 **********************************************/
void SetWashEnvState(void)
{
	static uint8_t u8TmpRoomSta_10ms = 0;
	uint16_t _u16Temp;
	int16_t _s16Temp;
	uint8_t _u8Temp, _u8Temp2;

	_u8Temp = temper_ntc_error_get(TMP_ROOM);
#if (Env_InTemp)
	_u8Temp2 = temper_ntc_error_get(TMP_IN);
#else
	_u8Temp2 = 1;//当做故障既不使用该探头数据
#endif
	if (++u8TmpRoomSta_10ms >= 50)
	{
		u8TmpRoomSta_10ms = 50;
		WashMode_t.stWashSign.bEnvErr = 1;
	}
	else
	{
		u8TmpRoomSta_10ms = 0;
		WashMode_t.stWashSign.bEnvErr = 0;
	}
		

	if (0 == WashMode_t.stWashSign.bEnvErr)
	{
		if ((!_u8Temp) || (!_u8Temp2))
		{
			u8TmpRoomSta_10ms = 0;
		}
		// 没有水才能更改只能防护温度,且探头正常才更改环境温度
		if ((0 == GetSystemRunData()->sysSta.BIT.bWater) && (!u8TmpRoomSta_10ms))
		{
			//优先判断室温探头，再判断进行探头
			if (0 == _u8Temp)
			{
				_s16Temp = GetSystemRunData()->TmpRoom;
				_u16Temp = _s16Temp / 100;//除10℃
				
			}
			else if(0 == _u8Temp2)
			{
				_s16Temp = GetSystemRunData()->TmpIn;
				_u16Temp = _s16Temp / 100;
			}

		    // 最大值为3
			if (EN_ENV_TMP_3 < _u16Temp)
			{
				_u16Temp = EN_ENV_TMP_3;
			}
			// 判断该值需要有没有可能需要改变
			if (_u16Temp > WashMode_t.u8EnvSta)
			{
				if (_s16Temp >= ((WashMode_t.u8EnvSta + 1) * 100 + 10))
				{
					WashMode_t.u8EnvSta = _u16Temp;
				}
			}
			else if (_u16Temp < WashMode_t.u8EnvSta)
			{
			    if (_s16Temp <= (WashMode_t.u8EnvSta * 100 - 10))
				{
					WashMode_t.u8EnvSta = _u16Temp;
				}
			}
		}
	}
}

/**********************************************
厨房模式
 **********************************************/
void KitFunction(void)
{
    if (EN_WASH_KITCHEN == GetCommSet()->u8ModeSet)
    {
        // 处于燃烧阶段或者厨房时间未结束，此时不退出厨房功能
        if ((0 < u32KitchenCnt) || (FSM_STATE_STABLE == GetFsmState()))
        {
#if (0 == UNITTEST)
            DEC(u32KitchenCnt);
#endif
        }
        else
        {
            // 恢复上一个模式的状态
            GetCommSet()->u8Priority = PRI_NONE;
            GetCommSet()->u8ModeSet = GetFlashDataSector1()->basicFlashData.u8ModeSet;
            GetCommSet()->u8TmpSet = GetFlashDataSector1()->basicFlashData.u8TmpSet;
            GetCommSet()->u8FluxSet = GetFlashDataSector1()->basicFlashData.u8FluxSet;
            GetCommSet()->u8WaterSet = GetFlashDataSector1()->basicFlashData.u8WaterSet;
        }
    }
}

void u16BathLiteTick(void) // 1MS
{
    if (WashMode_t.u16BathTick < 0xffff)
        WashMode_t.u16BathTick++;
}

/**********************************************
 *浴缸模式
 **********************************************/
void BathFunction(void) // 10MS
{
    uint16_t u16temp;
    // 确定是浴缸模式
    if ((EN_WASH_BATH == GetCommSet()->u8ModeSet) && (FSM_STATE_STABLE == GetFsmState()) && ((EN_PUMP_UNWORK == GetpPumpParm()->enState) || (EN_PUMP_BURST == GetpPumpParm()->enState)))
    {
        // 未放完水
        if (0 == WashMode_t.stWashSign.bBathFin)
        {
            if (0 < RecordLiterFunction(&u32WaterBath, EN_REC_WATER))
            {
                INC_W(WashMode_t.u16BathLite);
            }
            u16temp = GetCommSet()->u8WaterSet * 10;
            if (u16temp <= WashMode_t.u16BathLite)
            {
                WashMode_t.stWashSign.bBathFin = 1;
                BathRecordReset();
                GetCommSet()->u8Priority = PRI_NONE;
            }
        }
        else
        {
            BathRecordReset();
        }
    }
    else
    {
        BathRecordReset();
        WashMode_t.stWashSign.bBathFin = 0;
    }

    u16temp = WashMode_t.u8WaterSet * 10;
    if (u16temp <= WashMode_t.u16BathLite)
    {
        u16temp = 0;
    }
    else
    {
        u16temp -= WashMode_t.u16BathLite;
    }
    WashMode_t.u16BathRemainLite = u16temp;
}

void WashModeFunction(void) // 10MS
{
    // 模式判断接收
    WashModeJdg();
    // 智温感温度判断
    SetWashEnvState();
    // 厨房模式
    KitFunction();
    // 浴缸模式
    BathFunction();
}

void WashModeInit(void)
{
    memset(&WashMode_t, 0, sizeof(WashMode_t));
    if (EN_WASH_BATH != GetCommSet()->u8ModeSet)
    {
        GetCommSet()->u8WaterSet = BATH_WATER_DEF;
    }
    WashMode_t.u8EnvSta = 1;
    WashMode_t.stWashSign.bEnvErr = 1;
    BathInit();
}

#if (CommReSetBathLite)
/**********************************************
 *浴缸模式
 **********************************************/
void ReSetBathLite(void)
{
	//用于接收到新的浴缸模式指令清除浴缸完成标志位
	if ((EN_WASH_BATH == GetCommSet()->u8ModeSet) && WashMode_t.stWashSign.bBathFin)
	{
		BathInit();
	}
}
#endif

