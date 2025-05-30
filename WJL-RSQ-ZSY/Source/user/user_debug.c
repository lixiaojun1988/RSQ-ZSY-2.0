/*************************************************************************
 * Copyright:    wjl Kitchenware.
 * File:         led.c
 * Author:       xjli
 * Version:      v1.0
 * Brief:
 * Date:
 *************************************************************************/

#include "all_head.h"

extern SystemRunDataTypeDef SystemRunData_t;
static const ST_DBG_INFO_T astDbgInfo[] =
	{
		{(EN_ADJ_NO | EN_FB_NO | EN_DSP_HEX), EN_CODE_q, EN_CODE_U, &(FlashDataCache0_st.debugData.u8Null)}, // Save
		{(EN_ADJ_EN | EN_FB_NO | EN_DSP_DEC), EN_CODE_F, EN_CODE_A, &(FlashDataCache0_st.debugData.u8Fa)},	 // FA
#if (1 == ADD_DBG_AA)
		{(EN_ADJ_EN | EN_FB_NO | EN_DSP_DEC), EN_CODE_A, EN_CODE_A, &(FlashDataCache0_st.debugData.u8Aa)}, // Aa
#endif

		{(EN_ADJ_EN | EN_FB_TMP | EN_DSP_HEX), EN_CODE_P, EN_CODE_L, &(FlashDataCache0_st.debugData.u8Pl)}, // PL
		{(EN_ADJ_EN | EN_FB_TMP | EN_DSP_HEX), EN_CODE_d, EN_CODE_H, &(FlashDataCache0_st.debugData.u8Dh)}, // DH
		{(EN_ADJ_EN | EN_FB_TMP | EN_DSP_HEX), EN_CODE_P, EN_CODE_H, &(FlashDataCache0_st.debugData.u8Ph)}, // PH
		{(EN_ADJ_EN | EN_FB_NO | EN_DSP_DEC), EN_CODE_5, EN_CODE_P, &(FlashDataCache0_st.debugData.u8Sp)},	// SP
		{(EN_ADJ_EN | EN_FB_SPD | EN_DSP_HEX), EN_CODE_F, EN_CODE_H, &(FlashDataCache0_st.debugData.u8Fh)}, // FH
		{(EN_ADJ_EN | EN_FB_NO | EN_DSP_DEC), EN_CODE_5, EN_CODE_L, &(FlashDataCache0_st.debugData.u8Sl)},	// SL
		{(EN_ADJ_EN | EN_FB_SPD | EN_DSP_HEX), EN_CODE_F, EN_CODE_L, &(FlashDataCache0_st.debugData.u8Fl)}, // FL

		{(EN_ADJ_EN | EN_FB_NO | EN_DSP_DEC), EN_CODE_H, EN_CODE_C, &(FlashDataCache0_st.debugData.u8Hc)},	// HC
		{(EN_ADJ_EN | EN_FB_NO | EN_DSP_DEC), EN_CODE_L, EN_CODE_C, &(FlashDataCache0_st.debugData.u8Lc)},	// LC
		{(EN_ADJ_NO | EN_FB_TMP | EN_DSP_HEX), EN_CODE_H, EN_CODE_4, &(FlashDataCache0_st.debugData.u8Ph)}, // H4
		{(EN_ADJ_NO | EN_FB_TMP | EN_DSP_HEX), EN_CODE_L, EN_CODE_4, &(FlashDataCache0_st.debugData.u8Pl)}, // L4
		{(EN_ADJ_NO | EN_FB_TMP | EN_DSP_HEX), EN_CODE_H, EN_CODE_3, &(FlashDataCache0_st.debugData.u8Ph)}, // H3
		{(EN_ADJ_NO | EN_FB_TMP | EN_DSP_HEX), EN_CODE_L, EN_CODE_3, &(FlashDataCache0_st.debugData.u8Pl)}, // L3
		{(EN_ADJ_NO | EN_FB_TMP | EN_DSP_HEX), EN_CODE_H, EN_CODE_2, &(FlashDataCache0_st.debugData.u8Ph)}, // H2
		{(EN_ADJ_NO | EN_FB_TMP | EN_DSP_HEX), EN_CODE_L, EN_CODE_2, &(FlashDataCache0_st.debugData.u8Pl)}, // L2
		{(EN_ADJ_NO | EN_FB_TMP | EN_DSP_HEX), EN_CODE_H, EN_CODE_1, &(FlashDataCache0_st.debugData.u8Ph)}, // H1
		{(EN_ADJ_NO | EN_FB_TMP | EN_DSP_HEX), EN_CODE_L, EN_CODE_1, &(FlashDataCache0_st.debugData.u8Pl)}, // L1
        {(EN_ADJ_NO | EN_FB_TMP | EN_DSP_HEX), EN_CODE_H, EN_CODE_3, &(FlashDataCache0_st.debugData.u8Ph)}, // H3
		
		{(EN_ADJ_NO | EN_FB_TMP | EN_DSP_HEX), EN_CODE_H, EN_CODE_2, &(FlashDataCache0_st.debugData.u8Ph)}, // H2
        {(EN_ADJ_NO | EN_FB_TMP | EN_DSP_HEX), EN_CODE_L, EN_CODE_3, &(FlashDataCache0_st.debugData.u8Pl)}, // L3
		{(EN_ADJ_NO | EN_FB_TMP | EN_DSP_HEX), EN_CODE_L, EN_CODE_2, &(FlashDataCache0_st.debugData.u8Pl)}, // L2
#if (1 == ADD_DBG_INFO)
		{(EN_ADJ_EN | EN_FB_NO | EN_DSP_DEC), EN_CODE_d, EN_CODE_d, &(SystemRunData_t.u8ManaulSetSeg)}, // dd
		{(EN_ADJ_EN | EN_FB_NO | EN_DSP_DEC), EN_CODE_F, EN_CODE_F, &(SystemRunData_t.u8ManaulSetFan)}, // FF
		{(EN_ADJ_EN | EN_FB_NO | EN_DSP_DEC), EN_CODE_P, EN_CODE_P, &(SystemRunData_t.u8ManaulSetPer)}, // PP
#endif
		{(EN_ADJ_BOOL | EN_FB_NO | EN_DSP_HEX), EN_CODE_n, EN_CODE_E, &(FlashDataCache0_st.debugData.u8Ne)}, // nE
		{(EN_ADJ_EN | EN_FB_NO | EN_DSP_DEC), EN_CODE_C, EN_CODE_L, &(FlashDataCache0_st.debugData.u8Cl)},	 // CL
		{(EN_ADJ_EN | EN_FB_NO | EN_DSP_DEC), EN_CODE_C, EN_CODE_o, &(FlashDataCache0_st.debugData.u8Co)},	 // Co
		{(EN_ADJ_EN | EN_FB_SPD | EN_DSP_DEC), EN_CODE_d, EN_CODE_E, &(FlashDataCache0_st.debugData.u8De)},	 // dE
		{(EN_ADJ_EN | EN_FB_NO | EN_DSP_DEC), EN_CODE_L, EN_CODE_5, &(FlashDataCache0_st.debugData.u8Ls)},	 // LS
		{(EN_ADJ_EN | EN_FB_NO | EN_DSP_DEC), EN_CODE_E, EN_CODE_E, &(FlashDataCache0_st.debugData.u8Ee)},	 // ee
        {(EN_ADJ_EN | EN_FB_NO | EN_DSP_HEX), EN_CODE_P, EN_CODE_U, &(FlashDataCache0_st.debugData.u8Pu)},	 // ee
		{(EN_ADJ_NO | EN_FB_NO | EN_DSP_HEX), EN_CODE_q, EN_CODE_U, &(FlashDataCache0_st.debugData.u8Null)}, // Qu

		//{(EN_ADJ_NO | EN_FB_NO | EN_DSP_HEX),EN_CODE_H,EN_CODE_3,&(FlashDataCache0_st.debugData.stValve.u8Ph)},//H3
		//{(EN_ADJ_NO | EN_FB_NO | EN_DSP_HEX),EN_CODE_L,EN_CODE_4,&(FlashDataCache0_st.debugData.stValve.u8Pl)},//L4

		//{(EN_ADJ_EN | EN_FB_NO | EN_DSP_DEC),EN_CODE_n,EN_CODE_F,&(FlashDataCache0_st.debugData.u8Nf)},//nF
		//{(EN_ADJ_EN | EN_FB_NO | EN_DSP_DEC),EN_CODE_L,EN_CODE_5,&(FlashDataCache0_st.debugData.u8Ls)},//LS

		//{(EN_ADJ_EN | EN_FB_NO | EN_DSP_DEC),EN_CODE_5,EN_CODE_P,&(FlashDataCache0_st.debugData.u8Sp)},//SP
		//{(EN_ADJ_EN | EN_FB_NO | EN_DSP_DEC),EN_CODE_5,EN_CODE_L,&(FlashDataCache0_st.debugData.u8Sl)},//SL
};

// 强制参数初始化列表
//u8Pl;u8Dh;u8Ph;u8Fh;u8Fl;u8Hc;u8Lc;u8Sp;u8Sl;u8Ne;u8Cl;
	uint8_t u8Co;
	uint8_t u8Ls;
	uint8_t u8Nf;
	uint8_t u8De;

    uint8_t u8Nh;//后清扫时间

	uint8_t u8Ee;
    uint8_t u8Pu;
	uint8_t u8Null;
const ST_DBG_FLH_T stDbgFlhDef = {12, 0x34, 0x3F, 0x82, 0xA4, 0x51, 78, 59, 14, 17, 1, 0, 1, 
									//LS,NF,DE,NH,Ee,Pu
									0, 0, 0, 0, 0, 0xFF};

// 所有默认参数
const DBG_MACH_DEF astDebugMachDef[] =
	{
		{0x7A, 0x93, 0xB4, 0x5C, 0x1A, 85, 50}, // 4-13L12T
		{0x7A, 0x93, 0xB4, 0x5C, 0x1A, 85, 50}, // 4-13L12T
		{0x34, 0x3F, 0x82, 0xA4, 0x51, 78, 59}, // 12-16L12T
		{0x34, 0x3F, 0x82, 0xA4, 0x51, 78, 59}, // 12-16L12T
		{0x82, 0x8E, 0xC0, 0x68, 0x1A, 88, 52}, // 21-20L12T
		{0x82, 0x8E, 0xC0, 0x68, 0x1A, 88, 52}	// 21-20L12T
};
////风机类型频率范围判断
static const ST_FAN_TYPE_FREP_T astFanFrep[EN_FAN_MAX] =
	{
		{EN_FAN_YUXIANG, 540, 460},
		{EN_FAN_XIECHUANG, 640, 560}};

const ST_DBG_INFO_T *GetDbgInfo(uint8_t _DbgIndex)
{
	return &astDbgInfo[_DbgIndex];
}
uint8_t GetDebugIndexMax(void)
{
	return ARR_NUM(astDbgInfo);
}

void GetCommDebugData(ST_COMM_SET_T *pGetCommRevData)
{
	uint8_t _u8temp;
	static uint8_t u8LastDebugIndex = 0;
	_u8temp = GetDebugIndexMax();
	if (_u8temp <= pGetCommRevData->u8DebugIndex)
	{
		pGetCommRevData->u8DebugIndex = 0;
	}
	if (pGetCommRevData->u8DebugIndex)
	{
		u8LastDebugIndex = pGetCommRevData->u8DebugIndex;
		if ((0xAA == pGetCommRevData->u8DebugAct) && (0 != astDbgInfo[pGetCommRevData->u8DebugIndex].stAddInfo.BIT.u8AdjSet))
		{
            if (*(astDbgInfo[pGetCommRevData->u8DebugIndex].pu8Data) != pGetCommRevData->u8DebugData)
            {
                *(astDbgInfo[pGetCommRevData->u8DebugIndex].pu8Data) = pGetCommRevData->u8DebugData;
                // 写入数值为FA？
                if (((EN_CODE_F == astDbgInfo[pGetCommRevData->u8DebugIndex].u8CodeH) && (EN_CODE_A == astDbgInfo[pGetCommRevData->u8DebugIndex].u8CodeL)) || ((EN_CODE_n == astDbgInfo[pGetCommRevData->u8DebugIndex].u8CodeH) && (EN_CODE_5 == astDbgInfo[pGetCommRevData->u8DebugIndex].u8CodeL)))
                {
                    UpdateMaInfo();
                    memcpy(&(GetFlashDataSector0()->debugData.u8Pl), (astDebugMachDef + GetWorkCon()->u8MachineId), sizeof(DBG_MACH_DEF));
                }
            }
		}
		GetSystemRunData()->sysSta.BIT.bMainSta = EN_MAIN_STATE_DEBUG;
	}
	else
	{
		if (u8LastDebugIndex) // 写E2P
		{
			u8LastDebugIndex = 0;
			GetSystemRunData()->sysSta.BIT.bMainSta = EN_MAIN_STATE_NORMAL;
			GetFlashDataSector0()->adaptData.ucByte = 0;
			writeFlashSector0();
		}
		if (0 == GetCommSet()->u8RemoLink)
		{
			GetSystemRunData()->sysSta.BIT.bMainSta = EN_MAIN_STATE_NORMAL;
		}
		else
		{
			GetSystemRunData()->sysSta.BIT.bMainSta = GetCommSet()->u8RemoLink + 1;
		}
	}
}

#define FAN_FREP_MUL 15				// 风机频率放大倍数（转速转频率倍数）
static uint16_t u8FanInitCnt = 500; // 上电风机初始化时间（单位0.01s，共5s）
static uint8_t u8FanInitNum = 0;	// 风机初始化次数

/**********************************************
风机参数输入
**********************************************/
void FanTypeJudge(void)
{
	uint8_t u8i;
	static uint8_t enFanType = 0;
	if (u8FanInitCnt)
	{
		u8FanInitCnt--;
		for (u8i = 0; ARR_NUM(astFanFrep) > u8i; u8i++)
		{
			if (((astFanFrep[u8i].u16Max * FAN_FREP_MUL) > GetSystemRunData()->u16FanSpeed) && ((astFanFrep[u8i].u16Min * FAN_FREP_MUL) < GetSystemRunData()->u16FanSpeed))
			{
				if (enFanType == astFanFrep[u8i].enFanType)
				{
					u8FanInitNum++;
				}
				else
				{
					enFanType = astFanFrep[u8i].enFanType;
					u8FanInitNum = 0;
				}
				break;
			}
		}
	}
	// 同一状态满5次，锁定风机类型
	if (5 <= u8FanInitNum)
	{
		u8FanInitCnt = 0;
	}

	if (1 == GetFlashDataSector0()->debugData.u8De)
	{
		GetSystemRunData()->enFanType = EN_FAN_YUXIANG;
	}
	else if (2 == GetFlashDataSector0()->debugData.u8De)
	{
		GetSystemRunData()->enFanType = EN_FAN_XIECHUANG;
	}
	else
	{
		GetSystemRunData()->enFanType = (EN_FAN_TYPE_T)enFanType;
	}
}
