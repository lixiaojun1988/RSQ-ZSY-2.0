/*************************************************************************
 * Copyright:    wjl Kitchenware.
 * File:
 * Layer:
 * Author:       xjli
 * Version:
 * Brief:	     
 *************************************************************************/
#include "all_head.h"
#define RE_INIG_CNT 1
extern fsmVarTypedef fsmVar_t;
void FsmIgintionEnterHandler(void)
{
	SetFsmStage(FSM_STAGE_DOING);
	memset(&fsmVar_t, 0, sizeof(fsmVar_t));
	GetSystemRunData()->u8ReIgniTimes = RE_INIG_CNT;//配置点火次数
}

	uint16_t setBlfI;//风机电流
void FsmIgintionDoingHandler(void) // 10MS
{
	static uint8_t u8_FlameTime_100ms;
	EN_ERRORCODE_T _enErr;
	uint8_t u8CurDh;
#if (Adpt_DH)
	u8CurDh = GetFlashDataSector0()->debugData.u8Dh;
	if ((GetFlashDataSector1()->basicFlashData.u8RepairDh) || (0 == GetSystemRunData()->u8ReIgniTimes))
	{
		u8CurDh = u8CurDh + 5;
	}
#else
	u8CurDh = GetFlashDataSector0()->debugData.u8Dh;
#endif
	//	EN_ERRORCODE_T _enErrcode = EN_ERR_NO;
	// 0 ~ 0.4s 比例阀强吸时间
	if (++fsmVar_t.var1 <= 40)
	{
		Drv_OutPut_Set(EN_OUTPORT_IGINTION, 1);
		GetSystemRunData()->unSetValve.BYTE = 0;
		GetSystemRunData()->u16SetBlfI = ConvertDbgToCtrl_Blf(0xff); // 风机按照点火时的风速进行工作

		setBlfI = ConvertDbgToCtrl_Blf(u8CurDh);
		GetSystemRunData()->u16E6Check_10ms = 0;
	}
	//0.4~0.5s比例阀结束，回到目标开度
	else if (50 >= fsmVar_t.var1)
	{
		Drv_OutPut_Set(EN_OUTPORT_IGINTION, 1);
		GetSystemRunData()->unSetValve.BYTE = 0;
		GetSystemRunData()->u16SetBlfI = ConvertDbgToCtrl_Blf(u8CurDh); //
		setBlfI = GetSystemRunData()->u16SetBlfI;
		GetSystemRunData()->u16E6Check_10ms = 0;
	}
	//启动二次点火
	else if (fsmVar_t.var1 >= 1100)
	{
		//减少点火次数
		DEC(GetSystemRunData()->u8ReIgniTimes);
		//重置时间
		fsmVar_t.var1 = 0;
		Drv_OutPut_Set(EN_OUTPORT_IGINTION, 1);
		GetSystemRunData()->unSetValve.BYTE = 0;
		GetSystemRunData()->u16SetBlfI = ConvertDbgToCtrl_Blf(0xff); // 风机按照点火时的风速进行工作
		setBlfI = ConvertDbgToCtrl_Blf(u8CurDh);
		GetSystemRunData()->u16E6Check_10ms = 0;
	}
	else if (fsmVar_t.var1 >= 800)
	{
		// 若没机会点火，则进入后清扫
		if (0 == GetSystemRunData()->u8ReIgniTimes)
		{
			GetSystemRunData()->u8ErrorCode = EN_ERR_E1; 
			SetFsmState(FSM_STATE_ENDCLEAN);			
		}
		GetSystemRunData()->u16SetBlfI = SET_BLF_OFF; // 关闭比例阀
		GetSystemRunData()->unSetValve.BYTE = 0;	  // 关闭电磁阀
		Drv_OutPut_Set(EN_OUTPORT_IGINTION, 0);		  // 关闭点火
		setBlfI = ConvertDbgToCtrl_Blf(GetFlashDataSector0()->debugData.u8Ph);
	}
	else
	{ // 2~8s
		Drv_OutPut_Set(EN_OUTPORT_IGINTION, 1);
		GetSystemRunData()->unSetValve.BYTE = GetWorkCon()->aunSubSegSet[GetWorkCon()->u8FireSegSet].BYTE;
		GetSystemRunData()->u16SetBlfI = ConvertDbgToCtrl_Blf(u8CurDh);
		if (30 < u8_FlameTime_100ms)
		{
			u8_FlameTime_100ms = 0;
			Drv_OutPut_Set(EN_OUTPORT_IGINTION, 0);
			//根据当前状态，确认去强制模式还是正常工作模式
			if (EN_MAIN_STATE_DEBUG == GetSystemRunData()->sysSta.BIT.bMainSta)
				SetFsmState(FSM_STATE_DEBUG);
			else
				SetFsmState(FSM_STATE_STABLE);
		}
		setBlfI = GetSystemRunData()->u16SetBlfI;
	}

	SetDcFanUfromBlf(setBlfI);//控制风机工作（带堵塞加风）

	if (0 < GetSystemRunData()->sysSta.BIT.bFlame)
	{
		INC_B(u8_FlameTime_100ms);
		GetSystemRunData()->u16E6Check_10ms++;
	}
	else
	{
		u8_FlameTime_100ms = 0;
		GetSystemRunData()->u16E6Check_10ms = 0;
	}
	
	//E6故障判定（两次点火间间隔）
	if (GetSystemRunData()->u16E6Check_10ms >= 100)
	{
		GetSystemRunData()->u8ErrorCode = EN_ERR_E6;
		SetFsmState(FSM_STATE_ENDCLEAN);
	}

	
	//TODO:开关要更改成
	if (!GetSystemRunData()->bBurnAble || !GetSystemRunData()->sysSta.BIT.bWater)
		SetFsmState(FSM_STATE_ENDCLEAN);

	//E3故障
	if ((GetSystemRunData()->WkSw) || (GetValveError().BYTE)) // 温控故障
	{
		GetSystemRunData()->u8ErrorCode = EN_ERR_E3;
		SetFsmState(FSM_STATE_ENDCLEAN);
	}

	//E2故障报警
	if (0 < GetSystemRunData()->u8FanRunning)
	{
		fsmVar_t.var3 = 0;
	}
	if (++fsmVar_t.var3 >= 100)
	{
		GetSystemRunData()->u16SetDCFanI = SET_DCFAN_OFF;
		GetSystemRunData()->u8ErrorCode = EN_ERR_E2;
		SetFsmState(FSM_STATE_ENDCLEAN); // 2024
	}

	//E8故障
	if (GetSystemRunData()->u16FanSpeed < GetDCFanData()->u16FanSpdErr)
		fsmVar_t.var2 = 0;

	if (++fsmVar_t.var2 >= 50)
	{
		GetSystemRunData()->u8ErrorCode = EN_ERR_E8;
		SetFsmState(FSM_STATE_ENDCLEAN);
	}

	//探头故障报警
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


}
