/*************************************************************************
 * Copyright:    wjl Kitchenware.
 * File:
 * Layer:
 * Author:       xjli
 * Version:
 * Brief:		锟斤拷锟斤拷扫态
 *************************************************************************/
#include "all_head.h"

extern fsmVarTypedef fsmVar_t;
void FsmF6EnterHandler(void)
{
	memset(&fsmVar_t, 0, sizeof(fsmVar_t));
	SetFsmStage(FSM_STAGE_DOING);
}

void FsmF6DoingHandler(void)
{
	GetSystemRunData()->u16SetBlfI = SET_BLF_OFF;
	GetSystemRunData()->unSetValve.BYTE = 0;
	GetSystemRunData()->u16SetDCFanI = GetDCFanData()->u16FanCleanU;

	if ((0 == GetFunOutputs()->stSign.bCH4Warm) && (EN_CO_WARM != GetFunOutputs()->u8CoSat))
	{
		GetSystemRunData()->u8ErrorCode = EN_ERR_NO;
        GetSystemRunData()->u8ErrorCodePump = EN_ERR_NO;
	}

	if (EN_ERR_NO == GetSystemRunData()->u8ErrorCode)
	{
		//	GetSystemRunData()->u8ErrorCode = EN_ERR_E1;
		SetFsmState(FSM_STATE_ENDCLEAN);
	}

	//E0故障切换判断
	// E0
	if ((GetSystemRunData()->sysSta.BIT.bFlame) 
		&& ((EN_ERR_E6 != GetSystemRunData()->u8ErrorCode) || (EN_ERR_E6 != GetSystemRunData()->u8ErrorCodePump)))
		GetSystemRunData()->u16E0Check_10ms++;
	else
		GetSystemRunData()->u16E0Check_10ms = 0;
	if (GetSystemRunData()->u16E0Check_10ms >= 500)
	{
		GetSystemRunData()->u8ErrorCode = EN_ERR_E0;
		SetFsmState(FSM_STATE_E0);
	}
}
