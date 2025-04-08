/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_spi.h
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-5-27  
  * @brief   This file contains all the functions prototypes for the SPI firmware
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
#ifndef __SH30F9XX_SB0_SPI_H
#define __SH30F9XX_SB0_SPI_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "sh30f9xx_sb0_libcfg.h"

/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */

/** @addtogroup SPI_MODULE SPI
  * @{
  */

/** @defgroup SPI_Exported_Types SPI Exported Types
  * @{
  */

/** 
  * @brief SPI clock
  */
typedef enum
{
    SPR_PCLK1_DIV_2    = 0, /*!< PCLK1 / 2 */
    SPR_PCLK1_DIV_4    = 1, /*!< PCLK1 / 4 */
    SPR_PCLK1_DIV_8    = 2, /*!< PCLK1 / 8 */
    SPR_PCLK1_DIV_16   = 3, /*!< PCLK1 / 16 */
    SPR_PCLK1_DIV_32   = 4, /*!< PCLK1 / 32 */
    SPR_PCLK1_DIV_64   = 5, /*!< PCLK1 / 64 */
    SPR_PCLK1_DIV_128  = 6, /*!< PCLK1 / 128 */
    SPR_PCLK1_DIV_256  = 7, /*!< PCLK1 / 256 */
    SPR_PCLK1_DIV_512  = 8, /*!< PCLK1 / 512 */
    SPR_PCLK1_DIV_1024 = 9, /*!< PCLK1 / 1024 */
} SPI_SPR_TypeDef;


/** 
  * @brief SS Pin control
  * @details If in master mode disable NSS pin, no MODEF interrupt require occurs,
             and SS pin can be used as IO or other.
             If in slave mode and CPHA is 0, bypass this option bit.
   */
typedef enum
{
    PIN_SS_ENABLE  = 0, /*!< SS pin used */
    PIN_SS_DISABLE = 1, /*!< SS pin no used */
} SPI_SSDIS_TypeDef;


/** 
  * @brief CLK Pin status while in IDLE
  */
typedef enum
{
    CLK_IDLE_LOW  = 0, /*!< CLK pin is low while in IDLE */
    CLK_IDLE_HIGH = 1, /*!< CLK pin is high while in IDLE */
} SPI_CPOL_TypeDef;

/** 
  * @brief Capture edge
  */
typedef enum
{
    CAP_CLK_FIRST_EDGE  = 0, /*!< capture signal at clock's first edge */
    CAP_CLK_SECOND_EDGE = 1, /*!< capture signal at clock's second edge */
} SPI_CPHA_TypeDef;


/** 
  * @brief SPI mode
  */
typedef enum
{
    SPI_SLAVE_MODE  = 0, /*!< SPI slave mode */
    SPI_MASTER_MODE = 1, /*!< SPI master mode */
} SPI_MSTR_TypeDef;


/** 
  * @brief SPI transmit
  */
typedef enum
{
    SPI_TRANS_MSB = 0, /*!< Most Significant Bit first transmit*/
    SPI_TRANS_LSB = 1, /*!< Least Significant Bit first transmit */
} SPI_DIR_TypeDef;


/** 
  * @brief SPI transmit data length
  */    
typedef enum
{
    SPI_DATASIZE_8bit  = 0, /*!< 8 bits one transmit */
    SPI_DATASIZE_16bit = 1, /*!< 16 bits one transmit */
    SPI_DATASIZE_32bit = 2, /*!< 32 bits one transmit */
} SPI_SPDATL_TypeDef;


/** 
  * @brief SPI quick transmit mode for slave mode
  */    
typedef enum
{
    SPI_SLAVE_NORMAL = 0, /*!< SS must high after one data transmitted */
    SPI_SLAVE_FAST   = 1, /*!< no start bit after first data while continuous transmit */
} SPI_SPSFF_TypeDef;


/** 
  * @brief SPI transmit function type
  */    
typedef enum
{
    SPI_FUNC_SEND    = 1, /*!< SPI send function */
    SPI_FUNC_RECEIVE = 2, /*!< SPI receive function */
} SPI_FUNC_TypeDef;


/** 
  * @brief SPI flag
  */
typedef enum
{
    SPI_FLAG_RECEIVE_INT   = SPI_FR_SPRI_Msk, /*!< Interrupt flag for receive buffer ready */
    SPI_FLAG_SEND_INT      = SPI_FR_SPTI_Msk, /*!< Interrupt flag for send buffer empty */
    SPI_FLAG_MASTER_BUSY   = SPI_FR_BUSY_Msk, /*!< Host Busy status flag */
    SPI_FLAG_MODE_ERROR    = SPI_FR_MODF_Msk, /*!< More than one master in the transmit net */
    SPI_FLAG_RECIEVE_OVER  = SPI_FR_RXOV_Msk, /*!< Receive overflag */
    SPI_FLAG_SEND_CONFLICT = SPI_FR_WCOL_Msk  /*!< Write conflict flag */  
} SPI_FLAG_TypeDef; 


/*! 
  * @brief Structure for SPI initial
  */ 
typedef struct
{
    SPI_SPR_TypeDef    Clock         ;  /*!< SPI clock option @ref SPI_SPR_Type*/
    SPI_SSDIS_TypeDef  SSPin         ;  /*!< SS Pin in master mode: @ref SPI_SSDIS_TypeDef : PIN_SS_ENABLE or PIN_SS_DISABLE*/
    SPI_CPOL_TypeDef   ClkIdleState  ;  /*!< CLK Pin status while in IDLE: CLK_IDLE_LOW or CLK_IDLE_HIGH */
    SPI_CPHA_TypeDef   CaptureEdge   ;  /*!< capture signal time: CAP_CLK_EDGE1 or CAP_CLK_EDGE2 */
    SPI_MSTR_TypeDef   MasterOrSlave ;  /*!< SPI mode : SPI_SLAVE or SPI_MASTER */
    SPI_DIR_TypeDef    Direction     ;  /*!< SPI transmit direction type:SPI_TRANS_MSB or SPI_TRANS_LSB  */
    SPI_SPSFF_TypeDef  SlaveTransMode;  /*!< SPI transmit mode for slave mode:SPI_SLAVE_NORMAL or SPI_SLAVE_FAST */
    SPI_SPDATL_TypeDef DataSize      ;  /*!< SPI transmit data length: SPI_DATASIZE_8b or SPI_DATASIZE_16b*/
} SPI_InitTypeDef;

/**
  * @}
  */

/* Check Macro ------------------------------------------------------------*/

/** @defgroup SPI_Parameter_Check_Macro SPI Parameter Check Macro
  * @{
  */

/*! check SPI SPR type */
#define IS_SPI_SPR_Type(spr)        ((spr) <= SPR_PCLK1_DIV_1024)

/*! check SPI SPR type */
#define IS_SPI_SSDIS_Type(ss)       (((ss) == PIN_SS_ENABLE) || ((ss) == PIN_SS_DISABLE))

/*! check SPI CLK pin status in IDLE mode */
#define IS_SPI_CPOL_Type(clk)       (((clk) == CLK_IDLE_LOW) || ((clk) == CLK_IDLE_HIGH))

/*! check SPI Capture Phase type */
#define IS_SPI_CPHA_Type(clk)       (((clk) == CAP_CLK_FIRST_EDGE) || ((clk) == CAP_CLK_SECOND_EDGE))

/*! check SPI Master / Slave mode type */
#define IS_SPI_MSTR_Type(mode)      (((mode) == SPI_SLAVE_MODE) || ((mode) == SPI_MASTER_MODE))

/*! check SPI transmit direction type */
#define IS_SPI_DIR_Type(dir)        (((dir) == SPI_TRANS_MSB) || ((dir) == SPI_TRANS_LSB))

/*! check SPI transmit data length */
#define IS_SPI_SPDATL_Type(len)     (((len) == SPI_DATASIZE_8bit) \
                                         || ((len) == SPI_DATASIZE_16bit) \
                                         || ((len) == SPI_DATASIZE_32bit))
                                         
/*! check SPI slave transmit mode*/
#define IS_SPI_SPSFF_Type(mode)     (((mode) == SPI_SLAVE_NORMAL) || ((mode) == SPI_SLAVE_FAST))

/*! check SPI transmit mode*/
#define IS_SPI_FUNC_Type(func)      (((func) == SPI_FUNC_SEND) || ((func) == SPI_FUNC_RECEIVE))

/*! check if SPI Interrupt source is legal*/
#define IS_SPI_FLAG_Type(f)         (((f) == SPI_FLAG_RECEIVE_INT)   \
                                         ||((f) == SPI_FLAG_SEND_INT) \
                                         ||((f) == SPI_FLAG_MASTER_BUSY) \
                                         ||((f) == SPI_FLAG_MODE_ERROR) \
                                         ||((f) == SPI_FLAG_RECIEVE_OVER) \
                                         ||((f) == SPI_FLAG_SEND_CONFLICT))
/*! All SPI Flags */
#define SPI_FLAG_ALL                (SPI_FLAG_RECEIVE_INT   \
                                         | SPI_FLAG_SEND_INT     \
                                         | SPI_FLAG_MASTER_BUSY  \
                                         | SPI_FLAG_MODE_ERROR   \
                                         | SPI_FLAG_RECIEVE_OVER \
                                         | SPI_FLAG_SEND_CONFLICT)
/*! check if SPI Flags is legal */
#define IS_SPI_FLAG_Types(f)        ((((f)&SPI_FLAG_ALL) != 0) && (((f) & (~SPI_FLAG_ALL)) == 0))

/*! check SPI module pointer */
#define IS_SPI_MODULE(m)            (m == SPI0 || m == SPI1)

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/

/** @defgroup SPI_Exported_Functions SPI Exported Functions
  * @{
  */
  
extern void SPI_Reset(SPI_TypeDef *SPIx);

extern void SPI_StructInit(SPI_InitTypeDef *SPI_InitStruct);

extern void SPI_Init(SPI_TypeDef *SPIx, SPI_InitTypeDef *SPI_InitStruct);

extern void SPI_OnOff(SPI_TypeDef *SPIx, CmdState OnOffState);

extern void SPI_INTConfig(SPI_TypeDef *SPIx, SPI_FUNC_TypeDef SPIFunc, FunctionalState NewState);

extern FlagStatus SPI_GetINTStatus(SPI_TypeDef *SPIx, SPI_FUNC_TypeDef SPIFunc);

extern FlagStatus SPI_GetFlagStatus(SPI_TypeDef *SPIx, SPI_FLAG_TypeDef Flag);

extern void SPI_ClearFlag(SPI_TypeDef *SPIx, SPI_FLAG_TypeDef Flags);

extern void SPI_SendData(SPI_TypeDef *SPIx, uint32_t Data);

extern uint32_t SPI_ReceiveData(SPI_TypeDef *SPIx);

extern void SPI_FIFO_OnOff(SPI_TypeDef *SPIx, CmdState OnOffState);
    
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*__SH30F9XX_SB0_SPI_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT Sinowealth *****END OF FILE****/
