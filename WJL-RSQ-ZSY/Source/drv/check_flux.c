
#include "all_head.h"
#define WATER_ON_FLUX 800

struct SensorCDT
{
    struct GetWaveSensor WaveSensor;
    uint8_t nOneCycleOver;
    uint8_t nPulseCnt;
    uint8_t u8BufCnt;
    uint16_t nPulseBuf[3];
    uint16_t u8_Sum5;
};

struct SensorCDT objects[GET_WAVE_SENSOR_NUM];

static inline void f_NopDelay(uint8_t i_Cnt)
{
    uint8_t i;

    for (i = i_Cnt; i > 0; i--)
    {
        __nop();
    }
}

static inline void f_CalPulseWidth(uint8_t index)
{
    if (objects[index].nPulseCnt < 3)
    {
        objects[index].nPulseBuf[objects[index].nPulseCnt] = objects[index].WaveSensor.nPulseTmCnt;
        objects[index].nPulseCnt++;
        if (objects[index].nPulseCnt >= 3)
        {
            objects[index].nOneCycleOver = 1;
        }
    }
    objects[index].WaveSensor.nPulseTmCnt = 0;
}

void EXTI9_4_Handler(void) // 涓柇4~8 鍙峰叆鍙� 涓柇绋嬪簭鍑芥暟
{
    if (EXTI_GetPendingFlagStatus(EXTI_Line8) == SH_SET) // 澶栭儴涓柇8 鏍囧織鍒ゆ柇
    {
        EXTI_ClearPendingFlag(EXTI_Line8);
        f_NopDelay(15);
        if (IN_FLUX_0 == 0)
        {
            f_NopDelay(10);
            if (IN_FLUX_0 == 0)
            {
                f_CalPulseWidth(GET_WAVE_FLUX);
            }
        }
    }
}

void EXTI15_10_Handler(void) // 涓柇10~15 鍙峰叆鍙� 涓柇绋嬪簭鍑芥暟
{
    if (EXTI_GetPendingFlagStatus(EXTI_Line14) == SH_SET) // 澶栭儴涓柇14 鏍囧織鍒ゆ柇
    {
        EXTI_ClearPendingFlag(EXTI_Line14);
        f_NopDelay(15);
        if (IN_FAN_0 == 0)
        {
            f_NopDelay(10);
            if (IN_FAN_0 == 0)
            {
                f_CalPulseWidth(GET_WAVE_FAN);
            }
        }
        /* Add user code */
    }
}
uint16_t CalWaterFlux(uint16_t _u16GetPulse)
{ // f=8.1q-3
    uint32_t _u32res;
    _u32res = 1000000;
    _u32res /= _u16GetPulse;
    _u32res += 300;
    _u32res /= 81;
    _u32res = GETMIN(_u32res, 0xffff);
    return (uint16_t)_u32res;
}
uint16_t CalFanSpeed(uint16_t _u16GetPulse)
{ // rpm=f*15
    uint32_t _u32res;
    _u32res = 150000;
    _u32res /= _u16GetPulse;
    _u32res = GETMIN(_u32res, 0xffff);
    return (uint16_t)_u32res;
}
void CheckPulseTm(void)
{
    objects[GET_WAVE_FLUX].WaveSensor.nPulseTmCnt++;
    objects[GET_WAVE_FAN].WaveSensor.nPulseTmCnt++;
}

void GetWaveTask(void)
{
    uint16_t nTemp;
    for (uint8_t i = 0; i < GET_WAVE_SENSOR_NUM; i++)
    {
        /* <= 1.5L ,10Q-1为1.5L,8Q-3为2L, 6.9Q为1.9L*/

        if (objects[i].WaveSensor.nPulseTmCnt >= 7500)//椋庢満寰呭畾
        {
            objects[i].WaveSensor.nPulseTmCnt = 7500;
            objects[i].WaveSensor.Value = 0;
            objects[i].WaveSensor.nHasWaterTm = 0;
        }
        else if (objects[i].nOneCycleOver)
        {
            /* 锟斤拷锟斤拷 */
            if (objects[i].nPulseBuf[1] < objects[i].nPulseBuf[0])
            {
                nTemp = objects[i].nPulseBuf[0];
                objects[i].nPulseBuf[0] = objects[i].nPulseBuf[1];
                objects[i].nPulseBuf[1] = nTemp;
            }
            if (objects[i].nPulseBuf[2] >= objects[i].nPulseBuf[0] &&
                objects[i].nPulseBuf[2] <= objects[i].nPulseBuf[1])
            {
                nTemp = objects[i].nPulseBuf[2];
            }
            else if (objects[i].nPulseBuf[2] < objects[i].nPulseBuf[0])
            {
                nTemp = objects[i].nPulseBuf[0];
            }
            else
            {
                nTemp = objects[i].nPulseBuf[1];
            }
            if (i == GET_WAVE_FLUX)
            {
                nTemp = CalWaterFlux(nTemp);

                if (nTemp > 250)
                {
                    nTemp = 250;
                }
            }
            else if (i == GET_WAVE_FAN)
            {
                nTemp = CalFanSpeed(nTemp);
            }
            /*

            if (nTemp >= objects[i].WaveSensor.Value + 4 ||
                objects[i].WaveSensor.Value >= nTemp + 4)
            {

                if (++objects[i].WaveSensor.nHasWaterTm >= WATER_ON_FLUX)
                {
                    objects[i].WaveSensor.nHasWaterTm = WATER_ON_FLUX;
                    objects[i].WaveSensor.Value = nTemp;
                }

                objects[i].u8BufCnt = 0;
                objects[i].u8_Sum5 = 0;
            }
            else
            {
                            */
            objects[i].u8BufCnt++;
            objects[i].u8_Sum5 += nTemp;

            /* 锟斤拷锟�25L (1250 = 250*5) */
            if (objects[i].u8_Sum5 > 1250)
            {
                objects[i].u8_Sum5 = 1250;
            }

            if (objects[i].u8BufCnt >= 5)
            {
                objects[i].u8BufCnt = 0;
                objects[i].WaveSensor.Value = objects[i].u8_Sum5 / 5;
             //   if ((objects[i].u8_Sum5 % 5) >= 2)
                {
                    objects[i].WaveSensor.Value++;
                }
                objects[i].u8_Sum5 = 0;
            }
            // }

            objects[i].nPulseCnt = 0;
            objects[i].nOneCycleOver = 0;
        }
    }
}
