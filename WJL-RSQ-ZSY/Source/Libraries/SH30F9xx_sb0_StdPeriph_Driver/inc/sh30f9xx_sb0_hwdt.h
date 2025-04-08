/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_hwdt.h
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-5-21  
  * @brief   This file contains all the functions prototypes for the HWDT firmware
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
#ifndef __SH30F9XX_SB0_HWDT_H
#define __SH30F9XX_SB0_HWDT_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "sh30f9xx_sb0_libcfg.h"

/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */

/** @addtogroup HWDT_MODULE
  * @{
  */



/** @defgroup HWDT_Exported_Types HWDT Exported Types 
  * @{
  */
  
  
/** 
  * @brief HWDT Reload Period  
  */   
typedef enum
{ 
    HWDT_ReloadPeriod_2048MS                     = 0,  /*!< HWDT Reload Period 2048 ms  */     
    HWDT_ReloadPeriod_1536MS                     = 1,  /*!< HWDT Reload Period 1536 ms  */       
    HWDT_ReloadPeriod_1024MS                     = 2,  /*!< HWDT Reload Period 1024 ms  */        
    HWDT_ReloadPeriod_512MS                      = 3,  /*!< HWDT Reload Period 512 ms  */        
    HWDT_ReloadPeriod_256MS                      = 4,  /*!< HWDT Reload Period 256 ms  */        
    HWDT_ReloadPeriod_128MS                      = 5,  /*!< HWDT Reload Period 128 ms  */        
    HWDT_ReloadPeriod_64MS                       = 6,  /*!< HWDT Reload Period 64 ms  */        
    HWDT_ReloadPeriod_16MS                       = 7,  /*!< HWDT Reload Period 16 ms  */        
} HWDT_ReloadPeriod_TypeDef; 

/** 
  *@brief  Lock or Unlock the HWDT module's regsiter. 
  */
typedef enum
{ 
    HWDT_WriteAccess_Enable     = ((uint16_t)0x5AA5),/*!< The Unlock Key */
    HWDT_WriteAccess_Disable    = ((uint16_t)0x0000)/*!< The Lock Key */      
} HWDT_WriteAccess_TypeDef; 

/**
  * @}
  */

/** @defgroup HWDT_Parameter_Check_Macro HWDT Parameter Check Macro 
  * @{
  */

/*!Check the Key value */
#define IS_HWDT_WRITE_ACCESS(ACCESS)   (((ACCESS) == HWDT_WriteAccess_Enable) \
                                            ||((ACCESS) == HWDT_WriteAccess_Disable))



/*!Check the Realod Period for HWDT */
#define IS_HWDT_RELOAD_PERIOD(PERIOD)  (((PERIOD) == HWDT_ReloadPeriod_16MS)      \
                                            || ((PERIOD) == HWDT_ReloadPeriod_64MS)    \
                                            || ((PERIOD) == HWDT_ReloadPeriod_128MS)   \
                                            || ((PERIOD) == HWDT_ReloadPeriod_256MS)   \
                                            || ((PERIOD) == HWDT_ReloadPeriod_512MS)   \
                                            || ((PERIOD) == HWDT_ReloadPeriod_1024MS)  \
                                            || ((PERIOD) == HWDT_ReloadPeriod_1536MS)  \
                                            || ((PERIOD) == HWDT_ReloadPeriod_2048MS))                                     


/**
  * @}
  */
  
  

/** @addtogroup HWDT_Exported_Functions
  * @{
  */
extern void HWDT_ConfigPeriod(HWDT_ReloadPeriod_TypeDef ReloadPeriod);

extern void HWDT_OnOff(CmdState OnOffState);

extern void HWDT_ReloadCounter( void );

extern FlagStatus HWDT_GetFlagStatus( void );
 
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __SH30F9XX_SB0_HWDT_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT Sinowealth *****END OF FILE****/
