/*************************************************************************
 * Copyright:    WJL Kitchenware.
 * File:         singlecomm.c
 * Layer:
 * Author:       lxj
 * Version:      0.01
 * Brief:		    烟机4线通讯协议V9.0
 *************************************************************************/
#include "all_head.h"
#define COMM_SEND_TIME_TICK 20
#define COMM_SEND_FRAME_TICK 12
typedef enum
{
	DisBoardRead_PowerID,		 // 电源板地址
	DisBoardRead_temp1,			 // 温度探头1
	DisBoardRead_temp2,			 // 温度探头2
	DisBoardRead_temp3,			 // 温度探头3
	DisBoardRead_hum,			 // 湿度值
	DisBoardRead_pm25,			 // PM25浓度
	DisBoardRead_smoke,			 // 烟浓度
	DisBoardRead_smokeTemp,		 // 烟感温度值
	DisBoardRead_MotorCurrent,	 // 电机电流值
	DisBoardRead_SenserState,	 // 挡板位置(传感器状态)
	DisBoardRead_linkData,		 // 联动数据
	DisBoardRead_DoorMotorState, // 直流电机开合状态
	DisBoardRead_DcMotorRate,	 // 直流变频电机转速
	DisBoardRead_sum,
} DisBoardReadDataType; // 显示板读取数据类型

typedef enum
{
	DisBoardReadPowerData = 0x00,	  // 显示板读取电源板数据
	DisBoardCtrl_MainFunc = 0x08,	  // 显示板控制主功能
	DisBoardCtrl_AssistFunc = 0x09,	  // 显示板控制辅助功能
	DisBoardCtrl_DoorMotorSet = 0x0a, // 显示板控制电机开合数据
	DisBoardCtrl_sum,
} DisBoardCtrlDataType; // 显示板控制数据类型
typedef enum
{
	COMM_CTRL_CMD_NULL,
	COMM_CTRL_READ_ADDR,				   // 读地址
	COMM_CTRL_WRITE_DOOR_MOTOR_OPEN_DATA,  // 写开合电机开门数据
	COMM_CTRL_WRITE_DOOR_MOTOR_CLOSE_DATA, // 写开合电机关门数据
	COMM_CTRL_READ_DOOR_MOTOR_STATE,	   // 读开合电机更新状态
	COMM_CTRL_WRITE_MAIN,				   // 读主
	COMM_CTRL_WRITE_ASSIS,				   // 读辅
	COMM_CTRL_READ_TEMP1,
	COMM_CTRL_READ_TEMP2,
	COMM_CTRL_READ_TEMP3,
	COMM_CTRL_READ_HUM,
	COMM_CTRL_READ_PM25,
	COMM_CTRL_READ_SMOKE,
	COMM_CTRL_READ_SMOKE_TEMP,
	COMM_CTRL_READ_MOTOR_CURRENT,
	COMM_CTRL_READ_SENSER_STATE,
	COMM_CTRL_READ_LINK_DATA,
	COMM_CTRL_READ_DCMOTOR_RATE,
	COMM_CTRL_CMD_SUM,
} COMM_CTRL_CMD; // 控制命令
typedef enum
{
	COMM_CTRL_STATE_READ_ADDR,
	COMM_CTRL_STATE_WRITE_DOOR_MOTOR_OPEN,
	COMM_CTRL_STATE_WRITE_DOOR_MOTOR_CLOSE,
	COMM_CTRL_STATE_READ_DOOR_MOTOR_STATE,
	COMM_CTRL_STATE_DATA,
	COMM_CTRL_STATE_SUM,
} COMM_CTRL_STATE;
typedef struct
{
	COMM_CTRL_CMD cmd;
	uint8_t readAddrCnt1;
	uint8_t readAddrCnt2;
	uint8_t doorCnt1;
	uint8_t dataCnt1;
	uint8_t dataCnt2;
	uint8_t dataCnt3;
	uint8_t sendTimeTick;
} commCtrl_TypeDef;

commCtrl_TypeDef commCtrl_t;
DisBoardCtrlMainFuncData DisBoardCtrlMainFuncData_st;
DisBoardCtrlAssistFuncData DisBoardCtrlAssistFuncData_st;
DisBoardCtrlDoorMotorData DisBoardCtrlDoorMotorData_st;
PowBoardReCallData PowBoardReCallData_st;
/*
PowBoardReCallData *PowBoardReCallDataGet(void)
{
	return &PowBoardReCallData_st;
}*/
DisBoardCtrlMainFuncData *DisBoardCtrlMainFuncDataGet(void)
{
	return &DisBoardCtrlMainFuncData_st;
}
DisBoardCtrlDoorMotorData *DisBoardCtrlDoorMotorDataGet(void)
{
	return &DisBoardCtrlDoorMotorData_st;
}
DisBoardCtrlAssistFuncData *DisBoardCtrlAssistFuncDataGet(void)
{
	return &DisBoardCtrlAssistFuncData_st;
}
// 校验和
unsigned char singleCommCheckSumCalc(unsigned char *pData, unsigned char len)
{
	unsigned char temp = 0, i;
	temp = (*pData) & 0X0F;
	for (i = 1; i < len; i++)
	{
		temp += (*(pData + i)) & 0X0F;
		temp += ((*(pData + i)) >> 4) & 0X0F;
	}
	return temp & 0x0f;
}

// 发送处理
void CommSendProcessHandle(void) // 5MS
{
	unsigned char checkSum = 0;
	sinleCommData *pSendData = sinleCommDataGet();
	uint8_t sendCmd = 0;
	if (commCtrl_t.sendTimeTick < COMM_SEND_FRAME_TICK)
		commCtrl_t.sendTimeTick++;
	if (pSendData->sendIsBusy)
	{
		commCtrl_t.sendTimeTick = 0;
		return; // 未发送完成
	}
	if (commCtrl_t.sendTimeTick < COMM_SEND_FRAME_TICK) // 桢间间隔时间至少60ms
		return;
	commCtrl_t.sendTimeTick = 0;
	sendCmd = GetCommEvent();
	if (!sendCmd) // 队列没有需要发送的数据
		return;
	memset(&pSendData->sendBuff, 0, sizeof(pSendData->sendBuff));
	switch (sendCmd)
	{
	case COMM_CTRL_READ_ADDR: // 读数据
		pSendData->sendBuff[0] = 0;
		pSendData->sendBuff[1] = DisBoardRead_PowerID;
		pSendData->SendLen = 2;
		break;
	case COMM_CTRL_READ_DOOR_MOTOR_STATE:
		pSendData->sendBuff[0] = 0;
		pSendData->sendBuff[1] = DisBoardRead_DoorMotorState;
		pSendData->SendLen = 2;
		break;
	case COMM_CTRL_READ_TEMP1:
		pSendData->sendBuff[0] = 0;
		pSendData->sendBuff[1] = DisBoardRead_temp1;
		pSendData->SendLen = 2;
		break;
	case COMM_CTRL_READ_TEMP2:
		pSendData->sendBuff[0] = 0;
		pSendData->sendBuff[1] = DisBoardRead_temp2;
		pSendData->SendLen = 2;
		break;
	case COMM_CTRL_READ_TEMP3:
		pSendData->sendBuff[0] = 0;
		pSendData->sendBuff[1] = DisBoardRead_temp3;
		pSendData->SendLen = 2;
		break;
	case COMM_CTRL_READ_HUM:
		pSendData->sendBuff[0] = 0;
		pSendData->sendBuff[1] = DisBoardRead_hum;
		pSendData->SendLen = 2;
		break;
	case COMM_CTRL_READ_PM25:
		pSendData->sendBuff[0] = 0;
		pSendData->sendBuff[1] = DisBoardRead_pm25;
		pSendData->SendLen = 2;
		break;
	case COMM_CTRL_READ_SMOKE:
		pSendData->sendBuff[0] = 0;
		pSendData->sendBuff[1] = DisBoardRead_smoke;
		pSendData->SendLen = 2;
		break;
	case COMM_CTRL_READ_SMOKE_TEMP:
		pSendData->sendBuff[0] = 0;
		pSendData->sendBuff[1] = DisBoardRead_smokeTemp;
		pSendData->SendLen = 2;
		break;
	case COMM_CTRL_READ_MOTOR_CURRENT:
		pSendData->sendBuff[0] = 0;
		pSendData->sendBuff[1] = DisBoardRead_MotorCurrent;
		pSendData->SendLen = 2;
		break;
	case COMM_CTRL_READ_SENSER_STATE:
		pSendData->sendBuff[0] = 0;
		pSendData->sendBuff[1] = DisBoardRead_SenserState;
		pSendData->SendLen = 2;
		break;
	case COMM_CTRL_READ_LINK_DATA:
		pSendData->sendBuff[0] = 0;
		pSendData->sendBuff[1] = DisBoardRead_linkData;
		pSendData->SendLen = 2;
		break;
	case COMM_CTRL_READ_DCMOTOR_RATE:
		pSendData->sendBuff[0] = 0;
		pSendData->sendBuff[1] = DisBoardRead_DcMotorRate;
		pSendData->SendLen = 2;
		break;
	case COMM_CTRL_WRITE_DOOR_MOTOR_CLOSE_DATA: // 显示板控制关门
		DoorMotorCloseConfig();
		pSendData->SendLen = 4;
		pSendData->sendBuff[0] = DisBoardCtrl_DoorMotorSet;
		memcpy(&pSendData->sendBuff[1], &DisBoardCtrlDoorMotorData_st.ucData, pSendData->SendLen - 1);
		break;
	case COMM_CTRL_WRITE_DOOR_MOTOR_OPEN_DATA: // 显示板控制开门
		DoorMotorOpenConfig();
		pSendData->SendLen = 4;
		pSendData->sendBuff[0] = DisBoardCtrl_DoorMotorSet;
		memcpy(&pSendData->sendBuff[1], &DisBoardCtrlDoorMotorData_st.ucData, pSendData->SendLen - 1);
		break;
	case COMM_CTRL_WRITE_MAIN: // 主功能控制
		pSendData->SendLen = 4;
		pSendData->sendBuff[0] = DisBoardCtrl_MainFunc;
		memcpy(&pSendData->sendBuff[1], &DisBoardCtrlMainFuncData_st.ucData, pSendData->SendLen - 1);
		DisBoardCtrlMainFuncData_st.xsData.buzzer = BEEP_NONE;
		break;
	case COMM_CTRL_WRITE_ASSIS: // 辅助功能
		pSendData->SendLen = 4;
		pSendData->sendBuff[0] = DisBoardCtrl_AssistFunc;
		memcpy(&pSendData->sendBuff[1], &DisBoardCtrlAssistFuncData_st.ucData, pSendData->SendLen - 1);
		break;
	default:
		break;
	}
	checkSum = singleCommCheckSumCalc(&pSendData->sendBuff, pSendData->SendLen);
	pSendData->sendBuff[0] |= checkSum << 4;
	pSendData->allowSend = 1;
}
// 接收处理函数
void CommRevProcessHandle(void)
{
	sinleCommData *pRevData = sinleCommDataGet();
	if (!pRevData->validRev) // 接收数据有效
		return;
	pRevData->validRev = 0;
	if ((pRevData->revBuff[0] >> 4) != singleCommCheckSumCalc(&pRevData->revBuff, pRevData->revLen)) // 校验
		return;
	switch (pRevData->revBuff[0] & 0x0f)
	{
	case DisBoardRead_PowerID:
		if (pRevData->revLen != 2)
			return;
		PowBoardReCallData_st.id = pRevData->revBuff[1];
		break;
	case DisBoardRead_temp1:
		if (pRevData->revLen != 2)
			return;
		PowBoardReCallData_st.temp1 = pRevData->revBuff[1];
		break;
	case DisBoardRead_temp2:
		if (pRevData->revLen != 2)
			return;
		PowBoardReCallData_st.temp2 = pRevData->revBuff[1];
		break;
	case DisBoardRead_temp3:
		if (pRevData->revLen != 2)
			return;
		PowBoardReCallData_st.temp3 = pRevData->revBuff[1];
		break;
	case DisBoardRead_hum:
		if (pRevData->revLen != 2)
			return;
		PowBoardReCallData_st.hum = pRevData->revBuff[1];
		break;
	case DisBoardRead_pm25:
		if (pRevData->revLen != 2)
			return;
		PowBoardReCallData_st.pm25 = pRevData->revBuff[1];
	case DisBoardRead_smoke:
		if (pRevData->revLen != 2)
			return;
		PowBoardReCallData_st.smoke = pRevData->revBuff[1];
		break;
	case DisBoardRead_smokeTemp:
		if (pRevData->revLen != 2)
			return;
		PowBoardReCallData_st.smokeTemp = pRevData->revBuff[1];
		break;
	case DisBoardRead_MotorCurrent:
		if (pRevData->revLen != 2)
			return;
		PowBoardReCallData_st.MotorCurrent = pRevData->revBuff[1];
		break;
	case DisBoardRead_SenserState:
		if (pRevData->revLen != 2)
			return;
		PowBoardReCallData_st.SenserState.ucData = pRevData->revBuff[1];
		break;
	case DisBoardRead_linkData:
		if (pRevData->revLen != 2)
			return;
		PowBoardReCallData_st.linkData.ucData = pRevData->revBuff[1];
		break;
	case DisBoardRead_DoorMotorState:
		if (pRevData->revLen != 2)
			return;
		PowBoardReCallData_st.DoorMotorState.ucData = pRevData->revBuff[1];
		break;
	case DisBoardRead_DcMotorRate:
		if (pRevData->revLen != 2)
			return;
		PowBoardReCallData_st.DcMotorRate = pRevData->revBuff[1];
		break;
	default:
		break;
	}
	memset(&pRevData->revBuff, 0, sizeof(pRevData->revBuff));
	sysStateRunGet()->connect.revDataFlag = 1; // 接收数据成功标志
}

void commSendCtrlCmd(COMM_CTRL_CMD sendCmd)
{
	TriggerCommEvent((uint8_t)sendCmd);
}

void CommSendCtrlHandle(void)
{
	static volatile CommSendCtrlcmdLast = 0xff;
	if (CommSendCtrlcmdLast != commCtrl_t.cmd)
	{
		CommSendCtrlcmdLast = commCtrl_t.cmd;
		commCtrl_t.dataCnt3 = 0;
		commCtrl_t.dataCnt2 = 0;
		commCtrl_t.dataCnt1 = 0;
		commCtrl_t.readAddrCnt1 = 0;
		commCtrl_t.readAddrCnt2 = 0;
		commCtrl_t.doorCnt1 = 0;
	}
	switch (commCtrl_t.cmd)
	{
	case COMM_CTRL_STATE_READ_ADDR:
		if (++commCtrl_t.readAddrCnt1 >= COMM_SEND_TIME_TICK)
		{
			commCtrl_t.readAddrCnt1 = 0;
			commSendCtrlCmd(COMM_CTRL_READ_ADDR);
			if (commCtrl_t.readAddrCnt2 < 5)
				commCtrl_t.readAddrCnt2++;
		}
		if (commCtrl_t.readAddrCnt2 >= 5) // 开始连续发送5次地址查询指令
		{
			if (PowBoardReCallData_st.id) // 获取地址成功
			{
				// 判断地址码，如果是带开合的机型则进行开合电机配置
				commCtrl_t.cmd = COMM_CTRL_STATE_WRITE_DOOR_MOTOR_OPEN;
				// 如果是不带开合的机型直接跳转到
				// commCtrl_t.cmd = COMM_CTRL_STATE_DATA;
			}
			commCtrl_t.readAddrCnt2 = 0;
		}
		break;
	case COMM_CTRL_STATE_WRITE_DOOR_MOTOR_OPEN: // 写开门数据
		commSendCtrlCmd(COMM_CTRL_WRITE_DOOR_MOTOR_OPEN_DATA);
		commCtrl_t.cmd = COMM_CTRL_STATE_WRITE_DOOR_MOTOR_CLOSE;
		break;
	case COMM_CTRL_STATE_WRITE_DOOR_MOTOR_CLOSE: // 写关门数据
		commSendCtrlCmd(COMM_CTRL_WRITE_DOOR_MOTOR_CLOSE_DATA);
		commCtrl_t.cmd = COMM_CTRL_STATE_READ_DOOR_MOTOR_STATE;
		break;
	case COMM_CTRL_STATE_READ_DOOR_MOTOR_STATE: // 读配置状态
		commSendCtrlCmd(COMM_CTRL_READ_DOOR_MOTOR_STATE);
		// commCtrl_t.cmd = COMM_CTRL_STATE_READ_DOOR_MOTOR_STATE;
		// 开合电机配置成功
		if (PowBoardReCallData_st.DoorMotorState.bits.DoorCloseSetUpdate && PowBoardReCallData_st.DoorMotorState.bits.DoorOpenSetUpdate)
		{
			commCtrl_t.cmd = COMM_CTRL_STATE_DATA;
		}
		else
		{
			// 带开合款若操作器收到控制器地址码但未收到直流电机状态信息，仍自检成功，后续连续写3次开合电机数据配置
			if (++commCtrl_t.doorCnt1 >= 3)
			{
				commCtrl_t.doorCnt1 = 0;
				commCtrl_t.cmd = COMM_CTRL_STATE_DATA;
			}
			else
			{
				commCtrl_t.cmd = COMM_CTRL_STATE_WRITE_DOOR_MOTOR_OPEN;
			}
		}
		break;
	case COMM_CTRL_STATE_DATA:
		// 写主-写辅-读1-读2-写主-写辅-读3-读4
		if (++commCtrl_t.dataCnt1 >= COMM_SEND_TIME_TICK)
		{
			commCtrl_t.dataCnt1 = 0;
			switch (commCtrl_t.dataCnt2)
			{
			case 0:
				commSendCtrlCmd(COMM_CTRL_WRITE_MAIN);
				break;
			case 1:
				commSendCtrlCmd(COMM_CTRL_WRITE_ASSIS);
				break;
			case 2:
				if (commCtrl_t.dataCnt3 >= (COMM_CTRL_CMD_SUM - COMM_CTRL_READ_TEMP1))
					break; // 超限
				commSendCtrlCmd(COMM_CTRL_READ_TEMP1 + commCtrl_t.dataCnt3);
				break;
			case 3:
				if (commCtrl_t.dataCnt3 + 1 >= (COMM_CTRL_CMD_SUM - COMM_CTRL_READ_TEMP1))
					break; // 超限
				commSendCtrlCmd(COMM_CTRL_READ_TEMP1 + commCtrl_t.dataCnt3 + 1);
				break;
			default:
				break;
			}
			if (++commCtrl_t.dataCnt2 >= 4)
			{
				commCtrl_t.dataCnt3 += 2;
				if (commCtrl_t.dataCnt3 >= (COMM_CTRL_CMD_SUM - COMM_CTRL_READ_TEMP1))
					commCtrl_t.dataCnt3 = 0;
				commCtrl_t.dataCnt2 = 0;
			}
			// 对于带开合的机型
			// 若电源板出现复位情况，导致开合电机配置数据丢失，后续通过读直流电机状态数据确认，若配置更新数据回复为0，则需重新发送配置数据：写开合电机开门数据配置—写开合电机关门数据配置—读直流电机状态。
			// if((!PowBoardReCallData_st.DoorMotorState.bits.DoorCloseSetUpdate)|| (!PowBoardReCallData_st.DoorMotorState.bits.DoorOpenSetUpdate))
			//{
			//    commCtrl_t.cmd = COMM_CTRL_STATE_WRITE_DOOR_MOTOR_OPEN;
			//}
		}
		break;
	default:
		break;
	}
	if (commPowerConnectErrorGet())
		commCtrl_t.cmd = COMM_CTRL_STATE_READ_ADDR; // 通讯故障以后连续查询电源板地址
}

void singlePowerCommHandle(void) // 5MS
{
	CommSendCtrlHandle();
	CommRevProcessHandle();
	CommSendProcessHandle();
}

void singlePowerCommInit(void)
{
	singleCommInit();
	EventComm_Init();
	memset(&DisBoardCtrlMainFuncData_st, 0, sizeof(DisBoardCtrlMainFuncData_st));
	memset(&DisBoardCtrlAssistFuncData_st, 0, sizeof(DisBoardCtrlAssistFuncData_st));
	memset(&DisBoardCtrlDoorMotorData_st, 0, sizeof(DisBoardCtrlDoorMotorData_st));
	memset(&PowBoardReCallData_st, 0, sizeof(PowBoardReCallData_st));
	memset(&commCtrl_t, 0, sizeof(commCtrl_t));
}
