/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_adc.c
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-5-22  
  * @brief   This file provides firmware functions to manage the ADC module 
  *  
    @verbatim
    
  ==============================================================================
                         How to use this driver
  ==============================================================================
   [..]
     (#) Enable ADC Clock use RCC_APB0PeriphClockOnOff() function 

     (#) Fill ADC ADC_InitTypeDef type structure variable's member as default use ADC_StructInit() function
     
     (#) Config ADC Pin
         (++) Enable Pin Clock calling RCC_AHBPeriphClockOnOff() function 
         
         (++) Call GPIO_Init()function for config ADC pin(input mode, no-pullup),if not use all member for ADC 
              pin config ,makesure call GPIO_StructInit() function for GPIO_Init function  paramter set default value

         (++) Call GPIO_PinAFConfig() for config pin as ADC function
     (#) Config ADC Clock prescaler ,sample time,Conversion num etc use ADC_Init() function
     
     (#) ADC channel Init by calling ADC_ChannelConfig() function

     (#) Enable ADC Module by calling ADC_OnOff() function
     
     (#) Start ADC Module by calling ADC_SoftwareStartConversion() function when set trigger by software

     (#) Use ADC interrupt by following step
            
         (++) Config ADC Interrupt use ADC_INTConfig() function
         
         (++) Enable Kernel ADC's Global interrupt by calling NVIC_EnableIRQ() function
         
         (++) reference startup asm file for ADC interrupt function name and rewrite this function 	
      
     (#) When ADC interrupt happen, use ADC_GetFlagStatus() function to get ADC interrupt flag

     (#) Call ADC_ClearFlag() function for clear ADC interrupt flag

  *  @endverbatim
  *
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
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "sh30f9xx_sb0_adc.h"

/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */

/** @defgroup ADC_MODULE ADC
  * @brief SH30F9XX SB0 ADC module driver library,reference this topic macro and function for drive ADC
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Export function prototypes -----------------------------------------------*/

/** @defgroup ADC_Exported_Functions ADC Exported Functions
 *  @brief    This is ADC exported function code group
 *
  * @{
  */

/**
  * @brief  Reset the ADC peripherals registers to their default reset values.
  * @retval None
  */
void ADC_Reset(void)
{
    /* Check the parameters */
    RCC_APB0PeriphReset(RCC_APB0_ADC);
}

/**
  * @brief  Fills each ADC_InitStruct member with its default value. 
  * @param  ADC_InitStruct: pointer to a @ref ADC_InitTypeDef structure which will 
  *         be initialized.
  * @retval None
  */
void ADC_StructInit(ADC_InitTypeDef *ADC_InitStruct)
{
    /* Initialize the ADC_Resolution member */
    ADC_InitStruct->ADC_Resolution = ADC_Resolution_12Bit;

    /* Initialize the ADC_Trigger member */
    ADC_InitStruct->ADC_Trigger = ADC_Trigger_Software;

    /* Initialize the ADC_RefVoltage member */
    ADC_InitStruct->ADC_RefVoltage = ADC_RefVoltage_AVDD;

    /* Initialize the ADC_ConversionMode member */
    ADC_InitStruct->ADC_ConversionMode = ADC_ConversionMode_Single;

    /* Initialize the ADC_ChannelPointer member */
    ADC_InitStruct->ADC_SequencePointer = ADC_Sequence_0;

    /* Initialize the ADC_NumOfConversion member */
    ADC_InitStruct->ADC_NumOfConversion = ADC_NumOfConversion_1;

    /* Initialize the ADC_ClkPrescaler member */
    ADC_InitStruct->ADC_Prescaler = ADC_Prescaler_1;

    /* Initialize the ADC_TwoSampleGap member */
    ADC_InitStruct->ADC_TwoSampleGap = ADC_TwoSampleGap_0Cycles;

    /* Initialize the ADC_SampleTime member */
    ADC_InitStruct->ADC_SampleTime = ADC_SampleTime_2Cycles;
}

/**
  * @brief  Initializes the ADCx peripheral according to the specified parameters 
  *         in the ADC_InitStruct.
  * @param  ADCx:  ADC peripheral.
  * @param  ADC_InitStruct: pointer to a @ref ADC_InitTypeDef structure that contains
  *         the configuration information for the specified ADC peripheral.
  * @retval None
  * @note   Sample time
  *         Before setting TS[3:0], it is necessary to estimate the series resistance connected
  *         to the ADC input pin to obtain the best sampling accuracy. When selecting 1*tAD as  
  *         the sampling time, please ensure that the series resistnce connected to the ADC
  *         input pin meets the input impedance reuirements.
  *  
  * @note   Sample rate
  *         ADC channels AN4 (PB7) and AN5 (PB8) are high-speed ports, can reach up
  *         to conversion rate of 1MSPS, while the other low-speed ports channels
  *         (AN0-AN3, AN6-AN28) are 100kSPS. When both high-speed ports (AN4, AN5) and 
  *         low-speed ports(AN0-AN3, AN6-AN28) are used, the conversion rate synchronized
  *         with the low-speed port conversion rate.
  */
void ADC_Init(ADC_TypeDef *ADCx, ADC_InitTypeDef *ADC_InitStruct)
{
    uint8_t pos = 0;
    uint16_t tempReg1 = 0;
    uint32_t tempReg2 = 0;

    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_RESOLUTION(ADC_InitStruct->ADC_Resolution));
    assert_param(IS_ADC_TRIGGER(ADC_InitStruct->ADC_Trigger));
    assert_param(IS_ADC_REF_VOLTAGE(ADC_InitStruct->ADC_RefVoltage));
    assert_param(IS_ADC_CONVERSION_MODE(ADC_InitStruct->ADC_ConversionMode));
    assert_param(IS_ADC_CONVERSION_NUMBER(ADC_InitStruct->ADC_NumOfConversion));
    assert_param(IS_ADC_CLK_PRESCALER(ADC_InitStruct->ADC_Prescaler));
    assert_param(IS_ADC_TWO_SAMPLE_GAP(ADC_InitStruct->ADC_TwoSampleGap));
    assert_param(IS_ADC_SAMPLE_TIME(ADC_InitStruct->ADC_SampleTime));

    tempReg1 = ADCx->ADCON1.V32;
    tempReg2 = ADCx->ADCON2.V32;

    /* Clear the ADCTU/MOD/REFC/ADSTRS bits */
    tempReg1 &= ~(ADC_ADCON1_ADCTU_Msk \
                 | ADC_ADCON1_MOD_Msk  \
                 | ADC_ADCON1_ADSTRS_Msk);

    tempReg1 |=  (ADC_InitStruct->ADC_ConversionMode << ADC_ADCON1_ADCTU_Pos) \
                 | (ADC_InitStruct->ADC_Resolution   << ADC_ADCON1_MOD_Pos)   \
                 | (ADC_InitStruct->ADC_Trigger      << ADC_ADCON1_ADSTRS_Pos);

    ADCx->ADCON1.V32 = tempReg1;

    /* Clear the TADC/ADMAXCH/TS/TGAP bits */
    tempReg2 &= ~(ADC_ADCON2_TADC_Msk    \
                 | ADC_ADCON2_ADMAXCH_Msk\
                 | ADC_ADCON2_TS_Msk     \
                 | ADC_ADCON2_TGAP_Msk);

    tempReg2 |= ((ADC_InitStruct->ADC_Prescaler)          << ADC_ADCON2_TADC_Pos)    \
                 | ((ADC_InitStruct->ADC_NumOfConversion) << ADC_ADCON2_ADMAXCH_Pos) \
                 | (ADC_InitStruct->ADC_SampleTime        << ADC_ADCON2_TS_Pos)      \
                 | (ADC_InitStruct->ADC_TwoSampleGap      << ADC_ADCON2_TGAP_Pos);

    if (ADC_InitStruct->ADC_ConversionMode == ADC_ConversionMode_Discontinuous)
    {
        /* Check the channel point register */
        assert_param(IS_ADC_SEQUENCE(ADC_InitStruct->ADC_SequencePointer));

        pos = (ADC_InitStruct->ADC_SequencePointer >> 2) - 3;

        ADCx->ADPCH &= ~ADC_ADPCH_ADPCH_Msk;

        /* To change this bit, ADSOC must be 0 */
        ADCx->ADPCH |= pos;
    }
    ADCx->ADCON2.V32 = tempReg2;
}

/**
  * @brief  Open or close the specified ADC peripheral.  
  * @param  ADCx:  ADC peripheral.
  * @param  OnOffState: state of the ADC peripheral.
  *         This parameter can be one of the following values:
  *            @arg @b SH_ON
  *            @arg @b SH_OFF
  * @retval None
  */
void ADC_OnOff(ADC_TypeDef *ADCx, CmdState OnOffState)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_CMD_STATE(OnOffState));

    if (OnOffState != SH_OFF)
    {
        /* Enable the ADC peripheral */
        ADCx->ADCON1.V32 |= ADC_ADCON1_ADON_Msk;
    }
    else
    {
        /* Disable the ADC peripheral */
        ADCx->ADCON1.V32 &= ~ADC_ADCON1_ADON_Msk;
    }
}

/**
  * @brief  Start conversion of the specified ADC peripheral.  
  * @param  ADCx:  ADC peripheral.
  * @retval None
  * @note   when start ADC conversion, make sure ADON\@ADCON1 set 10us before ADSOC\@ADCON1 set.
  *         before converting analog channels, it is necessary to first set the port to   
  *         AD channel function in order to successfully convert.
  */
void ADC_SoftwareStartConversion(ADC_TypeDef *ADCx)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));

    ADCx->ADCON1.V32 |= ADC_ADCON1_ADSOC_Msk;
}

/**
  * @brief  Stop conversion of the specified ADC peripheral.  
  * @param  ADCx:  ADC peripheral.
  * @retval None
  */
void ADC_SoftwareStopConversion(ADC_TypeDef *ADCx)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));

    ADCx->ADCON1.V32 &= ~ADC_ADCON1_ADSOC_Msk;
}

/**
  * @brief  Get start conversion of the specified ADC peripheral.  
  * @param  ADCx:  ADC peripheral.
  * @retval ADC start status
  *         can be one of the following values:
  *         @arg @b SH_SET 
  *         @arg @b SH_RESET
  */
uint8_t ADC_GetSoftwareStartConversionStatus(ADC_TypeDef *ADCx)
{
    uint8_t ADC_StartStatus = 0;

    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));

    if ((ADCx->ADCON1.V32 & ADC_ADCON1_ADSOC_Msk) != (uint32_t)SH_RESET)
    {
        ADC_StartStatus = (uint8_t)SH_SET;
    }
    else
    {
        ADC_StartStatus = (uint8_t)SH_RESET;
    }

    return ADC_StartStatus;
}

/**
  * @brief  Configure the selected ADC channel for its sample sequence and whether enable or
  *         disable two sample gap function.
  * @param  ADCx: ADC peripheral.
  * @param  ADC_Channel: specifie ADC channel which to be configured.
  *         This parameter can be one of the following values:
  *           @arg @b ADC_Channel_0   
  *           @arg @b ADC_Channel_1   
  *           @arg @b ADC_Channel_2   
  *           @arg @b ADC_Channel_3   
  *           @arg @b ADC_Channel_4   
  *           @arg @b ADC_Channel_5   
  *           @arg @b ADC_Channel_6   
  *           @arg @b ADC_Channel_7   
  *           @arg @b ADC_Channel_8   
  *           @arg @b ADC_Channel_9   
  *           @arg @b ADC_Channel_10  
  *           @arg @b ADC_Channel_11  
  *           @arg @b ADC_Channel_12  
  *           @arg @b ADC_Channel_13  
  *           @arg @b ADC_Channel_14  
  *           @arg @b ADC_Channel_15  
  *           @arg @b ADC_Channel_16  
  *           @arg @b ADC_Channel_17  
  *           @arg @b ADC_Channel_18  
  *           @arg @b ADC_Channel_19  
  *           @arg @b ADC_Channel_20  
  *           @arg @b ADC_Channel_21  
  *           @arg @b ADC_Channel_22  
  *           @arg @b ADC_Channel_23  
  *           @arg @b ADC_Channel_24  
  *           @arg @b ADC_Channel_25  
  *           @arg @b ADC_Channel_26
  *           @arg @b ADC_Channel_27  
  *           @arg @b ADC_Channel_28  
  *           @arg @b ADC_Channel_vcc  
  * @param  ADC_Sequence: specifie sample sequence in sample group. 
  *         This parameter can be one of the following values:
  *           @arg @b ADC_Sequence_0
  *           @arg @b ADC_Sequence_1
  *           @arg @b ADC_Sequence_2
  *           @arg @b ADC_Sequence_3
  *           @arg @b ADC_Sequence_4
  *           @arg @b ADC_Sequence_5
  *           @arg @b ADC_Sequence_6
  *           @arg @b ADC_Sequence_7  
  * @param  ADC_TwoSampleGapCmd: enable or disable ADC two sample gap function.
  *          This parameter can be one of the following values:
  *            @arg @b ADC_TwoSampleGap_Enable
  *            @arg @b ADC_TwoSampleGap_Disable
  * @retval None
  */
void ADC_ChannelConfig(ADC_TypeDef *ADCx, ADC_Channel_TypeDef ADC_Channel, ADC_Seq_TypeDef ADC_Sequence,
                       uint8_t ADC_TwoSampleGapCmd)
{
    uint8_t pos = 0;
    uint32_t temp = 0;

    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_CHANNEL(ADC_Channel));
    assert_param(IS_ADC_SEQUENCE(ADC_Sequence));
    assert_param(IS_ADC_TWO_SAMPLE_GAP_CMD(ADC_TwoSampleGapCmd));

    pos = (ADC_Sequence >> 2) - 3;

    ADC->ADGAPON &= ~(0x01 << pos);
    ADC->ADGAPON |= (ADC_TwoSampleGapCmd << pos);
    if (pos < 4)
    {
        temp = ADCx->SEQCHSEL0.V32;
        temp &= ~(ADC_SEQCHSEL0_SEQCH0_Msk << (pos << 3));
        temp |= ADC_Channel << (pos << 3);
        ADCx->SEQCHSEL0.V32 = temp;
    }
    else if (pos < 8)
    {
        pos -= 4;
        temp = ADCx->SEQCHSEL1.V32;
        temp &= ~(ADC_SEQCHSEL1_SEQCH4_Msk << (pos << 3));
        temp |= ADC_Channel << (pos << 3);
        ADCx->SEQCHSEL1.V32 = temp;
    }
}

/**
  * @brief  Configure external peripherals trigger ADC start to convert.  
  * @param  ADCx:  ADC peripheral.
  * @param  ADC_Trigger: specifie the ADC trigger to start conversion.
  *         This parameter can be one of the following values:
  *            @arg @b ADC_Trigger_Software
  *            @arg @b ADC_Trigger_EXTI0   
  *            @arg @b ADC_Trigger_EXTI1   
  *            @arg @b ADC_Trigger_EXTI2   
  *            @arg @b ADC_Trigger_EXTI3   
  *            @arg @b ADC_Trigger_TIMTRG  
  *            @arg @b ADC_Trigger_PWM0TRG 
  *            @arg @b ADC_Trigger_PWM1TRG 
  *            @arg @b ADC_Trigger_PWM2TRG 
  *            @arg @b ADC_Trigger_PWM3TRG 
  *            @arg @b ADC_Trigger_PCA0TRG 
  *            @arg @b ADC_Trigger_PCA1TRG 
  *            @arg @b ADC_Trigger_PCA2TRG 
  *            @arg @b ADC_Trigger_PCA3TRG 
  * @retval None
  */
void ADC_ExternalTriggerConfig(ADC_TypeDef *ADCx, ADC_TRIGGER_TypeDef ADC_Trigger)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_TRIGGER(ADC_Trigger));

    ADCx->ADCON1.V32 &= ~ADC_ADCON1_ADSTRS_Msk;
    ADCx->ADCON1.V32 |= (ADC_Trigger << ADC_ADCON1_ADSTRS_Pos);
}

/**
  * @brief  Enable or disable the specified ADC interrupts.  
  * @param  ADCx:  ADC peripheral.
  * @param  ADC_INT: specifie the ADC interrupt sources to be enabled or disabled.
  *         This parameter can be any combination of the following values:
  *            @arg @b ADC_INT_EOC
  *            @arg @b ADC_INT_ADG
  *            @arg @b ADC_INT_ADL
  * @param  NewState: new state of ADC's interrupts.
  *         This parameter can be one of the following values:
  *            @arg @b SH_ENABLE
  *            @arg @b SH_DISABLE
  * @retval None
  * @note   ADIF, ADLIF and ADGIF are share an interrupt vector, when ADIE, ADLIE and ADGIE
  *         are all turn on, it can check ADIF, ADLIF or ADGIF to determine which
  *         interrupt is occurred.
  */
void ADC_INTConfig(ADC_TypeDef *ADCx, ADC_INT_TypeDef ADC_INT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_INT(ADC_INT));
    assert_param(IS_FUNCTION_STATE(NewState));

    if (NewState != SH_DISABLE)
    {
        if ((ADC_INT & ADC_INT_EOC) == ADC_INT_EOC)
        {
            ADCx->ADCON1.V32 |= ADC_ADCON1_ADIE_Msk;
        }
        if ((ADC_INT & ADC_INT_ADG) == ADC_INT_ADG)
        {
            ADCx->ADCMPCON.V32 |= ADC_ADCMPCON_ADGIE_Msk;
        }
        if ((ADC_INT & ADC_INT_ADL) == ADC_INT_ADL)
        {
            ADCx->ADCMPCON.V32 |= ADC_ADCMPCON_ADLIE_Msk;
        }
    }
    else
    {
        if ((ADC_INT & ADC_INT_EOC) == ADC_INT_EOC)
        {
            ADCx->ADCON1.V32 &= ~ADC_ADCON1_ADIE_Msk;
        }
        if ((ADC_INT & ADC_INT_ADG) == ADC_INT_ADG)
        {
            ADCx->ADCMPCON.V32 &= ~ADC_ADCMPCON_ADGIE_Msk;
        }
        if ((ADC_INT & ADC_INT_ADL) == ADC_INT_ADL)
        {
            ADCx->ADCMPCON.V32 &= ~ADC_ADCMPCON_ADLIE_Msk;
        }
    }
}

/**
  * @brief  Get the specified ADC's channel data.  
  * @param  ADCx:  ADC peripheral.
  * @param  ADC_Sequence: specifie the ADC's Sample Sequence to get data.
  *         This parameter can be one of the following values:
  *            @arg @b ADC_Sequence_0
  *            @arg @b ADC_Sequence_1
  *            @arg @b ADC_Sequence_2
  *            @arg @b ADC_Sequence_3  
  *            @arg @b ADC_Sequence_4
  *            @arg @b ADC_Sequence_5
  *            @arg @b ADC_Sequence_6
  *            @arg @b ADC_Sequence_7    
  * @retval The specified Channel data.
  */
uint16_t ADC_GetConversionValue(ADC_TypeDef *ADCx, ADC_Seq_TypeDef ADC_Sequence)
{
    uint16_t temp = 0;

    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));

    assert_param(IS_ADC_SEQUENCE(ADC_Sequence));

    temp = (*(volatile unsigned long *)((uint32_t)ADCx + ADC_Sequence));
    return temp;
}

/**
  * @brief  Compare the ADC conversion value with the Max and Min data set in registers.  
  * @param  ADCx:  ADC peripheral.
  * @param  ADC_UpLimit: specifie the Max value to be compared with ADC conversion value.
  *         This parameter can be 0 to 0xFFFF.
  * @param  ADC_LowerLimit: specifie the Min value to be compared with ADC conversion value.
  *         This parameter can be 0 to 0xFFFF.  
  * @param  ADC_Sequence: specifie the ADC sample sequence to be compared.
  *         This parameter can be one of the following values:
  *            @arg @b ADC_Sequence_0
  *            @arg @b ADC_Sequence_1
  *            @arg @b ADC_Sequence_2
  *            @arg @b ADC_Sequence_3
  *            @arg @b ADC_Sequence_4
  *            @arg @b ADC_Sequence_5
  *            @arg @b ADC_Sequence_6
  *            @arg @b ADC_Sequence_7  
  * @retval None
  */
void ADC_SequenceResultCompareConfig(ADC_TypeDef *ADCx, uint16_t ADC_UpLimit,
                                     uint16_t ADC_LowerLimit, ADC_Seq_TypeDef ADC_Sequence)
{
    uint8_t pos = 0;
    uint16_t temp = 0;

    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_SEQUENCE(ADC_Sequence));

    pos = (ADC_Sequence >> 2) - 3;

    temp = ADCx->ADCMPCON.V32;
    temp &= ~ADC_ADCMPCON_CSEL_Msk;
    temp |= pos;
    ADCx->ADCMPCON.V32 = temp;

    ADCx->ADDGT = ADC_UpLimit;
    ADCx->ADDLT = ADC_LowerLimit;
}

/**
  * @brief  Check whether the specified ADC flag is set or not
  * @param  ADCx:  ADC peripheral.
  * @param  ADC_Flag: specifie the flag to check.
  *         This parameter can be one of the following values:
  *            @arg @b ADC_FLAG_EOC
  *            @arg @b ADC_FLAG_ADG
  *            @arg @b ADC_FLAG_ADL  
  * @retval Flag Status
  *         can be one of the following values:
  *         @arg @b SH_SET
  *         @arg @b SH_RESET
  */
FlagStatus ADC_GetFlagStatus(ADC_TypeDef *ADCx, ADC_FLAG_TypeDef ADC_Flag)
{
    FlagStatus bitStatus;

    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_GET_ONE_FLAG(ADC_Flag));

    if ((ADCx->ADINTF.V32 & ADC_Flag) != (uint32_t)SH_RESET)
    {
        bitStatus = SH_SET;
    }
    else
    {
        bitStatus = SH_RESET;
    }

    return bitStatus;
}

/**
  * @brief  Clear the ADC's pending flags.
  * @param  ADCx:  ADC peripheral.
  * @param  ADC_Flag: specifie the flag to clear.
  *         This parameter can be any combination of the following values:
  *            @arg @b ADC_FLAG_EOC
  *            @arg @b ADC_FLAG_ADG
  *            @arg @b ADC_FLAG_ADL  
  * @retval None
  */
void ADC_ClearFlag(ADC_TypeDef *ADCx, ADC_FLAG_TypeDef ADC_Flag)
{
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_FLAG(ADC_Flag));

    ADCx->ADINTF.V32 |= ADC_Flag << 16;
}

/**
  * @brief  ADC convert sequence set.  
  * @param  ADCx:  ADC peripheral.
  * @param  ADC_Sequence: specifie the ADC sample sequence to be compared.
  *         This parameter can be one of the following values:
  *            @arg @b ADC_Sequence_0
  *            @arg @b ADC_Sequence_1
  *            @arg @b ADC_Sequence_2
  *            @arg @b ADC_Sequence_3
  *            @arg @b ADC_Sequence_4
  *            @arg @b ADC_Sequence_5
  *            @arg @b ADC_Sequence_6
  *            @arg @b ADC_Sequence_7  
  * @retval None
  * @note   ADPCH register can only be written by software when ADC is not 
  *         sampled (ADSOC=0) and in interval-sample mode.
  *         Writting ADPCH register in Continuous-sample mode or Single-sample
  *         mode is invalid.
  */
void ADC_ConvertSequenceConfig(ADC_TypeDef *ADCx, ADC_Seq_TypeDef ADC_Sequence)
{
    uint8_t pos = 0;
    
    /* Check the parameters */
    assert_param(IS_ADC_ALL_PERIPH(ADCx));
    assert_param(IS_ADC_SEQUENCE(ADC_Sequence));

    pos = (ADC_Sequence >> 2) - 3;

    ADCx->ADPCH = pos;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT Sinowealth *****END OF FILE****/
