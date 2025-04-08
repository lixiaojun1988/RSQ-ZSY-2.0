/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_wwdt.h
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-05-21  
  * @brief   This file contains all the functions prototypes for the WWDT firmware
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
#ifndef __SH30F9XX_SB0_WWDT_H
#define __SH30F9XX_SB0_WWDT_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "sh30f9xx_sb0_libcfg.h"

/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */

/** @defgroup WWDT_MODULE WWDT
  * @{
  */

/** @defgroup WWDT_Exported_Types WWDT Exported Types 
  * @{
  */
  
/** 
  * @brief WWDT Prescaler Enum Value  
  */   
typedef enum
{ 
    WWDT_Prescaler_1      = 0,  /*!< WWDT Precaler 1*/     
    WWDT_Prescaler_2      = 1,  /*!< WWDT Precaler 2*/     
    WWDT_Prescaler_4      = 2,  /*!< WWDT Precaler 4*/    
    WWDT_Prescaler_8      = 3,  /*!< WWDT Precaler 8*/     
    WWDT_Prescaler_16     = 4,  /*!< WWDT Precaler 16*/     
    WWDT_Prescaler_32     = 5,  /*!< WWDT Precaler 32*/     
    WWDT_Prescaler_64     = 6,  /*!< WWDT Precaler 64*/     
    WWDT_Prescaler_128    = 7,  /*!< WWDT Precaler 128*/     
} WWDR_Prescaler_TypeDef; 

/**
  * @}
  */


/** @defgroup WWDT_Parameter_Check_Macro WWDT Parameter Check Macro 
  * @{
  */
/*! Check the Prescaler of WWDT clock */
#define IS_WWDT_PRESCALER(PRESCALER)    (((PRESCALER) == WWDT_Prescaler_1)    \
                                        || ((PRESCALER) == WWDT_Prescaler_2)  \
                                        || ((PRESCALER) == WWDT_Prescaler_4)  \
                                        || ((PRESCALER) == WWDT_Prescaler_8)  \
                                        || ((PRESCALER) == WWDT_Prescaler_16) \
                                        || ((PRESCALER) == WWDT_Prescaler_32) \
                                        || ((PRESCALER) == WWDT_Prescaler_64) \
                                        || ((PRESCALER) == WWDT_Prescaler_128))

/*! Check the WWDT window value */
#define IS_WWDT_WINDOW_VALUE(VALUE)     ((VALUE) <= 0xFF)
/*! Check the WWDT counter value */
#define IS_WWDT_COUNTER(COUNTER)        ((COUNTER) <= 0xFF)
/**
  * @}
  */
  
  
/** @defgroup WWDT_Export_Func_Macro WWDT Export Function Macro 
  * @{
  */
  
/*! Clear WWDT */
#define CLR_WWDT()                      (WWDT->CLR = 0x5555)

/**
  * @}
  */
  
/** @addtogroup WWDT_Exported_Functions
  * @{
  */
void WWDT_Reset(void);

void WWDT_SetPrescaler(uint32_t WWDT_Prescaler);

void WWDT_SetWindowValue(uint8_t WindowValue);

void WWDT_INTConfig(FunctionalState NewState);

void WWDT_SetCounter(uint8_t Counter);

void WWDT_Enable(uint32_t WWDT_Prescaler, BoolType EnableINT, uint8_t Counter, uint8_t WindowValue);

FlagStatus WWDT_GetINTFlagStatus(void);

void WWDT_ClearFlag(void);

void WWDT_ReloadCounter(void);


#ifdef __cplusplus
}
#endif

#endif /* __SH32F9XX_SB0_WWDT_H */

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
