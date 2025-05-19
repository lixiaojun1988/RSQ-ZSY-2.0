#include "ALL_HEAD.H"

#define WATER_ON_FLUX 800
#define WAVE_FILT_CNT 10
typedef struct
{
    struct GetWaveSensor WaveSensor;
    uint8_t nOneCycleOver;
    uint8_t nPulseCnt;
    uint8_t u8BufCnt;
    uint16_t nPulseBuf[3];
    uint32_t u8_Sum5;
} SensorCDT;

SensorCDT objects[GET_WAVE_SENSOR_NUM];

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

void EXTI9_4_Handler(void) // 娑擃厽鏌�4~8 閸欏嘲鍙嗛崣锟� 娑擃厽鏌囩粙瀣碍閸戣姤鏆�
{
}

void EXTI15_10_Handler(void) // 娑擃厽鏌�10~15 閸欏嘲鍙嗛崣锟� 娑擃厽鏌囩粙瀣碍閸戣姤鏆�
{
    if (EXTI_GetPendingFlagStatus(EXTI_Line14) == SH_SET) // 婢舵牠鍎存稉顓熸焽14 閺嶅洤绻旈崚銈嗘焽
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
    }
    if (EXTI_GetPendingFlagStatus(EXTI_Line12) == SH_SET) // 婢舵牠鍎存稉顓熸焽8 閺嶅洤绻旈崚銈嗘焽
    {
        EXTI_ClearPendingFlag(EXTI_Line12);
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

void GetWaveTask(void) // 1MS
{
    uint16_t nTemp;
    for (uint8_t i = 0; i < GET_WAVE_SENSOR_NUM; i++)
    {
        /* <= 1.5L ,10Q-1涓�1.5L,8Q-3涓�2L, 6.9Q涓�1.9L*/
        if (objects[i].WaveSensor.nPulseTmCnt >= 1000) // 妞嬪孩婧€瀵板懎鐣�
        {
            objects[i].WaveSensor.nPulseTmCnt = 1000;
            objects[i].WaveSensor.Value = 0;
            objects[i].WaveSensor.nHasWaterTm = 0;
        }
        else if (objects[i].nOneCycleOver)
        {
            /* 閿熸枻鎷烽敓鏂ゆ嫹 */
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

                if (nTemp >= objects[i].WaveSensor.Value + 5 ||
                    objects[i].WaveSensor.Value >= nTemp + 5) // 缁愪礁褰�
                {
                    if (objects[i].WaveSensor.Value <= 30)
                    {
                        objects[i].WaveSensor.Value = nTemp;

                        objects[i].u8BufCnt = 0;
                        objects[i].u8_Sum5 = 0;
                        break;
                    }
                }
            }
            else if (i == GET_WAVE_FAN)
            {
                nTemp = CalFanSpeed(nTemp);
            }

            objects[i].u8BufCnt++;
            objects[i].u8_Sum5 += nTemp;

            /* 閿熸枻鎷烽敓锟�25L (1250 = 250*5) */
            if (i == GET_WAVE_FLUX)
            {
                if (objects[i].u8_Sum5 > 250 * WAVE_FILT_CNT)
                {
                    objects[i].u8_Sum5 = 250 * WAVE_FILT_CNT;
                }
            }

            if (objects[i].u8BufCnt >= WAVE_FILT_CNT)
            {
                objects[i].u8BufCnt = 0;
                objects[i].WaveSensor.Value = objects[i].u8_Sum5 / WAVE_FILT_CNT;
                objects[i].u8_Sum5 = 0;
            }

            objects[i].nPulseCnt = 0;
            objects[i].nOneCycleOver = 0;
        }
    }
}

uint16_t GetWaveSensorValue(GET_WAVE_TYPE index)
{
    return objects[index].WaveSensor.Value;
}

void GetWaveInit(void)
{
    uint8_t i;
    for (i = 0; i < GET_WAVE_SENSOR_NUM; i++)
    {
        memset(&objects[i], 0, sizeof(SensorCDT));
    }
}
