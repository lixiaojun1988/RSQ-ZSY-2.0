/*************************************************************************
 * Copyright:    wjl Kitchenware.
 * File:
 * Layer:
 * Author:       xjli
 * Version:
 * Brief:		
 *************************************************************************/
#include "all_head.h"

extern fsmVarTypedef fsmVar_t;
void FsmDebugEnterHandler(void)
{
	memset(&fsmVar_t, 0, sizeof(fsmVar_t));
	SetFsmStage(FSM_STAGE_DOING);
    SegCtrlInit();
}

void FsmDebugDoingHandler(void)
{
	const ST_DBG_INFO_T *_stDbgInfo;
	static	uint8_t _u8DdValve = 0;
	uint16_t _u16temp;
	EN_ERRORCODE_T _enErr;
	TmpCtrlPrepare();
	//获取当前引索值的相关信息
	_stDbgInfo = GetDbgInfo(GetCommSet()->u8DebugIndex);
	//PH,FH,HC状态：比例阀开度PH，风机转速FH，分段阀最大
	if (((EN_CODE_P == _stDbgInfo->u8CodeH) && (EN_CODE_H == _stDbgInfo->u8CodeL)) 
		|| ((EN_CODE_F == _stDbgInfo->u8CodeH) && (EN_CODE_H == _stDbgInfo->u8CodeL)) 
		|| ((EN_CODE_H == _stDbgInfo->u8CodeH) && (EN_CODE_C == _stDbgInfo->u8CodeL)))
	{
		SetManualSeg(GetWorkCon()->u8MaxSeg);
		GetSystemRunData()->u16SetBlfI = ConvertDbgToCtrl_Blf(GetFlashDataSector0()->debugData.u8Ph);
		GetSystemRunData()->u16SetDCFanI = ConvertDbgToCtrl_DCFan(GetFlashDataSector0()->debugData.u8Fh);
	}
	//SP,SL状态：比例阀开度PH+SP，风机转速FH+SL，分段阀最大
	else if (((EN_CODE_5 == _stDbgInfo->u8CodeH) && (EN_CODE_P == _stDbgInfo->u8CodeL)) 
		|| ((EN_CODE_5 == _stDbgInfo->u8CodeH) && (EN_CODE_L == _stDbgInfo->u8CodeL)))
	{
		SetManualSeg(GetWorkCon()->u8MaxSeg);
        //
		_u16temp = (uint16_t)GetFlashDataSector0()->debugData.u8Ph + GetFlashDataSector0()->debugData.u8Sp;
		_u16temp = GETMIN(_u16temp, 0xFF);
		GetSystemRunData()->u16SetBlfI = ConvertDbgToCtrl_Blf(_u16temp);

		_u16temp = (uint16_t)GetFlashDataSector0()->debugData.u8Fh + GetFlashDataSector0()->debugData.u8Sl;
		_u16temp = GETMIN(_u16temp, 0xFF);
		GetSystemRunData()->u16SetDCFanI = ConvertDbgToCtrl_DCFan(_u16temp);

		// GetSystemRunData()->u16SetBlfI = GetSystemRunData()->u16BlfIRun_Max;
		// GetSystemRunData()->u16SetDCFanI = GetDCFanData()->u16FanRunMax;
	}
	//PL,SL,LC状态：比例阀开度PL，风机转速FL，分段阀最大
	else if (((EN_CODE_P == _stDbgInfo->u8CodeH) && (EN_CODE_L == _stDbgInfo->u8CodeL)) 
		|| ((EN_CODE_F == _stDbgInfo->u8CodeH) && (EN_CODE_L == _stDbgInfo->u8CodeL)) 
		|| ((EN_CODE_L == _stDbgInfo->u8CodeH) && (EN_CODE_C == _stDbgInfo->u8CodeL)))
	{
		SetManualSeg(GetWorkCon()->u8MaxSeg);
		GetSystemRunData()->u16SetBlfI = ConvertDbgToCtrl_Blf(GetFlashDataSector0()->debugData.u8Pl);
		GetSystemRunData()->u16SetDCFanI = ConvertDbgToCtrl_DCFan(GetFlashDataSector0()->debugData.u8Fl);
	}
	//DH状态：比例阀开度DH，风机转速跟随比例阀开度，分段阀最大
	else if ((EN_CODE_d == _stDbgInfo->u8CodeH) && (EN_CODE_H == _stDbgInfo->u8CodeL))
	{
		SetManualSeg(GetWorkCon()->u8MaxSeg);
		GetSystemRunData()->u16SetBlfI = ConvertDbgToCtrl_Blf(GetFlashDataSector0()->debugData.u8Dh);
		GetSystemRunData()->u16SetDCFanI = GetFanUFromBlf(GetSystemRunData()->u16SetBlfI);
	}
	//FA状态：比例阀开度DH，风机转速跟随比例阀开度，分段阀最大
	else if ((EN_CODE_F == _stDbgInfo->u8CodeH) && (EN_CODE_A == _stDbgInfo->u8CodeL))
	{
		SetManualSeg(GetWorkCon()->u8MaxSeg);
		GetSystemRunData()->u16SetBlfI = ConvertDbgToCtrl_Blf(GetFlashDataSector0()->debugData.u8Dh);
		GetSystemRunData()->u16SetDCFanI = GetFanUFromBlf(GetSystemRunData()->u16SetBlfI);
	}
	//各分段的最大/最小工作状态
	// else if (((EN_CODE_L == _stDbgInfo->u8CodeH) && (EN_CODE_1 == _stDbgInfo->u8CodeL)) 
	// || ((EN_CODE_H == _stDbgInfo->u8CodeH) && (EN_CODE_1 == _stDbgInfo->u8CodeL)) 
	// || ((EN_CODE_L == _stDbgInfo->u8CodeH) && (EN_CODE_2 == _stDbgInfo->u8CodeL)) 
	// || ((EN_CODE_H == _stDbgInfo->u8CodeH) && (EN_CODE_2 == _stDbgInfo->u8CodeL)) 
	// || ((EN_CODE_L == _stDbgInfo->u8CodeH) && (EN_CODE_3 == _stDbgInfo->u8CodeL)) 
	// || ((EN_CODE_H == _stDbgInfo->u8CodeH) && (EN_CODE_3 == _stDbgInfo->u8CodeL)) 
	// || ((EN_CODE_L == _stDbgInfo->u8CodeH) && (EN_CODE_4 == _stDbgInfo->u8CodeL)))
	else if (((EN_CODE_L == _stDbgInfo->u8CodeH) || (EN_CODE_H == _stDbgInfo->u8CodeH))
			&& ((EN_CODE_1 == _stDbgInfo->u8CodeL) || (EN_CODE_2 == _stDbgInfo->u8CodeL) || (EN_CODE_3 == _stDbgInfo->u8CodeL) || (EN_CODE_4 == _stDbgInfo->u8CodeL)))
	{
		if (EN_CODE_1 == _stDbgInfo->u8CodeL)
		{
			_u16temp = 0;
		}
		else if (EN_CODE_2 == _stDbgInfo->u8CodeL)
		{
			_u16temp = 1;
		}
		else if (EN_CODE_3 == _stDbgInfo->u8CodeL)
		{
			_u16temp = 2;
		}
		else if (EN_CODE_4 == _stDbgInfo->u8CodeL)
		{
			_u16temp = 3;
		}
		_u16temp = GETMIN(_u16temp, GetWorkCon()->u8MaxSeg);
		SetManualSeg((uint8_t)_u16temp);
		if (EN_CODE_H == _stDbgInfo->u8CodeH)
			_u16temp = (uint16_t)GetFlashDataSector0()->debugData.u8Ph + GetWorkCon()->astSegIncrBLF[GetSegCtrl()->u8Set].s8MaxIncr;
		else
			_u16temp = (uint16_t)GetFlashDataSector0()->debugData.u8Pl + GetWorkCon()->astSegIncrBLF[GetSegCtrl()->u8Set].s8MinIncr;
		_u16temp = GETMIN(0xFF, _u16temp);
		GetSystemRunData()->u16SetBlfI = ConvertDbgToCtrl_Blf((uint8_t)_u16temp);
		GetSystemRunData()->u16SetDCFanI = GetFanUFromBlf(GetSystemRunData()->u16SetBlfI);//根据比例阀开度，进行风机工作
	}
	//特殊调试模式（调试整机使用）
	//控制分段阀
	else if ((EN_CODE_d == _stDbgInfo->u8CodeH) && (EN_CODE_d == _stDbgInfo->u8CodeL))
	{
		_u8DdValve = GetCommSet()->u8DebugData;
		SetManualSeg(GETMIN(_u8DdValve, GetWorkCon()->u8MaxSeg));
	}
	//控制风机工作
	else if ((EN_CODE_F == _stDbgInfo->u8CodeH) && (EN_CODE_F == _stDbgInfo->u8CodeL))
	{
		SetManualSeg(GETMIN(_u8DdValve, GetWorkCon()->u8MaxSeg));
		GetSystemRunData()->u16SetDCFanI = ConvertDbgToCtrl_DCFan(GetCommSet()->u8DebugData);
		GetSystemRunData()->u16SetBlfI = GetBlfIFromFanSet(GetCommSet()->u8DebugData);
	}
	//控制比例阀开度
	else if ((EN_CODE_P == _stDbgInfo->u8CodeH) && (EN_CODE_P == _stDbgInfo->u8CodeL))
	{
		SetManualSeg(GETMIN(_u8DdValve, GetWorkCon()->u8MaxSeg));
		_u16temp = GETMIN(GetCommSet()->u8DebugData, 9);
		GetSystemRunData()->u16SetBlfI = GetBlfIFromSeg((uint8_t)_u16temp);
		GetSystemRunData()->u16SetDCFanI = GetFanUFromBlf(GetSystemRunData()->u16SetBlfI);
	}
	//剩余的情况（没特定要求）
	else
	{
		SetManualSeg(GetWorkCon()->u8FireSegSet);
		GetSystemRunData()->u16SetBlfI = ConvertDbgToCtrl_Blf(GetFlashDataSector0()->debugData.u8Pl);
		GetSystemRunData()->u16SetDCFanI = ConvertDbgToCtrl_DCFan(GetFlashDataSector0()->debugData.u8Fl);
	}
	if (EN_MAIN_STATE_DEBUG != GetSystemRunData()->sysSta.BIT.bMainSta)
	{
		SetFsmState(FSM_STATE_STABLE);
	}
	SegCtrl_Timer();
	SwitchSeg(0); 

	//处于换挡的时候，换挡时的电流为目标电流和最小换挡电流的最大值决定
	if (0 != GetSegCtrl()->u8ChgStep)
		GetSystemRunData()->u16SetBlfI = GETMAX(GetSegCtrl()->u16SegSwitchBlfI, GetSystemRunData()->u16SetBlfI);
	
	//E1故障报警
	if (0 == GetSystemRunData()->sysSta.BIT.bFlame)
	{
		if (++fsmVar_t.var1 >= 50)
		{
			GetSystemRunData()->u8ErrorCode = EN_ERR_E1;
			SetFsmState(FSM_STATE_ENDCLEAN);
		}
	}
	else
	{
		fsmVar_t.var1 = 0;
	}

	//E5故障报警
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

    	//E3故障（2种）
	if ((GetSystemRunData()->WkSw) || (GetValveError().BYTE)) // 温控故障
	{
		GetSystemRunData()->u8ErrorCode = EN_ERR_E3;
		SetFsmState(FSM_STATE_ENDCLEAN);
		ReSetPercent();//燃烧过程出现E3，则重置偏差
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

	if (0 == GetSystemRunData()->sysSta.BIT.bWater || 0 == GetSystemRunData()->bBurnAble)
	{
		SetFsmState(FSM_STATE_ENDCLEAN);
	}
}
