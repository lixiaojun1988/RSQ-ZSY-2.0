/*************************************************************************
 * Copyright:    wjl Kitchenware.
 * File:         FsmPowerOnState.c
 * Layer:
 * Author:       xjli
 * Version:
 * Brief:		前清扫态
 *************************************************************************/
#include "all_head.h"

#define WAITINTMP_TIME 400
#define STARTCLEAN_TIME 150

extern fsmVarTypedef fsmVar_t;
void FsmPreFanEnterHandler(void)
{
	SetFsmStage(FSM_STAGE_DOING);
	memset(&fsmVar_t, 0, sizeof(fsmVar_t));
}

void FsmPreFanDoingHandler(void) // 10MS
{
	uint16_t _u16Temp = 0;
	uint16_t _u16FanSpdTemp;
	static uint16_t _u16SpdSave;
	static uint8_t _u8SpdStay_100ms;
    EN_ERRORCODE_T _enErr;
    //风机前清扫转速按照FH进行处理
	GetSystemRunData()->u16SetDCFanI = DCFan_GetStkSta(ConvertDbgToCtrl_DCFan(GetFlashDataSector0()->debugData.u8Fh));

    //
	if (0 < GetSystemRunData()->u8FanRunning)
	{
        //确认风机是否稳定
		_u16FanSpdTemp = RANGLMT(_u16SpdSave, GetSystemRunData()->u16FanSpeed - 100, GetSystemRunData()->u16FanSpeed + 100);
		if ((_u16SpdSave != _u16FanSpdTemp) || (50 >= fsmVar_t.var1))
		{
			_u8SpdStay_100ms = 0;
		}
		else
		{
			INC_B(_u8SpdStay_100ms);
		}
		_u16SpdSave = GetSystemRunData()->u16FanSpeed;
	}
	else
	{
		_u16SpdSave = 0;
		_u8SpdStay_100ms = 0;
	}

    //8s后，若仍处于前清扫，要么处于堵塞，要么风机未工作
	if (++fsmVar_t.var1 > 800)
	{
		if (0 == GetSystemRunData()->sysSta.BIT.bFlame)
		{
			if (0 == GetSystemRunData()->u8FanRunning)
			{
				GetSystemRunData()->u8ErrorCode = EN_ERR_E2;
				GetSystemRunData()->u16SetDCFanI = SET_DCFAN_OFF;
                SetFsmState(FSM_STATE_ENDCLEAN);
			}
			else
			{
				GetSystemRunData()->u8ErrorCode = EN_ERR_E8;
				SetFsmState(FSM_STATE_ENDCLEAN);
			}
		}
	}
    //保证最小的前清扫时间后，进行状态切换判断
	else if (STARTCLEAN_TIME <= fsmVar_t.var1)
	{
        //生成堵塞时候的转速（根据实际情况进行调整）
		_u16FanSpdTemp = GetDCFanData()->u16FanSpdErr - GetDCFanData()->u16FanSpdRaw;
        //预留风机渐变的过程
		_u16Temp = GETMIN((fsmVar_t.var1 - STARTCLEAN_TIME), 600);
		_u16FanSpdTemp = (uint16_t)((uint32_t)_u16FanSpdTemp * _u16Temp / 600);
		_u16FanSpdTemp += GetDCFanData()->u16FanSpdRaw;
        //风机稳定，且转速低于堵塞判定转速
		if ((0 < GetSystemRunData()->u8FanRunning) && (50 < _u8SpdStay_100ms) && (GetSystemRunData()->u16FanSpeed < _u16FanSpdTemp))
		{
			SetFsmState(FSM_STATE_IGINTION);
		}
	}

    //TODO:开关机判断状态切换后面要更改成燃烧请求
	if (!GetSystemRunData()->bBurnAble || !GetSystemRunData()->sysSta.BIT.bWater)
		SetFsmState(FSM_STATE_IDLE);

    //E6故障
	if (GetSystemRunData()->sysSta.BIT.bFlame)
		GetSystemRunData()->u16E6Check_10ms++;
	else
		GetSystemRunData()->u16E6Check_10ms = 0;
	if (GetSystemRunData()->u16E6Check_10ms >= 100)
	{
		GetSystemRunData()->u8ErrorCode = EN_ERR_E6;
		SetFsmState(FSM_STATE_ENDCLEAN);
	}

    //E3故障
	if ((GetSystemRunData()->WkSw) || (ValveCheckFunction())) // 温控故障
	{
		GetSystemRunData()->u8ErrorCode = EN_ERR_E3;
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
