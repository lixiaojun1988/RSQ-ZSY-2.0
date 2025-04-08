/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_libcfg.h
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-05-21  
  * @brief   This file includes all module's head files
  ******************************************************************************
  * @mainpage SH30F9XX SB0 Series Standard Library
  *           This document contains the User Manual  of the SH30F9XX SB0  Standard drivers
  * @note  2024-06-13 First release SH30F9XX SB0 Series Standard Driver Library V1.0.0
  * @attention  SINOWEALTH IS SUPPLYING THIS SOFTWARE FOR USE EXCLUSIVELY SH_ON SINOWEALTH'S 
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
#ifndef __SH30F9XX_SB0_LIBCFG_H
#define __SH30F9XX_SB0_LIBCFG_H

#ifdef __cplusplus
extern "C"
{
#endif

/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @brief  SH30F9XX_SB0_STANDARD_LIB contain all module driver for Sinowealth SH30F9XX SB0 MCU series.
  * @{
  */

/** @defgroup LIBCFG LIBCFG
  * @brief SH30F9XX SB0 series library configuration module,this module define some general purpose 
  *        macro and type for library driver use.
  * @{
  */

/** @defgroup LIBCFG_Exported_Types LIBCFG Exported Types 
  * @{
  */
/**
  * @brief Define Boolean data type
  * @arg   SH_TRUE:   logical  true
  * @arg   SH_FALSE:  logical  false
  */    
typedef enum
{
    SH_FALSE=0,
    SH_TRUE =!SH_FALSE,
} BoolType;


/**
  * @brief define  flag's data type
  * @arg   SH_SET:     the flag is setted
  * @arg   SH_RESET: the flag is cleared
  */    
typedef enum 
{ 
    SH_RESET = 0, 
    SH_SET = !SH_RESET, 
} FlagStatus;


/**
  * @brief define the functional state type
  * @arg   SH_ENABLE:    enable the feature
  * @arg   SH_DISABLE:  disable the feature
  */    
typedef enum 
{ 
    SH_DISABLE = 0, 
    SH_ENABLE = !SH_DISABLE, 
} FunctionalState;


/**
  * @brief define the peripheral state type
  * @arg   SH_ON:   peripheral is enabled
  * @arg   SH_OFF:  peripheral is disabled
  */    
typedef enum 
{ 
    SH_OFF = 0, 
    SH_ON = !SH_OFF, 
} CmdState; 


/**
  * @brief define the return data type
  * @arg   SH_ERROR:     error occurs
  * @arg   SH_SUCCESS:   operation succeeded
  */   
typedef enum 
{
    SH_ERROR = 0, 
    SH_SUCCESS = !SH_ERROR
} ErrorStatus;


/**
  * @brief define the peripheral Pin electrical level state type
  * @arg   SH_LOW:   SH_LOW level
  * @arg   SH_HIGH:  High level
  */    
typedef enum 
{ 
    SH_LOW = 0, 
    SH_HIGH = !SH_LOW, 
} LevelStatus; 



/**
  * @}
  */



/** @defgroup LIBCFG_Parameter_Check_Macro LIBCFG Parameter Check Macro 
  * @{
  */

/** 
  *@brief  check the input value is a legal boolean data
  */
#define IS_BOOL_TYPE(var)              (((var) == SH_FALSE) || ((var) == SH_TRUE))

/** 
  *@brief  check the input value is a legal flag data
  */
#define IS_FLAG_STATUS(flag)           (((flag) == SH_RESET) || ((flag) == SH_SET))

/** 
  *@brief  check the input value is a legal function state data
  */
#define IS_FUNCTION_STATE(state)       (((state) == SH_DISABLE) || ((state) == SH_ENABLE))

/** 
  *@brief  check the input value is a legal peripheral state data
  */
#define IS_CMD_STATE(state)            (((state) == SH_ON) || ((state) == SH_OFF))

/** 
  *@brief  check the input value is a legal error status data
  */
#define IS_ERROR_STATUS(status)        (((status) == SH_ERROR) || ((status) == SH_SUCCESS))

/** 
  *@brief  check the input value is a legal peripheral state data
  */
#define IS_LEVEL_STATE(state)          (((state) == SH_LOW) || ((state) == SH_HIGH))

/**
  * @}
  */
  
  
/** @defgroup LIBCFG_Export_Func_Macro LIBCFG Export Function Macro 
  * @{
  */

#ifdef DEFAULT_ASSERT_ENABLE
/** @brief  assert for function's input parameters. Enable or disable it by the macro DEFAULT_ASSERT_ENABLE  at compile time*/
#define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, (uint32_t)__LINE__))
/** @brief  break the program at the assert  failed satement . Enable or disable it by the macro DEFAULT_ASSERT_ENABLE at compile time */
#define assert_break(expr) if(!(expr)){ __BKPT(0);}

/** @brief  define the assert failed actions here. Enable or disable it by the macro DEFAULT_ASSERT_ENABLE  at compile time */
void assert_failed(unsigned char* file, unsigned int line);
#else
/** @brief  assert for function's input parameters. Enable or disable it by the macro DEFAULT_ASSERT_ENABLE  at compile time*/
#define assert_param(expr) ((void)0)
/** @brief  break the program at the assert  failed satement . Enable or disable it by the macro DEFAULT_ASSERT_ENABLE at compile time
*/
#define assert_break(expr) ((void)0)
#endif

/**
  * @}
  */
  
  
  
/* Exported types ------------------------------------------------------------*/



#include "sh30f9xx_sb0.h"

#ifndef _RTE_

#include "sh30f9xx_sb0_flash.h"
                    
#include "sh30f9xx_sb0_syscfg.h"
                    
#include "sh30f9xx_sb0_rcc.h"

#include "sh30f9xx_sb0_hwdt.h"
                    
#include "sh30f9xx_sb0_wwdt.h"
                                    
#include "sh30f9xx_sb0_gpio.h"
                                                   
#include "sh30f9xx_sb0_timer.h"
                            
#include "sh30f9xx_sb0_uart.h"

#include "sh30f9xx_sb0_fifo.h"

#include "sh30f9xx_sb0_pca.h"

#include "sh30f9xx_sb0_pwm.h"

#include "sh30f9xx_sb0_adc.h"

#include "sh30f9xx_sb0_exti.h"

#include "sh30f9xx_sb0_spi.h"

#include "sh30f9xx_sb0_twi.h"

#include "sh30f9xx_sb0_crc.h"

#include "sh30f9xx_sb0_nvic.h"
#else

#include "RTE_Components.h"

#ifdef RTE_MODULE_FLASH
#include "sh30f9xx_sb0_flash.h"
#endif

#ifdef RTE_MODULE_SYSCFG
#include "sh30f9xx_sb0_syscfg.h"
#endif

#ifdef RTE_MODULE_RCC
#include "sh30f9xx_sb0_rcc.h"
#endif

#ifdef RTE_MODULE_HWDT
#include "sh30f9xx_sb0_hwdt.h"
#endif

#ifdef RTE_MODULE_WWDT
#include "sh30f9xx_sb0_wwdt.h"
#endif

#ifdef RTE_MODULE_CRC
#include "sh30f9xx_sb0_crc.h"
#endif



#ifdef RTE_MODULE_GPIO
#include "sh30f9xx_sb0_gpio.h"
#endif

#ifdef RTE_MODULE_EXTI
#include "sh30f9xx_sb0_exti.h"
#endif



#ifdef RTE_MODULE_TIMER
#include "sh30f9xx_sb0_timer.h"
#endif

#ifdef RTE_MODULE_ADC
#include "sh30f9xx_sb0_adc.h"
#endif

#ifdef RTE_MODULE_UART
#include "sh30f9xx_sb0_uart.h"
#endif

#ifdef RTE_MODULE_SPI
#include "sh30f9xx_sb0_spi.h"
#endif

#ifdef RTE_MODULE_TWI
#include "sh30f9xx_sb0_twi.h"
#endif

#ifdef RTE_MODULE_NVIC
#include "sh30f9xx_sb0_nvic.h"
#endif

#ifdef RTE_MODULE_PWM
#include "sh30f9xx_sb0_pwm.h"
#endif

#ifdef RTE_MODULE_PCA
#include "sh30f9xx_sb0_pca.h"
#endif

#ifdef RTE_MODULE_FIFO
#include "sh30f9xx_sb0_fifo.h"
#endif


#ifdef RTE_MODULE_NVIC
#include "sh30f9xx_sb0_nvic.h"
#endif

#endif


#ifdef _MODULE_DBG_PRINTF
#include <stdio.h>
#endif


#ifdef __cplusplus
}
#endif

#endif /*__SH30F9XX_SB0_LIBCFG_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT Sinowealth *****END OF FILE****/
