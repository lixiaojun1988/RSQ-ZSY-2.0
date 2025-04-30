
/*************************************************************************
 * Copyright:    wjl Kitchenware.
 * File:         FsmPowerOnState.c
 * Layer:
 * Author:       xjli
 * Version:
 * Brief:		稳定燃烧态
 *************************************************************************/
#include "all_head.h"

#define LOST_FLAME_ERR_CNT 5
extern fsmVarTypedef fsmVar_t;
void FsmStableEnterHandler(void)
{
	memset(&fsmVar_t, 0, sizeof(fsmVar_t));
	GetSystemRunData()->u16HeatLast_1s = 0;
	TmpCtrlVarReset();
	SegCtrlInit();
	SetFsmStage(FSM_STAGE_DOING);
}

void FsmStableDoingHandler(void) // 10MS
{
	uint16_t _u16RunLimitTime_1s;
	EN_ERRORCODE_T _enErr;
    //配置恒温
	TmpCtrlProcess();
	SetDcFanUfromBlf(GetSystemRunData()->u16SetBlfI);

	//TODO:是否需要将GetSystemRunData()->u16SetBlfI
	//处于换挡
	if (GetSegCtrl()->u8ChgStep)
		GetSystemRunData()->u16SetBlfI = GETMAX(GetSystemRunData()->u16SetBlfI, GetSegCtrl()->u16SegSwitchBlfI);

	
	// E1火焰信号丢失
	if (0 == GetSystemRunData()->sysSta.BIT.bFlame)
	{
		INC_B(fsmVar_t.var1);
		if (LOST_FLAME_ERR_CNT <= fsmVar_t.var1)
		{
			GetSystemRunData()->u8ErrorCode = EN_ERR_E1;
			SetFsmState(FSM_STATE_ENDCLEAN);
		}
	}
	else
	{
		fsmVar_t.var1 = 0;
	}

	// 非故障类型的状态切换
	if (0 == GetSystemRunData()->sysSta.BIT.bWater || 0 == GetSystemRunData()->bBurnAble)
	{
		SetFsmState(FSM_STATE_ENDCLEAN);
	}
	else if (EN_MAIN_STATE_DEBUG == GetSystemRunData()->sysSta.BIT.bMainSta)
	{
		SetFsmState(FSM_STATE_DEBUG);
	}

	//故障类型状态切换
	//E3故障（2种）
	if ((GetSystemRunData()->WkSw) || (GetValveError().BYTE)) // 温控故障
	{
		GetSystemRunData()->u8ErrorCode = EN_ERR_E3;
		SetFsmState(FSM_STATE_ENDCLEAN);
		ReSetPercent();//燃烧过程出现E3，则重置偏差
	}

	//E2故障
	if (1 == GetSystemRunData()->u8FanRunning)
		fsmVar_t.var2 = 0;
	if (++fsmVar_t.var2 >= 50)
	{
		GetSystemRunData()->u8ErrorCode = EN_ERR_E2;
		GetSystemRunData()->u16SetDCFanI = SET_DCFAN_OFF;
		SetFsmState(FSM_STATE_ENDCLEAN); // 2024
	}

	//E8故障
	if (GetSystemRunData()->u16FanSpeed < GetDCFanData()->u16FanSpdErr)
		fsmVar_t.var3 = 0;

	if (++fsmVar_t.var3 >= 500)
	{
		GetSystemRunData()->u8ErrorCode = EN_ERR_E8;
		SetFsmState(FSM_STATE_ENDCLEAN);
	}

	//持续燃烧时间计时
	if (++fsmVar_t.var4 >= 100) // 1S
	{
		fsmVar_t.var4 = 0;
		GetSystemRunData()->u16HeatLast_1s++;
		GetSystemRunData()->u16HeatLast_1s = GETMIN(GetSystemRunData()->u16HeatLast_1s, 15300);
	}
	//En故障
	if (1 == GetFlashDataSector0()->debugData.u8Ne)
	{
		_u16RunLimitTime_1s = 3000;
	}
	else
	{
		_u16RunLimitTime_1s = 0xffff;
	}
	if (GetSystemRunData()->u16HeatLast_1s > _u16RunLimitTime_1s)
	{
		GetSystemRunData()->u8ErrorCode = EN_ERR_En;
		SetFsmState(FSM_STATE_ENDCLEAN);
	}

	//E5故障
	if (850 <= GetSystemRunData()->TmpOut)
		GetSystemRunData()->u16E5Check_10ms++;
	else
		GetSystemRunData()->u16E5Check_10ms = 0;

	if (300 < GetSystemRunData()->u16E5Check_10ms)
	{
		GetSystemRunData()->u8ErrorCode = EN_ERR_E5;
		SetFsmState(FSM_STATE_ENDCLEAN);
	}



#if (TEMP_ERR_TAB_EN)
	_enErr = temper_err_get();
	if (EN_ERR_NO != _enErr)
	{
		GetSystemRunData()->u8ErrorCode = _enErr;
		SetFsmState(FSM_STATE_ENDCLEAN);
	}
#else
	_enErr = EN_ERR_NO;
	if (temper_ntc_error_get(TMP_OUT))
		_enErr = EN_ERR_E4;
	if (temper_ntc_error_get(TMP_IN))
		_enErr = EN_ERR_F3;
	if (temper_ntc_error_get(TMP_FDTX))
		_enErr = EN_ERR_F5;

	if (EN_ERR_NO == _enErr)
	{
		GetSystemRunData()->u8ErrorCode = _enErr;
		SetFsmState(FSM_STATE_ENDCLEAN);
	}
#endif

	//退出稳定燃烧的同时，将时间重置
	if (FSM_STATE_STABLE != GetFsmState())
	{
		GetSystemRunData()->u16HeatLast_1s = 0;
	}
}
