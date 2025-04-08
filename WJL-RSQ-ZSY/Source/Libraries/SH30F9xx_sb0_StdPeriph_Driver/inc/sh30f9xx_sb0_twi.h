/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_twi.h
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-5-27  
  * @brief   This file contains all the functions prototypes for the TWI firmware
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
#ifndef __SH30F9XX_SB0_TWI_H
#define __SH30F9XX_SB0_TWI_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "sh30f9xx_sb0_libcfg.h"

/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */

/** @addtogroup TWI_MODULE TWI
  * @{
  */

/** @defgroup TWI_Exported_Types TWI Exported Types
  * @{
  */
    
/**
  *@brief TWI frequncy parameter cr
  */     
typedef enum
{
    TWI_CR_DIV64 = 0, /*!< PCLK1 / 64 */
    TWI_CR_DIV16 = 1, /*!< PCLK1 / 16 */
    TWI_CR_DIV4  = 2, /*!< PCLK1 / 4 */
    TWI_CR_DIV1  = 3, /*!< PCLK1 / 1 */
} TWI_CR_TypeDef;


/**
  *@brief TWI bus timeout
  */     
typedef enum
{
    TWI_CNT_25000   = 0, /*!< 25000 counter for bus timeout */
    TWI_CNT_50000   = 1, /*!< 50000 counter for bus timeout */
    TWI_CNT_100000  = 2, /*!< 100000 counter for bus timeout */
    TWI_CNT_200000  = 3, /*!< 200000 counter for bus timeout */
} TWI_CNT_TypeDef;

/**
  *@brief TWI Response 
  */
typedef enum
{
    TWI_AA_NACK = 0, /*!< Response NACK */
    TWI_AA_ACK  = 1,  /*!< Response ACK */
} TWI_AA_TypeDef;


/**
  *@brief TWI mode 
  */
typedef enum
{
    TWI_MASTER        = 0,  /*!< TWI as master */
    TWI_SLAVE         = 1,  /*!< TWI as slave */
    TWI_MASTER_SLAVE  = 3,  /*!< TWI as master and slave */
} TWI_MODE_TypeDef;


/**
  *@brief TWI transmit status
  */
typedef enum
{
    TWI_MASTER_START_SEND                = 0x08,  /*!< 0x08: TWI Master Send/Receive Mode: Start condition has sent */
    TWI_MASTER_RESTART_SEND              = 0x10,  /*!< 0x10: TWI Master Send/Receive Mode: Restart condition has sent*/
    TWI_MASTER_SLA_W_ACK                 = 0x18,  /*!< 0x18: TWI Master Send Mode: SLA+W sent; ACK received*/
    TWI_MASTER_SLA_W_NACK                = 0x20,  /*!< 0x20: TWI Master Send Mode: SLA+W sent; NACK received*/
    TWI_MASTER_DATA_SEND_ACK             = 0x28,  /*!< 0x28: TWI Master Send Mode: TWI_DR sent; ACK received*/
    TWI_MASTER_DATA_SEND_NACK            = 0x30,  /*!< 0x30: TWI Master Send Mode: TWI_DR sent; NACK received*/
    TWI_MASTER_LOST_ARBITRATION          = 0x38,  /*!< 0x38: TWI Master Send/receive Mode: Lost arbitration bit while SLA+W/R or data transmit*/
    TWI_MASTER_SLA_R_ACK                 = 0x40,  /*!< 0x40: TWI Master Receive Mode: SLA+R sent; ACK received*/
    TWI_MASTER_SLA_R_NACK                = 0x48,  /*!< 0x48: TWI Master Receive Mode: SLA+R sent; NACK received*/
    TWI_MASTER_DATA_RECEIVE_ACK          = 0x50,  /*!< 0x50: TWI Master Receive Mode: SLA+R received; ACK sent*/
    TWI_MASTER_DATA_RECEIVE_NACK         = 0x58,  /*!< 0x58: TWI Master Receive Mode: SLA+R received; NACK sent*/
    TWI_SLAVE_SLA_R_ACK                  = 0xA8,  /*!< 0xA8: TWI Slave Send Mode: SLA+R self received; ACK sent*/
    TWI_SLAVE_LOST_ARBITRATION           = 0xB0,  /*!< 0xB0: TWI Slave Send Mode: Lost arbitration bit;SLA+R received; ACK sent*/
    TWI_SLAVE_DATA_SEND_ACK              = 0xB8,  /*!< 0xB8: TWI Slave Send Mode: TWI_DR sent; ACK received*/
    TWI_SLAVE_DATA_SEND_NACK             = 0xC0,  /*!< 0xC0: TWI Slave Send Mode: TWI_DR sent; NACK received*/
    TWI_SLAVE_LAST_SEND_ACK              = 0xC8,  /*!< 0xC8: TWI Slave Send Mode: Last TWI_DR SENT; ACK received*/
    TWI_SLAVE_SLA_W_ACK                  = 0x60,  /*!< 0x60: TWI Slave Receive Mode:SLA+W self received; ACK sent */
    TWI_SLAVE_LOST_SLA_W_ACK             = 0x68,  /*!< 0x68: TWI Slave Receive Mode:Lost arbitration while SLA+R/W sending; SLA+W received; ACK sent */
    TWI_SLAVE_ADDR_RECEIVE_ACK           = 0x70,  /*!< 0x70: TWI Slave Receive Mode:General address received; ACK sent */
    TWI_SLAVE_LOST_ADDR_ACK              = 0x78,  /*!< 0x78: TWI Slave Receive Mode:Lost arbitration while SLA+R/W sending; addr received; ACK sent  */
    TWI_SLAVE_ADDR_DATA_RECEIVE_ACK      = 0x80,  /*!< 0x80: TWI Slave Receive Mode: In address valid mode; data received; ACK sent*/ 
    TWI_SLAVE_ADDR_DATA_RECEIVE_NACK     = 0x88,  /*!< 0x88: TWI Slave Receive Mode: In address valid mode; data received; NACK sent*/ 
    TWI_SLAVE_GADDR_DATA_RECEIVE_ACK     = 0x90,  /*!< 0x90: TWI Slave Receive Mode: In general address valid mode,data received; ACK sent*/ 
    TWI_SLAVE_GADDR_DATA_RECEIVE_NACK    = 0x98,  /*!< 0x98: TWI Slave Receive Mode: In general address valid mode,data received; NACK sent*/ 
    TWI_SLAVE_STOP_RESTART_RECEIVE       = 0xA0,  /*!< 0xA0: TWI Slave Receive Mode: Stop condition or Restart condition received*/
    TWI_NO_VALID_EVENT                   = 0xF8,  /*!< 0xF8: TWI other Mode: no valid event code found. TWINT=0*/
    TWI_INVALID_EVENT                    = 0x0,   /*!< 0x00: TWI other Mode: Invalid start or stop sent; internal logic error*/
} TWI_EVENT_TypeDef;

/**
  *@brief TWI transmit direction type
  */
typedef enum
{
    TWI_DIR_TRANSMITTER = 0,  /*!< Transmitter mode */
    TWI_DIR_RECEIVER    = 1,  /*!< Receiver mode */
} TWI_DIR_TypeDef;


/**
  *@brief TWI transmit direction type
  */
typedef enum
{
    TWI_CMD_START     = 0,  /*!< Send START Condition */
    TWI_CMD_ACK       = 1,  /*!< Response ACK */
    TWI_CMD_NACK      = 2,  /*!< Response NACK */
    TWI_CMD_STOPSTART = 3,  /*!< Send STOP condition then send START condition */    
} TWI_COMMAND_TypeDef;

/**
  *@brief TWI Flags
  */
typedef enum
{
    TWI_FLAG_INT            = TWI_FR_TWINT_Msk, /*!< TWI Interrupt Flag */
    TWI_FLAG_TFREE_TIMEOUT  = TWI_FR_TFREE_Msk, /*!< SCL high level timeout flag */
    TWI_FLAG_TOUT_TIMEOUT   = TWI_FR_TOUT_Msk,  /*!< Bus timeout flag */
} TWI_FLAG_TypeDef;


/*! 
  * @brief Structure for TWI initial
  */ 
typedef struct
{   
    FunctionalState      GeneralAddress         ;  /*!< General Address: SH_ENABLE or SH_DISABLE */
    TWI_CR_TypeDef       PresClock              ;  /*!< Clock prescale: @ref TWI_CR_TypeDef */
    TWI_CNT_TypeDef      BusTimeOut             ;  /*!< Bus timeout count */
    FunctionalState      HighLevelTimeOutEnable ;  /*!< High level timeout enable bit */
    FunctionalState      BusTimeOutEnable       ;  /*!< Bus timeout enable bit */
    uint8_t                   TWIAddress             ;  /*!< 7 bits address*/
    uint8_t                   MaskAddress            ;  /*!< TWI address mask bit 0:Must match 1:Ignore*/
    uint8_t                   BaudRate               ;  /*!< Baudrate */
    uint8_t                   HighLevelTimeOut       ;  /*!< High level timeout count*/    
} TWI_InitTypeDef;

/**
  * @}
  */ 

/* Check Macro ------------------------------------------------------------*/

/** @defgroup TWI_Parameter_Check_Macro TWI Parameter Check_Macro
  * @{
  */

/*! Check TWI Clock Rate Control Type */
#define IS_TWI_CR_Type(cr)             (((cr) == TWI_CR_DIV64)   \
                                            || ((cr) == TWI_CR_DIV16) \
                                            || ((cr) == TWI_CR_DIV4)  \
                                            || ((cr) == TWI_CR_DIV1))

/*! Check TWI Clock Rate Control Type */
#define IS_TWI_CNT_Type(cnt)           (((cnt) == TWI_CNT_25000)   \
                                            ||((cnt) == TWI_CNT_50000)  \
                                            ||((cnt) == TWI_CNT_100000) \
                                            ||((cnt) == TWI_CNT_200000))

/*! Check TWI Response Type */
#define IS_TWI_AA_Type(aa)             (((aa) == TWI_AA_NACK) || ((aa) == TWI_AA_ACK))

/*! Check TWI Response Type */
#define IS_TWI_MODE_Type(type)         (((type) == TWI_MASTER)  \
                                            || ((type) == TWI_SLAVE) \
                                            || ((type) == TWI_MASTER_SLAVE))

/*! check TWI direction type */
#define IS_TWI_DIR_Type(dir)           (((dir) == TWI_DIR_TRANSMITTER) || ((dir) == TWI_DIR_RECEIVER))

/*! check TWI command type */
#define IS_TWI_COMMAND_Type(cmd)       (((cmd) == TWI_CMD_START) || ((cmd) == TWI_CMD_ACK) \
                                            || ((cmd) == TWI_CMD_NACK) || ((cmd) == TWI_CMD_STOPSTART))
/*! check TWI Flag */
#define IS_TWI_FLAG_Type(t)            (((t) == TWI_FLAG_INT)             \
                                            || ((t) == TWI_FLAG_TFREE_TIMEOUT) \
                                            || ((t) == TWI_FLAG_TOUT_TIMEOUT))
/*! All TWI Flags */
#define TWI_FLAG_ALL                   (TWI_FLAG_INT|TWI_FLAG_TFREE_TIMEOUT|TWI_FLAG_TOUT_TIMEOUT)
/*! check TWI Flag */
#define IS_TWI_FLAG_Types(f)           ((((f) & TWI_FLAG_ALL) != 0) && (((f) & (~TWI_FLAG_ALL)) == 0))

/*! check TWI module pointer */
#define IS_TWI_MODULE(m)               (m == TWI0 || m == TWI1)

/*Waiting timeout*/
#define TWIT_FLAG_TIMEOUT                   ((uint32_t)0x1000UL)
#define TWIT_LONG_TIMEOUT                   ((uint32_t)(10 * TWIT_FLAG_TIMEOUT))
/**
  * @}
  */ 
/* Exported functions --------------------------------------------------------*/

/** @defgroup TWI_Exported_Functions TWI Exported Functions
  * @{
  */
extern void TWI_Reset(TWI_TypeDef *TWIx);

extern void TWI_Init(TWI_TypeDef *TWIx, const TWI_InitTypeDef *InitCfg);

extern void TWI_StructInit(TWI_InitTypeDef *TWI_InitStruct);

extern void TWI_OnOff(TWI_TypeDef *TWIx, CmdState OnOffState);

extern void TWI_AddrModeConfig(TWI_TypeDef *TWIx, FunctionalState TWIState);

extern void TWI_INTConfig(TWI_TypeDef *TWIx, FunctionalState NewState);

extern FlagStatus TWI_GetINTStatus(TWI_TypeDef *TWIx);

extern void TWI_ClearINTStatus(TWI_TypeDef *TWIx);

extern FlagStatus TWI_GetFlagStatus(TWI_TypeDef *TWIx, TWI_FLAG_TypeDef Flag);

extern void TWI_ClearFlag(TWI_TypeDef *TWIx, TWI_FLAG_TypeDef Flags);

extern void TWI_SendData(TWI_TypeDef *TWIx, uint8_t Data);

extern uint8_t TWI_ReceiveData(TWI_TypeDef *TWIx);

extern TWI_EVENT_TypeDef TWI_GetEvent(TWI_TypeDef *TWIx);

extern void TWI_START(TWI_TypeDef *TWIx, FunctionalState NewState);

extern void TWI_STOP(TWI_TypeDef *TWIx, FunctionalState NewState);

extern void TWI_ACK_Config(TWI_TypeDef *TWIx, TWI_AA_TypeDef NewAck);

extern void TWI_Send7bitAddress(TWI_TypeDef *TWIx, uint8_t Address, TWI_DIR_TypeDef TWI_Direction);

extern void TWI_OwnAddressConfig(TWI_TypeDef *TWIx, uint8_t Address, uint8_t AddressMsk, FunctionalState NewState);

extern TWI_EVENT_TypeDef TWI_SendCommand(TWI_TypeDef *TWIx, TWI_COMMAND_TypeDef CmdType);

extern TWI_EVENT_TypeDef TWI_SendWaitDone(TWI_TypeDef *TWIx);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*__SH30F9XX_SB0_TWI_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT Sinowealth *****END OF FILE****/
