/*************************************************************************
 * Copyright:    wjl Kitchenware.
 * File:         濠电偞鍨堕幐绋棵�?��澶樻晩鐎广儱�?��埢鏇㈡煙闁箑鏋涙繛鍛灦閹便劌螣缂佹ǚ鎷婚梺绋挎捣閹�?��锝炲┑瀣骇闁瑰鍋熻ぐ�?�??閹�?偞灏电紒顕呭灣閺侇喚�??〃绛�?-M01(濠电偞鍨堕幖鈺呭矗閸愵喓鈧倹绻濆顒侇棟闁荤喐鐟ョ�?��兘�?ㄩ幎鑺ョ叆婵炴垼娅曠�?���?
 * Layer:
 * Author:       xjli
 * Version:      V1
 * Brief:		     SH32F9010P  44PIN
 * date闂備焦瀵ч崘濠氬�?��燂拷        2024-11-27
 *************************************************************************/

#include "ALL_HEAD.H"

#define POWERBOARD_FIFOBUF_LENGTH 10
#define BOARD_UART Usart_0

static void MainFrame0LoadData(void);
static void MainFrame1LoadData(void);
static void MainFrame2LoadData(void);
static void MainFrame3LoadData(void);
static void MainFrame4LoadData(void);
static void MainFrame5LoadData(void);
static void MainFrame6LoadData(void);
static void MainFrame7LoadData(void);
static void MainFrame8LoadData(void);
static void MainFrame9LoadData(void);
static void MainFrame10LoadData(void);
static void MainFrame11LoadData(void);
static void MainFrame12LoadData(void);
static void MainFrame13LoadData(void);
static void MainFrame14LoadData(void);

static uint8_t Comm_Verify(uint8_t *_pu8DataPnt, uint8_t _u8datalength);

UN_FRAME_CONTROLLER_T unControllerFrame;
static UN_FRAME_MAINBOARD_T unMainBoardFrame;

static Fifo s_FrameFifo;
static uint8_t s_FrameFifoBuf[POWERBOARD_FIFOBUF_LENGTH];
static Fifo s_AddrFifo;
static uint8_t s_AddrFifoBuf[POWERBOARD_FIFOBUF_LENGTH];
static uint16_t u16PriorityReset_50ms;
static uint8_t u8WifiOffLine_50ms;
static uint16_t u16AlarmOffLine_50ms;
static ST_FLAMESET_T stFlameSet;
static uint8_t SnSendCnt = 0;
static ST_COMM_RUNDATA_T stCommRunData;
uint8_t GetDebugFaMax(void)
{
	return 22;
}
ST_COMM_SET_T stCommSet;
static uint8_t u8DevOnOffSta;
static const EN_DEVADDR_T au8CtrlAddrList[] =
	{
		EN_ADDR_WIFI,
		EN_ADDR_REMOTE,
		EN_ADDR_WIRE,
		EN_ADDR_CTRL,
		EN_ADDR_ALARM,
#if (ADD_DBG_INFO)
		EN_ADDR_MONITOR
#endif
};
static const EN_MAIN_FRAME_T aenMainFrameList[] =
	{
		EN_MAIN_FRAME_0,
		EN_MAIN_FRAME_1,
		EN_MAIN_FRAME_2,
		EN_MAIN_FRAME_3,
		EN_MAIN_FRAME_4,
		EN_MAIN_FRAME_5,
		EN_MAIN_FRAME_6,
		EN_MAIN_FRAME_7};
/*
static EN_MAIN_FRAME_T aenSpecFrameList[3] =
	{
		EN_MAIN_FRAME_MAX,
		EN_MAIN_FRAME_MAX,
		EN_MAIN_FRAME_MAX};*/
static const FUCTION afnMainFrameLoadData[] =
	{
		MainFrame0LoadData,
		MainFrame1LoadData,
		MainFrame2LoadData,
		MainFrame3LoadData,
		MainFrame4LoadData,
		MainFrame5LoadData,
		MainFrame6LoadData,
		MainFrame7LoadData,
		MainFrame8LoadData,
		MainFrame9LoadData,
		MainFrame10LoadData,
		MainFrame11LoadData,
		MainFrame12LoadData,
		MainFrame13LoadData,
		MainFrame14LoadData};

#define Head &stCommSet
#define DevSta &u8DevOnOffSta
#define Book24 unBook24Set.Bytes
#define EMPTY_DATA &stCommSet.u8Reserved
static const ST_CTRLDATA_T astCtrlData[] =
	{
		{EMPTY_DATA, DevSta, EMPTY_DATA, Head.u8DeviceType},							  // 0
		{Head.u8OnOffSet, DevSta, EMPTY_DATA, EMPTY_DATA},								  // 1
		{Head.u8DebugIndex, DevSta, Head.u8DebugAct, Head.u8DebugData},					  // 2
		{Head.u8RemoLink, EMPTY_DATA, EMPTY_DATA, EMPTY_DATA},							  // 3
		{Head.u8ModeSet, Head.u8TmpSet, Head.u8FluxSet, Head.u8WaterSet},				  // 4
		{Head.u8ClrWSum, Head.u8ClrGSum, EMPTY_DATA, EMPTY_DATA},						  // 5
		{Head.u8HealthWash, DevSta, EMPTY_DATA, EMPTY_DATA},							  // 6
		{Head.u8BurstMode, DevSta, EMPTY_DATA, EMPTY_DATA},								  // 7
		{Head.u8CoPPM, Head.u8CH4PPM, Head.u8AlarmSta, Head.u8DeviceType},				  // 8
		{Head.u8PreWarmSet, DevSta, EMPTY_DATA, EMPTY_DATA},							  // 9
		{Head.u8PreWarmTime, DevSta, EMPTY_DATA, EMPTY_DATA},							  // 10
		{Head.u8PreWarmDif, DevSta, EMPTY_DATA, EMPTY_DATA},							  // 11
		{Head.u8HourCur, DevSta, Head.u8MinCur, EMPTY_DATA},							  // 12
		{Head.Book24[0], Head.Book24[1], Head.Book24[2], EMPTY_DATA},					  // 13
		{Head.u8FastHeat, DevSta, EMPTY_DATA, EMPTY_DATA},								  // 14
		{Head.u8BookSet, DevSta, EMPTY_DATA, EMPTY_DATA},								  // 15
		{Head.u8ECOSet, DevSta, EMPTY_DATA, EMPTY_DATA},								  // 16
		{Head.u8WaterControl, DevSta, EMPTY_DATA, EMPTY_DATA},							  // 17
		{Head.u8X6F1Burst, DevSta, EMPTY_DATA, EMPTY_DATA},								  // 18
		{Head.u8SurSet, DevSta, EMPTY_DATA, EMPTY_DATA},								  // 19
		{Head.u8SN_Comm, DevSta, EMPTY_DATA, EMPTY_DATA},								  // 20
		{Head.u8LimitMaxSet, DevSta, EMPTY_DATA, EMPTY_DATA},							  // 21
		{Head.u8X0LearnSet, DevSta, EMPTY_DATA, EMPTY_DATA},							  // 22
		{Head.u8PWorkTime, DevSta, EMPTY_DATA, EMPTY_DATA},								  // 23
		{Head.u8PriAsk, DevSta, EMPTY_DATA, EMPTY_DATA},								  // 24
		{Head.u8DrySet, DevSta, EMPTY_DATA, EMPTY_DATA},								  // 25
		{Head.u8CycleMode, DevSta, EMPTY_DATA, EMPTY_DATA},								  // 26
		{Head.u8BookFuncSet, DevSta, EMPTY_DATA, EMPTY_DATA},							  // 27
		{Head.u8MorWashStart, Head.u8MorWashEnd, Head.u8NigWashStart, Head.u8NigWashEnd}, // 28
		{EMPTY_DATA, DevSta, EMPTY_DATA, EMPTY_DATA},									  // 29
};
static const ST_CTRLDATA_T astExCtrlData[] =
	{
		{Head.u8FastDry, DevSta, EMPTY_DATA, EMPTY_DATA},					 // 0
		{Head.u8NightDry, DevSta, EMPTY_DATA, EMPTY_DATA},					 // 1
		{Head.u8KitTimeSet, DevSta, EMPTY_DATA, EMPTY_DATA},				 // 2
		{Head.u8HeatOnce, DevSta, EMPTY_DATA, EMPTY_DATA},					 // 3
		{Head.u8SOSCall, DevSta, EMPTY_DATA, EMPTY_DATA},					 // 4
		{Head.GasErrorSet, DevSta, EMPTY_DATA, EMPTY_DATA},					 // 5
		{Head.u8OneWayValveTmp, DevSta, Head.u8OneWayValveLost, EMPTY_DATA}, // 6
		{Head.u8OneWayValveDifTmp, DevSta, EMPTY_DATA, EMPTY_DATA},			 // 7
		{Head.u8HandMoveCapEn, DevSta, Head.u8HandMoveCapMode, EMPTY_DATA},	 // 8
		{Head.u8SelfCleanEn, DevSta, EMPTY_DATA, EMPTY_DATA},				 // 9
		{Head.u8SmartLearnEn, DevSta, Head.u8WaterUseTime, EMPTY_DATA},		 // 10
		{Head.u8BubbleWaterSet, DevSta, EMPTY_DATA, EMPTY_DATA}				 // 11
};

/**********************************************
 *
 *
 **********************************************/
ST_COMM_SET_T *GetCommSet(void)
{
	return &stCommSet;
}

/**********************************************
 *
 *
 **********************************************/
static void MainFrame0LoadData(void)
{

	uint8_t _u8temp;
	const ST_DBG_INFO_T *_pstGetDbgInfo = GetDbgInfo(stCommSet.u8DebugIndex); // 闁�?孩�?�?��鏍箹椤愶箑鏄ユ俊銈�?��缁犱即鏌涢妷顔煎�?��庢熬鎷�
	unMainBoardFrame.u8DataBuff[3] = GetSystemRunData()->sysSta.Byte;
	unMainBoardFrame.u8DataBuff[4] = _pstGetDbgInfo->stAddInfo.Byte;
	unMainBoardFrame.u8DataBuff[5] = stCommSet.u8DebugIndex;
	unMainBoardFrame.u8DataBuff[6] = _pstGetDbgInfo->u8CodeH;
	unMainBoardFrame.u8DataBuff[7] = _pstGetDbgInfo->u8CodeL;
	unMainBoardFrame.u8DataBuff[8] = (*_pstGetDbgInfo->pu8Data);

	if ((EN_CODE_d == _pstGetDbgInfo->u8CodeH) && (EN_CODE_E == _pstGetDbgInfo->u8CodeL))
	{
		unMainBoardFrame.u8DataBuff[9] = GetSystemRunData()->enFanType + 1;
	}
	else
	{
		unMainBoardFrame.u8DataBuff[9] = (uint8_t)(GetSystemRunData()->u16FanSpeed / 60);
	}

	if (EN_ERR_NO != GetSystemRunData()->u8ErrorCode)
		_u8temp = GetSystemRunData()->u8ErrorCode;
	else if (EN_ERR_NO != GetSystemRunData()->u8ErrorCodePump)
		_u8temp = GetSystemRunData()->u8ErrorCodePump;
	else
		_u8temp = EN_ERR_NO;
	unMainBoardFrame.u8DataBuff[10] = _u8temp;

	_u8temp = GetSystemRunData()->TmpOut / 10;
	if ((GetSystemRunData()->TmpOut % 10) > 5)
	{
		_u8temp++;
	}

	unMainBoardFrame.u8DataBuff[11] = _u8temp;
	unMainBoardFrame.u8DataBuff[12] = GetFlashDataSector0()->debugData.u8Fa;

	_u8temp = stCommSet.u8BurstMode; // 2024
	if (EN_PUMP_BURST == GetpPumpParm()->enState)
	{
		_u8temp |= 0x04;
	}
	unMainBoardFrame.u8DataBuff[13] = _u8temp;
}
/**********************************************
 *
 *
 **********************************************/
static void MainFrame1LoadData(void)
{

	uint8_t _u8temp;
	unMainBoardFrame.u8DataBuff[3] = 0;
	unMainBoardFrame.u8DataBuff[4] = SOFT_VERSION; // 2024
	unMainBoardFrame.u8DataBuff[5] = (uint8_t)(GetMaInfo()->pstFA->u16Liter / 10);
	unMainBoardFrame.u8DataBuff[6] = stCommSet.u8ModeSet;
	_u8temp = 0;
	if (0 < GetFlashDataSector0()->debugData.u8Ne)
	{
		_u8temp |= 0x80;
	}
	if (0 < GetWashModeData()->stWashSign.bBathFin) // 2024
	{
		_u8temp |= 0x08;
	}
	unMainBoardFrame.u8DataBuff[7] = _u8temp;
	unMainBoardFrame.u8DataBuff[8] = (uint8_t)(GetSystemRunData()->Flux);

	_u8temp = (uint8_t)(GetSystemRunData()->TmpIn / 10);
	if ((GetSystemRunData()->TmpIn % 10) > 5)
	{
		_u8temp++;
	}

	unMainBoardFrame.u8DataBuff[9] = _u8temp;
	unMainBoardFrame.u8DataBuff[10] = GetCommSet()->u8TmpSet;
	unMainBoardFrame.u8DataBuff[11] = GetCommSet()->u8FluxSet;
	unMainBoardFrame.u8DataBuff[12] = GetCommSet()->u8WaterSet;
	unMainBoardFrame.u8DataBuff[13] = GetDebugIndexMax() - 1;
}
/**********************************************
 *
 *
 **********************************************/
static void MainFrame2LoadData(void)
{

	unMainBoardFrame.u8DataBuff[3] = (uint8_t)(GetFunOutputs()->u16WaterUsed >> 8);
	unMainBoardFrame.u8DataBuff[4] = (uint8_t)(GetFunOutputs()->u16WaterUsed);
	unMainBoardFrame.u8DataBuff[5] = (uint8_t)(GetFunOutputs()->u16GasUsed >> 8);
	unMainBoardFrame.u8DataBuff[6] = (uint8_t)(GetFunOutputs()->u16GasUsed);
	unMainBoardFrame.u8DataBuff[7] = (uint8_t)(GetFunOutputs()->u32GasUseSum >> 24);
	unMainBoardFrame.u8DataBuff[8] = (uint8_t)(GetFunOutputs()->u32GasUseSum >> 16);
	unMainBoardFrame.u8DataBuff[9] = (uint8_t)(GetFunOutputs()->u32GasUseSum >> 8);
	unMainBoardFrame.u8DataBuff[10] = (uint8_t)(GetFunOutputs()->u32GasUseSum);
	unMainBoardFrame.u8DataBuff[11] = GetDebugFaMax() - 1;
	unMainBoardFrame.u8DataBuff[12] = (uint8_t)(GetWashModeData()->u16BathRemainLite >> 8);
	unMainBoardFrame.u8DataBuff[13] = (uint8_t)(GetWashModeData()->u16BathRemainLite);
}
/**********************************************
 *
 *
 **********************************************/

static void MainFrame3LoadData(void)
{

	uint8_t _u8temp;
	unMainBoardFrame.u8DataBuff[3] = GetFlashDataSector0()->debugData.u8Ph;
	unMainBoardFrame.u8DataBuff[4] = GetFlashDataSector0()->debugData.u8Pl;
	unMainBoardFrame.u8DataBuff[5] = ConvertCtrlToDbg_Blf(GetSystemRunData()->u16SetBlfI);

	unMainBoardFrame.u8DataBuff[6] = (uint8_t)(GetSystemRunData()->u16HeatLast_1s / 60);
	unMainBoardFrame.u8DataBuff[7] = (uint8_t)(GetFunOutputs()->u32WaterUseSum >> 24);
	unMainBoardFrame.u8DataBuff[8] = (uint8_t)(GetFunOutputs()->u32WaterUseSum >> 16);
	unMainBoardFrame.u8DataBuff[9] = (uint8_t)(GetFunOutputs()->u32WaterUseSum >> 8);
	unMainBoardFrame.u8DataBuff[10] = (uint8_t)(GetFunOutputs()->u32WaterUseSum);
	_u8temp = 0;
	if (0 < GetFunOutputs()->stSign.bProWindPress)
	{
		_u8temp |= (1 << 6);
	}
	//_u8temp |= (pstCtrlTmp_Comm->unStableSta.BIT.u2GasSta << 2);//2025
	//_u8temp |= (pstCtrlTmp_Comm->unStableSta.BIT.u2WaterSta);
	unMainBoardFrame.u8DataBuff[11] = _u8temp;

	if (temper_ntc_error_get(TMP_FDTX))
		_u8temp = 0;
	else
		_u8temp = (uint8_t)(GetSystemRunData()->TmpFD / 10);

	unMainBoardFrame.u8DataBuff[12] = _u8temp;
	_u8temp = 0;
	_u8temp |= (GetWashModeData()->stWashSign.bEnvErr << 7);
	_u8temp |= (GetWashModeData()->u8EnvSta << 4);
	_u8temp |= (GetFunOutputs()->stSign.bCoWarm << 3);
	_u8temp |= (GetFunOutputs()->u8CoSat);
	unMainBoardFrame.u8DataBuff[13] = _u8temp;
}
/**********************************************
 *
 *
 **********************************************/
static void MainFrame4LoadData(void)
{

	uint8_t _u8temp;
	unMainBoardFrame.u8DataBuff[3] = stCommSet.u8PreWarmTime;
	unMainBoardFrame.u8DataBuff[4] = stCommSet.u8PreWarmDif;
	unMainBoardFrame.u8DataBuff[5] = (uint8_t)(GetSystemRunData()->Flux);
	unMainBoardFrame.u8DataBuff[6] = 0;

	_u8temp = (uint8_t)(GetSystemRunData()->TmpIn / 10);

	unMainBoardFrame.u8DataBuff[7] = _u8temp;

	_u8temp = 0;
    
	if (EN_PUMP_UNWORK != GetpPumpParm()->enState)
	{
		_u8temp |= (1 << 1);
	}
	if (EN_PUMP_COLD == GetpPumpParm()->enState)
	{
		_u8temp |= (1 << 5);
	}
	if (0 < GetpPumpParm()->bSign.bColdErr)
	{
		_u8temp |= (1 << 3);
	}
	if (0 < GetFunOutputs()->stSign.bProAir)
	{
		_u8temp |= (1 << 6);
	}
	unMainBoardFrame.u8DataBuff[8] = _u8temp;
	unMainBoardFrame.u8DataBuff[9] = stCommSet.u8CoPPM;

	_u8temp = 0;
	if (1 == stCommSet.u8FastHeat)
	{
		_u8temp |= (1 << 3);
	}
	if (1 == stCommSet.u8BookSet)
	{
		_u8temp |= (1 << 4);
	}
	if (1 == stCommSet.u8ECOSet)
	{
		_u8temp |= (1 << 5);
	}
	// TODO:WIFI濠�?槅鍨界换婵�?极閹捐�?��柕鍫濇椤忓爼�?虹捄銊ユ瀾闁哄�?���?��涙倻閼恒儲娅㈤梺鍝�?��堕崐鏍偓�?秺閺屻劑鎮㈠畡鏉跨紦闁荤姳娴囬～澶屸偓姘炬�?
	if (1 == stCommSet.u8SurSet)
	{
		_u8temp |= (1 << 7);
	}
	if (0 < u8WifiOffLine_50ms)
	{
		_u8temp |= (1 << 6);
	}
	unMainBoardFrame.u8DataBuff[10] = _u8temp;
	unMainBoardFrame.u8DataBuff[11] = GetSystemRunData()->u8HourCur;
	unMainBoardFrame.u8DataBuff[12] = GetSystemRunData()->u8MinCur;
	unMainBoardFrame.u8DataBuff[13] = 0;
}
/**********************************************
 *
 *
 **********************************************/
static void MainFrame5LoadData(void)
{

	uint8_t _u8temp;
	uint16_t _u16temp;

	unMainBoardFrame.u8DataBuff[3] = stCommSet.unBook24Set.Bytes[0];
	unMainBoardFrame.u8DataBuff[4] = stCommSet.unBook24Set.Bytes[1];
	unMainBoardFrame.u8DataBuff[5] = stCommSet.unBook24Set.Bytes[2];
	unMainBoardFrame.u8DataBuff[6] = 0;
	_u8temp = 0;
	if (1 == stCommSet.u8WaterControl)
	{
		_u8temp |= (1 << 0);
	}
	if (1 == stCommSet.u8HeatOnce)
	{
		_u8temp |= (1 << 1);
	}
	if (1 == stCommSet.u8CycleMode)
	{
		_u8temp |= (1 << 3);
	}
	if ((0 < u8WifiOffLine_50ms) && (0 < GetSystemRunData()->sysSta.BIT.bOnOff))
	{
		_u8temp |= (1 << 4);
	}
	_u8temp |= (stCommSet.u8LimitMaxSet << 6);
	if (0xFF != GetPreHeatStudy())
	{
		_u8temp |= (1 << 5);
	}
	unMainBoardFrame.u8DataBuff[7] = _u8temp;
	unMainBoardFrame.u8DataBuff[8] = stCommSet.u8PWorkTime;
	_u16temp = GetPreHeatCD();
	unMainBoardFrame.u8DataBuff[9] = (uint8_t)_u16temp;
	unMainBoardFrame.u8DataBuff[10] = (uint8_t)(_u16temp >> 8);
	unMainBoardFrame.u8DataBuff[11] = GetPreHeatStudy();

	_u16temp = GetPreHeatTime();
	unMainBoardFrame.u8DataBuff[12] = (uint8_t)_u16temp;
	unMainBoardFrame.u8DataBuff[13] = (uint8_t)(_u16temp >> 8);
}
/**********************************************
 *
 *
 **********************************************/
static void MainFrame6LoadData(void)
{

	uint8_t _u8temp;

	unMainBoardFrame.u8DataBuff[3] = stCommSet.unBook24Set.Bytes[3];
	unMainBoardFrame.u8DataBuff[4] = stCommSet.unBook24Set.Bytes[4];
	unMainBoardFrame.u8DataBuff[5] = stCommSet.unBook24Set.Bytes[5];
	unMainBoardFrame.u8DataBuff[6] = stCommSet.u8CycleMode;
	_u8temp = 0;
	if (0 < stCommSet.u8FastDry)
	{
		_u8temp |= (1 << 0);
	}
	if (0 < stCommSet.u8NightDry)
	{
		_u8temp |= (1 << 2);
	}
	if (0 < stCommSet.u8SOSCall)
	{
		_u8temp |= (1 << 5);
	}
	if (0 < stCommSet.GasErrorSet)
	{
		_u8temp |= (1 << 6);
	}
	unMainBoardFrame.u8DataBuff[7] = _u8temp;

	_u8temp = 0;
	_u8temp = stCommSet.u8CH4PPM & 0x7F;
	if (0 < GetFunOutputs()->stSign.bCH4Warm)
	{
		_u8temp |= (1 << 7);
	}
	unMainBoardFrame.u8DataBuff[8] = _u8temp;
	unMainBoardFrame.u8DataBuff[9] = stCommSet.u8BookFuncSet;
	unMainBoardFrame.u8DataBuff[10] = stCommSet.u8MorWashStart;
	unMainBoardFrame.u8DataBuff[11] = stCommSet.u8MorWashEnd;
	unMainBoardFrame.u8DataBuff[12] = stCommSet.u8NigWashStart;
	unMainBoardFrame.u8DataBuff[13] = stCommSet.u8NigWashEnd;
}
/**********************************************
 *
 *
 **********************************************/
static void MainFrame7LoadData(void)
{

	uint8_t _u8temp;
	unMainBoardFrame.u8DataBuff[3] = stCommSet.u8KitTimeSet;
	unMainBoardFrame.u8DataBuff[4] = 0;
	unMainBoardFrame.u8DataBuff[5] = 0;
	_u8temp = 0;
	_u8temp |= (stCommSet.u8HandMoveCapMode << 0);
	_u8temp |= (stCommSet.u8HandMoveCapEn << 3);
	_u8temp |= (pGetValveState()->BYTE << 4);
	unMainBoardFrame.u8DataBuff[6] = _u8temp;
	unMainBoardFrame.u8DataBuff[7] = stCommSet.au8SNCode[0];
	unMainBoardFrame.u8DataBuff[8] = stCommSet.au8SNCode[1];
	unMainBoardFrame.u8DataBuff[9] = stCommSet.au8SNCode[2];
	unMainBoardFrame.u8DataBuff[10] = GetSystemRunData()->u8SecCur;
	_u8temp = 0;
	if (0 < stCommSet.u8SmartLearnEn)
	{
		_u8temp |= 1 << 5;
	}
	_u8temp |= (stCommSet.u8WaterUseTime);
	unMainBoardFrame.u8DataBuff[11] = _u8temp;
	unMainBoardFrame.u8DataBuff[12] = 0;
	unMainBoardFrame.u8DataBuff[13] = 0;
}
/**********************************************
 *
 *
 **********************************************/
static void MainFrame8LoadData(void)
{
	uint16_t _u16Tmp;
	uint8_t _u8Tmp;
	//���ת��
	unMainBoardFrame.u8DataBuff[3] = (GetSystemRunData()->u16FanSpeed >> 8);
	unMainBoardFrame.u8DataBuff[4] = GetSystemRunData()->u16FanSpeed;
	//���ռ�ձȣ�ת����ǧ�ֱȣ�
	_u16Tmp = GetDCFanSet();
	_u16Tmp = (uint32_t)_u16Tmp * 1000 / 12000;
	unMainBoardFrame.u8DataBuff[5] = _u16Tmp >> 8;
	unMainBoardFrame.u8DataBuff[6] = _u16Tmp;
	//��ѹֵ
	_u16Tmp = getAdapData()->airPressValue;
	unMainBoardFrame.u8DataBuff[7] = _u16Tmp >> 8;
	unMainBoardFrame.u8DataBuff[8] = _u16Tmp;
	//���������ȣ�ת��ǧ�ֱȣ�
	_u16Tmp = GetBlfCurOut();
	_u16Tmp = (uint32_t)_u16Tmp * 1000 / 6000;
	unMainBoardFrame.u8DataBuff[9] = _u16Tmp >> 8;
	unMainBoardFrame.u8DataBuff[10] = _u16Tmp;
	//EE״̬
	_u8Tmp = 0;
    if (GetFlashDataSector0()->debugData.u8Ee)
		_u8Tmp |= 0x01;
	_u8Tmp |= (getAdapData()->lowTemperGrade & 0x03) << 1;//����״̬
	_u8Tmp |= (getAdapData()->airPressGrade & 0x03) << 3;//����״̬
	unMainBoardFrame.u8DataBuff[11] = _u8Tmp;
	
	unMainBoardFrame.u8DataBuff[12] = 0;
	unMainBoardFrame.u8DataBuff[13] = 0;
}
/**********************************************
 *
 *
 **********************************************/
static void MainFrame9LoadData(void)
{
}
/**********************************************
 *
 *
 **********************************************/
static void MainFrame10LoadData(void)
{
	unMainBoardFrame.u8DataBuff[3] = stCommSet.au8SNCode[0];
	unMainBoardFrame.u8DataBuff[4] = stCommSet.au8SNCode[1];
	unMainBoardFrame.u8DataBuff[5] = stCommSet.au8SNCode[2];
	unMainBoardFrame.u8DataBuff[6] = stCommSet.au8SNCode[3];
	unMainBoardFrame.u8DataBuff[7] = stCommSet.au8SNCode[4];
	unMainBoardFrame.u8DataBuff[8] = stCommSet.au8SNCode[5];
	unMainBoardFrame.u8DataBuff[9] = stCommSet.au8SNCode[6];
	unMainBoardFrame.u8DataBuff[10] = stCommSet.au8SNCode[7];
	unMainBoardFrame.u8DataBuff[11] = stCommSet.au8SNCode[8];
	unMainBoardFrame.u8DataBuff[12] = 0;
	unMainBoardFrame.u8DataBuff[13] = 0;
}
/**********************************************
 *
 *
 **********************************************/
static void MainFrame11LoadData(void)
{
	/*
	uint16_t _u16temp;
	_u16temp = GetSysInput()->u16FanSpeed;
	unMainBoardFrame.u8DataBuff[3] = (uint8_t)_u16temp;
	unMainBoardFrame.u8DataBuff[4] = (uint8_t)(_u16temp >> 8);

	_u16temp = GetWorkFlowData()->u16SetDCFanI;
	unMainBoardFrame.u8DataBuff[5] = (uint8_t)_u16temp;
	unMainBoardFrame.u8DataBuff[6] = (uint8_t)(_u16temp >> 8);

	_u16temp = GetDCFanData()->u16FanUGetNonStk;
	unMainBoardFrame.u8DataBuff[7] = (uint8_t)_u16temp;
	unMainBoardFrame.u8DataBuff[8] = (uint8_t)(_u16temp >> 8);
	_u16temp = GetTmpCtrlData()->u16LoadPercent;
	unMainBoardFrame.u8DataBuff[9] = (uint8_t)_u16temp;
	unMainBoardFrame.u8DataBuff[10] = (uint8_t)(_u16temp >> 8);
	unMainBoardFrame.u8DataBuff[11] = GetNomalOutputs()->enFanType;
	unMainBoardFrame.u8DataBuff[12] = (uint8_t)(pstNomal_Comm->stSysRunData.s16TmpRun / 10);
	unMainBoardFrame.u8DataBuff[13] = GetDCFanData()->u8StkPercent;
	*/
}
/**********************************************
 *
 *
 **********************************************/
static void MainFrame12LoadData(void)
{
	/*
	uint16_t _u16temp;
	S16_T _s16temp;
	//_u16temp = GetSysInput()->u16TmpIn;
	_u16temp = (S16_T)GetLAdrcCal()->v1;
	unMainBoardFrame.u8DataBuff[3] = (uint8_t)_u16temp;
	unMainBoardFrame.u8DataBuff[4] = (uint8_t)(_u16temp >> 8);

	_s16temp = GetSysInput()->s16TmpOut;
	unMainBoardFrame.u8DataBuff[5] = (uint8_t)_s16temp;
	unMainBoardFrame.u8DataBuff[6] = (uint8_t)(_s16temp >> 8);

	_u16temp = GetSysInput()->u16WaterFlux;
	unMainBoardFrame.u8DataBuff[7] = (uint8_t)_u16temp;
	unMainBoardFrame.u8DataBuff[8] = (uint8_t)(_u16temp >> 8);

	_u16temp = GetTmpCtrlData()->u16OutLoad;
	unMainBoardFrame.u8DataBuff[9] = (uint8_t)_u16temp;
	unMainBoardFrame.u8DataBuff[10] = (uint8_t)(_u16temp >> 8);

	_u16temp = GetBLFRealOut();
	//_u16temp = GetSysInput()->u16WaterFluxCap;
	unMainBoardFrame.u8DataBuff[11] = (uint8_t)_u16temp;
	unMainBoardFrame.u8DataBuff[12] = (uint8_t)(_u16temp >> 8);

	unMainBoardFrame.u8DataBuff[13] = GetWorkFlowData()->enWorkFlowCur;
	*/
}
/**********************************************
 *
 *
 **********************************************/
static void MainFrame13LoadData(void)
{
	/*
	unMainBoardFrame.u8DataBuff[3] = pstFct_Comm->u8Step;
	unMainBoardFrame.u8DataBuff[4] = pstFct_Comm->u8End;
	unMainBoardFrame.u8DataBuff[5] = (uint8_t)(pstFct_Comm->u32Error >> 24);
	unMainBoardFrame.u8DataBuff[6] = (uint8_t)(pstFct_Comm->u32Error >> 16);
	unMainBoardFrame.u8DataBuff[7] = (uint8_t)(pstFct_Comm->u32Error >> 8);
	unMainBoardFrame.u8DataBuff[8] = (uint8_t)(pstFct_Comm->u32Error);
	unMainBoardFrame.u8DataBuff[9] = pstFct_Comm->u8ErrNum;
	unMainBoardFrame.u8DataBuff[10] = SOFT_VERSION;
	unMainBoardFrame.u8DataBuff[11] = GetFctStepMax();
	unMainBoardFrame.u8DataBuff[12] = GetFctComm();
	*/
}
/**********************************************
 *
 *
 **********************************************/
static void MainFrame14LoadData(void)
{
}
const uint8_t *GetFlashData_SNCode(void)
{
	return stCommSet.au8SNCode;
}
/**********************************************
 *
 *
 **********************************************/
static uint8_t Comm_Verify(uint8_t *_pu8DataPnt, uint8_t _u8datalength)
{
	uint8_t _u8ret = 0;
	uint8_t _u8i = 1;
	for (_u8i = 1; _u8i < (_u8datalength - 1); _u8i++)
	{
		_u8ret ^= *(_pu8DataPnt + _u8i);
	}
	return _u8ret;
}

/**********************************************
 *
 *
 **********************************************/
extern const DBG_MACH_DEF astDebugMachDef;
extern const ST_DBG_INFO_T astDbgInfo;
static void CtrlCommDeal(uint8_t _u8CtrlComm, uint8_t _SourAddr, uint8_t _dataBuf[4])
{
	uint8_t *_pu8pnt = (void *)0;
	switch (_u8CtrlComm)
	{
#if (CommReSetBathLite)
	case CTRLCOMM_WASHMODE:
		*astCtrlData[_u8CtrlComm].apu8Data[0] = _dataBuf[0];
		*astCtrlData[_u8CtrlComm].apu8Data[1] = _dataBuf[1];
		*astCtrlData[_u8CtrlComm].apu8Data[2] = _dataBuf[2];
		*astCtrlData[_u8CtrlComm].apu8Data[3] = _dataBuf[3];
		_pu8pnt = astCtrlData[_u8CtrlComm].apu8Data[1];
		ReSetBathLite();
		break;
#endif
	case CTRLCOMM_SETBOOKTIME:
		if (0x55 == _dataBuf[3])
		{
			stCommSet.unBook24Set.Bytes[0] = _dataBuf[0];
			stCommSet.unBook24Set.Bytes[1] = _dataBuf[1];
			stCommSet.unBook24Set.Bytes[2] = _dataBuf[2];
		}
		else if (0xaa == _dataBuf[3])
		{
			stCommSet.unBook24Set.Bytes[3] = _dataBuf[0];
			stCommSet.unBook24Set.Bytes[4] = _dataBuf[1];
			stCommSet.unBook24Set.Bytes[5] = _dataBuf[2];
		}

		break;
	case CTRLCOMM_SN:
		stCommSet.u8SN_Comm = _dataBuf[0];
		// u8temp = GetSnUsed();
		if (SN_READ == stCommSet.u8SN_Comm)
		{
			SnSendCnt = 5;
			// if (EN_MAIN_FRAME_10 != enFlameSequence)
			//{
			// SetFrame(10, EN_MAIN_FRAME_10);
			//}
		}
		else if (SN_FCT == stCommSet.u8SN_Comm)
		{
			//	SetFctStart();
			// SetFctComm();
		}
		else if (SN_FCT_STEP == stCommSet.u8SN_Comm)
		{
			//	SetFctStep(_dataBuf[1]);
			// SetFctComm();
		}
		// else if (SN_NULL == _u8temp)
		//{
		if (SN_WRITE1 == stCommSet.u8SN_Comm)
		{
			stCommSet.au8SNCode[0] = _dataBuf[1];
			stCommSet.au8SNCode[1] = _dataBuf[2];
			stCommSet.au8SNCode[2] = _dataBuf[3];
		}
		else if (SN_WRITE2 == stCommSet.u8SN_Comm)
		{
			stCommSet.au8SNCode[3] = _dataBuf[1];
			stCommSet.au8SNCode[4] = _dataBuf[2];
			stCommSet.au8SNCode[5] = _dataBuf[3];
		}
		else if (SN_WRITE3 == stCommSet.u8SN_Comm)
		{
			stCommSet.au8SNCode[6] = _dataBuf[1];
			stCommSet.au8SNCode[7] = _dataBuf[2];
			stCommSet.au8SNCode[8] = _dataBuf[3];
		}
		SNReSet();
		//}
		break;
	case CTRLCOMM_EXCOMM:
        if (_dataBuf[3] < ARR_NUM(astExCtrlData))
		{
			*astExCtrlData[_dataBuf[3]].apu8Data[0] = _dataBuf[0];
			*astExCtrlData[_dataBuf[3]].apu8Data[1] = _dataBuf[1];
			*astExCtrlData[_dataBuf[3]].apu8Data[2] = _dataBuf[2];
			_pu8pnt = astExCtrlData[_dataBuf[3]].apu8Data[1];
		}
		break;
	default:
		*astCtrlData[_u8CtrlComm].apu8Data[0] = _dataBuf[0];
		*astCtrlData[_u8CtrlComm].apu8Data[1] = _dataBuf[1];
		*astCtrlData[_u8CtrlComm].apu8Data[2] = _dataBuf[2];
		*astCtrlData[_u8CtrlComm].apu8Data[3] = _dataBuf[3];
		_pu8pnt = astCtrlData[_u8CtrlComm].apu8Data[1];
		break;
	}
	if (_pu8pnt == DevSta)
	{
		stCommSet.au8AddrOnlineCnt[_SourAddr] = u8DevOnOffSta;
	}
	GetCommDebugData(&stCommSet);

	/*
	if (EN_ADDR_MONITOR == _SourAddr)
	{
		u8MonitorOnLine_50ms = 100;
		PushSpecFrame(EN_MAIN_FRAME_11);
		PushSpecFrame(EN_MAIN_FRAME_12);
	}
	if (0 < u8MonitorOnLine_50ms)
	{
		if (u8LastDebugIndex != stCommSet.u8DebugIndex)
		{
			u8AddDebugInfo_50ms = 40;
		}
	}
	else
	{
		if ((0 != stCommSet.u8DebugIndex) && (GetDebugIndexMax() > stCommSet.u8DebugIndex))
		{
			u8AddDebugInfo_50ms = 100;
		}
	}*
	u8LastDebugIndex = stCommSet.u8DebugIndex;
	 */
	if (EN_ADDR_ALARM == _SourAddr)
	{
		u16AlarmOffLine_50ms = 600;
		stCommSet.au8AddrOnlineCnt[EN_ADDR_ALARM] = 0x02;
	}
	else if (EN_ADDR_WIFI == _SourAddr)
	{
		u8WifiOffLine_50ms = 100;
	}

	if ((CTRLCOMM_STANDY != _u8CtrlComm) && (CTRLCOMM_GASALARM != _u8CtrlComm))
	{
		u16PriorityReset_50ms = 6000;
	}
}
static void PriorDeal(void)
{
	if (PRI_NONE == stCommSet.u8PriDev)
	{
		if (0 < stCommSet.u8PriAsk)
		{
			stCommSet.u8PriDev = unControllerFrame.stController.u3SourAddr;
		}
	}
	else
	{
		if (stCommSet.u8PriDev == unControllerFrame.stController.u3SourAddr)
		{
			if ((0 == stCommSet.au8AddrOnlineCnt[stCommSet.u8PriDev]) || ((CTRLCOMM_SETONOFF == unControllerFrame.stController.u5DeviceComm) && ((0 == stCommSet.u8OnOffSet))))
			{
				stCommSet.u8PriDev = PRI_NONE;
			}
		}
	}
	stCommSet.u8PriAsk = 0;
}
void user_comm_operator_unpack_receive(uint8_t *pBuf)
{
	memcpy(unControllerFrame.au8DataBuff, pBuf, sizeof(unControllerFrame.au8DataBuff));
	CtrlCommDeal(unControllerFrame.stController.u5DeviceComm,
				 unControllerFrame.stController.u3SourAddr,
				 unControllerFrame.stController.au8CommData);
}
void user_comm_operator_check_receive(UsartIndex index)
{
	UsartBufferStruct *pReceiveData = usartGetReceive(index);
	if (pReceiveData == 0) // 闂備浇顫夋�?浠�?磿閺屻儱鏋侀柣鐔稿閺岋箓鏌嶉妷銉э紞闁哄棙宀搁弻鐔告綇閻愵剙顏�
	{
		return;
	}
	if (*pReceiveData->pBuffer != COMM_HEAD_CODE) // 闂佹眹鍩�?��搴ㄥ礃閿旇法�?��┑鐐村灦閹�?摜绮旈幘顕呮�?��跨噦鎷�
	{
		usartClearReceive(index);
		return;
	}
	if (pReceiveData->counter != 8) // 闂佹眹鍩�?��搴ㄥ炊閵婏�?妲梺纭�?��閹活亞绱撳杈╃當濠㈣埖鍔曢崣濠囨煥閻曞倹瀚�8濠电偞鍨堕幖鈺傜濠婂牊鍋ょ憸鏃堝箠閻愮儤鏅搁柨鐕傛�?
	{
		usartClearReceive(index);
		return;
	}

	if (*(pReceiveData->pBuffer + pReceiveData->counter - 1) != Comm_Verify(pReceiveData->pBuffer, 8)) // 闂備礁鎼粙鍕崲濠靛鍋樻繛鍡樻尭濡炰粙鎮橀悙璺轰汗闁荤喐绻堥幃妤冨枈�?跺顏�
	{
		usartClearReceive(index);
		return;
	}
	if (0 == u16PriorityReset_50ms)
	{
		stCommSet.u8Priority = PRI_NONE;
	}
	if (0 == u16AlarmOffLine_50ms)
	{
		stCommSet.au8AddrOnlineCnt[EN_ADDR_ALARM] = 0x00;
	}
	if ((0 < unControllerFrame.stController.u5DeviceComm) && (EN_ADDR_ALARM != unControllerFrame.stController.u3SourAddr))
	{
		stCommSet.u8Priority = unControllerFrame.stController.u3SourAddr;
	}
	user_comm_operator_unpack_receive(pReceiveData->pBuffer);
	usartClearReceive(BOARD_UART);
	PriorDeal();
}

static void Comm_Recv_Deal(void)
{
	user_comm_operator_check_receive(BOARD_UART);
}
/**********************************************
 *
 *
 **********************************************/

static void Comm_Send_LoadData(void)
{

	EN_DEVADDR_T drvAddr = EN_ADDR_CTRL;
	EN_MAIN_FRAME_T frame_Index = EN_MAIN_FRAME_0;
	memset(unMainBoardFrame.u8DataBuff, 0, sizeof(unMainBoardFrame.u8DataBuff));
	fifoRead(&s_AddrFifo, &drvAddr);
	unMainBoardFrame.stMainBoard.u3TarDevice = drvAddr;
	fifoRead(&s_FrameFifo, &frame_Index);
	unMainBoardFrame.stMainBoard.u5MainComm = frame_Index;
	unMainBoardFrame.stMainBoard.u8Head = COMM_HEAD_CODE;
	unMainBoardFrame.stMainBoard.u3SourAddr = EN_ADDR_MAIN;
	unMainBoardFrame.stMainBoard.u3PriDevice = stCommSet.u8Priority;

	(*afnMainFrameLoadData[frame_Index])();
	unMainBoardFrame.stMainBoard.u8CheckSum = Comm_Verify(unMainBoardFrame.u8DataBuff, sizeof(unMainBoardFrame.u8DataBuff));
	usartSend(BOARD_UART, unMainBoardFrame.u8DataBuff, sizeof(unMainBoardFrame.u8DataBuff));
}

static void Comm_Send_Sequence(void)
{
	uint8_t addrIndex = 0;
	uint8_t frameIndex = 0;
#if (SpecMessage)
	static uint8_t u8SpecMsgSendCnt= 0;
#endif
	static uint8_t comm_send_sequence_debug_ctrl = 0;
	if (!s_AddrFifo.usedSize)
	{
		if (++stFlameSet.u4AddrPnt >= sizeof(au8CtrlAddrList))
			stFlameSet.u4AddrPnt = 0;
		addrIndex = au8CtrlAddrList[stFlameSet.u4AddrPnt];
		fifoWrite(&s_AddrFifo, &addrIndex);
	}

	if (!s_FrameFifo.usedSize)
	{
		if (SnSendCnt > 0)
		{
			SnSendCnt--;
			frameIndex = EN_MAIN_FRAME_10;
			fifoWrite(&s_FrameFifo, &frameIndex);
		}
		else
		{
			if ((0 != stCommSet.u8DebugIndex) && (GetDebugIndexMax() > stCommSet.u8DebugIndex)) // 濠电姰鍨煎▔娑氣偓�?间簽閼洪亶顢楅埀顒勵敋閿濆浼犻柛鏇ㄥ亞閸橆剙鈹戦埥鍡�?仚闁逞屽�?��绢厾娑甸埀锟�,闂備礁鎲″缁樻叏閹绢喖绠甸柍鍝�?��缁犳棃鏌ㄩ悤鍌涘�?0闂佹眹鍩�?��搴ㄥ礃閵娧冪煗闂傚倷绶￠崑鍌滄濮樿翰鈧倿宕奸弴鐔蜂虎闂佽法鍣﹂幏锟�?
			{
				if (comm_send_sequence_debug_ctrl)
				{
					frameIndex = aenMainFrameList[0];
					comm_send_sequence_debug_ctrl = 0;
				}
				else
				{
					comm_send_sequence_debug_ctrl = 1;
					if (++stFlameSet.u4FlamePnt >= sizeof(aenMainFrameList))
						stFlameSet.u4FlamePnt = 0;
					frameIndex = aenMainFrameList[stFlameSet.u4FlamePnt];
				}
			}
			else
			{
                //�����·���֡��
#if (SpecMessage)
				if (++u8SpecMsgSendCnt > SpecMessageTime)
				{
					u8SpecMsgSendCnt = 0;
					frameIndex = EN_MAIN_FRAME_8;
				}
				else
				{
					if (++stFlameSet.u4FlamePnt >= sizeof(aenMainFrameList))
					stFlameSet.u4FlamePnt = 0;
					frameIndex = aenMainFrameList[stFlameSet.u4FlamePnt];
				}
				comm_send_sequence_debug_ctrl = 0;
#else		
				if (++stFlameSet.u4FlamePnt >= sizeof(aenMainFrameList))
					stFlameSet.u4FlamePnt = 0;
				comm_send_sequence_debug_ctrl = 0;
				frameIndex = aenMainFrameList[stFlameSet.u4FlamePnt];
#endif
			}
			fifoWrite(&s_FrameFifo, &frameIndex);
		}
	}
}
void comm_tick(void) // 1MS
{
	static uint8_t comm_tick_50ms = 0;
	INC_B(stCommRunData.u8SendDelay);
	if (++comm_tick_50ms >= 50)
	{
		comm_tick_50ms = 0;
		DEC(u8WifiOffLine_50ms);
		DEC(u16PriorityReset_50ms);
		DEC(u16AlarmOffLine_50ms);
	}
}
void Comm_Send(void)
{
	//	uint8_t _u8delayset;
	//	if (0 < u8MonitorOnLine_50ms)
	//{
	//	_u8delayset = 1;
	//	}
	// else
	//{
	//_u8delayset = 3;
	//}
	// if (_u8delayset <= stCommRunData.u8SendDly_50ms)
	//{
	//	stCommRunData.u8SendDly_50ms = 0;
	if (stCommRunData.u8SendDelay >= 150)
	{
		stCommRunData.u8SendDelay = 0;
		Comm_Send_Sequence();
		Comm_Send_LoadData();
	}
	//}
}

void Comm_Recv(void) // 1MS
{

	Comm_Recv_Deal();
}

/**********************************************
 *
 **********************************************/

void Comm_Init(void)
{
	memset(&stCommSet, 0, sizeof(stCommSet));
	memset(&stCommRunData, 0, sizeof(stCommRunData));
	//	_pu8temp = (GetpFlhRead(EN_FLH_SN, SN_CODE_NUM));
	fifoInit(&s_FrameFifo, &s_FrameFifoBuf, POWERBOARD_FIFOBUF_LENGTH);
	fifoInit(&s_AddrFifo, &s_AddrFifoBuf, POWERBOARD_FIFOBUF_LENGTH);

	stCommSet.u8PriDev = PRI_NONE;
	stCommSet.u8Priority = PRI_NONE;
	stCommSet.GasErrorSet = 1;
	stCommSet.u8ModeSet = COMMSET_NULL;
	stCommSet.u8MinCur = COMMSET_NULL;
	stCommSet.u8HourCur = COMMSET_NULL;
}
