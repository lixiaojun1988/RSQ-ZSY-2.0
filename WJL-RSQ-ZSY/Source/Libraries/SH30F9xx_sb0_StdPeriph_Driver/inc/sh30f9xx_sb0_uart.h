/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_uart.h
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-05-21  
  * @brief   This file contains all the functions prototypes for the UART firmware
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
#ifndef __SH30F9XX_SB0_UART_H
#define __SH30F9XX_SB0_UART_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "sh30f9xx_sb0_libcfg.h"

/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */
 

 /** @addtogroup UART_MODULE
  * @{
  */
  
/** @defgroup UART_Exported_Types UART Exported Types 
  * @{
  */

/** 
  * @brief UART Fifo Channel Type
  */   
typedef enum 
{
    UART_FIFO_CH_TX        = 0, /*!< UART FIFO TX Channel */    
    UART_FIFO_CH_RX        = 1, /*!< UART FIFO RX Channel*/
} UART_FIFO_Channel_TypeDef;


/** 
  * @brief   Uart mode select parameter
  */
typedef enum 
{
   UART_Mode_0  = (uint16_t)0x0000,/*!< UART Mode 0*/
   UART_Mode_1  = (uint16_t)0x2000,/*!< UART Mode 1*/
   UART_Mode_2  = (uint16_t)0x4000,/*!< UART Mode 2*/
   UART_Mode_3  = (uint16_t)0x6000 /*!< UART Mode 3*/
} UART_Mode_TypeDef;

/** 
  * @brief  Uart stop bit select parameter 
  */
typedef enum 
{
   UART_StopBits_1 = (uint16_t)0x0001,/*!< UART Stop Bit 1*/
   UART_StopBits_2 = (uint16_t)0x0002 /*!< UART Stop Bit 2*/
} UART_StopBits_TypeDef;


/** 
  * @brief   Uart Ninth bit select parameter 
  */
typedef enum 
{
    UART_SoftWare_Custom = (uint16_t)0x0000,/*!< UART Ninth Bit Software Custom*/
    UART_Multi_Com       = (uint16_t)0x0800,/*!< UART Ninth Bit Multiple Communication */ 
    UART_Parity_Even     = (uint16_t)0x1400,/*!< UART Ninth Bit Parity Even */
    UART_Parity_Odd      = (uint16_t)0x1000 /*!< UART Ninth Bit Parity Odd */ 
} UART_Bit9_Function_TypeDef;

/** 
  * @brief   Uart enable select parameter
  */
typedef enum 
{
    UART_Disable_Tx = (uint32_t)(0UL<<UART_CR_TEN_Pos), /*!< UART Tx Disable*/
    UART_Enable_Tx  = (uint32_t)UART_CR_TEN_Msk,        /*!< UART Tx Enable*/
    UART_Disable_Rx = (uint32_t)(0UL<<UART_CR_REN_Pos), /*!< UART Rx Disable*/
    UART_Enable_Rx  = (uint32_t)UART_CR_REN_Msk,        /*!< UART Rx Enable*/
} UART_Enable_TypeDef;

/** 
  * @brief   Uart INT select parameter
  */
typedef enum 
{
    UART_INT_RX   = (uint16_t)UART_CR_RIE_Msk,  /*!< UART Interrupt RX*/
    UART_INT_TX   = (uint16_t)UART_CR_TIE_Msk,  /*!< UART Interrupt TX*/
    UART_INT_TC   = (uint16_t)UART_CR_TCIE_Msk, /*!< UART Interrupt TC*/
    UART_INT_LBD  = (uint16_t)UART_CR_LBDIE_Msk /*!< UART Interrupt LBD*/
} UART_INT_TypeDef;

/** 
  * @brief   Uart flag select parameter  
  */
typedef enum 
{
    UART_FLAG_RI    = (uint16_t)UART_FR_RI_Msk,   /*!< UART Flag RI*/
    UART_FLAG_TI    = (uint16_t)UART_FR_TI_Msk,   /*!< UART Flag TI*/
    UART_FLAG_TC    = (uint16_t)UART_FR_TC_Msk,   /*!< UART Flag TC*/
    UART_FLAG_TXCOL = (uint16_t)UART_FR_TXCOL_Msk,/*!< UART Flag TXCOL*/
    UART_FLAG_RXOV  = (uint16_t)UART_FR_RXOV_Msk, /*!< UART Flag RXOV*/
    UART_FLAG_FE    = (uint16_t)UART_FR_FE_Msk,   /*!< UART Flag FE*/
    UART_FLAG_PE    = (uint16_t)UART_FR_PE_Msk,   /*!< UART Flag PE*/
    UART_FLAG_LBD   = (uint16_t)UART_FR_LBD_Msk,   /*!< UART Flag LBD*/	
		UART_FLAG_ALL		= UART_FLAG_RI|UART_FLAG_TI|UART_FLAG_TC|UART_FLAG_TXCOL|UART_FLAG_RXOV|UART_FLAG_FE|UART_FLAG_PE|UART_FLAG_LBD
} UART_FLAG_TypeDef;


/** 
  * @brief   Uart LIN detect length select parameter  
  */
typedef enum 
{
    UART_LINDetectLength_10b = (uint16_t)0x0000,/*!< UART LIN Detect Length 10 Bit*/
    UART_LINDetectLength_11b = (uint16_t)0x0080,/*!< UART LIN Detect Length 11 Bit*/
} UART_LINDetectLength_TypeDef;

/** 
  * @brief   Uart Alternate function structure 
  */
typedef struct
{
    GPIO_TypeDef *UART_GPIOx; /*!< The corresponding port group number */
    uint16_t UART_GPIO_Pin;   /*!< The corresponding port number */
    uint8_t UART_GPIO_AF;     /*!< The corresponding function number */
} UART_AFTypeDef;


/** 
  * @brief  UART Init Structure definition  
  */

typedef struct
{
    UART_Mode_TypeDef UART_Mode;           /*!< Specifie the UART's clock sources.
                                                    This parameter can be a value of @ref UART_Mode_TypeDef */
    uint32_t UART_BaudRate;                     /*!< Specifie the UART's baud.
                                                    This parameter can be set according appication*/
    UART_StopBits_TypeDef UART_StopBits;   /*!< Specifie the UART's stop bit selection.
                                                    This parameter can be a value of @ref UART_StopBits_TypeDef */
    UART_Bit9_Function_TypeDef UART_Parity;/*!< Specifie the UART's parity check selection.
                                                    This parameter can be a value of @ref UART_Bit9_Function_TypeDef */
    uint32_t UART_Enable;                       /*!< Specifie the UART's enable or disable .
                                                    This parameter can be a value of @ref UART_Enable_TypeDef */
} UART_InitTypeDef;



/**
  * @}
  */

/** @defgroup UART_Exported_Constants UART Exported Constants 
  * @{
  */

#define UART_BaudRate_DIV_12               (uint32_t)0x80000000  /*! Only available for UART_Mode_0 */
#define UART_BaudRate_DIV_48               (uint32_t)0x40000000  /*! Only available for UART_Mode_0 */
#define UART_BaudRate_DIV_128              (uint32_t)0x20000000  /*! Only available for UART_Mode_2 */
#define UART_BaudRate_DIV_256              (uint32_t)0x10000000  /*! Only available for UART_Mode_2 */

/**
  * @}
  */
/** @defgroup UART_Parameter_Check_Macro UART Parameter Check Macro 
  * @{
  */

/**
  * @brief  Check is UART perioheral
  */
#define IS_UART_ALL_PERIPH(PERIPH)              ((PERIPH == UART0)   \
                                                || (PERIPH == UART1) \
                                                || (PERIPH == UART2) \
                                                || (PERIPH == UART3))

/**
  * @brief  Check is UART perioheral
  */
#define IS_UART_FIFO_CHANNEL(CHN)               ((CHN == UART_FIFO_CH_TX)   \
                                                || (CHN == UART_FIFO_CH_RX))


/**
  * @brief  Check is UART perioheral
  */
#define IS_UART_MODE(MODE)                      ((MODE == UART_Mode_0)   \
                                                || (MODE == UART_Mode_1) \
                                                || (MODE == UART_Mode_2) \
                                                || (MODE == UART_Mode_3))


/**
  * @brief  Check is UART perioheral
  */
#define IS_UART_MODE_0_BAUDRATE(BAUDRATE)       ((BAUDRATE == UART_BaudRate_DIV_12) \
                                                || (BAUDRATE == UART_BaudRate_DIV_48))
                                            
/**
  * @brief  Check is UART perioheral
  */
#define IS_UART_MODE_2_BAUDRATE(BAUDRATE)       ((BAUDRATE == UART_BaudRate_DIV_128) \
                                                || (BAUDRATE == UART_BaudRate_DIV_256))
/**
  * @brief  Check is UART perioheral
  */                                            
#define IS_UART_MODE_13_BAUDRATE(BAUDRATE)      ((BAUDRATE != UART_BaudRate_DIV_12) \
                                                && (BAUDRATE != UART_BaudRate_DIV_48) \
                                                && (BAUDRATE != UART_BaudRate_DIV_128) \
                                                && (BAUDRATE != UART_BaudRate_DIV_256))  
/**
  * @brief  Check is UART perioheral
  */                                            
#define IS_UART_MODE_123_STOP_BITS(BITS)        ((BITS == UART_StopBits_1)  \
                                                || (BITS == UART_StopBits_2))




/**
  * @brief  Check is UART perioheral
  */
#define IS_UART_MODE_23_PARITY(PARITY)          ((PARITY == UART_SoftWare_Custom)  \
                                                || (PARITY == UART_Multi_Com)      \
                                                || (PARITY == UART_Parity_Even)    \
                                                || (PARITY == UART_Parity_Odd))  


/**
  * @brief  Check is UART perioheral
  */
#define IS_UART_ENABLE(SH_ENABLE)               (((SH_ENABLE & (uint32_t)0xFFF9FFFF) == 0x00) \
                                                && (SH_ENABLE != (uint32_t)0x00))  



/**
  * @brief  Check is UART perioheral
  */
#define IS_UART_INT(INT)                        (((INT & (uint16_t)0xFF87) == 0x00) \
                                                && (INT != 0x00))




/**
  * @brief  Check is UART perioheral
  */
#define IS_UART_FLAG(FLAG)                      (((FLAG & (uint16_t)0xFF00) == 0x00) \
                                                && (FLAG != 0x00))
/**
  * @brief  Check is UART perioheral
  */
#define IS_UART_GET_ONE_FLAG(FLAG)              ((FLAG == UART_FLAG_RI)      \
                                                || (FLAG == UART_FLAG_TI)    \
                                                || (FLAG == UART_FLAG_TC)    \
                                                || (FLAG == UART_FLAG_TXCOL) \
                                                || (FLAG == UART_FLAG_RXOV)  \
                                                || (FLAG == UART_FLAG_FE)    \
                                                || (FLAG == UART_FLAG_PE)    \
                                                || (FLAG == UART_FLAG_LBD))



/**
  * @brief  Check is UART perioheral
  */
#define IS_UART_LIN_DETECT_LENGTH(LENGTH)       ((LENGTH == UART_LINDetectLength_10b) \
                                                || (LENGTH == UART_LINDetectLength_11b))
                                            
                                            
/**
  * @}
  */
  
  

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @addtogroup UART_Exported_Functions
  * @{
  */
extern void UART_ClockOnOff(UART_TypeDef *UARTx, CmdState OnOffState);

extern void UART_StructInit(UART_InitTypeDef *UART_InitStruct);

extern void UART_Init(UART_TypeDef *UARTx, UART_InitTypeDef *UART_InitStruct);

extern FlagStatus UART_GetFlagStatus(UART_TypeDef *UARTx, UART_FLAG_TypeDef UART_Flag);

extern void UART_ClearFlag(UART_TypeDef *UARTx, UART_FLAG_TypeDef UART_Flag);

extern void UART_INTConfig(UART_TypeDef *UARTx, UART_INT_TypeDef UART_INT, FunctionalState NewState);

extern void UART_SendData(UART_TypeDef *UARTx, uint16_t SendData);

extern uint16_t UART_ReceiveData(UART_TypeDef *UARTx);

extern void UART_LINDetectLengthConfig(UART_TypeDef *UARTx, UART_LINDetectLength_TypeDef UART_LINDetectLength);

extern void UART_Mode1_LIN_OnOff(UART_TypeDef *UARTx, FunctionalState NewState);

extern void UART_Mode1_LIN_SBK(UART_TypeDef *UARTx, FunctionalState NewState); 

extern void UART_Frame_Error_Rev(UART_TypeDef *UARTx, FunctionalState NewState);

extern void UART_TX_Polar(UART_TypeDef *UARTx, FunctionalState NewState);

extern void UART_RX_Polar(UART_TypeDef *UARTx, FunctionalState NewState);

extern FunctionalState UART_GetINTStatus(UART_TypeDef *UARTx, UART_INT_TypeDef UART_INTEnable);

extern void UART_FIFOCofig(UART_TypeDef *UARTx,UART_FIFO_Channel_TypeDef FifoChannel, FunctionalState NewState);

#ifdef __cplusplus
}
#endif

#endif /*__SH30F9XX_SB0_UART_H */


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
