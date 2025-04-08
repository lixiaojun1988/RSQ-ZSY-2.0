/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_fifo.h
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-05-24  
  * @brief   This file contains all the functions prototypes for the FIFO firmware
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
#ifndef __SH30F9XX_SB0_FIFO_H_
#define __SH30F9XX_SB0_FIFO_H_


#include "sh30f9xx_sb0_libcfg.h"

/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */

/** @addtogroup FIFO_MODULE
  * @{
  */

/** @defgroup FIFO_Exported_Types FIFO Exported Types 
  * @{
  */
/** 
  * @brief  FIFO Type  
  */
typedef enum 
{
    FIFO_RX    = 0, /*!< FIFO Channel RX */
    FIFO_TX    = 1, /*!< FIFO Channel TX */
} FIFO_TypeDef;
  
/** 
  * @brief  FIFO TX Trigger Point type  
  */
typedef enum 
{
    FIFO_TX_TRIG_1_4_FULL         = 0, /*!< FIFO Tx Trigger 1/4 Full*/
    FIFO_TX_TRIG_1_2_FULL         = 1, /*!< FIFO Tx Trigger 1/2 Full */
    FIFO_TX_TRIG_3_4_FULL         = 2, /*!< FIFO Tx Trigger 3/4 Full */
    FIFO_TX_TRIG_EMPTY            = 3, /*!< FIFO Tx Trigger Empty */
} FIFO_TX_Trig_TypeDef;


/** 
  * @brief  FIFO TX Trigger Point type  
  */
typedef enum 
{
    FIFO_RX_TRIG_1_4_FULL         = 0, /*!< FIFO Rx Trigger 1/4 Full */
    FIFO_RX_TRIG_1_2_FULL         = 1, /*!< FIFO Rx Trigger 1/2 Fullt */
    FIFO_RX_TRIG_3_4_FULL         = 2, /*!< FIFO Rx Trigger 3/4 Full */
    FIFO_RX_TRIG_HAVE_ONE         = 3, /*!< FIFO Rx Trigger have one */  
} FIFO_RX_Trig_TypeDef;

/** 
  * @brief  FIFO Bit Width
  */
typedef enum 
{
    FIFO_BITW_8          = 0, /*!< FIFO Bit width 8 bit */
    FIFO_BITW_16         = 1, /*!< FIFO Bit width 16 bit */
    FIFO_BITW_32         = 2, /*!< FIFO Bit width 32 bit */
} FIFO_BitWidth_TypeDef;

/** 
  * @brief  FIFO Map peripheral type
  */
typedef enum 
{
    FIFO_MAP_TO_UART0          = 0, /*!< FIFO Map to UART0  peripheral */ 
    FIFO_MAP_TO_UART1          = 1, /*!< FIFO Map to UART1  peripheral */ 
    FIFO_MAP_TO_UART2          = 2, /*!< FIFO Map to UART2  peripheral */ 
    FIFO_MAP_TO_SPI0           = 3, /*!< FIFO Map to SPI0  peripheral */ 
} FIFO_MapTo_TypeDef;


/**
  * @brief  FIFO RX mode bit type
  */
typedef enum 
{
    FIFO_RX_MODE_8_BIT          = 0, /*!< FIFO Rx Mode 8 Bit*/ 
    FIFO_RX_MODE_8_N_BIT        = 1, /*!< FIFO Rx Mode 8 + N Bit */ 
} FIFO_RX_Mode_TypeDef;



/**
  * @brief  FIFO FLAG Type
  */ 
typedef enum 
{
    FIFO_FLAG_RXTRIGF  = 1, /*!< FIFO Flag RXTRIGF */ 
    FIFO_FLAG_TXTRIGF  = 2, /*!< FIFO Flag TXTRIGF */ 
    FIFO_FLAG_RXOVF    = 3, /*!< FIFO Flag RXOVF */ 
    FIFO_FLAG_RXTMF    = 4, /*!< FIFO Flag RXTMF */ 
    FIFO_FLAG_RXEMPT   = 5, /*!< FIFO Flag RXEMPT */ 
    FIFO_FLAG_TXEMPT   = 6, /*!< FIFO Flag TXEMPT */ 
    FIFO_FLAG_RXFULL   = 7, /*!< FIFO Flag RXFULL */ 
    FIFO_FLAG_TXFULL   = 8  /*!< FIFO Flag TXFULL */ 
} FIFO_FLAG_TypeDef;

/**
  * @brief  FIFO Interrupt selection type
  */
typedef enum 
{
    FIFO_INT_RXTMF  = (uint16_t)FIFO_CR_RXTMFIE_Msk,  /*!< FIFO Interrupt RXTMF */ 
    FIFO_INT_RXOVF  = (uint16_t)FIFO_CR_RXOVFIE_Msk,  /*!< FIFO Interrupt RXOVF */ 
    FIFO_INT_TXTRIG = (uint16_t)FIFO_CR_TXTRIGIE_Msk, /*!< FIFO Interrupt TXTRIG */ 
    FIFO_INT_RXTRIG = (uint16_t)FIFO_CR_RXTRIGIE_Msk  /*!< FIFO Interrupt RXTRIG */ 
} FIFO_INT_TypeDef;

/** 
  * @brief  FIFO Init Structure definition  
  */

typedef struct
{
    FunctionalState FIFO_TxEnable;                 /*!< Specifie the FIFO Tx Channel Status,
                                                            This parameter c  an be a value of @ref FunctionalState */
    FunctionalState FIFO_RxEnable;                 /*!< Specifie the FIFO Rx Channel Status,
                                                            This parameter can be a value of @ref FunctionalState */
    FIFO_TX_Trig_TypeDef FIFO_TxTrigPoint;         /*!< Specifie the FIFO Tx Channel Trigger Point,
                                                            This parameter can be a value of @ref FIFO_TX_Trig_TypeDef */
    FIFO_RX_Trig_TypeDef FIFO_RxTrigPoint;         /*!< Specifie the FIFO Rx Channel Trigger Point,
                                                            This parameter can be a value of @ref FIFO_RX_Trig_TypeDef */
    FIFO_BitWidth_TypeDef FIFO_TxBitWidth;         /*!< Specifie the FIFO Tx Channel Bit Width,
                                                            This parameter can be a value of @ref FIFO_BitWidth_TypeDef */
    FIFO_BitWidth_TypeDef FIFO_RxBitWidth;         /*!< Specifie the FIFO Rx Channel Bit Width,
                                                            This parameter can be a value of @ref FIFO_BitWidth_TypeDef */
    FIFO_MapTo_TypeDef FIFO_TxMapTo;               /*!< Specifie the FIFO Tx Channel Map Peripheral,
                                                            This parameter can be a value of @ref FIFO_MapTo_TypeDef */
    FIFO_MapTo_TypeDef FIFO_RxMapTo;               /*!< Specifie the FIFO Rx Channel Map Peripheral,
                                                            This parameter can be a value of @ref FIFO_MapTo_TypeDef */
    FunctionalState FIFO_RxTimeoutINTStatus;       /*!< Specifie the FIFO Rx Channel Timeout Interrupt Status,
                                                            This parameter can be a value of @ref FunctionalState */
    FunctionalState FIFO_RxOverConflictINTStatus;  /*!< Specifie the FIFO Rx Channel Receive Over Conflict Interrupt Status,
                                                            This parameter can be a value of @ref FunctionalState */
    FunctionalState FIFO_TxTrigINTStatus;          /*!< Specifie the FIFO Tx Channel Trigger Interrupt Status,
                                                            This parameter can be a value of @ref FunctionalState */
    FunctionalState FIFO_RxTrigINTStatus;          /*!< Specifie the FIFO Rx Channel Trigger Interrupt Status,
                                                            This parameter can be a value of @ref FunctionalState */
    FIFO_RX_Mode_TypeDef FIFO_RxMode;              /*!< Specifie the FIFO Rx Channel Mode,
                                                            This parameter can be a value of @ref FIFO_RX_Mode_TypeDef */
    uint32_t FIFO_RxMaxCnt;                             /*!< Specifie the FIFO Rx Channel Receive Timeout Counter,
                                                            This parameter can be a value of 0x000000 - 0xFFFFFF */
} FIFO_InitTypeDef;



/**
  * @}
  */
  
  

/** @defgroup FIFO_Parameter_Check_Macro  FIFO Parameter Check Macro 
  * @{
  */
  
/**
  * @brief   Check is FIFO peripheral
  */
#define IS_FIFO_ALL_PERIPH(PERIPH)          ((PERIPH == ((void *)FIFO0))            \
                                            || (PERIPH == ((void *)FIFO1))          \
                                            || (PERIPH == ((void *)FIFO2))) 
                                                
                                            
/**
  * @brief   Check is FIFO TX trigger type
  */                         
#define IS_FIFO_TX_TRIG_TYPE(TYPE)          ((TYPE == FIFO_TX_TRIG_1_4_FULL)    \
                                            ||  (TYPE == FIFO_TX_TRIG_1_2_FULL) \
                                            ||  (TYPE == FIFO_TX_TRIG_3_4_FULL) \
                                            ||  (TYPE == FIFO_TX_TRIG_EMPTY))       

/**
  * @brief   Check is FIFO RX trigger type
  */       
#define IS_FIFO_RX_TRIG_TYPE(TYPE)          ((TYPE == FIFO_RX_TRIG_1_4_FULL)    \
                                            ||  (TYPE == FIFO_RX_TRIG_1_2_FULL) \
                                            ||  (TYPE == FIFO_RX_TRIG_3_4_FULL) \
                                            ||  (TYPE == FIFO_RX_TRIG_HAVE_ONE))                                                 
 /**
  * @brief   Check is FIFO flag
  */                                                  
#define IS_FIFO_GET_ONE_FLAG(FLAG)          ((FLAG == FIFO_FLAG_RXTRIGF)       \
                                            || (FLAG == FIFO_FLAG_TXTRIGF)     \
                                            || (FLAG == FIFO_FLAG_RXOVF)       \
                                            || (FLAG == FIFO_FLAG_RXTMF)       \
                                            || (FLAG == FIFO_FLAG_RXEMPT)      \
                                            || (FLAG == FIFO_FLAG_TXEMPT)      \
                                            || (FLAG == FIFO_FLAG_RXFULL)      \
                                            || (FLAG == FIFO_FLAG_TXFULL))     
                                            
/**
  * @brief   Check is FIFO interrupt type
  */                                                   
#define IS_FIFO_INT_TYPE(TYPE)              ((TYPE == FIFO_INT_RXTMF)          \
                                            ||  (TYPE == FIFO_INT_RXOVF)       \
                                            ||  (TYPE == FIFO_INT_TXTRIG)      \
                                            ||  (TYPE == FIFO_INT_RXTRIG))    


/**
  * @brief   Check is FIFO map peripheral
  */       
#define IS_FIFO_MAP_TO(MAP)                 ((MAP == FIFO_MAP_TO_UART0) \
                                            ||  (MAP == FIFO_MAP_TO_UART1)     \
                                            ||  (MAP == FIFO_MAP_TO_UART2)     \
                                            ||  (MAP == FIFO_MAP_TO_SPI0))   

/**
  * @brief   Check is FIFO type
  */ 
#define IS_FIFO_TYPE(TYPE)                  ((TYPE == FIFO_RX) \
                                            ||  (TYPE == FIFO_TX))     


/**
  * @brief   Check is FIFO bitwidth
  */ 
#define IS_FIFO_BIT_WIDTH(WIDTH)            ((WIDTH == FIFO_BITW_8)    \
                                            || (WIDTH == FIFO_BITW_16) \
                                            || (WIDTH == FIFO_BITW_32))
                                            
/**
  * @brief   Check is FIFO RX mode
  */                                             
#define IS_FIFO_RX_MODE(MODE)               ((MODE == FIFO_RX_MODE_8_BIT) \
                                            || (MODE == FIFO_RX_MODE_8_N_BIT))                                           


/**
  * @}
  */
  
  
/** @addtogroup FIFO_Exported_Functions
  * @{
  */
extern void FIFO_StructInit(FIFO_InitTypeDef *FIFO_InitStruct);

extern void FIFO_Init(void *FIFOx, FIFO_InitTypeDef *FIFO_InitStruct);

extern FlagStatus FIFO_GetFlagStatus(void *FIFOx, FIFO_FLAG_TypeDef FIFO_Flag);

extern void FIFO_ClearFlag(void *FIFOx, FIFO_FLAG_TypeDef FIFO_Flag);

extern void FIFO_INTConfig(void *FIFOx, FIFO_INT_TypeDef FIFO_Int, FunctionalState NewState);

extern void FIFO_RxTimeoutSet(void *FIFOx, uint32_t u32Timeout);

extern void FIFO_Reset(void *FIFOx, FIFO_TypeDef xFifoType);

extern void FIFO_WriteData(void *FIFOx, uint32_t u32Data);

extern uint32_t FIFO_ReadData(void *FIFOx);

extern void FIFO_SetTxTrigLevel(void *FIFOx, FIFO_TX_Trig_TypeDef xTirgLevel);

extern void FIFO_SetRxTrigLevel(void *FIFOx, FIFO_RX_Trig_TypeDef xTirgLevel);

extern void FIFO_SetMapTo(void *FIFOx, FIFO_TypeDef xFifoType, FIFO_MapTo_TypeDef xMapTo);

extern void FIFO_SetRxMode(void *FIFOx, FIFO_RX_Mode_TypeDef xRxMode);

extern void FIFO_BitWidthSet(void *FIFOx, FIFO_TypeDef xFifoType, FIFO_BitWidth_TypeDef xBitWidth);
/**
  * @}
  */
  
/**
  * @}
  */

/**
  * @}
  */
  
#endif /*__SH30F9XX_SB0_FIFO_H_*/

/************************ (C) COPYRIGHT Sinowealth *****END OF FILE****/
