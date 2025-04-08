/*************************************************************************
 * Copyright: .
 * File:
 * Layer:
 * Author:   xjli
 * Version:  V1.0
 * Date:2024-12-12
 * Decription:    燃热自适应
 *************************************************************************/

#include "all_head.h"

#define Pa_1300 1300
#define Pa_1000 1000
#define T_20C 200
#define T_15C 150
#define T_10C 100
#define TIM_7S 7
#define TIM_10S 10
#define TIM_12S 12

#define BasicAirPress	10132	//基础大气压（0.1hPa）

adaptive_typedef adaptive_t;

ST_HIGH_ALTITUDE_TYPE astHighAltitudeAddList[EN_AIR_PRESS_MAX] =
{
	//HC,LC		FH,   		PH
	{0,			0,			0},//H=0
	{2,			-15,		-6},//H=1
	{4,			-21,		-10},//H=2
	{6,			-26,		-10},//H=3
};


//@海拔模式判定
void user_adaptive_airPress_adjust(void)
{
	uint32_t _u32Tmp;
	//计算海拔
	if (adaptive_t.airPressValue >= BasicAirPress)
		_u32Tmp = 0;
	else
		_u32Tmp = (uint32_t)(BasicAirPress - adaptive_t.airPressValue) * 10;
	if (FSM_STATE_STABLE == GetFsmState())
	{
		if (EN_EE_AUTO == adaptive_t.EE_value)
		{
			if (_u32Tmp <= 1000)
				adaptive_t.airPressGrade = EN_AIR_PRESS_H0;
			else if (_u32Tmp <= 2000)
				adaptive_t.airPressGrade = EN_AIR_PRESS_H1;
			else if (_u32Tmp <= 2500)
				adaptive_t.airPressGrade = EN_AIR_PRESS_H2;
			else
                adaptive_t.airPressGrade = EN_AIR_PRESS_H3;
		}
		else // 手动
		{
		}
    }
}
//@低温模式判定
void user_adaptive_lowTemper_adjust(void)
{
    if (FSM_STATE_STABLE == GetFsmState())
    {
        if (EN_EE_AUTO == adaptive_t.EE_value) // 自动识别
        {
            if (GetSystemRunData()->TmpRoom > T_20C)
                adaptive_t.lowTemperGrade = EN_LOW_TEMPER_T0; // 非低气压
            else
            {
                if (GetSystemRunData()->TmpIn > T_15C)
                    adaptive_t.lowTemperGrade = EN_LOW_TEMPER_T0;
                else if (GetSystemRunData()->TmpIn <= T_15C && GetSystemRunData()->TmpIn > T_10C)
                {
                    adaptive_t.lowTemperGrade = EN_LOW_TEMPER_T1;
                }
                else if (GetSystemRunData()->TmpIn <= T_10C)
                {
                    adaptive_t.lowTemperGrade = EN_LOW_TEMPER_T2;
                }
            }
        }
        else // 手动
        {
        }
    }
}
//@耦合判定,燃气低气压耦合低温自适应
void user_adaptive_coupling_adjust(void)
{
	static unsigned char adapt_coupl_last = 0;
	if (EN_EE_AUTO == adaptive_t.EE_value) // 自动识别
	{
		adaptive_t.finalGrade = adaptive_t.lowTemperGrade;
		if (!adapt_coupl_last)
		{
			adapt_coupl_last = 1;
			if (1 == adaptive_t.finalGrade)
			{
				adaptive_t.Fl_add = 6;   //
				adaptive_t.Fh_add = -15; //
				adaptive_t.delayAddAirTime_1s = TIM_7S;
				adaptive_t.AddGasSpedPer = 90;
			}
			else if (2 == adaptive_t.finalGrade)
			{
				adaptive_t.Fl_add = 10;  //
				adaptive_t.Fh_add = -21; //
				adaptive_t.delayAddAirTime_1s = TIM_10S;
				adaptive_t.AddGasSpedPer = 80;
			}
			else
			{
				adaptive_t.Fl_add = 0; //
				adaptive_t.Fh_add = 0; //
				adaptive_t.delayAddAirTime_1s = 0;
				adaptive_t.AddGasSpedPer = 100;
			}
		}
	}
	else
	{
		adaptive_t.Fl_add = 10;  //
		adaptive_t.Fh_add = -26; //
		adaptive_t.delayAddAirTime_1s = TIM_12S;
		adaptive_t.AddGasSpedPer = 60;
	}

	//海拔影响
	adaptive_t.Hc_add = astHighAltitudeAddList[adaptive_t.airPressGrade].s8HcLc_Add;
	adaptive_t.Lc_add = astHighAltitudeAddList[adaptive_t.airPressGrade].s8HcLc_Add;
	adaptive_t.Fh_add = GETMIN(adaptive_t.Fh_add, astHighAltitudeAddList[adaptive_t.airPressGrade].s8Fh_Add);
	adaptive_t.Ph_add = astHighAltitudeAddList[adaptive_t.airPressGrade].s8Ph_Add;
}


void hb_snsor_process(void)
{
    spl0601_get_raw_temp();
		spl0601_get_raw_pressure();
		adaptive_t.fSplTemp = spl0601_get_temperature();
		adaptive_t.fSplPressure = spl0601_get_pressure();
        adaptive_t.airPressValue = (uint16_t)(adaptive_t.fSplPressure / 10);
}

void user_adaptive(void) // 10MS
{
	static uint8_t user_adaptive_tick;
	if(++user_adaptive_tick>=10)
		{
			user_adaptive_tick=0;
			hb_snsor_process();
		}
    user_adaptive_airPress_adjust();
    user_adaptive_lowTemper_adjust();
    user_adaptive_coupling_adjust();
	
}

void user_adaptive_init(void)
{
    memset(&adaptive_t, 0, sizeof(adaptive_t));
	spl0601_init();
	// sampling rate = 32Hz; Pressure oversample = 8;
    spl0601_rateset(PRESSURE_SENSOR,32, 8);   
	// sampling rate = 32Hz; Temperature oversample = 8; 
    spl0601_rateset(TEMPERATURE_SENSOR,32, 8);
	spl0601_start_continuous(CONTINUOUS_P_AND_T);
}

adaptive_typedef *getAdapData(void)
{
    return &adaptive_t;
}
