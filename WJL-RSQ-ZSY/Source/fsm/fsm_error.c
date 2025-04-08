

/*************************************************************************
 * Copyright:    wjl Kitchenware.
 * File:
 * Layer:
 * Author:       xjli
 * Version:
 * Brief:		故障态
 *************************************************************************/
#include "all_head.h"
extern fsmVarTypedef fsmVar_t;
void FsmErrorEnterHandler(void)
{
    memset(&fsmVar_t, 0, sizeof(fsmVar_t));
    GetSystemRunData()->u16SetBlfI = SET_BLF_OFF;     // 关闭比例阀
    GetSystemRunData()->unSetValve.BYTE = 0;          // 关闭分段阀
    Drv_OutPut_Set(EN_OUTPORT_IGINTION, 0);           // 关闭点火
    SetFsmStage(FSM_STAGE_DOING);
	GetSystemRunData()->u8ErrorCodePump = GetSystemRunData()->u8ErrorCode;
}

void FsmErrorDoingHandler(void)
{

	//负载处理
	GetSystemRunData()->u16SetBlfI = SET_BLF_OFF;     // 关闭比例阀
	GetSystemRunData()->unSetValve.BYTE = 0;          // 关闭分段阀
	Drv_OutPut_Set(EN_OUTPORT_IGINTION, 0);           // 关闭点火
	//风机负载
	if (GetSystemRunData()->u16EndClean_10ms)
	{
		GetSystemRunData()->u16EndClean_10ms--;
		GetSystemRunData()->u16SetDCFanI = GetDCFanData()->u16FanCleanU;
	}
	else
		GetSystemRunData()->u16SetDCFanI = SET_DCFAN_OFF;

	//清除故障判断（区分洗浴故障和即热故障两种，两种故障不同的清除方法）
	//TODO:开关机指令清除故障代码（包括即热和洗浴）,既在user_systemRun.c中进行清除
	//清除即热故障
	// 水泵不工作一段时间后，仍有水流信号
	if ((GetSystemRunData()->sysSta.BIT.bWater) && (!GetpPumpParm()->u8PumpWorkCnt))
	{
		GetSystemRunData()->u8ErrorCodePump = EN_ERR_NO;
	}

	//清除洗浴故障
	//没有水流量时，清除清除故障
    if (!GetSystemRunData()->sysSta.BIT.bWater)
    {
        GetSystemRunData()->u8ErrorCode = EN_ERR_NO;
    }

	//所有故障清除后，返回到待机状态
	if ((EN_ERR_NO == GetSystemRunData()->u8ErrorCode) && (EN_ERR_NO == GetSystemRunData()->u8ErrorCodePump))
	{
		SetFsmState(FSM_STATE_IDLE);
	}

	//TODO:增加E6判断

	//E0故障切换判断
	// E0
	if ((GetSystemRunData()->sysSta.BIT.bFlame) 
		&& ((EN_ERR_E6 != GetSystemRunData()->u8ErrorCode) && (EN_ERR_E6 != GetSystemRunData()->u8ErrorCodePump)))
		GetSystemRunData()->u16E0Check_10ms++;
	else
		GetSystemRunData()->u16E0Check_10ms = 0;
	if (GetSystemRunData()->u16E0Check_10ms >= 500)
	{
		GetSystemRunData()->u8ErrorCode = EN_ERR_E0;
		SetFsmState(FSM_STATE_E0);
	}

	
}
