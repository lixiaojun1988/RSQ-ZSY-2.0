/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_rcc.h
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-05-21  
  * @brief   This file contains all the functions prototypes for the RCC firmware

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
#ifndef __SH30F9XX_SB0_RCC_H
#define __SH30F9XX_SB0_RCC_H

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "sh30f9xx_sb0_libcfg.h"
#include "system_sh30f9xx_sb0.h"

/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */

/** @addtogroup RCC_MODULE
  * @{
  */



/* Exported types ------------------------------------------------------------*/

/** @defgroup RCC_Exported_Types RCC Exported Types 
  * @{
  */
/**
  * @brief  Clock Source Type
  */
typedef enum
{
    RCC_SYS_SRC_HSI = 0,
    RCC_SYS_SRC_HSE = 1,
    RCC_SYS_SRC_LSI = 2,
} RCC_SysSource_TypeDef;

/**
  * @brief AHB bus frequency division
  */
typedef enum
{
    RCC_AHB_DIV1  = 0, /*!< SYSCLK does not divide frequency*/
    RCC_AHB_DIV2  = 1, /*!< SYSCLK 2 frequency division*/
    RCC_AHB_DIV4  = 2, /*!< SYSCLK 4 frequency division*/
    RCC_AHB_DIV8  = 3, /*!< SYSCLK 8 frequency division*/
    RCC_AHB_DIV16 = 4, /*!< SYSCLK 16 frequency division*/
    RCC_AHB_DIV32 = 5, /*!< SYSCLK 32 frequency division*/
}RCC_AHB_DIVISION_TypeDef;

/**
  * @brief APB0 bus frequency division
  */
typedef enum
{
    RCC_APB0_DIV1  = 0, /*!< PCK0 does not divide frequency*/
    RCC_APB0_DIV2  = 1, /*!< PCK0 2 frequency division*/
    RCC_APB0_DIV4  = 2, /*!< PCK0 4 frequency division*/
    RCC_APB0_DIV8  = 3, /*!< PCK0 8 frequency division*/
    RCC_APB0_DIV16 = 4, /*!< PCK0 16 frequency division*/
}RCC_APB0_DIVISION_TypeDef;

/**
  * @brief APB1 bus frequency division
  */
typedef enum
{
    RCC_APB1_DIV1  = 0, /*!< PCK1 does not divide frequency*/
    RCC_APB1_DIV2  = 1, /*!< PCK1 2 frequency division*/
    RCC_APB1_DIV4  = 2, /*!< PCK1 4 frequency division*/
    RCC_APB1_DIV8  = 3, /*!< PCK1 8 frequency division*/
    RCC_APB1_DIV16 = 4, /*!< PCK1 16 frequency division*/
}RCC_APB1_DIVISION_TypeDef;

/**
  * @brief  Structure for RCC clock
  */
typedef struct
{
    uint32_t sysFreq;   /*!<  System clock frequency, unit is Hz */
    uint32_t hclkFreq;  /*!< AHB bus clock frequency, unit is Hz */
    uint32_t pclk0Freq; /*!< APB0 bus clock frequency, unit is Hz */
    uint32_t pclk1Freq; /*!< APB1 bus clock frequency, unit is Hz */
} RCC_Clocks_TypeDef;

/**
  * @brief RCC AHB Modules
  */
typedef enum
{
    RCC_AHB_SYSCFG = RCC_AHBENR_SYSCFGEN_Msk,/*!< AHB Module SYSCFG  */   
    RCC_AHB_CRC    = RCC_AHBENR_CRCEN_Msk,   /*!< AHB Module CRC  */  
    RCC_AHB_GPIO   = RCC_AHBENR_IOCLKEN_Msk, /*!< AHB Module GPIO  */  
} RCC_AHB_TypeDef;


/**
  * @brief RCC APB0 Modules
  */
typedef enum
{
    RCC_APB0_TIM0 = RCC_APB0ENR_TIM0EN_Msk ,   /*!< APB0 Module TIM0  */      
    RCC_APB0_TIM1 = RCC_APB0ENR_TIM1EN_Msk ,   /*!< APB0 Module TIM1  */     
    RCC_APB0_TIM2 = RCC_APB0ENR_TIM2EN_Msk ,   /*!< APB0 Module TIM2  */    
    RCC_APB0_TIM3 = RCC_APB0ENR_TIM3EN_Msk ,   /*!< APB0 Module TIM3  */    
    RCC_APB0_PWM0 = RCC_APB0ENR_PWM0EN_Msk ,   /*!< APB0 Module PWM0  */    
    RCC_APB0_PWM1 = RCC_APB0ENR_PWM1EN_Msk ,   /*!< APB0 Module PWM1  */    
    RCC_APB0_PWM2 = RCC_APB0ENR_PWM2EN_Msk ,   /*!< APB0 Module PWM2  */    
    RCC_APB0_PWM3 = RCC_APB0ENR_PWM3EN_Msk ,   /*!< APB0 Module PWM3  */    
    RCC_APB0_PCA0 = RCC_APB0ENR_PCA0EN_Msk ,   /*!< APB0 Module PCA0  */   
    RCC_APB0_PCA1 = RCC_APB0ENR_PCA1EN_Msk ,   /*!< APB0 Module PCA1  */    
    RCC_APB0_PCA2 = RCC_APB0ENR_PCA2EN_Msk ,   /*!< APB0 Module PCA2  */    
    RCC_APB0_PCA3 = RCC_APB0ENR_PCA3EN_Msk ,   /*!< APB0 Module PCA3  */    
    RCC_APB0_ADC  = RCC_APB0ENR_ADCEN_Msk  ,   /*!< APB0 Module ADC  */    
    RCC_APB0_EXTI = RCC_APB0ENR_EXTIEN_Msk ,   /*!< APB0 Module EXTI  */    
    RCC_APB0_WWDT = RCC_APB0ENR_WWDTEN_Msk ,   /*!< APB0 Module WWDT  */    
} RCC_APB0_TypeDef;                               

/**
  * @brief RCC APB1 Modules
  */
typedef enum
{
    RCC_APB1_UART0 = RCC_APB1ENR_UART0EN_Msk ,    /*!< APB1 Module UART0  */
    RCC_APB1_UART1 = RCC_APB1ENR_UART1EN_Msk ,    /*!< APB1 Module UART1  */
    RCC_APB1_UART2 = RCC_APB1ENR_UART2EN_Msk ,    /*!< APB1 Module UART2  */
    RCC_APB1_UART3 = RCC_APB1ENR_UART3EN_Msk ,    /*!< APB1 Module UART3  */ 
    RCC_APB1_UART4 = RCC_APB1ENR_UART4EN_Msk ,    /*!< APB1 Module UART4  */ 
    RCC_APB1_UART5 = RCC_APB1ENR_UART5EN_Msk ,    /*!< APB1 Module UART5  */ 
    RCC_APB1_SPI0  = RCC_APB1ENR_SPI0EN_Msk  ,    /*!< APB1 Module SPI0  */ 
    RCC_APB1_SPI1  = RCC_APB1ENR_SPI1EN_Msk  ,    /*!< APB1 Module SPI1  */ 
    RCC_APB1_TWI0  = RCC_APB1ENR_TWI0EN_Msk  ,    /*!< APB1 Module TWI0  */ 
    RCC_APB1_TWI1  = RCC_APB1ENR_TWI1EN_Msk  ,    /*!< APB1 Module TWI1  */ 
    RCC_APB1_FIFO0 = RCC_APB1ENR_FIFO0EN_Msk ,    /*!< APB1 Module FIFO0  */ 
    RCC_APB1_FIFO1 = RCC_APB1ENR_FIFO1EN_Msk ,    /*!< APB1 Module FIFO1  */ 
    RCC_APB1_FIFO2 = RCC_APB1ENR_FIFO2EN_Msk ,    /*!< APB1 Module FIFO2  */ 
} RCC_APB1_TypeDef;                                  

/**
  * @brief RCC RSTSTR Type
  */
typedef enum
{
    RCC_RST_PIN      = RCC_RSTCLR_PINRSTFC_Msk, /*!< RESET Type PIN  */
    RCC_RST_LVR      = RCC_RSTCLR_LVRSTFC_Msk,  /*!< RESET Type LVR  */
    RCC_RST_POWERON  = RCC_RSTCLR_PORSTFC_Msk,  /*!< RESET Type POWERON  */
    RCC_RST_SOFTWARE = RCC_RSTCLR_SWRSTFC_Msk,  /*!< RESET Type SOFTWARE  */
    RCC_RST_WWDT     = RCC_RSTCLR_WWDTRSTFC_Msk,/*!< RESET Type WWDT  */
    RCC_RST_HWDT     = RCC_RSTCLR_HWDTRSTFC_Msk,/*!< RESET Type HWDT  */
} RCC_RESET_TypeDef;                               

/**
  * @brief RCC Clock Interrupt Enable/Disable Bits
  */
typedef enum
{
    RCC_CIENR_HSERDYIE = RCC_CIENR_HSERDYIE_Msk,/*!< HSE ready interrupt enable  */
} RCC_CIENR_TypeDef;                               

/**
  * @brief RCC Clock Interrupt Status Bits
  */
typedef enum
{
    RCC_INT_HSE_RF = RCC_CISTR_HSERDYIF_Msk,/*!< HSE ready interrupt flag  */
    RCC_CSM_HSE_IF = RCC_CISTR_HSECSMF_Msk, /*!< HSE clock security monitor interrupt flag  */
} RCC_INT_TypeDef;

/**
  * @brief RCC Clock Interrupt Status Clear Bits
  */
typedef enum
{
    RCC_CICCLR_CSMC    = RCC_CICLR_CSMC_Msk,   /*!< Clock security monitor interrupt clear  */
    RCC_CICCLR_HSERDYC = RCC_CICLR_HSERDYC_Msk,/*!< HSE ready interrupt clear  */ 
} RCC_CICCLR_TypeDef;
                                   
/**
* @}
*/


/* Exported constants --------------------------------------------------------*/

/** @defgroup RCC_Exported_Constants  RCC Exported Constants 
  * @{
  */
  
/**
  * @brief All AHB  Modules
  */
#define RCC_AHB_ALL                (0x000F)
  

/**
  * @brief All APB0  Modules
  */
#define RCC_APB0_ALL               (0x07FFF)



/**
  * @brief All APB1  Modules
  */
#define RCC_APB1_ALL               (0x07FF)



/**
  * @brief All RST  Modules
  */
#define RCC_RST_ALL                (0x003F)


/**
  * @brief All CIENR  Register
  */
#define RCC_CIENR_ALL              (0x001C)


/**
  * @brief All INT  Modules
  */
#define RCC_INT_ALL                (0x0048)


/**
  * @brief All INT  CICCLR
  */
#define RCC_CICCLR_ALL             (0x0088)

/**
* @}
*/

                                 
                                

/* Exported Parameter Check Macro ----------------------------------------------*/

/** @defgroup RCC_Parameter_Check_Macro RCC Parameter Check Macro 
  * @{
  */
/**
  * @brief check AHB Module source
  */
#define IS_AHB_MODULES(m)          ((((m)&RCC_AHB_ALL) != 0) \
                                   && (((m) & (~RCC_AHB_ALL)) == 0))  
                                    
                                    
/**
  * @brief check APB0 clock source
  */
#define IS_APB0_MODULES(m)         ((((m)&RCC_APB0_ALL) != 0) \
                                   && (((m) & (~RCC_APB0_ALL)) == 0))
       


/**
  * @brief check APB1 clock source
  */
#define IS_APB1_MODULES(m)         ((((m)&RCC_APB1_ALL) != 0) \
                                   && (((m) & (~RCC_APB1_ALL)) == 0))
                                    
/**
  * @brief check RST clock source
  */
#define IS_RST_MODULES(m)          ((((m)&RCC_RST_ALL) != 0) \
                                   && (((m) & (~RCC_RST_ALL)) == 0))
     


/**
  * @brief check CIENR source
  */
#define IS_CIENR_REGISTER(m)       ((((m)&RCC_CIENR_ALL) != 0) \
                                   && (((m) & (~RCC_CIENR_ALL)) == 0))
                                    
                                    
/**
  * @brief check CISTR interrupt flag
  */
#define IS_RCC_INT_FLAG(m)         ((((m)&RCC_INT_ALL) != 0) \
                                   && (((m) & (~RCC_INT_ALL)) == 0)) 


/**
  * @brief check CICCLR 
  */
#define IS_CICCLR_FLAG(m)          ((((m)&RCC_CICCLR_ALL) != 0) \
                                   && (((m) & (~RCC_CICCLR_ALL)) == 0)) 

/**
  * @brief check AHB  Division
  */
#define IS_AHB_CLOCK_DIVSION(Div)  (Div <= (RCC_CFGR_HPRE_Msk>>RCC_CFGR_HPRE_Pos))

/**
  * @brief check APB0  Division
  */
#define IS_APB0_CLOCK_DIVSION(Div) (Div <= (RCC_CFGR_PPRE0_Msk>>RCC_CFGR_PPRE0_Pos))

/**
  * @brief check APB1  Division
  */
#define IS_APB1_CLOCK_DIVSION(Div) (Div <= (RCC_CFGR_PPRE1_Msk>>RCC_CFGR_PPRE1_Pos))

/**
  * @brief check system clock source
  */
#define IS_SYSTEM_CLOCK_SOURCE(Src) (Src <= RCC_CR_SW_Msk)
/**
* @}
*/  

/* Exported Function  Macro ------------------------------------------------*/

/** @defgroup RCC_Export_Func_Macro  RCC Export Function Macro 
  * @{
  */
  
/** 
  *@brief  Unlock the RCC module's regsiter. RCC registers can be modified.
  */
#define RCC_REGS_UNLOCK()          (RCC->RCCLOCK = 0x33CC)


/** 
  *@brief  Lock the RCC module's regsiter. RCC registers cannot be modified.
  */
#define RCC_REGS_LOCK()            (RCC->RCCLOCK = 0)


/**
  * @}
  */
  
  
/* Exported Function ------------------------------------------------------*/
  
/** @addtogroup RCC_Exported_Functions    RCC Exported Functions
  * @{
  */

/* Returns the frequencies of different on chip clocks. */
extern void RCC_GetClocksFreq(RCC_Clocks_TypeDef *Clocks);

/* open or close AHB modules' clock gate */
extern void RCC_AHBPeriphClockOnOff(RCC_AHB_TypeDef AHBModules, CmdState OnOffState);

/* open or close APB1 modules' clock gate */
extern void RCC_APB1PeriphClockOnOff(RCC_APB1_TypeDef APB1Modules, CmdState OnOffState);

/* open or close APB0 modules' clock gate */
extern void RCC_APB0PeriphClockOnOff(RCC_APB0_TypeDef APB0Modules, CmdState OnOffState);

/* reset AHB modules */
extern void RCC_AHBPeriphReset(RCC_AHB_TypeDef AHBModules);

/* reset APB1 modules */
extern void RCC_APB0PeriphReset(RCC_APB0_TypeDef APB0Modules);

/* reset APB1 modules */
extern void RCC_APB1PeriphReset(RCC_APB1_TypeDef APB1Modules);

/* Get RCC reset flag */
extern FlagStatus RCC_GetResetFlag(RCC_RESET_TypeDef ResetFlag);

/* clear RCC reset flags */
extern void RCC_ClearResetFlag(RCC_RESET_TypeDef ResetFlags);

/* open or close CSM modules */
extern void RCC_CSMModuleOnOff(CmdState OnOffState);

/* Get RCC Interrupt flag */
extern FlagStatus RCC_GetFlagStatus(RCC_INT_TypeDef RCCIntFlag);

/* Clear RCC clock interrupt flag */
extern void RCC_ClearITPendingBit(RCC_CICCLR_TypeDef CLK_INTFLG_CLR);

/* RCC interrupt enable configure */
extern void RCC_INTConfig(RCC_CIENR_TypeDef RDYINTIE, FunctionalState NewState);

/* Set AHB Clock Division */
extern void RCC_SetAHBClockDivision(RCC_AHB_DIVISION_TypeDef AHBDiv);

/* Set APB0 Clock Division */
extern void RCC_SetAPB0ClockDivision(RCC_APB0_DIVISION_TypeDef APB0Div);

/* Set APB1 Clock Division */
extern void RCC_SetAPB1ClockDivision(RCC_APB1_DIVISION_TypeDef APB1Div);

/* Get AHB Clock Division */
extern uint8_t RCC_GetAHBClockDivision(void);

/* Get APB0 Clock Division */
extern uint8_t RCC_GetAPB0ClockDivision(void);

/* Get APB1 Clock Division */
extern uint8_t RCC_GetAPB1ClockDivision(void);

/* Returns the frequencies of different on chip clocks. */
extern void RCC_SetSystemClockSource(RCC_SysSource_TypeDef ClockSourceSelect);

/* Turn on or off external crystal oscillator. */
extern void RCC_HSEOnOff(CmdState OnOffState);

/* Turn on or off the internal high-frequency oscillator. */
extern void RCC_HSIOnOff(CmdState OnOffState);

/* Turn on or off the internal low-frequency oscillator. */
extern void RCC_LSIOnOff(CmdState OnOffState);

/* Get HSI Clock Ready Flag */
extern FlagStatus RCC_GetHSIReadyFlag(void);

/* Get HSE Clock Ready Flag */
extern FlagStatus RCC_GetHSEReadyFlag(void);

/* Get LSI Clock Ready Flag */
extern FlagStatus RCC_GetLSIReadyFlag(void);
/**
  * @}
  */
#ifdef __cplusplus
}
#endif

/**
  * @}
  */

 /**
  * @}
  */
#endif /*__SH30F9XX_SB0_RCC_H */
/************************ (C) COPYRIGHT Sinowealth *****END OF FILE****/
