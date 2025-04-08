
#ifndef _MDL_ADC_H
#define _MDL_ADC_H
// 采样通道
enum Channel
{
    AIN0 = 0,
    AIN1,
    AIN2,
    AIN3,
    AIN4,
    AIN5,
    AIN6,
    AIN7,
    AIN8,
    AIN9,
    AIN10,
    AIN11,
    AIN12,
    AIN13,
    AIN14,
    AIN15,
    VDD4 = 31
};
// 采样频率
enum ADCFreq
{
    F32 = 0,
    F24,
    F16,
    F12,
    F8,
    F6,
    F4,
    F3
};

typedef enum
{
    ADC_POWER,
    ADC_SIZE,
} adc_Type;

typedef struct
{
    unsigned int Read[ADC_SIZE];
    unsigned int Max[ADC_SIZE];
    unsigned int Min[ADC_SIZE];
    unsigned int Sum[ADC_SIZE];
    unsigned char Cnt[ADC_SIZE];
    unsigned int Result[ADC_SIZE];
    unsigned char ErrorCnt[ADC_SIZE];
} ADC;

extern void mdl_powerDown_handle(void);
#endif