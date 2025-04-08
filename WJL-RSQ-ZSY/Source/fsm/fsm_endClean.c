    /*************************************************************************
 * Copyright:    wjl Kitchenware.
 * File:
 * Layer:
 * Author:       xjli
 * Version:
 * Brief:		閿熸枻鎷烽敓鏂ゆ嫹锟�?锟�?
 *************************************************************************/
#include "all_head.h"

extern fsmVarTypedef fsmVar_t;
// static uint8_t tapClosed; /* 閺冪姵鎸夊ù浣烘樊閹镐浇顓搁弫锟�? */
void FsmEndCleanEnterHandler(void)
{
	memset(&fsmVar_t, 0, sizeof(fsmVar_t));
	SetFsmStage(FSM_STAGE_DOING);
	// tapClosed=0;
}

void FsmEndCleanDoingHandler(void)
{
	GetSystemRunData()->u16SetBlfI = SET_BLF_OFF;
	GetSystemRunData()->unSetValve.BYTE = 0;
	GetSystemRunData()->u16SetDCFanI = GetDCFanData()->u16FanCleanU;

	//配置风机后清扫时间
	SetEndCleanTimer();

	//根据状态进行相应的切换
	if (GetSystemRunData()->u8ErrorCode)
		SetFsmState(FSM_STATE_ERROR);
	else
		SetFsmState(FSM_STATE_IDLE);
	

	// //TODO:需要根据热水器的工作类型，修改后清扫时间（带nH强制代码，0:30s,1:60s；若在即热保温时间范围内，后清扫时间为3s）
	// //根据强制参数，确认基本的后清扫时间
	// /* 退出 */
	// if (++fsmVar_t.var1 > _u16temp)
	// {
	// 	SetFsmState(FSM_STATE_IDLE);
	// 	if(GetSystemRunData()->sysSta.BIT.bWater)
	// 		SetFsmState(FSM_STATE_ERROR);
	// }
	// else
	// {
	// 	/*  */
	// 	if (GetSystemRunData()->sysSta.BIT.bWater)
	// 	{
	// 		fsmVar_t.var2 = 0;
	// 		if (fsmVar_t.var3 < 255)
	// 			fsmVar_t.var3++;
	// 		/* 娴犲孩妫わ拷?锟斤拷?锟藉Ц锟�?浣稿綁娑撶儤婀侊拷?锟斤拷?锟藉Ц锟�?渚婄礉楠炴湹绗栭張澶嬫寜閻樿埖锟�?浣瑰瘮缂侊拷0.5缁夋帊浜掓稉锟�? */
	// 		if (tapClosed && fsmVar_t.var3 > 50)
	// 		{
	// 			SetFsmState(FSM_STATE_IDLE);
	// 		}
	// 	}
	// 	/* 閹镐胶锟�?0.5缁夋帗妫わ拷?锟斤拷?锟界箻閸氬孩绔婚幍顐︽▉濞堬拷 */
	// 	if (!GetSystemRunData()->sysSta.BIT.bWater)
	// 	{
	// 		fsmVar_t.var3 = 0;

	// 		if (++fsmVar_t.var2 > 50)
	// 		{
	// 			tapClosed = 1;
	// 		}
	// 	}
	// }

	// /* E0鏁呴殰 */
	// if (GetSystemRunData()->sysSta.BIT.bFlame)
	// 	fsmVar_t.var2++;
	// else
	// 	fsmVar_t.var2 = 0;
	// if (fsmVar_t.var2 >= 500)
	// {
	// 	GetSystemRunData()->u8ErrorCode = EN_ERR_E0;
	// 	SetFsmState(FSM_STATE_ERROR);
	// }
}
