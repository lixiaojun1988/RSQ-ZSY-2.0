#ifndef MDL_GETWAVE_H
#define MDL_GETWAVE_H
typedef  enum
{
 GET_WAVE_FLUX,
 GET_WAVE_FAN,
 GET_WAVE_SENSOR_NUM   
}GET_WAVE_TYPE;

#define IN_FLUX_0              GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)
#define IN_FAN_0               GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)
struct GetWaveSensor
{
    uint16_t Value;                                   
    uint16_t nPulseTmCnt;                           
    uint16_t nHasWaterTm;                                
};

typedef struct GetWaveSensor *GetWaveSensor_t;

extern void GetWaveSensor_Init(void);
extern void GetWaveTask(void);

extern void CheckPulseTm(void);
extern uint16_t GetWaveSensorValue(GET_WAVE_TYPE index);
extern void GetWaveInit(void);
#endif


