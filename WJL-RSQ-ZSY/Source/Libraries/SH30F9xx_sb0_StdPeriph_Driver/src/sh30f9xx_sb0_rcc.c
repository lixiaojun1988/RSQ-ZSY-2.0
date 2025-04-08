/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_rcc.c
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-05-21  
  * @brief   This file provides firmware functions to manage the RCC module
  *         
  @verbatim
  ==============================================================================
                         How to use this driver
  ==============================================================================
 [..]
   (#) Enable the AHB module clock using the following function : RCC_AHBPeriphClockOnOff().
 
   (#) Enable the APB0 module clock using the following function : RCC_APB0PeriphClockOnOff().
   
   (#) Enable the APB1 module clock using the following function : RCC_APB1PeriphClockOnOff().
   
   (#) Reset the AHB module clock using the following function : RCC_AHBPeriphReset().
   
   (#) Reset the APB0 module clock using the following function : RCC_APB0PeriphReset().
   
   (#) Reset the APB1 module clock using the following function : RCC_APB1PeriphReset().
   
   (#) Get current sytem clock config use RCC_GetClocksFreq() function
   
   (#) Config CSM function by:
      (++) call RCC_CSMModuleOnOff() function to turn on the csm function



  @endverbatim
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
#include "sh30f9xx_sb0_rcc.h"
#include "system_sh30f9xx_sb0.h"

/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */

/** @defgroup RCC_MODULE RCC
  * @brief SH30F9XX SB0 GPIO module driver library,reference this topic macro and function for drive GPIO
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/


  
  
/* Export function prototypes -----------------------------------------------*/

/** @defgroup RCC_Exported_Functions  RCC Exported Functions
  *
  *  @brief    This is RCC exported function code group
  * @{
  */

/**
  * @brief     Returns the frequencies of different on chip clocks.
  * @param     Clocks: pointer to a RCC_Clocks_TypeDef structure which will hold
  *            the clocks frequencies.
  * @retval    None
  */
void RCC_GetClocksFreq(RCC_Clocks_TypeDef *Clocks)
{
    uint8_t tmp = 0;
    uint8_t desOpt = 0;
    uint8_t hpre_div = 0;
    uint32_t sysClk = 0;

    /* Get SYSCLK source -------------------------------------------------------*/
    tmp = RCC->CR.BIT.SWS;
    hpre_div = RCC->CFGR.BIT.HPRE;
    desOpt = (*(volatile uint8_t *)0x0FFFC048);
    switch (tmp)
    {
    case RCC_SYS_SRC_HSI: /* HSI used as system clock */
        if ((desOpt&0x40) == 0x40)
        {
            sysClk = SystemCoreClock;
        }
        else
        {
            sysClk = SystemCoreClock>>1;
        }
        break;
    case RCC_SYS_SRC_HSE: /* HSE used as system clock */
        if (RCC->CISTR.BIT.HSECSMF)
        {
            if ((desOpt&0x40) == 0x40)
            {
                sysClk = SystemCoreClock>>3;
            }
            else
            {
                sysClk = SystemCoreClock>>4;
            }
        }
        else
        {
            sysClk = SystemCoreClock;
        }
        break;
    case RCC_SYS_SRC_LSI: /* LSI used as system clock */
        sysClk = SystemCoreClock;
        break;
    default: /* HSI used as system clock */
        sysClk = SystemCoreClock;
        break;
    }

    Clocks->sysFreq = sysClk;

    
    if (tmp != RCC_SYS_SRC_LSI)
    {   //Calculate HCLK
        switch (hpre_div)
        {
        case 0:
            Clocks->hclkFreq = sysClk;
            break;
        case 1:
            Clocks->hclkFreq = sysClk / 2;
            break;
        case 2:
            Clocks->hclkFreq = sysClk / 4;
            break;
        case 3:
            Clocks->hclkFreq = sysClk / 8;
            break;
        case 4:
            Clocks->hclkFreq = sysClk / 16;
            break;
        case 5:
            Clocks->hclkFreq = sysClk / 32;
            break;
        default:
            Clocks->hclkFreq = sysClk / 32;
            break;
        }
    
        /*Calculate PCLK0 PCLK1*/
        Clocks->pclk0Freq = Clocks->hclkFreq >> (RCC->CFGR.BIT.PPRE0 > 4 ? 4 : (RCC->CFGR.BIT.PPRE0));
        Clocks->pclk1Freq = Clocks->hclkFreq >> (RCC->CFGR.BIT.PPRE1 > 4 ? 4 : (RCC->CFGR.BIT.PPRE1));
    }
    else
    {
        Clocks->hclkFreq  = sysClk;
        Clocks->pclk0Freq = sysClk;
        Clocks->pclk1Freq = sysClk;
    }
}

/**
  * @brief     open or close AHB modules' clock gate
  * @param  AHBModules:  AHB module bits @ref RCC_AHB_Type
  *     This parameter can be any combination of following values:
  *     @arg @b  RCC_AHB_SYSCFG :  for SYSCFG Module
  *     @arg @b  RCC_AHB_CRC    :  for CRC Module
  *     @arg @b  RCC_AHB_GPIO   :  for GPIO Config Module
  * @param OnOffState  open or close related clock gate
  *     @arg @b  SH_ON  open related clock gate
  *     @arg @b  SH_OFF  close related clock gate
  * @retval   None
   */
void RCC_AHBPeriphClockOnOff(RCC_AHB_TypeDef AHBModules, CmdState OnOffState)
{
    assert_param(IS_AHB_MODULES(AHBModules));
    assert_param(IS_CMD_STATE(OnOffState));

    /* Unlock RCC registers */
    RCC_REGS_UNLOCK();

    if (OnOffState == SH_ON)
    {
        RCC->AHBENR.V32 |= AHBModules;
    }
    else
    {
        RCC->AHBENR.V32 &= ~AHBModules;
    }

    /* Lock RCC registers */
    RCC_REGS_LOCK();
}

/**
  * @brief  open or close APB1 modules' clock gate
  * @param  APB1Modules:  APB1 module bits @ref RCC_APB1_Type
  *   This parameter can be any combination of following values:
  *     @arg @b RCC_APB1_UART0 :    for UART0 Module
  *     @arg @b RCC_APB1_UART1 :    for UART1 Module 
  *     @arg @b RCC_APB1_UART2 :    for UART2 Module 
  *     @arg @b RCC_APB1_UART3 :    for UART3 Module
  *     @arg @b RCC_APB1_UART4 :    for UART4  Module
  *     @arg @b RCC_APB1_UART5 :    for UART5  Module
  *     @arg @b RCC_APB1_SPI0  :    for SPI0  Module
  *     @arg @b RCC_APB1_SPI1  :    for SPI1 Module
  *     @arg @b RCC_APB1_TWI0  :    for TWI0 Module 
  *     @arg @b RCC_APB1_TWI1  :    for TWI1 Module 
  *     @arg @b RCC_APB1_FIFO0 :    for FIFO0 Module
  *     @arg @b RCC_APB1_FIFO1 :    for FIFO1  Module
  *     @arg @b RCC_APB1_FIFO2 :    for FIFO2  Module
  * @param OnOffState  open or close related clock gate
  *     @arg @b  SH_ON  open related clock gate
  *     @arg @b  SH_OFF  close related clock gate
  * @retval  None
  */
void RCC_APB1PeriphClockOnOff(RCC_APB1_TypeDef APB1Modules, CmdState OnOffState)
{
    assert_param(IS_APB1_MODULES(APB1Modules));
    assert_param(IS_CMD_STATE(OnOffState));

    /* Unlock RCC registers */
    RCC_REGS_UNLOCK();

    if (OnOffState == SH_ON)
    {
        RCC->APB1ENR.V32 |= APB1Modules;
    }
    else
    {
        RCC->APB1ENR.V32 &= ~APB1Modules;
    }

    /* Lock RCC registers */
    RCC_REGS_LOCK();
}

/**
  * @brief      open or close APB0 modules' clock gate
  * @param  APB0Modules:  APB0 module bits @ref RCC_APB0_Type
  *   This parameter can be any combination of following values:
  *     @arg @b    RCC_APB0_TIM0 : for TIM0 Module
  *     @arg @b    RCC_APB0_TIM1 : for TIM1 Module
  *     @arg @b    RCC_APB0_TIM2 : for TIM2 Module
  *     @arg @b    RCC_APB0_TIM3 : for TIM3 Module
  *     @arg @b    RCC_APB0_PWM0 : for PWM0 Module
  *     @arg @b    RCC_APB0_PWM1 : for PWM1 Module
  *     @arg @b    RCC_APB0_PWM2 : for PWM2 Module
  *     @arg @b    RCC_APB0_PWM3 : for PWM3 Module
  *     @arg @b    RCC_APB0_PCA0 : for PCA0 Module
  *     @arg @b    RCC_APB0_PCA1 : for PCA1 Module
  *     @arg @b    RCC_APB0_PCA2 : for PCA2 Module
  *     @arg @b    RCC_APB0_PCA3 : for PCA3 Module
  *     @arg @b    RCC_APB0_EXTI : for EXTI Module
  *     @arg @b    RCC_APB0_WWDT : for WWDT Module
  *     @arg @b    RCC_APB0_ADC  : for ADC  Module
  * @param OnOffState  open or close related clock gate
  *     @arg @b  SH_ON  open related clock gate
  *     @arg @b  SH_OFF  close related clock gate
  * @retval None
  */
void RCC_APB0PeriphClockOnOff(RCC_APB0_TypeDef APB0Modules, CmdState OnOffState)
{
    assert_param(IS_APB0_MODULES(APB0Modules));
    assert_param(IS_CMD_STATE(OnOffState));

    /* Unlock RCC registers */
    RCC_REGS_UNLOCK();

    if (OnOffState == SH_ON)
    {
        RCC->APB0ENR.V32 |= APB0Modules;
    }
    else
    {
        RCC->APB0ENR.V32 &= ~APB0Modules;
    }

    /* Lock RCC registers */
    RCC_REGS_LOCK();
}

/**
  * @brief     reset AHB modules
  * @param  AHBModules: AHB module bits @ref RCC_AHB_Type
  *   This parameter can be any combination of following values:
  *     @arg @b  RCC_AHB_SYSCFG :  for SYSCFG Module
  *     @arg @b  RCC_AHB_CRC    :  for CRC Module
  *     @arg @b  RCC_AHB_GPIO   :  for GPIO Config Module
  * @retval   None
  */
void RCC_AHBPeriphReset(RCC_AHB_TypeDef AHBModules)
{
    assert_param(IS_AHB_MODULES(AHBModules));

    /* Unlock RCC registers */
    RCC_REGS_UNLOCK();

    RCC->AHBRSTR.V32 = AHBModules;

    /* Lock RCC registers */
    RCC_REGS_LOCK();
}

/**
  * @brief    reset APB0 modules
  * @param  APB0Modules:  APB0 module bits @ref RCC_APB0_Type
  *   This parameter can be any combination of following values:
  *     @arg @b    RCC_APB0_TIM0 : for TIM0 Module
  *     @arg @b    RCC_APB0_TIM1 : for TIM1 Module
  *     @arg @b    RCC_APB0_TIM2 : for TIM2 Module
  *     @arg @b    RCC_APB0_TIM3 : for TIM3 Module
  *     @arg @b    RCC_APB0_PWM0 : for PWM0 Module
  *     @arg @b    RCC_APB0_PWM1 : for PWM1 Module
  *     @arg @b    RCC_APB0_PWM2 : for PWM2 Module
  *     @arg @b    RCC_APB0_PWM3 : for PWM3 Module
  *     @arg @b    RCC_APB0_PCA0 : for PCA0 Module
  *     @arg @b    RCC_APB0_PCA1 : for PCA1 Module
  *     @arg @b    RCC_APB0_PCA2 : for PCA2 Module
  *     @arg @b    RCC_APB0_PCA3 : for PCA3 Module
  *     @arg @b    RCC_APB0_EXTI : for EXTI Module
  *     @arg @b    RCC_APB0_WWDT : for WWDT Module
  *     @arg @b    RCC_APB0_ADC  : for ADC  Module
  * @retval  None
  */
void RCC_APB0PeriphReset(RCC_APB0_TypeDef APB0Modules)
{
    assert_param(IS_APB0_MODULES(APB0Modules));

    /* Unlock RCC registers */
    RCC_REGS_UNLOCK();

    RCC->APB0RSTR.V32 = APB0Modules;

    /* Lock RCC registers */
    RCC_REGS_LOCK();
}

/**
  * @brief  reset APB1 modules
  * @param  APB1Modules:  APB1 module bits @ref RCC_APB1_Type
  *   This parameter can be any combination of following values:
  *     @arg @b RCC_APB1_UART0 :    for UART0 Module
  *     @arg @b RCC_APB1_UART1 :    for UART1 Module 
  *     @arg @b RCC_APB1_UART2 :    for UART2 Module 
  *     @arg @b RCC_APB1_UART3 :    for UART3 Module
  *     @arg @b RCC_APB1_UART4 :    for UART4  Module
  *     @arg @b RCC_APB1_UART5 :    for UART5  Module
  *     @arg @b RCC_APB1_SPI0  :    for SPI0  Module
  *     @arg @b RCC_APB1_SPI1  :    for SPI1 Module
  *     @arg @b RCC_APB1_TWI0  :    for TWI0 Module 
  *     @arg @b RCC_APB1_TWI1  :    for TWI1 Module 
  *     @arg @b RCC_APB1_FIFO0 :    for FIFO0 Module
  *     @arg @b RCC_APB1_FIFO1 :    for FIFO1  Module
  *     @arg @b RCC_APB1_FIFO2 :    for FIFO2  Module
  * @retval None
  */
void RCC_APB1PeriphReset(RCC_APB1_TypeDef APB1Modules)
{
    assert_param(IS_APB1_MODULES(APB1Modules));

    /* Unlock RCC registers */
    RCC_REGS_UNLOCK();

    RCC->APB1RSTR.V32 = APB1Modules;

    /* Lock RCC registers */
    RCC_REGS_LOCK();
}

/**
  * @brief     Get RCC reset flag
  * @param  ResetFlag:   RCC reset flag mask
  *     @arg @b  RCC_RST_PIN
  *     @arg @b  RCC_RST_LVR
  *     @arg @b  RCC_RST_POWERON
  *     @arg @b  RCC_RST_SOFTWARE
  *     @arg @b  RCC_RST_WWDT
  *     @arg @b  RCC_RST_HWDT
  * @retval  FlagStatus RCC reset flag
  *     @arg @b  SH_SET    the flag is setted
  *     @arg @b  SH_RESET  the flag is cleared
  */
FlagStatus RCC_GetResetFlag(RCC_RESET_TypeDef ResetFlag)
{
    assert_param(IS_RST_MODULES(ResetFlag));

    return ((RCC->RSTSTR.V32 & ResetFlag) ? SH_SET : SH_RESET);
}

/**
  * @brief      clear RCC reset flags
  * @param  ResetFlags:   RCC reset flag clear bits
  *     @arg @b  RCC_RST_PIN
  *     @arg @b  RCC_RST_LVR
  *     @arg @b  RCC_RST_POWERON
  *     @arg @b  RCC_RST_SOFTWARE
  *     @arg @b  RCC_RST_WWDT
  *     @arg @b  RCC_RST_HWDT
   * @retval None
  */
void RCC_ClearResetFlag(RCC_RESET_TypeDef ResetFlags)
{
    assert_param(IS_RST_MODULES(ResetFlags));

    /* Unlock RCC registers */
    RCC_REGS_UNLOCK();

    RCC->RSTCLR.V32 = ResetFlags;

    /* Lock RCC registers */
    RCC_REGS_LOCK();
}

/**
  * @brief open or close CSM modules
  * @param OnOffState   open or close related clock gate
  *     @arg @b  SH_ON  open related clock gate
  *     @arg @b  SH_OFF close related clock gate
  * @retval   None
  */
void RCC_CSMModuleOnOff(CmdState OnOffState)
{
    assert_param(IS_CMD_STATE(OnOffState));

    /* Unlock RCC registers */
    RCC_REGS_UNLOCK();

    RCC->CR.BIT.CSMON = OnOffState;

    /* Lock RCC registers */
    RCC_REGS_LOCK();
}

/**
  * @brief     Get RCC Interrupt flag
  * @param     RCCIntFlag:   RCC Interrupt flag mask
  *     @arg @b  RCC_INT_HSE_RF
  *     @arg @b  RCC_CSM_HSE_IF
  * @retval  FlagStatus RCC Interrupt flag
  *     @arg @b  SH_SET    the flag is setted
  *     @arg @b  SH_RESET  the flag is cleared
  */
FlagStatus RCC_GetFlagStatus(RCC_INT_TypeDef RCCIntFlag)
{
    assert_param(IS_RCC_INT_FLAG(CSMFlag));

    return ((RCC->CISTR.V32 & RCCIntFlag) ? SH_SET : SH_RESET);
}

/**
  * @brief  Clear RCC clock interrupt flag
  * @param  CLK_INTFLG_CLR: CICCLR Flag
  *     @arg @b  RCC_CICCLR_CSMC   
  *     @arg @b  RCC_CICCLR_HSERDYC
  * @retval None
  */
void RCC_ClearITPendingBit(RCC_CICCLR_TypeDef CLK_INTFLG_CLR)
{
    assert_param(IS_CICCLR_FLAG(CLK_INTFLG_CLR));
    /* Unlock RCC registers */
    RCC_REGS_UNLOCK();
    if (CLK_INTFLG_CLR == RCC_CICCLR_CSMC)
    {
        RCC->CICLR.BIT.CSMC = 1;
    }
    else if (CLK_INTFLG_CLR == RCC_CICCLR_HSERDYC)
    {
        RCC->CICLR.BIT.HSERDYC = 1;
    }
    /* Unlock RCC registers */
    RCC_REGS_LOCK();
}

/**
  * @brief RCC interrupt enable configure
  * @param RDYINTIE 
  *     @arg @b  RCC_CIENR_HSERDYIE 
  * @param NewState
  *     @arg @b  SH_DISABLE  
  *     @arg @b  SH_ENABLE 
  * @retval  None
  */
void RCC_INTConfig(RCC_CIENR_TypeDef RDYINTIE, FunctionalState NewState)
{
    assert_param(IS_CIENR_REGISTER(RDYINTIE));
    assert_param(IS_FUNCTION_STATE(NewState));

    RCC_REGS_UNLOCK();
    if (NewState != SH_DISABLE)
    {
        RCC->CIENR.V32 |= RDYINTIE;
    }
    else
    {
        RCC->CIENR.V32 &= (~RDYINTIE);
    }
    RCC_REGS_LOCK();
}

/**
  * @brief  Set AHB Clock Division
  * @param  AHBDiv:  AHB module bits @ref RCC_AHB_DIVISION_TypeDef
  *   This parameter can be any combination of following values:
  *     @arg @b RCC_AHB_DIV1
  *     @arg @b RCC_AHB_DIV2
  *     @arg @b RCC_AHB_DIV4
  *     @arg @b RCC_AHB_DIV8
  *     @arg @b RCC_AHB_DIV16
  *     @arg @b RCC_AHB_DIV32
  * @retval None
  */
void RCC_SetAHBClockDivision(RCC_AHB_DIVISION_TypeDef AHBDiv)
{
    assert_param(IS_AHB_CLOCK_DIVSION(AHBDiv));
    RCC_REGS_UNLOCK();
    RCC->CFGR.BIT.HPRE = AHBDiv;
    RCC_REGS_LOCK();
}

/**
  * @brief  Set APB0 Clock Division
  * @param  AHBDiv:  APB0 module bits @ref RCC_APB0_DIVISION_TypeDef
  *   This parameter can be any combination of following values:
  *     @arg @b RCC_APB0_DIV1
  *     @arg @b RCC_APB0_DIV2
  *     @arg @b RCC_APB0_DIV4
  *     @arg @b RCC_APB0_DIV8
  *     @arg @b RCC_APB0_DIV16
  * @retval None
  */
void RCC_SetAPB0ClockDivision(RCC_APB0_DIVISION_TypeDef APB0Div)
{
    assert_param(IS_APB0_CLOCK_DIVSION(APB0Div));
    RCC_REGS_UNLOCK();
    RCC->CFGR.BIT.PPRE0 = APB0Div;
    RCC_REGS_LOCK();
}

/**
  * @brief  Set APB1 Clock Division
  * @param  AHBDiv:  APB1 module bits @ref RCC_APB1_DIVISION_TypeDef
  *   This parameter can be any combination of following values:
  *     @arg @b RCC_APB1_DIV1
  *     @arg @b RCC_APB1_DIV2
  *     @arg @b RCC_APB1_DIV4
  *     @arg @b RCC_APB1_DIV8
  *     @arg @b RCC_APB1_DIV16
  * @retval None
  */
void RCC_SetAPB1ClockDivision(RCC_APB1_DIVISION_TypeDef APB1Div)
{
    assert_param(IS_APB1_CLOCK_DIVSION(APB1Div));
    RCC_REGS_UNLOCK();
    RCC->CFGR.BIT.PPRE1 = APB1Div;
    RCC_REGS_LOCK();
}

/**
  * @brief     Get AHB Clock Division
  * @param     None
  * @retval    Return bus division coefficient
  */
uint8_t RCC_GetAHBClockDivision(void)
{
    return RCC->CFGR.BIT.HPRE;
}

/**
  * @brief     Get APB0 Clock Division
  * @param     None
  * @retval    Return bus division coefficient
  */
uint8_t RCC_GetAPB0ClockDivision(void)
{
     return RCC->CFGR.BIT.PPRE0;
}

/**
  * @brief     Get APB1 Clock Division
  * @param     None
  * @retval    Return bus division coefficient
  */
uint8_t RCC_GetAPB1ClockDivision(void)
{
     return RCC->CFGR.BIT.PPRE1;
}

/**
  * @brief     Turn on or off external crystal oscillator.
  * @param     OnOffState: state of the HSE.
  *         This parameter can be one of the following values:
  *            @arg @b SH_ON
  *            @arg @b SH_OFF
  * @retval    None
  */
void RCC_HSEOnOff(CmdState OnOffState)
{
    /* Check the parameters */
    assert_param(IS_CMD_STATE(OnOffState));

    RCC_REGS_UNLOCK();
    if (OnOffState != SH_OFF)
    {
        /* Turn on HSE clock */
        RCC->CR.BIT.HSEON = SH_ON;
    }
    else
    {
        /* Turn off HSE clock */
        RCC->CR.BIT.HSEON = SH_OFF;
    }
    RCC_REGS_LOCK();
}

/**
  * @brief     Turn on or off the internal high-frequency oscillator.
  * @param     OnOffState: state of the HSI.
  *         This parameter can be one of the following values:
  *            @arg @b SH_ON
  *            @arg @b SH_OFF
  * @retval    None
  */
void RCC_HSIOnOff(CmdState OnOffState)
{
    /* Check the parameters */
    assert_param(IS_CMD_STATE(OnOffState));
    RCC_REGS_UNLOCK();
    if (OnOffState != SH_OFF)
    {
        /* Turn on HSE clock */
        RCC->CR.BIT.HSION = SH_ON;
    }
    else
    {
        /* Turn off HSE clock */
        RCC->CR.BIT.HSION = SH_OFF;
    }
    RCC_REGS_LOCK();
}

/**
  * @brief     Turn on or off the internal low-frequency oscillator.
  * @param     OnOffState: state of the LSI.
  *         This parameter can be one of the following values:
  *            @arg @b SH_ON
  *            @arg @b SH_OFF
  * @retval    None
  */
void RCC_LSIOnOff(CmdState OnOffState)
{
    /* Check the parameters */
    assert_param(IS_CMD_STATE(OnOffState));
    RCC_REGS_UNLOCK();
    if (OnOffState != SH_OFF)
    {
        /* Turn on HSE clock */
        RCC->CR.BIT.LSION = SH_ON;
    }
    else
    {
        /* Turn off HSE clock */
        RCC->CR.BIT.LSION = SH_OFF;
    }
    RCC_REGS_LOCK();
}

/**
  * @brief     Get HSI Clock Ready Flag
  * @param     None
  * @retval    Return HSIRDY flag
  */
FlagStatus RCC_GetHSIReadyFlag(void)
{
    return (RCC->CR.BIT.HSIRDY?SH_SET:SH_RESET);
}

/**
  * @brief     Get HSE Clock Ready Flag
  * @param     None
  * @retval    Return HSERDY flag
  */
FlagStatus RCC_GetHSEReadyFlag(void)
{
    return (RCC->CR.BIT.HSERDY?SH_SET:SH_RESET);
}

/**
  * @brief     Get LSI Clock Ready Flag
  * @param     None
  * @retval    Return LSIRDY flag
  */
FlagStatus RCC_GetLSIReadyFlag(void)
{
    return (RCC->CR.BIT.LSIRDY?SH_SET:SH_RESET);
}

/**
  * @brief     Set system clock source
  * @param     ClockSourceSelect Description
  * @retval    None
  */
void RCC_SetSystemClockSource(RCC_SysSource_TypeDef ClockSourceSelect)
{
    assert_param(IS_SYSTEM_CLOCK_SOURCE(ClockSourceSelect));
    RCC_REGS_UNLOCK();
    RCC->CR.BIT.SW = ClockSourceSelect;
    while(RCC->CR.BIT.SWS != RCC->CR.BIT.SW);
    RCC_REGS_LOCK();
    /*Update SystemCoreClock*/
    SystemCoreClockUpdate();
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
