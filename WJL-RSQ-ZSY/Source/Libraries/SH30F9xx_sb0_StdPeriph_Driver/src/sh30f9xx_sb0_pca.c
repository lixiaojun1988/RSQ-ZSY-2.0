/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_pca.c
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-5-24  
  * @brief   This file provides firmware functions to manage the PCA module 
  *  
    @verbatim
  ==============================================================================
                         How to use this driver
  ==============================================================================
   [..]
     (#) Enable PCA Clock use RCC_APB0PeriphClockOnOff() function 

     (#) Fill PCA Init Structure member as default use PCA_StructInit() function
     
     (#) Config PCA Pin
        (++) Enable Pin Clock calling RCC_AHBPeriphClockOnOff() function 
         
        (++) Call GPIO_Init()function for config PCA pin(input or output ,pull up or no-pull etc ...),if not use all member for PCA 
              pin config ,makesure call GPIO_StructInit() function for GPIO_Init function  paramter set default value

        (++) Call GPIO_PinAFConfig() for config pin as PCA function

     (#) Config PCA Clock ,mode etc use PCA_Init() function

     (#) Enable PCA Module by calling PCA_OnOff() function
     
     (#) Use PCA interrupt by following step

        (++) Config PCA Interrupt use PCA_INTConfig() function
        
        (++) Config PCA capture Interrupt use PCA_CapINTConfig() function
        
        (++) Enable Kernel PCA's Global interrupt by calling NVIC_EnableIRQ() function
        
        (++) reference startup asm file for PCA interrupt function name and rewrite this function 	
      
     (#) When PCA interrupt happen, use PCA_GetFlagStatus() function to get PCA interrupt flag

     (#) Call PCA_ClearFlag() function for clear PCA interrupt flag
     
     (#) Call PCA_GetCaptureValue() function to get capture value, and call PCA_GetCapterLevelStatus()
         to get capture trigger level
         
     (#) Using 32bit PCA Module by calling PCA_CascadeOnOff() function,
         the configuration method is the same as above
  
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
#include "sh30f9xx_sb0_pca.h"

/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */

/** @defgroup PCA_MODULE PCA
  * @brief SH30F9XX SB0 PCA module driver library,reference this topic macro and function for drive PCA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Export function prototypes -----------------------------------------------*/

/** @defgroup PCA_Exported_Functions PCA Exported Functions
 *  @brief    This is PCA exported function code group
 *
  * @{
  */
  
/**
  * @brief  Deinitializes the PCAx peripheral registers to their default
  *         reset values 
  * @param  PCAx: where x can be 0 to 3 to select the PCA peripheral.
  * @retval None
  */
void PCA_Reset(PCA_TypeDef *PCAx)
{
    /* check the parameters */
    // assert_param(IS_PCA_MODULE(PCAx));
    assert_param(IS_PCA_ALL_PERIPH(PCAx));
    
    if (PCAx == PCA0)
    {
        /* Reset PCA0 module */
        RCC_APB0PeriphReset(RCC_APB0_PCA0);
    }
    else if (PCAx == PCA1)
    {
        /* Reset PCA1 module */
        RCC_APB0PeriphReset(RCC_APB0_PCA1);
    }
    else if (PCAx == PCA2)
    {
        /* Reset PCA2 module */
        RCC_APB0PeriphReset(RCC_APB0_PCA2);
    }
    else if (PCAx == PCA3)
    {
        /* Reset PCA3 module */
        RCC_APB0PeriphReset(RCC_APB0_PCA3);
    }
    else{}
}

/**
  * @brief  Fills each PCA_InitStruct member with its default value.
  * @param  PCA_InitStruct : pointer to a @ref PCA_InitTypeDef structure which will
  *         be initialized.
  * @retval None
  */
void PCA_StructInit(PCA_InitTypeDef *PCA_InitStruct)
{
    PCA_InitStruct->PCA_CFGR.ClkSel = PCLK0;
    PCA_InitStruct->PCA_CFGR.PCACounterMode = SINGLEEDGE;
    PCA_InitStruct->PCA_CFGR.CCR0UPDATAEN = SH_DISABLE;
    PCA_InitStruct->PCA_CFGR.CCR1UPDATAEN = SH_DISABLE;
    PCA_InitStruct->PCA_CFGR.PUEUPDATAEN = SH_DISABLE;
    PCA_InitStruct->PCA_CFGR.ECFSel = FILTERING_NONE;
    PCA_InitStruct->PCA_CFGR.rev1 = 0;
    PCA_InitStruct->PCA_CFGR.rev2 = 0;
    PCA_InitStruct->PCA_CFGR.rev3 = 0;
    PCA_InitStruct->PCA_CFGR.rev4 = 0;

    PCA_InitStruct->PCA_CCMR0.PCA_CC = SH_DISABLE;
    PCA_InitStruct->PCA_CCMR0.ModeSelect = CAPTUREMODE;
    PCA_InitStruct->PCA_CCMR0.FunctionSelect = RISING_EDGE_TRIGGER;
    PCA_InitStruct->PCA_CCMR0.PCA_CEN = SH_DISABLE;
    PCA_InitStruct->PCA_CCMR0.PCA_TCP = SH_DISABLE;
    PCA_InitStruct->PCA_CCMR0.rev5 = 0;
    PCA_InitStruct->PCA_CCMR0.rev6 = 0;

    PCA_InitStruct->PCA_CCMR1.PCA_CC = SH_DISABLE;
    PCA_InitStruct->PCA_CCMR1.ModeSelect = CAPTUREMODE;
    PCA_InitStruct->PCA_CCMR1.FunctionSelect = RISING_EDGE_TRIGGER;
    PCA_InitStruct->PCA_CCMR1.PCA_CEN = SH_DISABLE;
    PCA_InitStruct->PCA_CCMR1.PCA_TCP = SH_DISABLE;
    PCA_InitStruct->PCA_CCMR1.rev5 = 0;
    PCA_InitStruct->PCA_CCMR1.rev6 = 0;

    PCA_InitStruct->PCA_FORCE = 0;
    PCA_InitStruct->PCA_CNT = 0;
    PCA_InitStruct->PCA_PSC = 0;

    PCA_InitStruct->PCA_PR = 0;

    PCA_InitStruct->PCA_CCR0 = 0;
    PCA_InitStruct->PCA_CCR1 = 0;
}

/**
  * @brief  Initializes the PCAx peripheral according to the specified parameters 
  *         in the PCA_InitStruct.
  * @param  PCAx: where x can be  0 to 3 to select the PCA peripheral.
  * @param  PCA_InitStruct: pointer to a @ref PCA_InitTypeDef structure that contains 
  *         the configuration information for the specified PCA peripheral.
  * @retval None
  *
  * @note
  *     If PCAx in simulation mode, the CPU state is stopped (such as setting
  * a breakpoint or single step debugging), the output pin level will automatically
  * switch to a high resistance state to avoid damaging external devices. The same
  * in stop mode, but what's different is that when the MCU wakeup from STOP mode,
  * the register PEN bit must write '1' again in order to output the waveform normally.
  */
void PCA_Init(PCA_TypeDef *PCAx, const PCA_InitTypeDef *PCA_InitStruct)
{
    static uint8_t *pv = 0;
    /* check the parameters */
    assert_param(IS_PCA_ALL_PERIPH(PCAx));

    assert_param(IS_PCACLKSOURCE_DIV(PCA_InitStruct->PCA_CFGR.ClkSel));
    assert_param(IS_PCACOUNTERMODE_DIV(PCA_InitStruct->PCA_CFGR.PCACounterMode));

    pv = (uint8_t *)PCA_InitStruct;
    PCA_UNLOCK();
    PCAx->CFGR.V32  = *((uint32_t *)(pv + 0));
    PCAx->FORCE.V32 = *((uint32_t *)(pv + 4));
    PCAx->CNT.V32   = *((uint32_t *)(pv + 8));
    PCAx->PSC       = *((uint32_t *)(pv + 12));
    PCAx->PR.V32    = *((uint32_t *)(pv + 16));
    PCAx->CCMR0.V32 = *((uint32_t *)(pv + 20));
    PCAx->CCMR1.V32 = *((uint32_t *)(pv + 24));
    PCAx->CCR0.V32  = *((uint32_t *)(pv + 28));
    PCAx->CCR1.V32  = *((uint32_t *)(pv + 32));
    PCA_LOCK();
}

/**
  * @brief  Open or close the PCA peripheral.
  * @param  PCAx: where x can be 0 to 3 to select the PCA peripheral.
  * @param  OnOffState: state of the PCA peripheral.
  *         This parameter can be one of the following values:
  *            @arg @b SH_ON
  *            @arg @b SH_OFF
  * @retval None
  */
void PCA_OnOff(PCA_TypeDef *PCAx, CmdState OnOffState)
{
    /* check the parameters */
    assert_param(IS_PCA_ALL_PERIPH(PCAx));
    assert_param(IS_CMD_STATE(OnOffState));

    PCA_UNLOCK();
    if (OnOffState != SH_OFF)
    {
        PCAx->CR = PCA_CR_PEN_Msk;
    }
    else
    {
        PCAx->CR = (~PCA_CR_PEN_Msk);
    }
    PCA_LOCK();
}

/**
  * @brief  Open or close the PCA cascade.
  * @param  PCAx: where x can be 0 2 to select the PCA peripheral.
  * @param  OnOffState: state of the Cascade PCA peripheral.
  *         This parameter can be one of the following values:
  *            @arg @b SH_ON
  *            @arg @b SH_OFF
  * @retval None
  */
void PCA_CascadeOnOff(PCA_TypeDef *PCAx, CmdState OnOffState)
{
    /* check the parameters */
    assert_param(IS_PCA_CASCADE_PERIPH(PCAx));
    assert_param(IS_CMD_STATE(OnOffState));

    PCA_UNLOCK();
    if (OnOffState != SH_OFF)
    {
        PCAx->CFGR.V32 |= PCA_CFGR_CASCEN_Msk;
    }
    else
    {
        PCAx->CFGR.V32 &= (~PCA_CFGR_CASCEN_Msk);
    }
    PCA_LOCK();
}

/**
  * @brief  Set the PCAx Frequency output mode frequency value
  * @param  PCAx: where x can be 0 to 3 to select the PCA peripheral.
  * @param  PCAChannel: Specifie the PCA Channel to Set.
  *         This parameter can be any combination of the following values:
  *            @arg @b PCA_CHANNEL_A
  *            @arg @b PCA_CHANNEL_B
  * @param  Value: Specifie the PCA CCRxH to set(0-255).
  * @retval None
  */
void PCA_FreqOutValue(PCA_TypeDef *PCAx, PCA_CHANNEL_TypeDef PCAChannel, uint8_t Value)
{
    /* check the parameters */
    assert_param(IS_PCA_ALL_PERIPH(PCAx));
    assert_param(IS_PCACHANEEL_CHANNEL(PCAChannel));

    PCA_UNLOCK();
    if (PCA_CHANNEL_A == PCAChannel)
    {
        PCAx->CCR0.BIT.CCR0H = (uint8_t)Value;
    }
    else
    {
        PCAx->CCR1.BIT.CCR1H = (uint8_t)Value;
    }
    PCA_LOCK();
}

/**
  * @brief  Set the PCAx Forced match control bit
  * @param  PCAx: where x can be 0 to 3 to select the PCA peripheral.
  * @param  PCAChannel: Specifie the PCA Channel to Set.
  *         This parameter can be any combination of the following values:
  *            @arg @b PCA_CHANNEL_A
  *            @arg @b PCA_CHANNEL_B
  * @param  OnOffState: Switch status.
  *         This parameter can be any combination of the following values:
  *            @arg @b SH_ON
  *            @arg @b SH_OFF
  * @retval None
  */
void PCA_ForceMatchControl(PCA_TypeDef *PCAx, PCA_CHANNEL_TypeDef PCAChannel, CmdState OnOffState)
{
    /* check the parameters */
    assert_param(IS_PCA_ALL_PERIPH(PCAx));
    assert_param(IS_PCACHANEEL_CHANNEL(PCAChannel));
    assert_param(IS_CMD_STATE(OnOffState));

    PCA_UNLOCK();
    if (PCA_CHANNEL_A == PCAChannel)
    {
        if (OnOffState != SH_OFF)
        {
            PCAx->FORCE.V32 |= PCA_FORCE_FCO0_Msk;
        }
        else
        {
            PCAx->FORCE.V32 &= (~PCA_FORCE_FCO0_Msk);
        }
    }
    else
    {
        if (OnOffState != SH_OFF)
        {
            PCAx->FORCE.V32 |= PCA_FORCE_FCO1_Msk;
        }
        else
        {
            PCAx->FORCE.V32 &= (~PCA_FORCE_FCO1_Msk);
        }
    }
    PCA_LOCK();
}

/**
  * @brief  Set the PCAx PR Register value
  * @param  PCAx: where x can be 0 to 3 to select the PCA peripheral.
  * @param  Value: Specifie the PCA PR to Set.
  * @retval None
  */
void PCA_SetPRValue(PCA_TypeDef *PCAx, uint32_t Value)
{
    /* check the parameters */
    assert_param(IS_PCA_ALL_PERIPH(PCAx));

    PCA_UNLOCK();
    PCAx->PR.V32 = Value;
    PCA_LOCK();
}

/**
  * @brief  Set the PCAx CCR Register value
  * @param  PCAx: where x can be 0 to 3 to select the PCA peripheral.
  * @param  PCAChannel: Specifie the PCA Channel to set.
  *         This parameter can be any combination of the following values:
  *            @arg @b PCA_CHANNEL_A
  *            @arg @b PCA_CHANNEL_B
  * @param  Value: Specifie the PCA CCRx to set.
  * @retval None
  */
void PCA_SetCCRValue(PCA_TypeDef *PCAx, PCA_CHANNEL_TypeDef PCAChannel, uint32_t Value)
{
    /* check the parameters */
    assert_param(IS_PCA_ALL_PERIPH(PCAx));
    assert_param(IS_PCACHANEEL_CHANNEL(PCAChannel));

    PCA_UNLOCK();
    if (PCA_CHANNEL_A == PCAChannel)
    {
        PCAx->CCR0.V32 = Value;
    }
    else
    {
        PCAx->CCR1.V32 = Value;
    }
    PCA_LOCK();
}

/**
  * @brief  Get the PCAx CCR Register value
  * @param  PCAx: where x can be 0 to 3 to select the PCA peripheral.
  * @param  PCAChannel: Specifie the PCA Channel to set.
  *         This parameter can be any combination of the following values:
  *            @arg @b PCA_CHANNEL_A
  *            @arg @b PCA_CHANNEL_B
  * @retval CPR_Count
  */
uint32_t PCA_GetCaptureValue(PCA_TypeDef *PCAx, PCA_CHANNEL_TypeDef PCAChannel)
{
    static uint32_t CCR_Count = 0;
    /* check the parameters */
    assert_param(IS_PCA_ALL_PERIPH(PCAx));
    assert_param(IS_PCACHANEEL_CHANNEL(PCAChannel));

    if (PCA_CHANNEL_A == PCAChannel)
    {
        CCR_Count = PCAx->CCR0.V32;
    }
    else
    {
        CCR_Count = PCAx->CCR1.V32;
    }

    return CCR_Count;
}

/**
  * @brief  Check whether the specified PCA flag is set or not.
  * @param  PCAx: where x can be 0 to 3 to select the PCA peripheral.
  * @param  PCA_IntFlag: Specifie the flag to check.
  *         This parameter can be any combination of the following values:
  *            @arg @b PCA_FLAG_CC0IF  
  *            @arg @b PCA_FLAG_CC1IF
  *            @arg @b PCA_FLAG_PIF
  *            @arg @b PCA_FLAG_CIF
  * @retval The new state of PCAx's flag.
  */
FlagStatus PCA_GetFlagStatus(PCA_TypeDef *PCAx, PCA_FLAG_TypeDef PCA_IntFlag)
{
    FlagStatus bitStatus;
    /* check the parameters */
    assert_param(IS_PCA_ALL_PERIPH(PCAx));
    assert_param(IS_PCA_GET_ONE_FLAG(PCA_IntFlag));
    if ((PCAx->SR.V32 & PCA_IntFlag) != SH_RESET)
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
  * @brief  Get the status of the trigger level in the capture mode.
  * @param  PCAx: where x can be 0 to 3 to select the PCA peripheral.
  * @param  PCAChannel: Specifie the PCA Channel to Set.
  *         This parameter can be any combination of the following values:
  *            @arg @b PCA_CHANNEL_A
  *            @arg @b PCA_CHANNEL_B
  * @retval CCMR_TCP value
  */
FlagStatus PCA_GetCapterLevelStatus(PCA_TypeDef *PCAx, PCA_CHANNEL_TypeDef PCAChannel)
{
    FlagStatus bitStatus;
    /* check the parameters */
    assert_param(IS_PCA_ALL_PERIPH(PCAx));
    assert_param(IS_PCACHANEEL_CHANNEL(PCAChannel));

    if (PCA_CHANNEL_A == PCAChannel)
    {
        bitStatus = (FlagStatus)PCAx->CCMR0.BIT.TCP;
    }
    else
    {
        bitStatus = (FlagStatus)PCAx->CCMR1.BIT.TCP;
    }

    return bitStatus;
}

/**
  * @brief  Clear the PCA's pending flags.
  * @param  PCAx: where x can be 0 to 3 to select the PCA peripheral.
  * @param  PCA_IntFlag: Specifie the flag to check.
  *         This parameter can be any combination of the following values:
  *            @arg @b PCA_FLAG_CC0IF  
  *            @arg @b PCA_FLAG_CC1IF
  *            @arg @b PCA_FLAG_PIF
  *            @arg @b PCA_FLAG_CIF
  * @retval None
  */
void PCA_ClearFlag(PCA_TypeDef *PCAx, PCA_FLAG_TypeDef PCA_IntFlag)
{
    /* Check the parameters */
    assert_param(IS_PCA_ALL_PERIPH(PCAx));
    assert_param(IS_PCA_GET_ONE_FLAG(PCA_IntFlag));

    PCA_UNLOCK();
    PCAx->SR.V32 |= ((uint32_t)PCA_IntFlag << 16);
    PCA_LOCK();
}
/**
  * @brief  Clear the PCAx's All pending flag.
  * @param  PCAx: where x can be 0 to 3 to select the PCA peripheral.
  * @retval None
  */
void PCA_ClearAllFlag(PCA_TypeDef *PCAx)
{
    /* check the parameters */
    assert_param(IS_PCA_ALL_PERIPH(PCAx));

    PCA_UNLOCK();
    PCAx->SR.V32 = PCA_SR_CC0IFC_Msk | PCA_SR_CC1IFC_Msk \
                   | PCA_SR_PIFC_Msk | PCA_SR_CIFC_Msk;
    PCA_LOCK();
}

/**
  * @brief  Enable or disable the specified PCA's interrupts.
  * @param  PCAx: where x can be 0 to 3 to select the PCA peripheral.
  * @param  IntSource: PCA Interrupt source.
  *         This parameter can be one of the following values:
  *            @arg @b PCA_COUNTER_OVERFLOW_INT
  *            @arg @b PCA_COUNTER_CYCLE_INT
  * @param  NewState: New state of PCA interrupt.
  *         This parameter can be one of the following values:
  *            @arg @b SH_ENABLE
  *            @arg @b SH_DISABLE
  * @retval None
  * @note  PIF flag only when SDEN=1, the counter value will be 
  *        set by the hardware when it reaches the cycle value. 
  *        When SDEN=0, this bit is invalid and remains 0.
  */
void PCA_INTConfig(PCA_TypeDef *PCAx, PCA_INT_TypeDef IntSource, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_PCA_ALL_PERIPH(PCAx));
    assert_param(IS_PCA_INT_SOURCE(IntSource));
    assert_param(IS_FUNCTION_STATE(NewState));

    PCA_UNLOCK();
    if (NewState != SH_DISABLE)
    {
        PCAx->CFGR.V32 |= IntSource;
    }
    else
    {
        PCAx->CFGR.V32 &= (~IntSource);
    }
    PCA_LOCK();
}

/**
  * @brief  Enable or disable the specified PCAx Capture's interrupts.
  * @param  PCAx: where x can be 0 to 3 to select the PCA peripheral.
  * @param  PCAChannel: channel of PCA Capture's interrupt.
  *         This parameter can be one of the following values:
  *            @arg @b PCA_CHANNEL_A
  *            @arg @b PCA_CHANNEL_B
  * @param  NewState: New state of PCA interrupt.
  *         This parameter can be one of the following values:
  *            @arg @b SH_ENABLE
  *            @arg @b SH_DISABLE
  * @retval None
  */
void PCA_CapINTConfig(PCA_TypeDef *PCAx, PCA_CHANNEL_TypeDef PCAChannel, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_PCA_ALL_PERIPH(PCAx));
    assert_param(IS_PCACHANEEL_CHANNEL(PCAChannel));
    assert_param(IS_FUNCTION_STATE(NewState));

    PCA_UNLOCK();
    if (PCA_CHANNEL_A == PCAChannel)
    {
        if (NewState != SH_DISABLE)
        {
            PCAx->CCMR0.V32 |= PCA_CCMR0_CCIE_Msk;
        }
        else
        {
            PCAx->CCMR0.V32 &= (~PCA_CCMR0_CCIE_Msk);
        }
    }
    else
    {
        if (NewState != SH_DISABLE)
        {
            PCAx->CCMR1.V32 |= PCA_CCMR1_CCIE_Msk;
        }
        else
        {
            PCAx->CCMR1.V32 &= (~PCA_CCMR1_CCIE_Msk);
        }
    }
    PCA_LOCK();
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
