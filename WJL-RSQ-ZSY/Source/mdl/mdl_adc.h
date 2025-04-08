
#ifndef _MDL_ADC_H
#define _MDL_ADC_H

typedef enum
{
  ADC_FJ_DL,
  ADC_TMP_HY,
  //ADC_TMP_BLFDL,
  ADC_TMP_FDTX,
  ADC_TMP_IN,
  ADC_TMP_ROOM,
  ADC_TMP_OUT,
  ADC_TMP_LNS,
  ADC_SUM,
} ADC_TYPE;

typedef struct
{
  uint16_t Read[ADC_SUM];
  uint16_t Max[ADC_SUM];
  uint16_t Min[ADC_SUM];
  uint16_t Sum[ADC_SUM];
  uint16_t Result[ADC_SUM];
  unsigned char ErrorCnt[ADC_SUM];
  unsigned char Cnt[ADC_SUM];
} ADC_ST;

extern ADC_ST adc_st;
extern void AdProcess(void);
extern uint16_t AdcGet(ADC_TYPE i);
extern void mdl_adc_handle(void);
extern void adcSysTick(void);
extern uint16_t adc_result_get(ADC_TYPE index);
extern void mdl_adc_init(void);
#endif
