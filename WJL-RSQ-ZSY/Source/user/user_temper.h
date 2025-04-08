#ifndef __USER_TEMPER_H__
#define __USER_TEMPER_H__

#define TEMP_ERR_TAB_EN	1

typedef enum
{
    TMP_LNS,
    TMP_OUT,
    TMP_ROOM,
    TMP_IN,
    TMP_FDTX,
    TEMP_SUM,
} TEMP_TYPE;

typedef struct
{
    uint16_t ntc_error_flag[TEMP_SUM];
    uint16_t ntc_error_cnt[TEMP_SUM];
    uint16_t ntc_ok_cnt[TEMP_SUM];
    uint16_t ntc_adc[TEMP_SUM];
    uint16_t value_array[TEMP_SUM];
} Temper_TypeDef;

#if (TEMP_ERR_TAB_EN)
//增加表格和故障
typedef struct
{
	TEMP_TYPE TempType;
	EN_ERRORCODE_T enErr;
}TemperErr_TypeDef;
#endif


extern uint8_t temper_ntc_error_get(TEMP_TYPE index);
extern void user_temper(void);
extern uint16_t temper_result_get(TEMP_TYPE index);
extern void user_temper_init(void);
#if (TEMP_ERR_TAB_EN)
extern EN_ERRORCODE_T temper_err_get(void);
#endif
#endif
