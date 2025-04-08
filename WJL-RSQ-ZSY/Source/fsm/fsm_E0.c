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
void FsmE0EnterHandler(void)
{
	memset(&fsmVar_t, 0, sizeof(fsmVar_t));
	GetSystemRunData()->u16SetBlfI = SET_BLF_OFF;     // 关闭比例阀
	GetSystemRunData()->unSetValve.BYTE = 0;          // 关闭分段阀
	Drv_OutPut_Set(EN_OUTPORT_IGINTION, 0);           // 关闭点火
	SetFsmStage(FSM_STAGE_DOING);
}

void FsmE0DoingHandler(void)
{
	// E0
	// 有火焰时时候风机关闭
	if (GetSystemRunData()->sysSta.BIT.bFlame)
		GetSystemRunData()->u16SetDCFanI = SET_DCFAN_OFF;
	// 无火焰时，风机启动
	else
		GetSystemRunData()->u16SetDCFanI = GetDCFanData()->u16FanCleanU;
}
