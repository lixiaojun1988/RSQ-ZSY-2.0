

/*************************************************************************
 * Copyright:    wjl Kitchenware.
 * File:         FsmPowerOnState.c
 * Layer:
 * Author:       xjli
 * Version:
 * Brief:	   閿熻緝纰夋嫹閿熺粸缁э拷
 *************************************************************************/
#include "all_head.h"
fsmVarTypedef fsmVar_t;
void FsmIdleEnterHandler(void)
{
	SetFsmStage(FSM_STAGE_DOING);
	memset(&fsmVar_t, 0, sizeof(fsmVar_t));
	GetSystemRunData()->u16SetBlfI = SET_BLF_OFF;	  // 关闭比例阀
	GetSystemRunData()->unSetValve.BYTE = 0;		  // 关闭分段阀
	Drv_OutPut_Set(EN_OUTPORT_IGINTION, 0);			  // 关闭点火
}

void FsmIdleEnterDoingHandler(void) // 10MS
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

    //TODO:需要更改成有燃烧请求（零冷水机型在待机状态，防冻同样会触发工作）
	if ( GetSystemRunData()->bBurnAble && GetSystemRunData()->sysSta.BIT.bWater) // 燃烧状态切换
	{
		if (++fsmVar_t.var1 >= 1)
		{
			SetFsmState(FSM_STATE_INIT);
		}
	}
	else
	{
		fsmVar_t.var1 = 0;
	}

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
