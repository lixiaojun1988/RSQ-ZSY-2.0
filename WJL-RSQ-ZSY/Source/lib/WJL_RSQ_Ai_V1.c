
/*************************************************************************
 * Copyright:    wjl Kitchenware.
 * File:
 * Layer:
 * Author:       xjli
 * Version:      V1
 * Brief:       锟斤拷锟斤拷锟斤拷锟剿拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷惴拷锟絇ID锟斤拷
  2024-12-05
 *************************************************************************/
#include "all_head.h"


#define PID_INCR_LIMIT_MAX (500)  // Pid澧為噺闄愬埗鍊煎缓璁彇鍊艰寖鍥�50-3000
#define PID_INCR_LIMIT_MIN (-500) //
#define PID_ADJUST_SPEED 500	  // pid璋冭妭閫熷害,寤鸿鍙栧€艰寖鍥�100-2000
#define PID_DIGIT_P (20)		  // PID绯绘暟 P寤鸿鍙栧€�5-30
#define PID_DIGIT_I (1)			  // I寤鸿鍙栧€�0-10
#define PID_DIGIT_D (5)			  // D寤鸿鍙栧€�0-50
typedef struct
{
    int32_t ActualValue;
    uint16_t Proportion;
    uint16_t Integral;
    uint16_t Derivative;
    int32_t LastError;
    int32_t PrevError;
    int32_t totalValue;
} PID;
PID Pid_t;
int16_t PID_realize(SystemRunDataTypeDef *pSysData)
{
    int16_t Error = 0;
    uint16_t u16PidGetBlfI;
    pSysData->u16ELoad /= 10;
    u16PidGetBlfI = (uint16_t)(((uint32_t)pSysData->u16BlfIRun_E * pSysData->Flux) / pSysData->u16ELoad);
    Pid_t.Proportion = PID_DIGIT_P;
    Pid_t.Integral = PID_DIGIT_I;
    Pid_t.Derivative = PID_DIGIT_D;
    Error = (int16_t)pSysData->TmpSet - pSysData->TmpOut;
    Pid_t.ActualValue = (int32_t)Pid_t.Proportion * (Error - Pid_t.LastError) + Pid_t.Integral * Error + Pid_t.Derivative * (Error - 2 * Pid_t.LastError + Pid_t.PrevError);
    Pid_t.PrevError = Pid_t.LastError;
    Pid_t.LastError = Error;
    if (Pid_t.ActualValue > PID_INCR_LIMIT_MAX)
        Pid_t.ActualValue = PID_INCR_LIMIT_MAX;
    else if (Pid_t.ActualValue < PID_INCR_LIMIT_MIN)
        Pid_t.ActualValue = PID_INCR_LIMIT_MIN;
    Pid_t.totalValue += Pid_t.ActualValue;
    if (Pid_t.totalValue < -2147483648)
        Pid_t.totalValue = -2147483648;
    if (Pid_t.totalValue > 2147483647)
        Pid_t.totalValue = 2147483647;
    return (int16_t)((int32_t)Pid_t.totalValue * u16PidGetBlfI / PID_ADJUST_SPEED); // 1000
}

void Pid_Init(void)
{
    Pid_t.LastError = 0;
    Pid_t.PrevError = 0;
    Pid_t.ActualValue = 0;
    Pid_t.totalValue = 0;
}
