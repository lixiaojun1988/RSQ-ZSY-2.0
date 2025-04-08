

/*************************************************************************
 * Copyright:    wjl Kitchenware.
 * File:         FsmPowerOnState.c
 * Layer:
 * Author:       xjli
 * Version:
 * Brief:		初始化态
 *************************************************************************/
#include "all_head.h"

extern fsmVarTypedef fsmVar_t;
void FsmInitEnterHandler(void)
{
	SetFsmStage(FSM_STAGE_DOING);
	memset(&fsmVar_t, 0, sizeof(fsmVar_t));
    //重置E0，E6的判定时间
    GetSystemRunData()->u16E0Check_10ms = 0;
    GetSystemRunData()->u16E6Check_10ms = 0;
	GetSystemRunData()->u16E5Check_10ms = 0;
}

void FsmInitDoingHandler(void)
{
	uint8_t _u8Temp = 0;
	EN_ERRORCODE_T _enErr;
	// EN_ERRORCODE_T _enErrcode = EN_ERR_NO;
	ValveErrorReset();
    //阀体自检判断（E3）
	_u8Temp = ValveCheckFunction();
	if (0 < _u8Temp)
	{
		GetSystemRunData()->u8ErrorCode = EN_ERR_E3;
		SetFsmState(FSM_STATE_ERROR);
	}

	if (!GetSystemRunData()->bBurnAble || !GetSystemRunData()->sysSta.BIT.bWater)
		SetFsmState(FSM_STATE_IDLE);
	
	if (GetSystemRunData()->WkSw) // 温控故障
	{
		GetSystemRunData()->u8ErrorCode = EN_ERR_E3;
		SetFsmState(FSM_STATE_ERROR);
	}

	//探头故障报警
#if (TEMP_ERR_TAB_EN)
	_enErr = temper_err_get();
	if (EN_ERR_NO != _enErr)
	{
		GetSystemRunData()->u8ErrorCode = _enErr;
		SetFsmState(FSM_STATE_ERROR);
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

	//若状态仍是初始化，未进行其它更改，此时进行状态切换
	if (FSM_STATE_INIT == GetFsmState())
	{
		UpdateMaInfo(); // 机型识别
		SetFsmState(FSM_STATE_PREFAN);
	}
	
}
