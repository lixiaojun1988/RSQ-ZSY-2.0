/*************************************************************************
 * Copyright:    WJL Kitchenware.
 * File:         adc.c
 * Layer:
 * Author:       lxj
 * Version:      0.01
 * Brief:
 *************************************************************************/
#include "all_head.h"

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

unsigned int ADC_Scan(unsigned char B_CH)
{
	static xdata unsigned int ADC_Value = 0;
	ADC_ChannelInit(B_CH, F8);
	ADCCON |= 0X40;
	NOP;
	NOP;
	NOP;
	NOP;
	NOP;
	while ((ADCCON & 0x20) == 0)
		;
	ADC_Value = ADCVH;
	ADC_Value = ((ADC_Value << 4) | (ADCVL >> 4));//12位
	return ADC_Value;
}