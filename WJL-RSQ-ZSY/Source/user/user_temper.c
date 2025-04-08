/*************************************************************************
 * Copyright: .
 * File:
 * Layer:
 * Author:   xjli
 * Version:  V1.0
 * Date:2022-12-27
 * Decription:
 *************************************************************************/

#include "all_head.h"
#define TEMPER_NTC_ADC_MAX 3900
#define TEMPER_NTC_ADC_MIN 100
#define TMP_LMT_MAX 999
#define TMP_LMT_MIN 0
#define TMP_ERROR 0x00
#define TEMPER_NTC_DELAY_1S (1000 / 20) // 20MS赌一次
Temper_TypeDef sTemper;

typedef enum
{
    EN_TMP0_INDEX,
    EN_TMP10_INDEX,
    EN_TMP20_INDEX,
    EN_TMP30_INDEX,
    EN_TMP40_INDEX,
    EN_TMP50_INDEX,
    EN_TMP60_INDEX,
    EN_TMP70_INDEX,
    EN_TMP80_INDEX,
    EN_TMP90_INDEX,
    EN_TMP100_INDEX,
    EN_TMP_INDEX_MAX
} EN_TAB_INDEX_T;
typedef enum
{
    EN_TAB3450,
    EN_TAB3950,
    EN_TAB_MAX
} EN_TAB_T;

#define ADC12 1
// 12λAD
#if (0 < ADC12)
const uint16_t au16TmpTab[EN_TAB_MAX][EN_TMP_INDEX_MAX] =
    {
        {3401, 3124, 2800, 2447, 2088, 1744, 1432, 1161, 934, 748, 598}, // 3450
        {2537, 2042, 1575, 1176, 860, 623, 450, 326, 238, 175, 129}      // 3950
};

// 10λAD
#else
CONST U16_T au16TmpTab[EN_TAB_MAX][EN_TMP_INDEX_MAX] =
    {
        {850, 781, 700, 611, 522, 436, 358, 290, 233, 187, 149},
        {634, 510, 394, 290, 215, 156, 112, 81, 59, 44, 32}};

#endif

const EN_TAB_T TEMP_CHOOSE_TAB[TEMP_SUM] =
    {
        EN_TAB3950,
        EN_TAB3450,
        EN_TAB3450,
        EN_TAB3450,
        EN_TAB3950,
};

const ADC_TYPE TEMP_COMPARE_ADC_TAB[TEMP_SUM] =
    {
        ADC_TMP_LNS,
        ADC_TMP_OUT,
        ADC_TMP_ROOM,
        ADC_TMP_IN,
        ADC_TMP_FDTX,
};

#if (TEMP_ERR_TAB_EN)
//故障判断
const TemperErr_TypeDef TEMP_ERR_TAB[] =
{
    {TMP_OUT,EN_ERR_E4},//出水探头
    {TMP_IN,EN_ERR_F3},//进水探头（回水探头）
    {TMP_FDTX,EN_ERR_F5},//防冻探头
};
#endif

uint16_t GetMdlTmpVal(uint16_t _AdcVal, EN_TAB_T _TmpTab)
{
    uint8_t _u8i;
    uint16_t _s16Temp;
    if (_AdcVal >= au16TmpTab[_TmpTab][EN_TMP0_INDEX])
    {
        _s16Temp = TMP_LMT_MIN;
    }
    else if (_AdcVal <= au16TmpTab[_TmpTab][EN_TMP100_INDEX])
    {
        _s16Temp = TMP_LMT_MAX;
    }
    else
    {
        for (_u8i = 0; (EN_TMP_INDEX_MAX - 1) > _u8i; _u8i++)
        {
            if (_AdcVal >= au16TmpTab[_TmpTab][_u8i + 1])
            {
                _s16Temp = au16TmpTab[_TmpTab][_u8i] - au16TmpTab[_TmpTab][_u8i + 1];
                _s16Temp = ((uint32_t)au16TmpTab[_TmpTab][_u8i] - _AdcVal) * 100 / _s16Temp;
                _s16Temp = (_s16Temp + _u8i * 100);
                break;
            }
        }
    }
    return _s16Temp;
}

void user_temper(void) // 10MS
{
    uint8_t i;
    static uint8_t user_temper_tick;

    if (++user_temper_tick < 2)
        return;
    user_temper_tick = 0;
    for (i = 0; i < TEMP_SUM; i++)
    {
        sTemper.ntc_adc[i] = adc_result_get((ADC_TYPE)TEMP_COMPARE_ADC_TAB[i]);
        if (sTemper.ntc_adc[i] >= TEMPER_NTC_ADC_MAX || sTemper.ntc_adc[i] <= TEMPER_NTC_ADC_MIN)
        {
            sTemper.ntc_ok_cnt[i] = 0;
            if ((++sTemper.ntc_error_cnt[i] >= TEMPER_NTC_DELAY_1S) && !sTemper.ntc_error_flag[i])
            {
                sTemper.ntc_error_flag[i] = 1;
            }
        }
        else
        {
            sTemper.ntc_error_cnt[i] = 0;
            if ((++sTemper.ntc_ok_cnt[i] >= TEMPER_NTC_DELAY_1S) && sTemper.ntc_error_flag[i])
            {
                sTemper.ntc_error_flag[i] = 0;
            }
        }
        sTemper.value_array[i] = (sTemper.ntc_error_flag[i] == 0) ? GetMdlTmpVal(sTemper.ntc_adc[i], TEMP_CHOOSE_TAB[i]) : TMP_ERROR;
    }
}
uint16_t temper_result_get(TEMP_TYPE index)
{
    return sTemper.value_array[index];
}

uint8_t temper_ntc_error_get(TEMP_TYPE index)
{
#if (DIS_TMP_ERR)
    return 0;
#else
    return sTemper.ntc_error_flag[index];
#endif
}

#if (TEMP_ERR_TAB_EN)
/**********************************************
 * 查看温度探头故障判定
 * 
 * 返回数值：故障代码
 **********************************************/
EN_ERRORCODE_T temper_err_get(void)
{
    uint8_t _i;
    EN_ERRORCODE_T _enErrCode;
    _enErrCode = EN_ERR_NO;
    for (_i = 0; _i < ARR_NUM(TEMP_ERR_TAB); _i++)
    {
        if (temper_ntc_error_get(TEMP_ERR_TAB[_i].TempType))
        {
            _enErrCode = TEMP_ERR_TAB[_i].enErr;
            break;
        }
    }
    return _enErrCode;
}
#endif

void user_temper_init(void)
{
    mdl_adc_init();
    memset(&sTemper, 0, sizeof(sTemper));
}
