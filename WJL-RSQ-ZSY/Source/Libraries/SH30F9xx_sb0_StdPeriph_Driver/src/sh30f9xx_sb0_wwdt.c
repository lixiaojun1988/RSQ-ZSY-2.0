/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_wwdt.c
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-5-21    
  * @brief   This file provides firmware functions to manage the WWDT module
  *         
  @verbatim  
  ==============================================================================
                         How to use this driver
  ==============================================================================
   [..]
     (#) Enable WWWDT Clock use  RCC_APB0PeriphClockOnOff() function
       
     (#) Config WWDT period ,enable,window value and reload value by calling WWDT_Enable() function 
  
     (#) Use WWDT interrupt by following step

        (++) Config WWDT Interrupt use WWDT_EnableINT() function
        
        (++) Enable Kernel WWDT's Global interrupt by calling NVIC_EnableIRQ() function
        
        (++) reference startup asm file for WWDT interrupt function name and rewrite this function 
            
     (#) When WWDT interrupt happen, use WWDT_GetINTFlagStatus() function to get WWDT interrupt flag

     (#) Call WWDT_ClearFlag() function for clear WWDT interrupt flag
      
     (#) Set WWDT window value by calling WWDT_SetWindowValue() function

     (#) Set WWDT reload counter value by calling WWDT_SetCounter() function

     (#) Set WWDT Prescaler by calling 	WWDT_SetPrescaler() function 

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
#include "sh30f9xx_sb0_wwdt.h"

/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */

/** @defgroup WWDT_MODULE WWDT 
  * @brief SH30F9XX SB0 WWDT module driver library,reference this topic macro and function  for drive WWDT
  * @{
  */


/** @defgroup WWDT_Exported_Functions  WWDT Exported Functions 
  * 
  *  @brief    This is WWDT  exported function code group
  * @{
  */
  
  
/**
  * @brief  Deinitializes the WWDT peripheral registers to their default reset values.
  * @retval  None
  */
void WWDT_Reset(void)
{
    RCC_APB0PeriphReset(RCC_APB0_WWDT);
}

/**
  * @brief  Sets the WWDT Prescaler.
  * @param  WWDT_Prescaler: specifies the WWDT Prescaler.
  *   This parameter can be one of the following values:
  *     @arg @b WWDT_Prescaler_1:     WWDT counter clock = (PCLK0/256)/1
  *     @arg @b WWDT_Prescaler_2:     WWDT counter clock = (PCLK0/256)/2
  *     @arg @b WWDT_Prescaler_4:     WWDT counter clock = (PCLK0/256)/4
  *     @arg @b WWDT_Prescaler_8:     WWDT counter clock = (PCLK0/256)/8
  *     @arg @b WWDT_Prescaler_16:    WWDT counter clock = (PCLK0/256)/16
  *     @arg @b WWDT_Prescaler_32:    WWDT counter clock = (PCLK0/256)/32
  *     @arg @b WWDT_Prescaler_64:    WWDT counter clock = (PCLK0/256)/64
  *     @arg @b WWDT_Prescaler_128    WWDT counter clock = (PCLK0/256)/128  
  * @retval None
  */
void WWDT_SetPrescaler(uint32_t WWDT_Prescaler)
{
    uint32_t tmpreg = 0;
    /* Check the parameters */
    assert_param(IS_WWDT_PRESCALER(WWDT_Prescaler));
    /* Clear WWDTPR[2:0] bits */
    tmpreg = WWDT->CR.V32 & (~WWDT_CR_WWDTPR_Msk);
    /* Set WWDTPR[2:0] bits according to WWDT_Prescaler value */
    tmpreg |= ((0x5AA5 << WWDT_CR_LOCK_Pos) | WWDT_Prescaler << WWDT_CR_WWDTPR_Pos);
    /* Store the new value */
    WWDT->CR.V32 = tmpreg;
}

/**
  * @brief  Sets the WWDT window value.
  * @param  WindowValue: specifies the window value to be compared to the downcounter.
  * @retval None
  */
void WWDT_SetWindowValue(uint8_t WindowValue)
{
    /* Set WWDTWTR[7:0] bits according to WindowValue value */
    WWDT->WTR.V32 = ((0x5AA5 << WWDT_WTR_LOCK_Pos) | WindowValue);
}

/**
  * @brief  Controles the WWDT Early Wakeup interrupt.(Enable or disable)
  * @note   Once enabled this interrupt cannot be disabled except by a system reset.
  * @param  NewState: Enable or disable the WWDT Early Wakeup interrupt
  *         This parameter can be one of the following values:
  *         @arg @b SH_ENABLE  Enable WWDT Interrupt
  *         @arg @b SH_DISABLE  Disable WWDT Interrupt
  * @retval None
  */
void WWDT_INTConfig(FunctionalState NewState)
{
    uint32_t tmpreg = 0;

    /* Clear WWDTIE bits */
    tmpreg = WWDT->CR.V32 & (~WWDT_CR_WWDTIE_Msk);

    if(NewState == SH_ENABLE)
    {
        /* Set WWDTIE bits according to WWDT_Prescaler value */
        tmpreg |= ((0x5AA5 << WWDT_CR_LOCK_Pos) | (0x01 << WWDT_CR_WWDTIE_Pos));
    }
    else 
    {
        /* Set WWDTIE bits according to WWDT_Prescaler value */
        tmpreg |= ((0x5AA5 << WWDT_CR_LOCK_Pos) | (0x00 << WWDT_CR_WWDTIE_Pos));
    }

    /* Store the new value */
    WWDT->CR.V32 = tmpreg;
}

/**
  * @brief  Sets the WWDT counter value.
  * @param  Counter: specifies the watchdog counter value.
  *   This parameter must be a number between 0x0 and 0xFF (to prevent generating
  *   an immediate reset) 
  * @retval None
  */
void WWDT_SetCounter(uint8_t Counter)
{
    uint32_t tmpreg = 0;

    /* Clear WWDTRLR[7:0] bits */
    tmpreg = WWDT->CR.V32 & (~WWDT_CR_WWDTRLR_Msk);
    /* Sets the WWDT counter value */
    tmpreg |= ((0x5AA5 << WWDT_CR_LOCK_Pos) | (Counter & 0xFF));
    /* Store the new value */
    WWDT->CR.V32 = tmpreg;
}

/**
  * @brief  Enables WWDT and load the counter value.                  
  * @param  WWDT_Prescaler: specifies the WWDT Prescaler.
  *   This parameter can be one of the following values:
  *     @arg @b WWDT_Prescaler_1:      WWDT counter clock = (PCLK0/256)/1
  *     @arg @b WWDT_Prescaler_2:      WWDT counter clock = (PCLK0/256)/2
  *     @arg @b WWDT_Prescaler_4:      WWDT counter clock = (PCLK0/256)/4
  *     @arg @b WWDT_Prescaler_8:      WWDT counter clock = (PCLK0/256)/8
  *     @arg @b WWDT_Prescaler_16:     WWDT counter clock = (PCLK0/256)/16
  *     @arg @b WWDT_Prescaler_32:     WWDT counter clock = (PCLK0/256)/32
  *     @arg @b WWDT_Prescaler_64:     WWDT counter clock = (PCLK0/256)/64
  *     @arg @b WWDT_Prescaler_128:    WWDT counter clock = (PCLK0/256)/128  
  * @param  EnableINT: Controles the WWDT Early Wakeup interrupt. can be value SH_TRUE or SH_FALSE
  * @param  Counter: specifies the watchdog counter value.
  *   This parameter must be a number between 0x0 and 0xFF (to prevent generating
  *   an immediate reset)
  * @param  WindowValue: specifies the window value to be compared to the downcounter.
  * @retval None
  */
void WWDT_Enable(uint32_t WWDT_Prescaler, BoolType EnableINT, uint8_t Counter, uint8_t WindowValue)
{
    /* Check the parameters */
    assert_param(IS_WWDT_PRESCALER(WWDT_Prescaler));

    WWDT->CR.V32 = ((0x5AA5 << WWDT_CR_LOCK_Pos) \
                   | (EnableINT << WWDT_CR_WWDTIE_Pos) \
                   | (SH_ENABLE << WWDT_CR_WWDTON_Pos) \
                   | (WWDT_Prescaler << WWDT_CR_WWDTPR_Pos) | (Counter & 0xFF));

    /* Set WWDTWTR[7:0] bits according to WindowValue value */
    WWDT->WTR.V32 = ((0x5AA5 << WWDT_WTR_LOCK_Pos) | WindowValue);
}

/**
  * @brief  Checks whether the Early Wakeup interrupt flag is set or not.
  * @retval The new state of the Early Wakeup interrupt flag (SH_SET or SH_RESET)
  */
FlagStatus WWDT_GetINTFlagStatus(void)
{
    /* Return the flag status */
    return (WWDT->SR.BIT.WWDTIF ? SH_SET : SH_RESET);
}

/**
  * @brief  Clears Early Wakeup interrupt flag.
  * @retval None
  */
void WWDT_ClearFlag(void)
{
    WWDT->SR.BIT.WWDTIF = SH_RESET;
}


/**
  * @brief  Clear WWDT Counter
  * @retval None
  */
void WWDT_ReloadCounter(void)
{
    WWDT->CLR = 0x5555;
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
