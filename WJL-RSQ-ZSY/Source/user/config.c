
/*************************************************************************
 * Copyright:    wjl Kitchenware.
 * File:
 * Layer:
 * Author:       xjli
 * Version:
 * Brief:		配置信息
 *************************************************************************/
#include "all_head.h"

static const DisBoardCtrlDoorMotorData *pDisBoardCtrlDoorMotorData;

DoorMotorConfigData DoorMotorConfigData_t;

code DoorMotorConfigData DoorMotorConfigDefault1 =
    {
        0,  // 开门防夹手时间(空闲)
        40, // 开门堵转电流值
        16, // 开门保护时间
        6,  // 关门防夹手时间
        40, // 关门堵转电流值
        16, // 关门保护时间
};
unsigned char ucMachineModel;

// 设备地址
unsigned char MachineIdGet(void)
{
    return ucMachineModel;
}

void DoorMotorOpenConfig(void)
{
    memcpy(&DoorMotorConfigData_t, &DoorMotorConfigDefault1, sizeof(DoorMotorConfigData_t));
    pDisBoardCtrlDoorMotorData = DisBoardCtrlDoorMotorDataGet();
    pDisBoardCtrlDoorMotorData->xsData.openCloseDoor = 0; // 开门
    pDisBoardCtrlDoorMotorData->xsData.currentVal = DoorMotorConfigData_t.OpenDoor_currentVal;
    pDisBoardCtrlDoorMotorData->xsData.ptime = DoorMotorConfigData_t.OpenDoor_ptime;
}

void DoorMotorCloseConfig(void)
{
    memcpy(&DoorMotorConfigData_t, &DoorMotorConfigDefault1, sizeof(DoorMotorConfigData_t));
    pDisBoardCtrlDoorMotorData = DisBoardCtrlDoorMotorDataGet();
    pDisBoardCtrlDoorMotorData->xsData.openCloseDoor = 1; // 关门
    pDisBoardCtrlDoorMotorData->xsData.currentVal = DoorMotorConfigData_t.CloseDoor_currentVal;
    pDisBoardCtrlDoorMotorData->xsData.ptime = DoorMotorConfigData_t.CloseDoor_ptime;
    pDisBoardCtrlDoorMotorData->xsData.cautionPinchTime = DoorMotorConfigData_t.CloseDoor_cautionPinchTime;
}