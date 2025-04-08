/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_timer.c
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-5-21    
  * @brief   This file provides firmware functions to manage the TIMER module
  *         
  @verbatim
  
  ==============================================================================
                         How to use this driver
  ==============================================================================
   [..]
     (#) Enable TIMER Clock use  RCC_APB0PeriphClockOnOff() function 
  
     (#) Fill TIMER TIM_InitTypeDef type structure variable's member as default use TIM_StructInit() function

     (#) Config TIMER Clock source ,period,TIMER Clock Prescaler etc use TIM_Init() function

     (#) Enable TIMER Module by calling TIM_OnOff() function

     (#) Use TIMER interrupt by following step
         
         (++) Config TIMER Interrupt use TIM_INTConfig() function
         
         (++) Enable Kernel TIMER's Global interrupt by calling NVIC_EnableIRQ() function
         
         (++) reference startup asm file for timer interrupt function name and rewrite this function 	
      
     (#) When timer interrupt happen, use TIM_GetFlagStatus() function to get timer interrupt flag
 
     (#) Call TIM_ClearFlag() function for clear timer interrupt flag

  @endverbatim
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
#include "sh30f9xx_sb0_timer.h"

/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */

/** @defgroup TIM_MODULE TIM 
  * @brief SH30F9XX SB0 TIM module driver library,reference this topic macro and function for drive TIM
  * @{
  */
  
/** @defgroup TIM_Exported_Functions  TIM Exported Functions 
  *
  * @brief    This is TIMER  exported function code group
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Reset the TIMx peripheral registers to their default reset values.
  * @param  TIMx: where x can be 0 to 3 to select the TIM peripheral.
  * @retval None
  */
void TIM_Reset(void *TIMx)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    if (TIMx == (void *)TIM0)
    {
        RCC_APB0PeriphReset(RCC_APB0_TIM0);
    }
    else if (TIMx == (void *)TIM1)
    {
        RCC_APB0PeriphReset(RCC_APB0_TIM1);
    }
    else if (TIMx == (void *)TIM2)
    {
        RCC_APB0PeriphReset(RCC_APB0_TIM2);
    }
    else
    {
        RCC_APB0PeriphReset(RCC_APB0_TIM3);
    }
}

/**
  * @brief  Fills each TIM_InitStruct member with its default value.
  * @param  TIM_InitStruct : pointer to a @ref TIM_InitTypeDef
  *         structure which will be initialized.
  * @retval None
  */
void TIM_StructInit(TIM_InitTypeDef *TIM_InitStruct)
{
    TIM_InitStruct->TIM_Prescaler = 0x00;
    TIM_InitStruct->TIM_Period    = 0x00;
    TIM_InitStruct->TIM_CLKSource = TIM_CLKSource_PCLK;
    TIM_InitStruct->TIM_OPMode    = TIM_OPMode_Continue;
    TIM_InitStruct->TIM_TCMode    = TIM_TCMode_Disable;
    TIM_InitStruct->TIM_ECFFilter = TIM_ECF_FLITER_TIME_NONE;
}

/**
  * @brief  Initializes the TIMx peripheral according to the specified parameters 
  *         in the TIM_InitStruct.
  * @param  TIMx: where x can be  0 to 3 to select the TIM peripheral.
  * @param  TIM_InitStruct: pointer to a @ref TIM_InitTypeDef structure that contains 
  *         the configuration information for the specified TIM peripheral.
  * @retval None
  */
void TIM_Init(void *TIMx, TIM_InitTypeDef *TIM_InitStruct)
{
    /* check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_CLK_SOURCE(TIM_InitStruct->TIM_CLKSource));
    assert_param(IS_TIM_OP_MODE(TIM_InitStruct->TIM_OPMode));

    if((TIMx == (void *)TIM0) || (TIMx == (void *)TIM1) || (TIMx == (void *)TIM2))
    {
        ((TIMER16_TypeDef *)TIMx)->PSQ = TIM_InitStruct->TIM_Prescaler;
        ((TIMER16_TypeDef *)TIMx)->TPR = TIM_InitStruct->TIM_Period;

        ((TIMER16_TypeDef *)TIMx)->CR.V32 &= ~(TIM_CR_CLKS_Msk | TIM_CR_OPM_Msk | TIM_CR_TC_Msk | TIM_CR_ECF_Msk);
        ((TIMER16_TypeDef *)TIMx)->CR.V32 |= TIM_InitStruct->TIM_CLKSource | TIM_InitStruct->TIM_OPMode  \
                        | TIM_InitStruct->TIM_TCMode | TIM_InitStruct->TIM_ECFFilter;
    }
    else if(TIMx == (void *)TIM3)
    {
        ((TIMER32_TypeDef *)TIMx)->PSQ = TIM_InitStruct->TIM_Prescaler;
        ((TIMER32_TypeDef *)TIMx)->TPR = TIM_InitStruct->TIM_Period;

        ((TIMER32_TypeDef *)TIMx)->CR.V32 &= ~(TIM_CR_CLKS_Msk | TIM_CR_OPM_Msk | TIM_CR_TC_Msk | TIM_CR_ECF_Msk);
        ((TIMER32_TypeDef *)TIMx)->CR.V32 |= TIM_InitStruct->TIM_CLKSource | TIM_InitStruct->TIM_OPMode  \
                        | TIM_InitStruct->TIM_TCMode | TIM_InitStruct->TIM_ECFFilter;
    }
}
/**
  * @brief  Enable or disable the specified TIM interrupt.
  * @param  TIMx: where x can be 0 to 3 to select the TIM peripheral.       
  * @param  NewState: new state of the TIM interrupt.
  *         This parameter can be one of the following values:
  *            @arg @b SH_ENABLE
  *            @arg @b SH_DISABLE
  * @retval None
  */
void TIM_INTConfig(void *TIMx, FunctionalState NewState)
{
    /* check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_FUNCTION_STATE(NewState));

    if (NewState != SH_DISABLE)
    {
        if((TIMx == (void *)TIM0) || (TIMx == (void *)TIM1) || (TIMx == (void *)TIM2))
        {
            ((TIMER16_TypeDef *)TIMx)->CR.V32 |= TIM_CR_IE_Msk;
        }
        else if(TIMx == (void *)TIM3)
        {
            ((TIMER32_TypeDef *)TIMx)->CR.V32 |= TIM_CR_IE_Msk;
        }
    }
    else
    {
        if((TIMx == (void *)TIM0) || (TIMx == (void *)TIM1) || (TIMx == (void *)TIM2))
        {
            ((TIMER16_TypeDef *)TIMx)->CR.V32 &= (~TIM_CR_IE_Msk);
        }
        else if(TIMx == (void *)TIM3)
        {
            ((TIMER32_TypeDef *)TIMx)->CR.V32 &= (~TIM_CR_IE_Msk);
        }
    }
}

/**
  * @brief  Open or close the TIM peripheral.
  * @param  TIMx: where x can be 0 to 3 to select the TIM peripheral.
  * @param  OnOffState: state of the TIM peripheral.
  *         This parameter can be one of the following values:
  *            @arg @b SH_ON
  *            @arg @b SH_OFF
  * @retval None
  */
void TIM_OnOff(void *TIMx, CmdState OnOffState)
{
    /* check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_CMD_STATE(OnOffState));

    if (OnOffState != SH_OFF)
    {
        if((TIMx == (void *)TIM0) || (TIMx == (void *)TIM1) || (TIMx == (void *)TIM2))
        {
            ((TIMER16_TypeDef *)TIMx)->CR.V32 |= TIM_CR_STR_Msk;
        }
        else if(TIMx == (void *)TIM3)
        {
            ((TIMER32_TypeDef *)TIMx)->CR.V32 |= TIM_CR_STR_Msk;
        }
    }
    else
    {
        if((TIMx == (void *)TIM0) || (TIMx == (void *)TIM1) || (TIMx == (void *)TIM2))
        {
            ((TIMER16_TypeDef *)TIMx)->CR.V32 &= (~TIM_CR_STR_Msk);
        }
        else if(TIMx == (void *)TIM3)
        {
            ((TIMER32_TypeDef *)TIMx)->CR.V32 &= (~TIM_CR_STR_Msk);
        }
    }
}

/**
  * @brief  Enable or disable the TIMx's output pin.
  * @param  TIMx: where x can be 0 to 3 to select the TIM peripheral.
  * @param  NewState: new state of the TIMx's output pin.
  *          This parameter can be one of the following values:
  *            @arg @b SH_ENABLE
  *            @arg @b SH_DISABLE
  * @retval None
  */
void TIM_OutPutConfig(void *TIMx, FunctionalState NewState)
{
    /* check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_FUNCTION_STATE(NewState));

    if (NewState != SH_DISABLE)
    {
        if((TIMx == (void *)TIM0) || (TIMx == (void *)TIM1) || (TIMx == (void *)TIM2))
        {
            ((TIMER16_TypeDef *)TIMx)->CR.V32 |= TIM_CR_TC_Msk;
        }
        else if(TIMx == (void *)TIM3)
        {
            ((TIMER32_TypeDef *)TIMx)->CR.V32 |= TIM_CR_TC_Msk;
        }
    }
    else
    {
        if((TIMx == (void *)TIM0) || (TIMx == (void *)TIM1) || (TIMx == (void *)TIM2))
        {
            ((TIMER16_TypeDef *)TIMx)->CR.V32 &= ~TIM_CR_TC_Msk;
        }
        else if(TIMx == (void *)TIM3)
        {
            ((TIMER32_TypeDef *)TIMx)->CR.V32 &= ~TIM_CR_TC_Msk;
        }
    }
}

/**
  * @brief  Enable or disable the TIMx's trigger CM0+ core function.
  * @param  TIMx: where x can be 0 to 3 to select the TIM peripheral.
  * @param  NewState: new state of the TIMx's trigger function.
  *          This parameter can be one of the following values:
  *            @arg @b SH_ENABLE
  *            @arg @b SH_DISABLE
  * @retval None
  */
void TIM_CM0EventTrigger(void *TIMx, FunctionalState NewState)
{
    /* check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_FUNCTION_STATE(NewState));

    if (NewState != SH_DISABLE)
    {
        if((TIMx == (void *)TIM0) || (TIMx == (void *)TIM1) || (TIMx == (void *)TIM2))
        {        
            ((TIMER16_TypeDef *)TIMx)->CR.V32 |= TIM_CR_ETEN_Msk;
        }
        else if(TIMx == (void *)TIM3)
        {
            ((TIMER32_TypeDef *)TIMx)->CR.V32 |= TIM_CR_ETEN_Msk;
        }
    }
    else
    {
        if((TIMx == (void *)TIM0) || (TIMx == (void *)TIM1) || (TIMx == (void *)TIM2))
        {  
            ((TIMER16_TypeDef *)TIMx)->CR.V32 &= ~TIM_CR_ETEN_Msk;
        }
        else if(TIMx == (void *)TIM3)
        {
            ((TIMER32_TypeDef *)TIMx)->CR.V32 &= ~TIM_CR_ETEN_Msk;
        }
    }
}

/**
  * @brief  Enable or disable the TIMx's trigger function.
  * @param  TIMx: where x can be 0 to 3 to select the TIM peripheral.
  * @param  NewState: new state of the TIMx's trigger function.
  *          This parameter can be one of the following values:
  *            @arg @b SH_ENABLE
  *            @arg @b SH_DISABLE
  * @retval None
  */
void TIM_ExtPeripheralsTrigger(void *TIMx, FunctionalState NewState)
{
    /* check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_FUNCTION_STATE(NewState));

    if (NewState != SH_DISABLE)
    {
        if((TIMx == (void *)TIM0) || (TIMx == (void *)TIM1) || (TIMx == (void *)TIM2))
        {     
            ((TIMER16_TypeDef *)TIMx)->CR.V32 |= TIM_CR_TRIGEN_Msk;
        }
        else if(TIMx == (void *)TIM3)
        {
            ((TIMER32_TypeDef *)TIMx)->CR.V32 |= TIM_CR_TRIGEN_Msk;
        }
    }
    else
    {
        if((TIMx == (void *)TIM0) || (TIMx == (void *)TIM1) || (TIMx == (void *)TIM2))
        { 
            ((TIMER16_TypeDef *)TIMx)->CR.V32 &= ~TIM_CR_TRIGEN_Msk;
        }
        else if(TIMx == (void *)TIM3)
        {
            ((TIMER32_TypeDef *)TIMx)->CR.V32 &= ~TIM_CR_TRIGEN_Msk;
        }
    }
}

/**
  * @brief  Check whether the specified TIM flag is set or not.
  * @param  TIMx: where x can be 0 to 3 to select the TIM peripheral.
  * @retval The new state of TIMx's flag(SH_SET or SH_RESET).
  */
FlagStatus TIM_GetFlagStatus(void *TIMx)
{
    FlagStatus bitStatus;

    /* check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    if((TIMx == (void *)TIM0) || (TIMx == (void *)TIM1) || (TIMx == (void *)TIM2))
    {
        if ((((TIMER16_TypeDef *)TIMx)->TIMINTF.V32 & TIM_TIMINTF_TF_Msk) != SH_RESET)
        {
            bitStatus = SH_SET;
        }
        else
        {
            bitStatus = SH_RESET;
        }
    }
    else if(TIMx == (void *)TIM3)
    {
        if ((((TIMER32_TypeDef *)TIMx)->TIMINTF.V32 & TIM_TIMINTF_TF_Msk) != SH_RESET)
        {
            bitStatus = SH_SET;
        }
        else
        {
            bitStatus = SH_RESET;
        }
    }

    return bitStatus;
}

/**
  * @brief  Clear the TIMx's pending flag.
  * @param  TIMx: where x can be 0 to 3 to select the TIM peripheral.
  * @retval None
  */
void TIM_ClearFlag(void *TIMx)
{
    /* check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    if((TIMx == (void *)TIM0) || (TIMx == (void *)TIM1) || (TIMx == (void *)TIM2))
    {
        ((TIMER16_TypeDef *)TIMx)->TIMINTF.V32 = TIM_TIMINTF_TFC_Msk;
    }
    else if(TIMx == (void *)TIM3)
    {
        ((TIMER32_TypeDef *)TIMx)->TIMINTF.V32 = TIM_TIMINTF_TFC_Msk;
    }
}

/**
  * @brief  Get the TIMx Period Register value
  * @param  TIMx: where x can be 0 to 3 to select the TIM peripheral.
  * @retval Counter Register value
  */
uint32_t TIM_GetCounter(void *TIMx)
{

    /* check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    if((TIMx == (void *)TIM0) || (TIMx == (void *)TIM1) || (TIMx == (void *)TIM2))
    {
        return ((TIMER16_TypeDef *)TIMx)->TCNT;
    }
    else if(TIMx == (void *)TIM3)
    {
        return ((TIMER32_TypeDef *)TIMx)->TCNT;
    }

    return 0;
}
/**
  * @brief  Get the TIMx PSQ Register value
  * @param  TIMx: where x can be 0 to 3 to select the TIM peripheral.
  * @retval Prescaler Register value TIM peripheral
  * @note May return zero if TIM peripheral parameter is illegal
  */
uint32_t TIM_GetPrescaler(void *TIMx)
{
    /* check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    
    if((TIMx == (void *)TIM0) || (TIMx == (void *)TIM1) || (TIMx == (void *)TIM2))
    {
        return ((TIMER16_TypeDef *)TIMx)->PSQ;
    }
    else if(TIMx == (void *)TIM3)
    {
        return ((TIMER32_TypeDef *)TIMx)->PSQ;
    }
    
    return 0;
}

/**
  * @brief  Get the TIMx Period Register value
  * @param  TIMx: where x can be 0 to 3 to select the TIM peripheral.
  * @retval Period Register value
  * @note May return zero if TIM peripheral parameter is illegal
  */
uint32_t TIM_GetPeriod(void *TIMx)
{
    /* check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    if((TIMx == (void *)TIM0) || (TIMx == (void *)TIM1) || (TIMx == (void *)TIM2))
    {
        return ((TIMER16_TypeDef *)TIMx)->TPR;
    }
    else if(TIMx == (void *)TIM3)
    {
        return ((TIMER32_TypeDef *)TIMx)->TPR;
    }

    return 0;
}

/**
  * @brief  Set the TIMx Counter Register value
  * @param  TIMx: where x can be 0 to 3 to select the TIM peripheral.
  * @param  Counter: specifies the Counter register new value.
  * @retval None
  */
void TIM_SetCounter(void *TIMx, uint32_t Counter)
{

    /* check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    if((TIMx == (void *)TIM0) || (TIMx == (void *)TIM1) || (TIMx == (void *)TIM2))
    {
        ((TIMER16_TypeDef *)TIMx)->TCNT = Counter;
    }
    else if(TIMx == (void *)TIM3)
    {
        ((TIMER32_TypeDef *)TIMx)->TCNT = Counter;
    }
    

}

/**
  * @brief  Set the TIMx Prescaler Register value
  * @param  TIMx: where x can be 0 to 3 to select the TIM peripheral.
  * @param  Prescaler: specifies the Prescaler register new value.
  * @retval None
  */
void TIM_SetPrescaler(void *TIMx, uint32_t Prescaler)
{
    /* check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    if((TIMx == (void *)TIM0) || (TIMx == (void *)TIM1) || (TIMx == (void *)TIM2))
    {
        ((TIMER16_TypeDef *)TIMx)->PSQ = Prescaler;
    }
    else if(TIMx == (void *)TIM3)
    {
        ((TIMER32_TypeDef *)TIMx)->PSQ = Prescaler;
    }
    

}

/**
  * @brief  Set the TIMx Period Register value
  * @param  TIMx: where x can be 0 to 3 to select the TIM peripheral.
  * @param  Period: specifies the Period register new value.
  * @retval None
  */
void TIM_SetPeriod(void *TIMx, uint32_t Period)
{
    /* check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    
    if((TIMx == (void *)TIM0) || (TIMx == (void *)TIM1) || (TIMx == (void *)TIM2))
    {
        ((TIMER16_TypeDef *)TIMx)->TPR = Period;
    }
    else if(TIMx == (void *)TIM3)
    {
        ((TIMER32_TypeDef *)TIMx)->TPR = Period;
    }
    
    

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
