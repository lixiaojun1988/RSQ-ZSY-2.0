/*************************************************************************
 * Copyright:    WJL Kitchenware.
 * File:         adc.c
 * Layer:
 * Author:       lxj
 * Version:      0.01
 * Brief:      AD滤波处理
 *************************************************************************/
#include "all_head.h"

ADC_ST adc_st;

extern ADC_Channel_TypeDef const tab_adc_ch[];
unsigned char adcTick;

void mdl_adc_handle(void)
{
	unsigned char i;
	uint16_t AD_In;
	static uint8_t mdl_adc_tick;
	if (++mdl_adc_tick < 2)
		return;
	mdl_adc_tick = 0;
	for (i = 0; i < ADC_SUM; i++)
	{
	
		AD_In = bps_adc_get_value(tab_adc_ch[i]);
		adc_st.Sum[i] += AD_In;
		if (AD_In > adc_st.Max[i])
		{
			adc_st.Max[i] = AD_In;
		}
		if (AD_In < adc_st.Min[i])
		{
			adc_st.Min[i] = AD_In;
		}
		if (++adc_st.Cnt[i] >= 18)
		{
			adc_st.Cnt[i] = 0;
			adc_st.Sum[i] = adc_st.Sum[i] - adc_st.Max[i];
			adc_st.Sum[i] = adc_st.Sum[i] - adc_st.Min[i];
			adc_st.Sum[i] >>= 4;
			adc_st.Result[i] = adc_st.Sum[i];
			adc_st.Sum[i] = 0;
			adc_st.Max[i] = 0;
			adc_st.Min[i] = 4095;
		}
	}
}
// 获取处理后的AD值
uint16_t adc_result_get(ADC_TYPE index)
{
	return adc_st.Result[index];
}

void mdl_adc_init(void)
{
	memset(&adc_st,0,sizeof(adc_st));
}
