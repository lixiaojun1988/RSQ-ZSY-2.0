#ifndef __CONFIG_H__
#define __CONFIG_H__
typedef enum
{
	FullFuncAcMachDoorMotor = 0x01,	  //全功能带开合机型
	NomalAcMachDoorMotor = 0x02,	  //带开合机型
	BasicAcMachDoorMotor = 0x06,	  //基础机型
	//DcMachDoorMotor = 0x07,			  //直流变频机型
	FullFuncAcMachNoDoorMotor = 0x09, //全功能交流不带开合机型
	MachineTypeSum,
} MachineType;

// 开合电机数据值配置
typedef struct
{
    unsigned char OpenDoor_cautionPinchTime;  // 开门防夹手时间(空闲)
    unsigned char OpenDoor_currentVal;        // 开门堵转电流值
    unsigned char OpenDoor_ptime;             // 开门保护时间
    unsigned char CloseDoor_cautionPinchTime; // 关门防夹手时间
    unsigned char CloseDoor_currentVal;       // 关门堵转电流值
    unsigned char CloseDoor_ptime;            // 关门保护时间
} DoorMotorConfigData;
extern unsigned char MachineIdGet(void);
extern void DoorMotorOpenConfig(void);
extern void DoorMotorCloseConfig(void);
#endif