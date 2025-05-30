#include "ALL_HEAD.H"
#include "user_DCFanTable.h"
#include "user_MaInfoTable.h"

static void GetMachineId(void);
static ST_MA_INFO_T stMaInfoCur;
ST_WORK_CONDITION_T stWrkConCur;

static EN_FAN_TYPE_T enFanTypeCur;
static EN_MA_T enMaSet;
void GetMachineInfo_Z8L(void);
static const ST_MODEL_T astModel[EN_MODEL_MAX] =
	{
		{EN_MODEL_Z8L, ARR_NUM(astMaInfo_Z8L), astMaInfo_Z8L, GetMachineInfo_Z8L},
};

static void GetMachineId(void)
{
	uint8_t _u8ID = 0;
	uint8_t _u8temp = 0;
	stMaInfoCur.enFanType = enFanTypeCur;
	stMaInfoCur.pstFA = (au8FaInfoList[enMaSet] + GetFlashDataSector0()->debugData.u8Fa);
	if (ARR_NUM(astModel) <= GetFlashDataSector0()->debugData.u8Ns)
	{
		_u8temp = 0;
	}
	else
	{
		_u8temp = GetFlashDataSector0()->debugData.u8Ns;
	}

	for (_u8ID = 0; _u8ID < astModel[_u8temp].u8IndexMax; _u8ID++)
	{
		// if ((stMaInfoCur.pstFA == (apstModelInfo[_u8temp] + _u8ID)->pstFA)
		// 	&& (stMaInfoCur.enFanType == (apstModelInfo[_u8temp] + _u8ID)->enFanType))
		if ((stMaInfoCur.pstFA == (astModel[_u8temp].stInfo + _u8ID)->pstFA) && (stMaInfoCur.enFanType == (astModel[_u8temp].stInfo + _u8ID)->enFanType))
		{
			break;
		}
	}
	stWrkConCur.u8MachineId = _u8ID;
}

void GetMachineInfo_Z8L(void)
{
	memset(stWrkConCur.as16LoadArray, 0, sizeof(stWrkConCur.as16LoadArray));
	memset(stWrkConCur.au8DeadZone, 0, sizeof(stWrkConCur.au8DeadZone));

	stWrkConCur.au8DeadZone[0] = 45;
	stWrkConCur.au8DeadZone[1] = 25;
	stWrkConCur.au8DeadZone[2] = 15;

    stWrkConCur.u16AddGasSpd = 1000;
	stWrkConCur.u16DecGasSpd = 1000;
	stWrkConCur.u8GasQucikReachPer = 0;

	if (0 == stWrkConCur.u8MachineId)
	{ // 13L_YUXIANG
		stWrkConCur.pstFanRawData = astFanRawData_13_12T_YX;

		stWrkConCur.u8FireSegSet = 0;

		stWrkConCur.u8MaxSeg = 3;

		stWrkConCur.aunSubSegSet[0].BYTE = EN_MAIN_BIT | EN_RES_BIT;
		stWrkConCur.aunSubSegSet[1].BYTE = EN_MAIN_BIT | EN_SUB1_BIT | EN_RES_BIT;
		stWrkConCur.aunSubSegSet[2].BYTE = EN_MAIN_BIT | EN_SUB1_BIT | EN_SUB2_BIT | EN_RES_BIT;

		stWrkConCur.astSegLoad[0].u16MaxLoad = 10394;
		stWrkConCur.astSegLoad[0].u16MinLoad = 4333;
		stWrkConCur.astSegLoad[1].u16MaxLoad = 20898;
		stWrkConCur.astSegLoad[1].u16MinLoad = 8410;
		stWrkConCur.astSegLoad[2].u16MaxLoad = 30331;
		stWrkConCur.astSegLoad[2].u16MinLoad = 12164;

		stWrkConCur.as16LoadArray[0] = 0;
		stWrkConCur.as16LoadArray[1] = 98;
		stWrkConCur.as16LoadArray[2] = 132;
		stWrkConCur.as16LoadArray[3] = 139;
		stWrkConCur.as16LoadArray[4] = 122;
		stWrkConCur.as16LoadArray[5] = 110;
		stWrkConCur.as16LoadArray[6] = 91;
		stWrkConCur.as16LoadArray[7] = 69;
		stWrkConCur.as16LoadArray[8] = 48;
		stWrkConCur.as16LoadArray[9] = 0;

		stWrkConCur.astSegIncrBLF[0].s8MinIncr = 0;
		stWrkConCur.astSegIncrBLF[0].s8MaxIncr = 0;
		stWrkConCur.astSegIncrBLF[1].s8MinIncr = 0;
		stWrkConCur.astSegIncrBLF[1].s8MaxIncr = 0;
		stWrkConCur.astSegIncrBLF[2].s8MinIncr = 0;
		stWrkConCur.astSegIncrBLF[2].s8MaxIncr = 0;

		stWrkConCur.KP_X_ECK_N = 15;
		stWrkConCur.KI_X_EK_N = 13;
		stWrkConCur.KD_X_ECCK_N = 2;

		stWrkConCur.u8GasBlfPidOutTimeS = 12;
		stWrkConCur.u8GasBlfPidOutTimeR = 8;
		stWrkConCur.u8GasBlfPidOutTimeE = 4;

		stWrkConCur.au8DeadZone[0] = 35;
		stWrkConCur.au8DeadZone[1] = 22;
		stWrkConCur.au8DeadZone[2] = 13;

		stWrkConCur.stFanErrData.u8FanMaxMux = 10;
		stWrkConCur.stFanErrData.u8FanMinMux = 10;
	}
	else if (1 == stWrkConCur.u8MachineId)
	{
		// 13L_XIECHUANG
		stWrkConCur.pstFanRawData = astFanRawData_13_12T_XC;

		stWrkConCur.u8FireSegSet = 0;

		stWrkConCur.u8MaxSeg = 3;

		stWrkConCur.aunSubSegSet[0].BYTE = EN_MAIN_BIT | EN_RES_BIT;
		stWrkConCur.aunSubSegSet[1].BYTE = EN_MAIN_BIT | EN_SUB1_BIT | EN_RES_BIT;
		stWrkConCur.aunSubSegSet[2].BYTE = EN_MAIN_BIT | EN_SUB1_BIT | EN_SUB2_BIT | EN_RES_BIT;

		stWrkConCur.astSegLoad[0].u16MaxLoad = 10394;
		stWrkConCur.astSegLoad[0].u16MinLoad = 4333;
		stWrkConCur.astSegLoad[1].u16MaxLoad = 20898;
		stWrkConCur.astSegLoad[1].u16MinLoad = 8410;
		stWrkConCur.astSegLoad[2].u16MaxLoad = 30331;
		stWrkConCur.astSegLoad[2].u16MinLoad = 12164;

		stWrkConCur.as16LoadArray[0] = 0;
		stWrkConCur.as16LoadArray[1] = 98;
		stWrkConCur.as16LoadArray[2] = 132;
		stWrkConCur.as16LoadArray[3] = 139;
		stWrkConCur.as16LoadArray[4] = 122;
		stWrkConCur.as16LoadArray[5] = 110;
		stWrkConCur.as16LoadArray[6] = 91;
		stWrkConCur.as16LoadArray[7] = 69;
		stWrkConCur.as16LoadArray[8] = 48;
		stWrkConCur.as16LoadArray[9] = 0;

		stWrkConCur.astSegIncrBLF[0].s8MinIncr = 0;
		stWrkConCur.astSegIncrBLF[0].s8MaxIncr = 0;
		stWrkConCur.astSegIncrBLF[1].s8MinIncr = 0;
		stWrkConCur.astSegIncrBLF[1].s8MaxIncr = 0;
		stWrkConCur.astSegIncrBLF[2].s8MinIncr = 0;
		stWrkConCur.astSegIncrBLF[2].s8MaxIncr = 0;

		stWrkConCur.KP_X_ECK_N = 15;
		stWrkConCur.KI_X_EK_N = 13;
		stWrkConCur.KD_X_ECCK_N = 2;

		stWrkConCur.au8DeadZone[0] = 35;
		stWrkConCur.au8DeadZone[1] = 22;
		stWrkConCur.au8DeadZone[2] = 13;

		stWrkConCur.u8GasBlfPidOutTimeS = 12;
		stWrkConCur.u8GasBlfPidOutTimeR = 8;
		stWrkConCur.u8GasBlfPidOutTimeE = 4;

		stWrkConCur.stFanErrData.u8FanMaxMux = 10;
		stWrkConCur.stFanErrData.u8FanMinMux = 10;
	}
	else if (2 == stWrkConCur.u8MachineId)
	{
		// 16L_YUXIANG
		stWrkConCur.pstFanRawData = astFanRawData_16_12T_YX;

		stWrkConCur.u8FireSegSet = 0;

		stWrkConCur.u8MaxSeg = 3;

		// stWrkConCur.aunSubSegSet[0].BYTE = EN_MAIN_BIT | EN_RES_BIT;
		// stWrkConCur.aunSubSegSet[1].BYTE = EN_MAIN_BIT | EN_SUB1_BIT | EN_RES_BIT;
		// stWrkConCur.aunSubSegSet[2].BYTE = EN_MAIN_BIT | EN_SUB1_BIT | EN_SUB2_BIT | EN_RES_BIT;
		// stWrkConCur.aunSubSegSet[3].BYTE = EN_MAIN_BIT | EN_SUB1_BIT | EN_SUB2_BIT | EN_SUB3_BIT | EN_RES_BIT;
		stWrkConCur.aunSubSegSet[0].BYTE = EN_MAIN_BIT | EN_SUB1_BIT;
		stWrkConCur.aunSubSegSet[1].BYTE = EN_MAIN_BIT | EN_SUB1_BIT | EN_SUB3_BIT;
		stWrkConCur.aunSubSegSet[2].BYTE = EN_MAIN_BIT | EN_SUB2_BIT;
		stWrkConCur.aunSubSegSet[3].BYTE = EN_MAIN_BIT | EN_SUB1_BIT | EN_SUB2_BIT | EN_SUB3_BIT;

        /*
	    stWrkConCur.astSegLoad[0].u16MaxLoad = 9032;
		stWrkConCur.astSegLoad[0].u16MinLoad = 4624;
		stWrkConCur.astSegLoad[1].u16MaxLoad = 16226;
		stWrkConCur.astSegLoad[1].u16MinLoad = 7295;
		stWrkConCur.astSegLoad[2].u16MaxLoad = 29140;
		stWrkConCur.astSegLoad[2].u16MinLoad = 10538;
		stWrkConCur.astSegLoad[3].u16MaxLoad = 34750;
		stWrkConCur.astSegLoad[3].u16MinLoad = 17125;
    */

        stWrkConCur.astSegLoad[0].u16MaxLoad = 9014;
		stWrkConCur.astSegLoad[0].u16MinLoad = 4530;
		stWrkConCur.astSegLoad[1].u16MaxLoad = 16667;
		stWrkConCur.astSegLoad[1].u16MinLoad = 8445;
		stWrkConCur.astSegLoad[2].u16MaxLoad = 22878;
		stWrkConCur.astSegLoad[2].u16MinLoad = 11084;
		stWrkConCur.astSegLoad[3].u16MaxLoad = 36080;
		stWrkConCur.astSegLoad[3].u16MinLoad = 16913;

		stWrkConCur.as16LoadArray[0] = 0;
		stWrkConCur.as16LoadArray[1] = 58;
		stWrkConCur.as16LoadArray[2] = 93;
		stWrkConCur.as16LoadArray[3] = 107;
		stWrkConCur.as16LoadArray[4] = 90;
		stWrkConCur.as16LoadArray[5] = 87;
		stWrkConCur.as16LoadArray[6] = 70;
		stWrkConCur.as16LoadArray[7] = 48;
		stWrkConCur.as16LoadArray[8] = 29;
		stWrkConCur.as16LoadArray[9] = 0;

		stWrkConCur.astSegIncrBLF[0].s8MinIncr = 0;
		stWrkConCur.astSegIncrBLF[0].s8MaxIncr = 0;
		stWrkConCur.astSegIncrBLF[1].s8MinIncr = 0;
		stWrkConCur.astSegIncrBLF[1].s8MaxIncr = 0;
		stWrkConCur.astSegIncrBLF[2].s8MinIncr = 0;
		stWrkConCur.astSegIncrBLF[2].s8MaxIncr = 0;
		stWrkConCur.astSegIncrBLF[3].s8MinIncr = 0;
		stWrkConCur.astSegIncrBLF[3].s8MaxIncr = 0;

		stWrkConCur.KP_X_ECK_N = 15;
		stWrkConCur.KI_X_EK_N = 13;
		stWrkConCur.KD_X_ECCK_N = 2;

		stWrkConCur.au8DeadZone[0] = 55;
		stWrkConCur.au8DeadZone[1] = 35;
		stWrkConCur.au8DeadZone[2] = 25;

		stWrkConCur.u8GasBlfPidOutTimeS = 12;
		stWrkConCur.u8GasBlfPidOutTimeR = 8;
		stWrkConCur.u8GasBlfPidOutTimeE = 4;

		stWrkConCur.stFanErrData.u8FanMaxMux = 10;
		stWrkConCur.stFanErrData.u8FanMinMux = 10;

        stWrkConCur.u16AddGasSpd = 1000;
        stWrkConCur.u16DecGasSpd = 100;
	}
	else if (3 == stWrkConCur.u8MachineId)//2024-12-5
	{ // 16L_XIECHUANG
		stWrkConCur.pstFanRawData = astFanRawData_16_12T_XC;

		
		stWrkConCur.u8FireSegSet = 0;

		stWrkConCur.u8MaxSeg = 3;

		// stWrkConCur.aunSubSegSet[0].BYTE = EN_MAIN_BIT | EN_RES_BIT;
		// stWrkConCur.aunSubSegSet[1].BYTE = EN_MAIN_BIT | EN_SUB1_BIT | EN_RES_BIT;
		// stWrkConCur.aunSubSegSet[2].BYTE = EN_MAIN_BIT | EN_SUB1_BIT | EN_SUB2_BIT | EN_RES_BIT;
		// stWrkConCur.aunSubSegSet[3].BYTE = EN_MAIN_BIT | EN_SUB1_BIT | EN_SUB2_BIT | EN_SUB3_BIT | EN_RES_BIT;
		stWrkConCur.aunSubSegSet[0].BYTE = EN_MAIN_BIT | EN_SUB1_BIT;
		stWrkConCur.aunSubSegSet[1].BYTE = EN_MAIN_BIT | EN_SUB1_BIT | EN_SUB3_BIT;
		stWrkConCur.aunSubSegSet[2].BYTE = EN_MAIN_BIT | EN_SUB2_BIT;
		stWrkConCur.aunSubSegSet[3].BYTE = EN_MAIN_BIT | EN_SUB1_BIT | EN_SUB2_BIT | EN_SUB3_BIT;

        stWrkConCur.astSegLoad[0].u16MaxLoad = 9014;
		stWrkConCur.astSegLoad[0].u16MinLoad = 4530;
		stWrkConCur.astSegLoad[1].u16MaxLoad = 16667;
		stWrkConCur.astSegLoad[1].u16MinLoad = 8445;
		stWrkConCur.astSegLoad[2].u16MaxLoad = 22878;
		stWrkConCur.astSegLoad[2].u16MinLoad = 11084;
		stWrkConCur.astSegLoad[3].u16MaxLoad = 36080;
		stWrkConCur.astSegLoad[3].u16MinLoad = 16913;

		stWrkConCur.as16LoadArray[0] = 0;
		stWrkConCur.as16LoadArray[1] = 58;
		stWrkConCur.as16LoadArray[2] = 93;
		stWrkConCur.as16LoadArray[3] = 107;
		stWrkConCur.as16LoadArray[4] = 90;
		stWrkConCur.as16LoadArray[5] = 87;
		stWrkConCur.as16LoadArray[6] = 70;
		stWrkConCur.as16LoadArray[7] = 48;
		stWrkConCur.as16LoadArray[8] = 29;
		stWrkConCur.as16LoadArray[9] = 0;

		stWrkConCur.astSegIncrBLF[0].s8MinIncr = 0;
		stWrkConCur.astSegIncrBLF[0].s8MaxIncr = 0;
		stWrkConCur.astSegIncrBLF[1].s8MinIncr = 0;
		stWrkConCur.astSegIncrBLF[1].s8MaxIncr = 0;
		stWrkConCur.astSegIncrBLF[2].s8MinIncr = 0;
		stWrkConCur.astSegIncrBLF[2].s8MaxIncr = 0;
		stWrkConCur.astSegIncrBLF[3].s8MinIncr = 0;
		stWrkConCur.astSegIncrBLF[3].s8MaxIncr = 0;

		stWrkConCur.KP_X_ECK_N = 15;
		stWrkConCur.KI_X_EK_N = 13;
		stWrkConCur.KD_X_ECCK_N = 2;

		stWrkConCur.au8DeadZone[0] = 55;
		stWrkConCur.au8DeadZone[1] = 35;
		stWrkConCur.au8DeadZone[2] = 25;

		stWrkConCur.u8GasBlfPidOutTimeS = 12;
		stWrkConCur.u8GasBlfPidOutTimeR = 8;
		stWrkConCur.u8GasBlfPidOutTimeE = 4;

		stWrkConCur.stFanErrData.u8FanMaxMux = 10;
		stWrkConCur.stFanErrData.u8FanMinMux = 10;

        stWrkConCur.u16AddGasSpd = 1000;
        stWrkConCur.u16DecGasSpd = 100;
	}
	else if (4 == stWrkConCur.u8MachineId)
	{
		// 18L_YUXIANG
		stWrkConCur.pstFanRawData = astFanRawData_18_12T_YX;

		stWrkConCur.u8FireSegSet = 0;

		stWrkConCur.u8MaxSeg = 3;

		stWrkConCur.aunSubSegSet[0].BYTE = EN_MAIN_BIT | EN_RES_BIT;
		stWrkConCur.aunSubSegSet[1].BYTE = EN_MAIN_BIT | EN_SUB1_BIT | EN_RES_BIT;
		stWrkConCur.aunSubSegSet[2].BYTE = EN_MAIN_BIT | EN_SUB1_BIT | EN_SUB2_BIT | EN_RES_BIT;

		stWrkConCur.astSegLoad[0].u16MaxLoad = 12270;
		stWrkConCur.astSegLoad[0].u16MinLoad = 5298;
		stWrkConCur.astSegLoad[1].u16MaxLoad = 24796;
		stWrkConCur.astSegLoad[1].u16MinLoad = 10245;
		stWrkConCur.astSegLoad[2].u16MaxLoad = 40459;
		stWrkConCur.astSegLoad[2].u16MinLoad = 16411;

		stWrkConCur.as16LoadArray[0] = 0;
		stWrkConCur.as16LoadArray[1] = 60;
		stWrkConCur.as16LoadArray[2] = 110;
		stWrkConCur.as16LoadArray[3] = 130;
		stWrkConCur.as16LoadArray[4] = 120;
		stWrkConCur.as16LoadArray[5] = 100;
		stWrkConCur.as16LoadArray[6] = 90;
		stWrkConCur.as16LoadArray[7] = 70;
		stWrkConCur.as16LoadArray[8] = 50;
		stWrkConCur.as16LoadArray[9] = 0;

		stWrkConCur.astSegIncrBLF[0].s8MinIncr = 0;
		stWrkConCur.astSegIncrBLF[0].s8MaxIncr = 0;
		stWrkConCur.astSegIncrBLF[1].s8MinIncr = 0;
		stWrkConCur.astSegIncrBLF[1].s8MaxIncr = 0;
		stWrkConCur.astSegIncrBLF[2].s8MinIncr = 0;
		stWrkConCur.astSegIncrBLF[2].s8MaxIncr = 0;

		stWrkConCur.KP_X_ECK_N = 15;
		stWrkConCur.KI_X_EK_N = 13;
		stWrkConCur.KD_X_ECCK_N = 2;

		stWrkConCur.au8DeadZone[0] = 45;
		stWrkConCur.au8DeadZone[1] = 25;
		stWrkConCur.au8DeadZone[2] = 15;

		stWrkConCur.u8GasBlfPidOutTimeS = 12;
		stWrkConCur.u8GasBlfPidOutTimeR = 8;
		stWrkConCur.u8GasBlfPidOutTimeE = 4;

		stWrkConCur.stFanErrData.u8FanMaxMux = 10;
		stWrkConCur.stFanErrData.u8FanMinMux = 10;
	}
	else if (5 == stWrkConCur.u8MachineId)
	{
		// 18L_XIECHUANG
		stWrkConCur.pstFanRawData = astFanRawData_18_12T_XC;

		stWrkConCur.u8FireSegSet = 0;

		stWrkConCur.u8MaxSeg = 2;

		stWrkConCur.aunSubSegSet[0].BYTE = EN_MAIN_BIT | EN_RES_BIT;
		stWrkConCur.aunSubSegSet[1].BYTE = EN_MAIN_BIT | EN_SUB1_BIT | EN_RES_BIT;
		stWrkConCur.aunSubSegSet[2].BYTE = EN_MAIN_BIT | EN_SUB1_BIT | EN_SUB2_BIT | EN_RES_BIT;

		stWrkConCur.astSegLoad[0].u16MaxLoad = 12270;
		stWrkConCur.astSegLoad[0].u16MinLoad = 5298;
		stWrkConCur.astSegLoad[1].u16MaxLoad = 24796;
		stWrkConCur.astSegLoad[1].u16MinLoad = 10245;
		stWrkConCur.astSegLoad[2].u16MaxLoad = 40459;
		stWrkConCur.astSegLoad[2].u16MinLoad = 16411;

		stWrkConCur.as16LoadArray[0] = 0;
		stWrkConCur.as16LoadArray[1] = 60;
		stWrkConCur.as16LoadArray[2] = 110;
		stWrkConCur.as16LoadArray[3] = 130;
		stWrkConCur.as16LoadArray[4] = 120;
		stWrkConCur.as16LoadArray[5] = 100;
		stWrkConCur.as16LoadArray[6] = 90;
		stWrkConCur.as16LoadArray[7] = 70;
		stWrkConCur.as16LoadArray[8] = 50;
		stWrkConCur.as16LoadArray[9] = 0;

		stWrkConCur.astSegIncrBLF[0].s8MinIncr = 0;
		stWrkConCur.astSegIncrBLF[0].s8MaxIncr = 0;
		stWrkConCur.astSegIncrBLF[1].s8MinIncr = 0;
		stWrkConCur.astSegIncrBLF[1].s8MaxIncr = 0;
		stWrkConCur.astSegIncrBLF[2].s8MinIncr = 0;
		stWrkConCur.astSegIncrBLF[2].s8MaxIncr = 0;

		stWrkConCur.KP_X_ECK_N = 15;
		stWrkConCur.KI_X_EK_N = 13;
		stWrkConCur.KD_X_ECCK_N = 2;

		stWrkConCur.au8DeadZone[0] = 45;
		stWrkConCur.au8DeadZone[1] = 25;
		stWrkConCur.au8DeadZone[2] = 15;

		stWrkConCur.u8GasBlfPidOutTimeS = 12;
		stWrkConCur.u8GasBlfPidOutTimeR = 8;
		stWrkConCur.u8GasBlfPidOutTimeE = 4;

		stWrkConCur.stFanErrData.u8FanMaxMux = 10;
		stWrkConCur.stFanErrData.u8FanMinMux = 10;
	}
	else
	{
		// 16L_YUXIANG
		stWrkConCur.pstFanRawData = astFanRawData_16_12T_YX;

		stWrkConCur.u8FireSegSet = 0;

		stWrkConCur.u8MaxSeg = 2;

		stWrkConCur.aunSubSegSet[0].BYTE = EN_MAIN_BIT | EN_RES_BIT;
		stWrkConCur.aunSubSegSet[1].BYTE = EN_MAIN_BIT | EN_SUB1_BIT | EN_RES_BIT;
		stWrkConCur.aunSubSegSet[2].BYTE = EN_MAIN_BIT | EN_SUB1_BIT | EN_SUB2_BIT | EN_RES_BIT;

		stWrkConCur.astSegLoad[0].u16MaxLoad = 10480;
		stWrkConCur.astSegLoad[0].u16MinLoad = 4480;
		stWrkConCur.astSegLoad[1].u16MaxLoad = 20000;
		stWrkConCur.astSegLoad[1].u16MinLoad = 7920;
		stWrkConCur.astSegLoad[2].u16MaxLoad = 34321;
		stWrkConCur.astSegLoad[2].u16MinLoad = 13250;

		stWrkConCur.as16LoadArray[0] = 0;
		stWrkConCur.as16LoadArray[1] = 70;
		stWrkConCur.as16LoadArray[2] = 130;
		stWrkConCur.as16LoadArray[3] = 140;
		stWrkConCur.as16LoadArray[4] = 140;
		stWrkConCur.as16LoadArray[5] = 120;
		stWrkConCur.as16LoadArray[6] = 80;
		stWrkConCur.as16LoadArray[7] = 70;
		stWrkConCur.as16LoadArray[8] = 50;
		stWrkConCur.as16LoadArray[9] = 0;

		stWrkConCur.astSegIncrBLF[0].s8MinIncr = 0;
		stWrkConCur.astSegIncrBLF[0].s8MaxIncr = 0;
		stWrkConCur.astSegIncrBLF[1].s8MinIncr = 0;
		stWrkConCur.astSegIncrBLF[1].s8MaxIncr = 0;
		stWrkConCur.astSegIncrBLF[2].s8MinIncr = 0;
		stWrkConCur.astSegIncrBLF[2].s8MaxIncr = 0;

		stWrkConCur.KP_X_ECK_N = 15;
		stWrkConCur.KI_X_EK_N = 13;
		stWrkConCur.KD_X_ECCK_N = 2;

		stWrkConCur.au8DeadZone[0] = 45;
		stWrkConCur.au8DeadZone[1] = 25;
		stWrkConCur.au8DeadZone[2] = 15;

		stWrkConCur.u8GasBlfPidOutTimeS = 12;
		stWrkConCur.u8GasBlfPidOutTimeR = 8;
		stWrkConCur.u8GasBlfPidOutTimeE = 4;

		stWrkConCur.stFanErrData.u8FanMaxMux = 10;
		stWrkConCur.stFanErrData.u8FanMinMux = 10;
	}
}
void UpdateMaInfo(void)
{
	uint8_t _u8temp;
	enMaSet = EN_MA_UPFAN;
	enFanTypeCur = GetSystemRunData()->enFanType;
	GetMachineId();
	if (ARR_NUM(astModel) <= GetFlashDataSector0()->debugData.u8Ns)
	{
		_u8temp = 0;
	}
	else
	{
		_u8temp = GetFlashDataSector0()->debugData.u8Ns;
	}
	(*(astModel[_u8temp].GetInfo))();
}
const ST_MA_INFO_T *GetMaInfo(void)
{
	return &stMaInfoCur;
}
const ST_WORK_CONDITION_T *GetWorkCon(void)
{
	return &stWrkConCur;
}
