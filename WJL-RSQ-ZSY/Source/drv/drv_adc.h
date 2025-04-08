
#ifndef _DRV_ADC_H
#define _DRV_ADC_H
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

extern unsigned int ADC_Scan(unsigned char B_CH);
extern void ADC_ChannelInit(unsigned char Channel, unsigned char ADCFreq);
#endif