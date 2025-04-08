#ifndef FLUX_SENSOR_H
#define FLUX_SENSOR_H

    

typedef  enum
{
 GET_WAVE_FLUX,
 GET_WAVE_FAN,
 GET_WAVE_SENSOR_NUM   
}GET_WAVE_TYPE;

#define IN_FLUX_0              GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)
#define IN_FAN_0               GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)
struct GetWaveSensor
{
    uint8_t Value;
    uint8_t FanValue;                                        
    uint16_t nPulseTmCnt;                           
    uint16_t nHasWaterTm;                                
};

typedef struct GetWaveSensor *GetWaveSensor_t;

extern void GetWaveSensor_Init(void);
extern void GetWaveTask(void);

extern void CheckPulseTm(void);
#endif

