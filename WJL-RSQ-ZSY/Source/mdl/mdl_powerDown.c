/*************************************************************************
 * Copyright:    WJL Kitchenware.
 * File:
 * Layer:
 * Author:       lxj
 * Version:      0.01
 * Brief:       掉电记忆
 *************************************************************************/
#include "all_head.h"
ADC adc_st;
static volatile unsigned char powerDownCtrlF = 0;
static volatile unsigned int powerDownCtrlCnt = 0;
static volatile unsigned int powerDownCtrldelay = 0;
void ADC_ChannelInit(unsigned char Channel, unsigned char ADCFreq)
{
	ADCCON = 0X80 | Channel;
	if (Channel < 8)
	{
		ADCCFG0 = 1 << Channel;
	}
	else
	{
		ADCCFG1 = 1 << (Channel - 8);
	}
	ADCCFG2 = ADCFreq;
}

uint16_t ADC_Scan(void)
{
	unsigned char C2_SAVE;
	uint16_t ADCValue;
	ADC_ChannelInit(VDD4, F8);
	OPINX = 0xC2;
	C2_SAVE = OPREG; // 保存C2数据
	OPREG &= 0x3F;	 //
	OPREG |= 0x40;	 // 基准电压改为2.048V
	ADCCON |= 0X40;	 // 开始ADC转换
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	while (!(ADCCON & 0X20))
		;			   // 等待 ADC转换完成;
	ADCCON &= ~(0X20); // 清中断标志位
	ADCValue = (ADCVH << 4) + (ADCVL >> 4);
	OPREG = C2_SAVE; // 恢复C2数据
	return ADCValue;
}

void powerDownE2pWrite(void)
{
	if (++powerDownCtrldelay < 5 * 200 * 1)
		return;
	powerDownCtrldelay = 5 * 200 * 1;
	if (adc_st.Result[ADC_POWER] <= 2100) //  小于4.2V才去做IAP
	{
		if (!powerDownCtrlF)
		{
			powerDownCtrlF = 1;
			// 关闭负载，写erprom
			led_hard_dispAllClose(); // 所有LED关闭
			user_iap_write();
		}
	}
	if (powerDownCtrlF)
		powerDownCtrlCnt++;
	else
		powerDownCtrlCnt = 0;
	if (powerDownCtrlCnt >= 5 * 1000)
		powerDownCtrlF = 0;
}

void mdl_powerDown_handle(void) // 5MS*4
{
	unsigned char i;
	unsigned int AD_In;
	for (i = 0; i < ADC_SIZE; i++)
	{
		AD_In = ADC_Scan();
		adc_st.Sum[i] += AD_In;
		if (AD_In > adc_st.Max[i])
		{
			adc_st.Max[i] = AD_In;
		}
		if (AD_In < adc_st.Min[i])
		{
			adc_st.Min[i] = AD_In;
		}
		if (++adc_st.Cnt[i] >= 4)
		{
			adc_st.Cnt[i] = 0;
			adc_st.Sum[i] = adc_st.Sum[i] - adc_st.Max[i];
			adc_st.Sum[i] = adc_st.Sum[i] - adc_st.Min[i];
			adc_st.Sum[i] >>= 1;
			adc_st.Result[i] = adc_st.Sum[i];
			adc_st.Sum[i] = 0;
			adc_st.Max[i] = 0;
			adc_st.Min[i] = 4095;
		}
	}
	powerDownE2pWrite();
}
