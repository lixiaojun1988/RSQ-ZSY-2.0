/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_adc.h
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-5-24  
  * @brief   This file contains all the functions prototypes for the ADC firmware
  *          library.
  ******************************************************************************
  * @attention
  *
  * SINOWEALTH IS SUPPLYING THIS SOFTWARE FOR USE EXCLUSIVELY SH_ON SINOWEALTH'S 
  * MICROCONTROLLER PRODUCTS. IT IS PROTECTED UNDER APPLICABLE COPYRIGHT LAWS. 
  * THIS SOFTWARE IS FOR GUIDANCE IN ORDER TO SAVE TIME. AS A RESULT, SINOWEALTH 
  * SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES 
  * WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT OF SUCH FIRMWARE AND/OR
  * THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN 
  * CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2024 Sinowealth</center></h2>
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SH30F9XX_SB0_ADC_H
#define __SH30F9XX_SB0_ADC_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "sh30f9xx_sb0_libcfg.h"

/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */

/** @addtogroup ADC_MODULE ADC
  * @{
  */

/** @defgroup ADC_Exported_Types ADC Exported Types
  * @{
  */
    
/** 
  * @brief   ADC_Conversion_Mode
  */
typedef enum 
{
    ADC_ConversionMode_Single        = 0,    /*!< Single sequence conversion */
    ADC_ConversionMode_Discontinuous = 1,    /*!< Discontinuous sequence transformation */
    ADC_ConversionMode_Continuous    = 2,    /*!< Continuous sequence transformation */
} ADC_CONVERSION_TypeDef;

/** 
  * @brief ADC_Resolution
  */
typedef enum 
{
    ADC_Resolution_12Bit = 0,     /*!< 12bit */
    ADC_Resolution_10Bit = 1,     /*!< 10bit */
} ADC_RESOLUTION_TypeDef; 


/** 
  * @brief ADC_Reference_Voltage_Sources
  */
typedef enum 
{
    ADC_RefVoltage_AVDD    = 0,    /*!< Select AVDD as the reference voltage*/
    ADC_RefVoltage_ExtVref = 1,    /*!< Select external VREF port input as reference voltage */
} ADC_VREF_TypeDef;

/** 
  * @brief ADC_Num_Of_Conversion
  */         
typedef enum 
{
    ADC_NumOfConversion_1         =    0,     /*!< set number of ADC conversion */
    ADC_NumOfConversion_2         =    1,     /*!< set number of ADC conversion */
    ADC_NumOfConversion_3         =    2,     /*!< set number of ADC conversion */
    ADC_NumOfConversion_4         =    3,     /*!< set number of ADC conversion */
    ADC_NumOfConversion_5         =    4,     /*!< set number of ADC conversion */
    ADC_NumOfConversion_6         =    5,     /*!< set number of ADC conversion */
    ADC_NumOfConversion_7         =    6,     /*!< set number of ADC conversion */
    ADC_NumOfConversion_8         =    7,     /*!< set number of ADC conversion */
} ADC_NumOfConversion_TypeDef;

/**  
  * @brief ADC_Trigger
  */
typedef enum 
{
    ADC_Trigger_Software   =    0x00,     /*!< Software startup      */
    ADC_Trigger_EXTI0      =    0x01,     /*!< External interrupt 0  */
    ADC_Trigger_EXTI1      =    0x02,     /*!< External interrupt 1  */
    ADC_Trigger_EXTI2      =    0x04,     /*!< External interrupt 2  */
    ADC_Trigger_EXTI3      =    0x08,     /*!< External interrupt 3  */
    ADC_Trigger_TIMTRG     =    0x10,     /*!< Timer 2 overflow      */
    ADC_Trigger_PWM0TRG    =    0x20,     /*!< Pwm0 overflow         */
    ADC_Trigger_PWM1TRG    =    0x30,     /*!< Pwm1 overflow         */
    ADC_Trigger_PWM2TRG    =    0x40,     /*!< Pwm2 overflow         */
    ADC_Trigger_PWM3TRG    =    0x50,     /*!< Pwm3 overflow         */
    ADC_Trigger_PCA0TRG    =    0x61,     /*!< Pca0 interrupt comparison trigger  */
    ADC_Trigger_PCA1TRG    =    0x62,     /*!< PCa1 interrupt comparison trigger  */
    ADC_Trigger_PCA2TRG    =    0x71,     /*!< Pca2 interrupt comparison trigger  */
    ADC_Trigger_PCA3TRG    =    0x72,     /*!< PCA3 interrupt comparison trigger  */
} ADC_TRIGGER_TypeDef;  

/**  
  * @brief ADC_Prescaler
  */
typedef enum 
{
    ADC_Prescaler_1     =    0,     /*!< tAD = 1*tBusCLK */
    ADC_Prescaler_2     =    1,     /*!< tAD = 2*tBusCLK */
    ADC_Prescaler_3     =    2,     /*!< tAD = 3*tBusCLK */
    ADC_Prescaler_4     =    3,     /*!< tAD = 4*tBusCLK */
    ADC_Prescaler_5     =    4,     /*!< tAD = 5*tBusCLK */
    ADC_Prescaler_6     =    5,     /*!< tAD = 6*tBusCLK */
    ADC_Prescaler_8     =    6,     /*!< tAD = 8*tBusCLK */
    ADC_Prescaler_12    =    7,     /*!< tAD = 12*tBusCLK */
    ADC_Prescaler_16    =    8,     /*!< tAD = 16*tBusCLK */
    ADC_Prescaler_24    =    9,     /*!< tAD = 24*tBusCLK */
    ADC_Prescaler_32    =    10,    /*!< tAD = 32*tBusCLK */
    ADC_Prescaler_48    =    11,    /*!< tAD = 48*tBusCLK */
    ADC_Prescaler_64    =    12,    /*!< tAD = 64*tBusCLK */
    ADC_Prescaler_128   =    13,    /*!< tAD = 128*tBusCLK */
    ADC_Prescaler_256   =    14,    /*!< tAD = 256*tBusCLK */
    ADC_Prescaler_320   =    15,    /*!< tAD = 320*tBusCLK */
} ADC_CLKCYCLE_TypeDef;

/** 
  * @brief ADC_Sample_Times
  */
typedef enum 
{
    ADC_SampleTime_1Cycles    =    0,     /*!< 1 ADC Clock Cycle  */
    ADC_SampleTime_2Cycles    =    1,     /*!< 2 ADC Clock Cycle  */
    ADC_SampleTime_3Cycles    =    2,     /*!< 3 ADC Clock Cycle  */
    ADC_SampleTime_4Cycles    =    3,     /*!< 4 ADC Clock Cycle  */
    ADC_SampleTime_5Cycles    =    4,     /*!< 5 ADC Clock Cycle  */
    ADC_SampleTime_6Cycles    =    5,     /*!< 6 ADC Clock Cycle  */
    ADC_SampleTime_7Cycles    =    6,     /*!< 7 ADC Clock Cycle  */
    ADC_SampleTime_8Cycles    =    7,     /*!< 8 ADC Clock Cycle  */
    ADC_SampleTime_9Cycles    =    8,     /*!< 9 ADC Clock Cycle  */
    ADC_SampleTime_10Cycles   =    9,     /*!< 10 ADC Clock Cycle  */
    ADC_SampleTime_11Cycles   =    10,    /*!< 11 ADC Clock Cycle  */
    ADC_SampleTime_12Cycles   =    11,    /*!< 12 ADC Clock Cycle  */
    ADC_SampleTime_13Cycles   =    12,    /*!< 13 ADC Clock Cycle  */
    ADC_SampleTime_14Cycles   =    13,    /*!< 14 ADC Clock Cycle  */
    ADC_SampleTime_15Cycles   =    14,    /*!< 15 ADC Clock Cycle  */
    ADC_SampleTime_16Cycles   =    15,    /*!< 16 ADC Clock Cycle  */
} ADC_SAMPLE_TIME_TypeDef;

/** 
  * @brief ADC_Two_Sample_Gap
  */

typedef enum 
{
    ADC_TwoSampleGap_0Cycles      =    0,     /*!< No wait time */
    ADC_TwoSampleGap_2Cycles      =    1,     /*!< 2 ADC Clock Cycle */
    ADC_TwoSampleGap_4Cycles      =    2,     /*!< 4 ADC clock cycle */
    ADC_TwoSampleGap_8Cycles      =    3,     /*!< 8 ADC clock cycle */
    ADC_TwoSampleGap_16Cycles     =    4,     /*!< 16 ADC clock cycle */
    ADC_TwoSampleGap_32Cycles     =    5,     /*!< 32 ADC clock cycle */
    ADC_TwoSampleGap_64Cycles     =    6,     /*!< 64 ADC clock cycle */
    ADC_TwoSampleGap_128Cycles    =    7,     /*!< 128 ADC clock cycle */
} ADC_TGAP_TypeDef;

/** 
  * @brief ADC_Sample_Sequence
  */

typedef enum 
{
    ADC_Sequence_0  = 0x0C,     /*!< ADC currunt sample channel is SEQCH0 */
    ADC_Sequence_1  = 0x10,     /*!< ADC currunt sample channel is SEQCH1 */
    ADC_Sequence_2  = 0x14,     /*!< ADC currunt sample channel is SEQCH2 */
    ADC_Sequence_3  = 0x18,     /*!< ADC currunt sample channel is SEQCH3 */
    ADC_Sequence_4  = 0x1C,     /*!< ADC currunt sample channel is SEQCH4 */
    ADC_Sequence_5  = 0x20,     /*!< ADC currunt sample channel is SEQCH5 */
    ADC_Sequence_6  = 0x24,     /*!< ADC currunt sample channel is SEQCH6 */
    ADC_Sequence_7  = 0x28,     /*!< ADC currunt sample channel is SEQCH7 */
} ADC_Seq_TypeDef;

/** 
  * @brief ADC_Channel
  */

typedef enum 
{
    ADC_Channel_0      = 0x00,     /*!< ADC channel 0 */
    ADC_Channel_1      = 0x01,     /*!< ADC channel 1 */
    ADC_Channel_2      = 0x02,     /*!< ADC channel 2 */
    ADC_Channel_3      = 0x03,     /*!< ADC channel 3 */
    ADC_Channel_4      = 0x04,     /*!< ADC channel 4 */
    ADC_Channel_5      = 0x05,     /*!< ADC channel 5 */
    ADC_Channel_6      = 0x06,     /*!< ADC channel 6 */
    ADC_Channel_7      = 0x07,     /*!< ADC channel 7 */
    ADC_Channel_8      = 0x08,     /*!< ADC channel 8 */
    ADC_Channel_9      = 0x09,     /*!< ADC channel 9 */
    ADC_Channel_10     = 0x0A,     /*!< ADC channel 10 */
    ADC_Channel_11     = 0x0B,     /*!< ADC channel 11 */
    ADC_Channel_12     = 0x0C,     /*!< ADC channel 12 */
    ADC_Channel_13     = 0x0D,     /*!< ADC channel 13 */
    ADC_Channel_14     = 0x0E,     /*!< ADC channel 14 */
    ADC_Channel_15     = 0x0F,     /*!< ADC channel 15 */
    ADC_Channel_16     = 0x10,     /*!< ADC channel 16 */
    ADC_Channel_17     = 0x11,     /*!< ADC channel 17 */
    ADC_Channel_18     = 0x12,     /*!< ADC channel 18 */
    ADC_Channel_19     = 0x13,     /*!< ADC channel 19 */
    ADC_Channel_20     = 0x14,     /*!< ADC channel 20 */
    ADC_Channel_21     = 0x15,     /*!< ADC channel 21 */
    ADC_Channel_22     = 0x16,     /*!< ADC channel 22 */
    ADC_Channel_23     = 0x17,     /*!< ADC channel 23 */
    ADC_Channel_24     = 0x18,     /*!< ADC channel 24 */
    ADC_Channel_25     = 0x19,     /*!< ADC channel 25 */
    ADC_Channel_26     = 0x1A,     /*!< ADC channel 26 */
    ADC_Channel_27     = 0x1B,     /*!< ADC channel 27 */
    ADC_Channel_28     = 0x1C,     /*!< ADC channel 28 */
    ADC_Channel_VCC    = 0x1D,     /*!< ADC channel VCC */
} ADC_Channel_TypeDef;

/**
 * @brief     ADC INT
 */
typedef enum
{
    ADC_INT_EOC = (uint16_t)ADC_ADINTF_ADIF_Msk,
    ADC_INT_ADG = (uint16_t)ADC_ADINTF_ADGIF_Msk,
    ADC_INT_ADL = (uint16_t)ADC_ADINTF_ADLIF_Msk,
} ADC_INT_TypeDef;

/**
 * @brief     ADC FLAG
 */
typedef enum
{
    ADC_FLAG_EOC = (uint16_t)ADC_ADINTF_ADIF_Msk,
    ADC_FLAG_ADG = (uint16_t)ADC_ADINTF_ADGIF_Msk,
    ADC_FLAG_ADL = (uint16_t)ADC_ADINTF_ADLIF_Msk,
} ADC_FLAG_TypeDef;

/**  
  * @brief Structure for ADC initial
  */
typedef struct
{
    ADC_CONVERSION_TypeDef ADC_ConversionMode; /* Configure the ADC conversion Mode
                                               This parameter can be a value of ADC_CONVERSION_TypeDef */
    
    ADC_RESOLUTION_TypeDef ADC_Resolution;     /* Configure the ADC_Resolution Mode
                                               This parameter can be a value of ADC_RESOLUTION_TypeDef */
  
    ADC_VREF_TypeDef ADC_RefVoltage;           /* Select the ADC reference voltage sources
                                               This parameter can be a value of ADC_VREF_TypeDef */
                                          
    ADC_TRIGGER_TypeDef ADC_Trigger;           /* Configure the ADC external trigger sources
                                               This parameter can be a value of ADC_TRIGGER_TypeDef */

    ADC_CLKCYCLE_TypeDef ADC_Prescaler;        /* Configure the ADC clock prescaler
                                               This parameter can be a value of ADC_CLKCYCLE_TypeDef */
    
    ADC_NumOfConversion_TypeDef ADC_NumOfConversion;           /* Configure the ADC conversion channel numbers
                                               This parameter can be 0 to 7 */
                                          
    ADC_SAMPLE_TIME_TypeDef ADC_SampleTime;    /* Configure the ADC sample time
                                               This parameter can be a value of ADC_Sample_Times */

    ADC_TGAP_TypeDef ADC_TwoSampleGap;         /* Configure the ADC two sample gap time
                                               This parameter can be a value of ADC_TGAP_TypeDef */
                                               
    ADC_Seq_TypeDef ADC_SequencePointer;       /* Set the ADC channel pointer to select which channel to be start converted*/
    
} ADC_InitTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/

/** @defgroup ADC_Exported_Constants ADC Exported Constants
  * @{
  */

/** 
  * @brief ADC_Interrupt_Sources
  */

// #define ADC_INT_EOC                     (uint16_t)ADC_ADINTF_ADIF_Msk
// #define ADC_INT_ADG                     (uint16_t)ADC_ADINTF_ADGIF_Msk
// #define ADC_INT_ADL                     (uint16_t)ADC_ADINTF_ADLIF_Msk

    /**
     * @brief ADC_Flags
     */

// #define ADC_FLAG_EOC                    (uint16_t)ADC_ADINTF_ADIF_Msk
// #define ADC_FLAG_ADG                    (uint16_t)ADC_ADINTF_ADGIF_Msk
// #define ADC_FLAG_ADL                    (uint16_t)ADC_ADINTF_ADLIF_Msk

/**  
  * @brief ADC_Two_Sample_Gap_Cmd
  */

#define ADC_TwoSampleGap_Enable          (uint8_t)0x01
#define ADC_TwoSampleGap_Disable         (uint8_t)0x00 

/**
  * @}
  */

/* Check Macro ------------------------------------------------------------*/

/** @defgroup ADC_Parameter_Check_Macro ADC Parameter Check Macro 
  * @{
  */
  
  /* check ADC Conversion Mode */
#define  IS_ADC_CONVERSION_MODE(MODE)         ((MODE == ADC_ConversionMode_Single)           \
                                              || (MODE == ADC_ConversionMode_Discontinuous)  \
                                              || (MODE == ADC_ConversionMode_Continuous))

/* check ADC Resolution */
#define IS_ADC_RESOLUTION(RESOLUTION)         ((RESOLUTION == ADC_Resolution_12Bit) \
                                              || (RESOLUTION == ADC_Resolution_10Bit)) 

/* check ADC Reference Voltage */
#define IS_ADC_REF_VOLTAGE(VOLTAGE)           ((VOLTAGE == ADC_RefVoltage_AVDD)     \
                                              || (VOLTAGE == ADC_RefVoltage_ExtVref))

/* check ADC Conversion Number */
#define IS_ADC_CONVERSION_NUMBER(NUM)         ((NUM == ADC_NumOfConversion_1)   \
                                              || (NUM == ADC_NumOfConversion_2) \
                                              || (NUM == ADC_NumOfConversion_3) \
                                              || (NUM == ADC_NumOfConversion_4) \
                                              || (NUM == ADC_NumOfConversion_5) \
                                              || (NUM == ADC_NumOfConversion_6) \
                                              || (NUM == ADC_NumOfConversion_7) \
                                              || (NUM == ADC_NumOfConversion_8))

/* check ADC Trigger Source */
#define IS_ADC_TRIGGER(TRIGGER)               ((TRIGGER  == ADC_Trigger_Software)\
                                              ||(TRIGGER == ADC_Trigger_EXTI0   )\
                                              ||(TRIGGER == ADC_Trigger_EXTI1   )\
                                              ||(TRIGGER == ADC_Trigger_EXTI2   )\
                                              ||(TRIGGER == ADC_Trigger_EXTI3   )\
                                              ||(TRIGGER == ADC_Trigger_TIMTRG  )\
                                              ||(TRIGGER == ADC_Trigger_PWM0TRG )\
                                              ||(TRIGGER == ADC_Trigger_PWM1TRG )\
                                              ||(TRIGGER == ADC_Trigger_PWM2TRG )\
                                              ||(TRIGGER == ADC_Trigger_PWM3TRG )\
                                              ||(TRIGGER == ADC_Trigger_PCA0TRG )\
                                              ||(TRIGGER == ADC_Trigger_PCA1TRG )\
                                              ||(TRIGGER == ADC_Trigger_PCA2TRG )\
                                              ||(TRIGGER == ADC_Trigger_PCA3TRG ))

/* check ADC CLOCK */
#define IS_ADC_CLK_PRESCALER(DIV)             ((DIV == ADC_Prescaler_1)     || (DIV == ADC_Prescaler_2)      \
                                              || (DIV == ADC_Prescaler_3)   || (DIV == ADC_Prescaler_4)      \
                                              || (DIV == ADC_Prescaler_5)   || (DIV == ADC_Prescaler_6)      \
                                              || (DIV == ADC_Prescaler_8)   || (DIV == ADC_Prescaler_12)     \
                                              || (DIV == ADC_Prescaler_16)  || (DIV == ADC_Prescaler_24)     \
                                              || (DIV == ADC_Prescaler_32)  || (DIV == ADC_Prescaler_48)     \
                                              || (DIV == ADC_Prescaler_64)  || (DIV == ADC_Prescaler_128)    \
                                              || (DIV == ADC_Prescaler_256) || (DIV == ADC_Prescaler_320))

/* check ADC Sampling time */
#define IS_ADC_SAMPLE_TIME(TIME)              ((TIME == ADC_SampleTime_1Cycles)    || (TIME == ADC_SampleTime_2Cycles)   \
                                              || (TIME == ADC_SampleTime_3Cycles)  || (TIME == ADC_SampleTime_4Cycles)   \
                                              || (TIME == ADC_SampleTime_5Cycles)  || (TIME == ADC_SampleTime_6Cycles)   \
                                              || (TIME == ADC_SampleTime_7Cycles)  || (TIME == ADC_SampleTime_8Cycles)   \
                                              || (TIME == ADC_SampleTime_9Cycles)  || (TIME == ADC_SampleTime_10Cycles)  \
                                              || (TIME == ADC_SampleTime_11Cycles) || (TIME == ADC_SampleTime_12Cycles)  \
                                              || (TIME == ADC_SampleTime_13Cycles) || (TIME == ADC_SampleTime_14Cycles)  \
                                              || (TIME == ADC_SampleTime_15Cycles) || (TIME == ADC_SampleTime_16Cycles))

/* check ADC Time interval between adjacent channels in a sequence */
#define IS_ADC_TWO_SAMPLE_GAP(GAP)            ((GAP == ADC_TwoSampleGap_0Cycles)    || (GAP == ADC_TwoSampleGap_2Cycles)   \
                                              || (GAP == ADC_TwoSampleGap_4Cycles)  || (GAP == ADC_TwoSampleGap_8Cycles)   \
                                              || (GAP == ADC_TwoSampleGap_16Cycles) || (GAP == ADC_TwoSampleGap_32Cycles)  \
                                              || (GAP == ADC_TwoSampleGap_64Cycles) || (GAP == ADC_TwoSampleGap_128Cycles))
                                              
#define IS_ADC_ALL_PERIPH(PERIPH)             (PERIPH == ADC)                                      
                                      
#define IS_ADC_INT(INT)                       (((INT & (uint16_t)(~(ADC_ADINTF_ADIF_Msk|ADC_ADINTF_ADGIF_Msk|ADC_ADINTF_ADLIF_Msk))) == 0x00) \
                                              && (INT != 0x00))    
                                     
#define IS_ADC_FLAG(FLAG)                     (((FLAG & (uint16_t)(~(ADC_ADINTF_ADIF_Msk|ADC_ADINTF_ADGIF_Msk|ADC_ADINTF_ADLIF_Msk))) == 0x00) \
                                              && (FLAG != 0x00))

#define IS_ADC_GET_ONE_FLAG(FLAG)             ((FLAG == ADC_FLAG_EOC) \
                                              || (FLAG == ADC_FLAG_ADG) \
                                              || (FLAG == ADC_FLAG_ADL))
                                              
#define IS_ADC_SEQUENCE(SEQUENCE)             ((SEQUENCE == ADC_Sequence_0) || (SEQUENCE == ADC_Sequence_1) \
                                              || (SEQUENCE == ADC_Sequence_2) || (SEQUENCE == ADC_Sequence_3) \
                                              || (SEQUENCE == ADC_Sequence_4) || (SEQUENCE == ADC_Sequence_5) \
                                              || (SEQUENCE == ADC_Sequence_6) || (SEQUENCE == ADC_Sequence_7))
                                              
#define IS_ADC_TWO_SAMPLE_GAP_CMD(CMD)        ((CMD == ADC_TwoSampleGap_Enable) \
                                              || (CMD == ADC_TwoSampleGap_Disable)) 


#define IS_ADC_CHANNEL(CHANNEL)               ((CHANNEL == ADC_Channel_0)  || (CHANNEL == ADC_Channel_1)    \
                                              || (CHANNEL == ADC_Channel_2)  || (CHANNEL == ADC_Channel_3)  \
                                              || (CHANNEL == ADC_Channel_4)  || (CHANNEL == ADC_Channel_5)  \
                                              || (CHANNEL == ADC_Channel_6)  || (CHANNEL == ADC_Channel_7)  \
                                              || (CHANNEL == ADC_Channel_8)  || (CHANNEL == ADC_Channel_9)  \
                                              || (CHANNEL == ADC_Channel_10) || (CHANNEL == ADC_Channel_11) \
                                              || (CHANNEL == ADC_Channel_12) || (CHANNEL == ADC_Channel_13) \
                                              || (CHANNEL == ADC_Channel_14) || (CHANNEL == ADC_Channel_15) \
                                              || (CHANNEL == ADC_Channel_16) || (CHANNEL == ADC_Channel_17) \
                                              || (CHANNEL == ADC_Channel_18) || (CHANNEL == ADC_Channel_19) \
                                              || (CHANNEL == ADC_Channel_20) || (CHANNEL == ADC_Channel_21) \
                                              || (CHANNEL == ADC_Channel_22) || (CHANNEL == ADC_Channel_23) \
                                              || (CHANNEL == ADC_Channel_24) || (CHANNEL == ADC_Channel_25) \
                                              || (CHANNEL == ADC_Channel_26) || (CHANNEL == ADC_Channel_27) \
                                              || (CHANNEL == ADC_Channel_28) || (CHANNEL == ADC_Channel_VCC))

/**
  * @}
  */  
/* Exported functions --------------------------------------------------------*/

/** @defgroup ADC_Exported_Functions ADC Exported Functions
  * @{
  */
  
extern void ADC_Reset(void);

extern void ADC_StructInit(ADC_InitTypeDef *ADC_InitStruct);

extern void ADC_Init(ADC_TypeDef *ADCx, ADC_InitTypeDef *ADC_InitStruct);

extern void ADC_OnOff(ADC_TypeDef *ADCx, CmdState OnOffState);

extern void ADC_SoftwareStartConversion(ADC_TypeDef *ADCx);

extern void ADC_SoftwareStopConversion(ADC_TypeDef *ADCx);

extern uint8_t ADC_GetSoftwareStartConversionStatus(ADC_TypeDef *ADCx);

extern void ADC_ChannelConfig(ADC_TypeDef *ADCx, ADC_Channel_TypeDef ADC_Channel, ADC_Seq_TypeDef ADC_Sequence, uint8_t ADC_TwoSampleGapCmd);

extern void ADC_ExternalTriggerConfig(ADC_TypeDef *ADCx, ADC_TRIGGER_TypeDef ADC_Trigger);

extern void ADC_INTConfig(ADC_TypeDef *ADCx, ADC_INT_TypeDef ADC_INT, FunctionalState NewState);

extern uint16_t ADC_GetConversionValue(ADC_TypeDef *ADCx, ADC_Seq_TypeDef ADC_Sequence);

extern void ADC_SequenceResultCompareConfig(ADC_TypeDef *ADCx, uint16_t ADC_UpLimit, uint16_t ADC_LowerLimit, ADC_Seq_TypeDef ADC_Sequence);

extern FlagStatus ADC_GetFlagStatus(ADC_TypeDef *ADCx, ADC_FLAG_TypeDef ADC_Flag);

extern void ADC_ClearFlag(ADC_TypeDef *ADCx, ADC_FLAG_TypeDef ADC_Flag);

extern void ADC_ConvertSequenceConfig(ADC_TypeDef *ADCx, ADC_Seq_TypeDef ADC_Sequence);
  
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*__SH30F9XX_SB0_ADC_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT Sinowealth *****END OF FILE****/
