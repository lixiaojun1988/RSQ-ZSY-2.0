#ifndef USER_POWERCOMM_H
#define USER_POWERCOMM_H

typedef union tagDisBoardCtrlMainFuncData
{
    unsigned char ucData[3];
    struct
    {
        unsigned char rightFanSpeed; // 右风机（单风机）
        unsigned char leftFanSpeed;  // 左风机
        unsigned char buzzer : 3;    // 蜂鸣
        unsigned char lamp : 5;      // 照明
    } xsData;
} DisBoardCtrlMainFuncData; // 主功能

typedef union tagDisBoardCtrlAssistFuncData
{
    unsigned char ucData[3];
    struct
    {
        unsigned char doorMotorDir : 1; // 开合电机方向
        unsigned char dcUd_study : 1;   // 直流升降学习
        unsigned char pressor : 1;      // 增压
        unsigned char disCode : 1;      // 消码
        unsigned char coded : 1;        // 对码
        unsigned char selfCurrent : 1;  // 自检电流
        unsigned char powerOff : 1;     // 关机
        unsigned char powerOn : 1;      // 开机

        unsigned char reserved1 : 1;         // 预留1
        unsigned char reserved2 : 1;         // 预留2
        unsigned char normalReverseWash : 1; // 正反转洗
        unsigned char flz : 1;               // 负离子
        unsigned char pm25Senor : 1;         // pm25
        unsigned char smokeSenor : 1;        // 烟感
        unsigned char closeDoor : 1;         // 关门
        unsigned char openDoor : 1;          // 开门

        unsigned char manualOpenDoor : 1; // 强制开门
        unsigned char reserved3 : 1;
        unsigned char reserved4 : 1;
        unsigned char reserved5 : 1;
        unsigned char reserved6 : 1;
        unsigned char atmosphereLight : 1; // 氛围灯
        unsigned char pump : 1;            // 水泵
        unsigned char heat : 1;            // 加热
    } xsData;
} DisBoardCtrlAssistFuncData; ////辅功能助


typedef union tagDisBoardCtrlOcMotorData
{
    unsigned char ucData[3];
    struct
    {
        unsigned char cautionPinchTime : 7; // 防夹手时间
        unsigned char openCloseDoor : 1;    // 开关门
        unsigned char currentVal;           // 电流值
        unsigned char ptime;                // 保护时间
    } xsData;
} DisBoardCtrlDoorMotorData;// 开合电机数据值配置

typedef struct
{
    unsigned char id;           // 地址码
    unsigned char temp1;        // 温度1
    unsigned char temp2;        // 温度2
    unsigned char temp3;        // 温度3
    unsigned char hum;          // 湿度数据
    unsigned char pm25;         // pm25数据
    unsigned char smoke;        // 烟浓度
    unsigned char smokeTemp;    // 烟温度
    unsigned char MotorCurrent; // 电机电流
    union
    {
        unsigned char ucData;
        struct
        {
            unsigned char upOk : 1;       // 开合电机上升到位
            unsigned char DownOk : 1;     // 开合电机下降到位
            unsigned char temp1Error : 1; // 温度探头故障1
            unsigned char temp2Error : 1; // 温度探头故障2
            unsigned char temp3Error : 1; // 温度探头故障3
            unsigned char humError : 1;   // 湿度传感器故障
            unsigned char pm25Error : 1;  // PM2.5故障
            unsigned char smokeError : 1; // 烟感故障
        } bits;
    } SenserState; // 传感器状态
    union
    {
        unsigned char ucData;
        struct
        {
            unsigned char stoveFire : 3;        // 灶具火力
            unsigned char decodeFlag : 1;       // 回传对码标志
            unsigned char decodeSucessFlag : 1; // 对码成功标志
            unsigned char reserved : 2;         // 预留
            unsigned char valid : 1;            // bit0-bit3有效
        } bits;
    } linkData; // 联动数据
    union
    {
        unsigned char ucData;
        struct
        {
            unsigned char upDownLearn : 3;        // 上升下降学习
            unsigned char DoorOpenSetUpdate : 1;  // 开合电机开门配置更新
            unsigned char DoorCloseSetUpdate : 1; // 开合电机关门配置更新
            unsigned char reserved : 3;           // 预留
        } bits;
    } DoorMotorState; // 开合电机状态
    unsigned char DcMotorRate;
} PowBoardReCallData; // 电源板回传数据

//extern PowBoardReCallData *PowBoardReCallDataGet(void);
extern DisBoardCtrlMainFuncData *DisBoardCtrlMainFuncDataGet(void);
extern DisBoardCtrlDoorMotorData *DisBoardCtrlDoorMotorDataGet(void);
extern DisBoardCtrlAssistFuncData *DisBoardCtrlAssistFuncDataGet(void);
extern void singlePowerCommHandle(void);
extern void singlePowerCommInit(void);
#endif