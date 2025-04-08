/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_fifo.c
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-5-28  
  * @brief   This file provides firmware functions to manage the FIFO module 
  *       
    @verbatim
  ==============================================================================
                         How to use this driver
  ==============================================================================
 [..]
   (#) Enable the FIFO  clock using the following function : RCC_APB1PeriphClockOnOff.

   (#) Fill FIFO FIFO_InitTypeDef type structure variable's member as default use FIFO_StructInit() function

   (#) Config fifo depth,bit width  etc ... use FIFO_Init() function

   (#) Use FIFO interrupt by following step

        (++) Config FIFO Interrupt use FIFO_INTConfig() function

        (++) Enable Kernel FIFO's Global interrupt by calling NVIC_EnableIRQ() function

        (++) reference startup asm file for FIFO interrupt function name and rewrite this function 	

   (#) When FIFO interrupt happen, use FIFO_GetFlagStatus() function to get FIFO interrupt flag
     
   (#) Call FIFO_ClearFlag() function for clear FIFO interrupt flag
    @endverbatim
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
#include "sh30f9xx_sb0_fifo.h"



/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */

/** @defgroup FIFO_MODULE FIFO
  * @brief SH30F9XX SB0 FIFO module driver library,reference this topic macro and function for drive FIFO
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/





/** @defgroup FIFO_Exported_Functions  FIFO Exported Functions
  *
  * @brief    This is SINO FIFO  exported function code group
  * @{
  */
  
  
/**
  * @brief  Fills each FIFO_InitStruct member with its default value.
  * @param  FIFO_InitStruct : pointer to a @ref FIFO_InitTypeDef
  *         structure which will be initialized.
  * @retval None
  */
void FIFO_StructInit(FIFO_InitTypeDef *FIFO_InitStruct)
{
    FIFO_InitStruct->FIFO_TxMapTo                 = FIFO_MAP_TO_UART0;
    FIFO_InitStruct->FIFO_TxTrigPoint             = FIFO_TX_TRIG_1_4_FULL;
    FIFO_InitStruct->FIFO_TxBitWidth              = FIFO_BITW_8;
    FIFO_InitStruct->FIFO_TxTrigINTStatus         = SH_DISABLE;
    FIFO_InitStruct->FIFO_RxTrigINTStatus         = SH_DISABLE;
    FIFO_InitStruct->FIFO_TxEnable                = SH_DISABLE;
    FIFO_InitStruct->FIFO_RxMapTo                 = FIFO_MAP_TO_UART0;
    FIFO_InitStruct->FIFO_RxMaxCnt                = 0xFFFFFF;
    FIFO_InitStruct->FIFO_RxTrigPoint             = FIFO_RX_TRIG_1_4_FULL;
    FIFO_InitStruct->FIFO_RxBitWidth              = FIFO_BITW_8;
    FIFO_InitStruct->FIFO_RxEnable                = SH_DISABLE;
    FIFO_InitStruct->FIFO_RxOverConflictINTStatus = SH_DISABLE;
    FIFO_InitStruct->FIFO_RxTimeoutINTStatus      = SH_DISABLE;
    FIFO_InitStruct->FIFO_RxMode                  = FIFO_RX_MODE_8_BIT;                         
}

/**
  * @brief  Initialize the FIFO peripheral according to the specified parameters
  *         in the FIFO_InitStruct.
  * @param  FIFOx: where x can be 0,1 or 2 to select the FIFO peripheral.
  * @param  FIFO_InitStruct: pointer to a @ref UART_InitTypeDef structure that contains
  *         the configuration information for the specified UART peripheral.
  * @retval None
  */
void FIFO_Init(void * FIFOx, FIFO_InitTypeDef *FIFO_InitStruct)
{
    /* Check the parameters */
    assert_param(IS_FIFO_ALL_PERIPH(FIFOx));
    
    
    if(FIFOx == (void *)FIFO0  || FIFOx == (void *)FIFO1)
    {
       //FIFO4L
        ((FIFO4L_TypeDef *)FIFOx)->CR.BIT.RXFEN    =  FIFO_InitStruct->FIFO_RxEnable;
        ((FIFO4L_TypeDef *)FIFOx)->CR.BIT.RXMAP    =  FIFO_InitStruct->FIFO_RxMapTo;
        ((FIFO4L_TypeDef *)FIFOx)->CR.BIT.RXMODE   =  FIFO_InitStruct->FIFO_RxMode;
        ((FIFO4L_TypeDef *)FIFOx)->CR.BIT.RXOVFIE  =  FIFO_InitStruct->FIFO_RxOverConflictINTStatus;
        ((FIFO4L_TypeDef *)FIFOx)->CR.BIT.RXRDBITW =  FIFO_InitStruct->FIFO_RxBitWidth;
        ((FIFO4L_TypeDef *)FIFOx)->CR.BIT.RXTMFIE  =  FIFO_InitStruct->FIFO_RxTimeoutINTStatus;
        ((FIFO4L_TypeDef *)FIFOx)->CR.BIT.RXTRIG   =  FIFO_InitStruct->FIFO_RxTrigPoint;
        ((FIFO4L_TypeDef *)FIFOx)->CR.BIT.RXTRIGIE =  FIFO_InitStruct->FIFO_RxTrigINTStatus;
        ((FIFO4L_TypeDef *)FIFOx)->CR.BIT.TXFEN    =  FIFO_InitStruct->FIFO_TxEnable;
        ((FIFO4L_TypeDef *)FIFOx)->CR.BIT.TXMAP    =  FIFO_InitStruct->FIFO_TxMapTo;
        ((FIFO4L_TypeDef *)FIFOx)->CR.BIT.TXTRIG   =  FIFO_InitStruct->FIFO_TxTrigPoint;
        ((FIFO4L_TypeDef *)FIFOx)->CR.BIT.TXTRIGIE =  FIFO_InitStruct->FIFO_TxTrigINTStatus;
        ((FIFO4L_TypeDef *)FIFOx)->CR.BIT.TXWRBITW =  FIFO_InitStruct->FIFO_TxBitWidth;
        
        
    }
    else if(FIFOx ==  (void *)FIFO2)
    {
        //FIFO8L
        ((FIFO8L_TypeDef *)FIFOx)->CR.BIT.RXFEN    =  FIFO_InitStruct->FIFO_RxEnable;
        ((FIFO8L_TypeDef *)FIFOx)->CR.BIT.RXMAP    =  FIFO_InitStruct->FIFO_RxMapTo;
        ((FIFO8L_TypeDef *)FIFOx)->CR.BIT.RXMODE   =  FIFO_InitStruct->FIFO_RxMode;
        ((FIFO8L_TypeDef *)FIFOx)->CR.BIT.RXOVFIE  =  FIFO_InitStruct->FIFO_RxOverConflictINTStatus;
        ((FIFO8L_TypeDef *)FIFOx)->CR.BIT.RXRDBITW =  FIFO_InitStruct->FIFO_RxBitWidth;
        ((FIFO8L_TypeDef *)FIFOx)->CR.BIT.RXTMFIE  =  FIFO_InitStruct->FIFO_RxTimeoutINTStatus;
        ((FIFO8L_TypeDef *)FIFOx)->CR.BIT.RXTRIG   =  FIFO_InitStruct->FIFO_RxTrigPoint;
        ((FIFO8L_TypeDef *)FIFOx)->CR.BIT.RXTRIGIE =  FIFO_InitStruct->FIFO_RxTrigINTStatus;
        ((FIFO8L_TypeDef *)FIFOx)->CR.BIT.TXFEN    =  FIFO_InitStruct->FIFO_TxEnable;
        ((FIFO8L_TypeDef *)FIFOx)->CR.BIT.TXMAP    =  FIFO_InitStruct->FIFO_TxMapTo;
        ((FIFO8L_TypeDef *)FIFOx)->CR.BIT.TXTRIG   =  FIFO_InitStruct->FIFO_TxTrigPoint;
        ((FIFO8L_TypeDef *)FIFOx)->CR.BIT.TXTRIGIE =  FIFO_InitStruct->FIFO_TxTrigINTStatus;
        ((FIFO8L_TypeDef *)FIFOx)->CR.BIT.TXWRBITW =  FIFO_InitStruct->FIFO_TxBitWidth;
    }
    
}


/**
  * @brief  Check whether the specified FIFO flag is set or not.
  * @param  FIFOx: where x can be 0,1,2 or 3 to select the FIFO peripheral.
  * @param  FIFO_Flag: specifie the flag to check,This parameter can be one of the following values:                      
  *            @arg @b FIFO_FLAG_RXTRIGF
  *            @arg @b FIFO_FLAG_TXTRIGF
  *            @arg @b FIFO_FLAG_RXOVF
  *            @arg @b FIFO_FLAG_RXTMF
  *            @arg @b FIFO_FLAG_RXEMPT
  *            @arg @b FIFO_FLAG_TXEMPT
  *            @arg @b FIFO_FLAG_RXFULL  
  *            @arg @b FIFO_FLAG_TXFULL  
  * @retval The new state of UART_Flag(SH_SET or SH_RESET).
  */
FlagStatus FIFO_GetFlagStatus(void * FIFOx, FIFO_FLAG_TypeDef FIFO_Flag)
{
    FlagStatus bitStatus;

    /* Check the parameters */
    assert_param(IS_FIFO_ALL_PERIPH(FIFOx));
    assert_param(IS_FIFO_GET_ONE_FLAG(FIFO_Flag));

    if(FIFOx == (void *)FIFO0  || FIFOx == (void *)FIFO1)
    {
        if ((((FIFO4L_TypeDef *)FIFOx)->FR.V32 & FIFO_Flag) != SH_RESET)
        {
            bitStatus = SH_SET;
        }
        else
        {
            bitStatus = SH_RESET;
        }
    }
    else if(FIFOx ==  (void *)FIFO2)
    {
        if ((((FIFO8L_TypeDef *)FIFOx)->FR.V32  & FIFO_Flag) != SH_RESET)
        {
            bitStatus = SH_SET;
        }
        else
        {
            bitStatus = SH_RESET;
        }
    }

    return bitStatus;
}




/**
  * @brief  Check whether the specified FIFO flag is set or not.
  * @param  FIFOx: where x can be 0,1,2 or 3 to select the FIFO peripheral.
  * @param  FIFO_Flag: specifie the flag to check,This parameter can be one of the following values:                      
  *            @arg @b FIFO_FLAG_RXTRIGF
  *            @arg @b FIFO_FLAG_TXTRIGF
  *            @arg @b FIFO_FLAG_RXOVF
  *            @arg @b FIFO_FLAG_RXTMF
  * @retval None
  */
void FIFO_ClearFlag(void * FIFOx, FIFO_FLAG_TypeDef FIFO_Flag)
{
    /* Check the parameters */
    assert_param(IS_FIFO_ALL_PERIPH(FIFOx));
    assert_param(IS_FIFO_GET_ONE_FLAG(FIFO_Flag));

    if(FIFOx == (void *)FIFO0  || FIFOx == (void *)FIFO1)
    {
        switch(FIFO_Flag)
        {
            case FIFO_FLAG_RXTRIGF: 
            ((FIFO4L_TypeDef *)FIFOx)->FR.BIT.RXTRIGFC = 1;
            break;
            case FIFO_FLAG_TXTRIGF: 
            ((FIFO4L_TypeDef *)FIFOx)->FR.BIT.TXTRIGFC = 1;
            break;
            case FIFO_FLAG_RXOVF: 
            ((FIFO4L_TypeDef *)FIFOx)->FR.BIT.RXOVFC = 1;
            break;
            case FIFO_FLAG_RXTMF: 
            ((FIFO4L_TypeDef *)FIFOx)->FR.BIT.RXTMFC = 1;
            break;
            default : break;
        }
    }
    else if(FIFOx ==  (void *)FIFO2)
    {
        switch(FIFO_Flag)
        {
            case FIFO_FLAG_RXTRIGF: 
            ((FIFO8L_TypeDef *)FIFOx)->FR.BIT.RXTRIGFC = 1;
            break;
            case FIFO_FLAG_TXTRIGF: 
            ((FIFO8L_TypeDef *)FIFOx)->FR.BIT.TXTRIGFC = 1;
            break;
            case FIFO_FLAG_RXOVF: 
            ((FIFO8L_TypeDef *)FIFOx)->FR.BIT.RXOVFC = 1;
            break;
            case FIFO_FLAG_RXTMF: 
            ((FIFO8L_TypeDef *)FIFOx)->FR.BIT.RXTMFC = 1;
            break;
            default : break;
        }
    }

}



/**
  * @brief  Check whether the specified FIFO flag is set or not.
  * @param  FIFOx: where x can be 0,1,2 or 3 to select the FIFO peripheral.
  * @param  FIFO_Int: specifie the interrupt to be config,This parameter can be one of the following values:                      
  *            @arg @b FIFO_INT_RXTMF 
  *            @arg @b FIFO_INT_RXOVF 
  *            @arg @b FIFO_INT_TXTRIG
  *            @arg @b FIFO_INT_RXTRIG
  * @param  NewState: new state of the FIFO's interrupt
  *          This parameter can be one of the following values:
  *            @arg @b SH_ENABLE
  *            @arg @b SH_DISABLE
  * @retval  None
  */
void FIFO_INTConfig(void * FIFOx, FIFO_INT_TypeDef FIFO_Int,FunctionalState NewState)
{
    assert_param(IS_FIFO_ALL_PERIPH(FIFOx));
    assert_param(IS_FIFO_INT_TYPE(FIFO_Int));
    assert_param(IS_FUNCTION_STATE(NewState));
    
    
    if(NewState == SH_ENABLE)
    {
        if(FIFOx == (void *)FIFO0  || FIFOx == (void *)FIFO1)
        {
            ((FIFO4L_TypeDef *)FIFOx)->CR.V32 |= FIFO_Int;
        }
        else if(FIFOx ==  (void *)FIFO2)
        {
            ((FIFO8L_TypeDef *)FIFOx)->CR.V32 |= FIFO_Int;
        }
    }
    else if(NewState == SH_DISABLE)
    {
         if(FIFOx == (void *)FIFO0  || FIFOx == (void *)FIFO1)
        {
            ((FIFO4L_TypeDef *)FIFOx)->CR.V32 &= ~FIFO_Int;
        }
        else if(FIFOx ==  (void *)FIFO2)
        {
            ((FIFO8L_TypeDef *)FIFOx)->CR.V32 &= ~FIFO_Int;
        }
    }
}



/**
  * @brief  Check whether the specified FIFO flag is set or not.
  * @param  FIFOx: where x can be 0,1,2 or 3 to select the FIFO peripheral.
  * @param  u32Timeout: specifie the timeout counter for fifo receive timeout                     
  * @retval  None
  */
void FIFO_RxTimeoutSet(void * FIFOx, uint32_t u32Timeout)
{
    if(FIFOx == (void *)FIFO0  || FIFOx == (void *)FIFO1)
    {
      ((FIFO4L_TypeDef *)FIFOx)->RXTMCNT = u32Timeout & 0x0FFFFFFF;
    }
    else if(FIFOx ==  (void *)FIFO2)
    {
      ((FIFO8L_TypeDef *)FIFOx)->RXTMCNT = u32Timeout & 0x0FFFFFFF;
    }
}


/**
  * @brief  Check whether the specified FIFO flag is set or not.
  * @param  FIFOx: where x can be 0,1,2 or 3 to select the FIFO peripheral.
  * @param  xTirgLevel: specifie the trigger point for fifo                     
  * @retval  None
  */
void FIFO_SetTxTrigLevel(void * FIFOx,FIFO_TX_Trig_TypeDef xTirgLevel)
{
    assert_param(IS_FIFO_ALL_PERIPH(FIFOx));
    assert_param(IS_FIFO_TX_TRIG_TYPE(xTirgLevel));
    
    if(FIFOx == (void *)FIFO0  || FIFOx == (void *)FIFO1)
    {
      ((FIFO4L_TypeDef *)FIFOx)->CR.BIT.TXTRIG = xTirgLevel;
    }
    else if(FIFOx ==  (void *)FIFO2)
    {
      ((FIFO8L_TypeDef *)FIFOx)->CR.BIT.TXTRIG = xTirgLevel;
    }
}



/**
  * @brief  Check whether the specified FIFO flag is set or not.
  * @param  FIFOx: where x can be 0,1,2 or 3 to select the FIFO peripheral.
  * @param  xTirgLevel: specifie the trigger point for fifo                      
  * @returns None
  */
void FIFO_SetRxTrigLevel(void * FIFOx,FIFO_RX_Trig_TypeDef xTirgLevel)
{
    assert_param(IS_FIFO_ALL_PERIPH(FIFOx));
    assert_param(IS_FIFO_RX_TRIG_TYPE(xTirgLevel));
    
    if(FIFOx == (void *)FIFO0  || FIFOx == (void *)FIFO1)
    {
        ((FIFO4L_TypeDef *)FIFOx)->CR.BIT.RXTRIG = xTirgLevel;
    }
    else if(FIFOx ==  (void *)FIFO2)
    {
        ((FIFO8L_TypeDef *)FIFOx)->CR.BIT.RXTRIG = xTirgLevel;
    }
   
}


/**
  * @brief  Check whether the specified FIFO flag is set or not.
  * @param  FIFOx: where x can be 0,1,2 or 3 to select the FIFO peripheral.
  * @param  xFifoType: specifie the fifo channel to be config
  *          This parameter can be one of the following values:
  *         @arg @b  FIFO_RX 
  *         @arg @b  FIFO_TX   
  * @param  xBitWidth: specifie the bit width for fifo channel  
  *          This parameter can be one of the following values:
  *         @arg @b  FIFO_BITW_8 
  *         @arg @b  FIFO_BITW_16  
  *         @arg @b  FIFO_BITW_32  
  * @return None
  */
void FIFO_BitWidthSet(void * FIFOx, FIFO_TypeDef xFifoType,FIFO_BitWidth_TypeDef xBitWidth)
{
    assert_param(IS_FIFO_ALL_PERIPH(FIFOx));
    assert_param(IS_FIFO_TYPE(xFifoType));
    assert_param(IS_FIFO_BIT_WIDTH(xBitWidth));
    
    if(FIFOx == (void *)FIFO0  || FIFOx == (void *)FIFO1)
    {
        if(xFifoType == FIFO_RX)
        {
            ((FIFO4L_TypeDef *)FIFOx)->CR.BIT.RXRDBITW = xBitWidth;
        }
        else if(xFifoType == FIFO_TX)
        {
            ((FIFO4L_TypeDef *)FIFOx)->CR.BIT.TXWRBITW = xBitWidth;
        }
    }
    else if(FIFOx ==  (void *)FIFO2)
    {
        if(xFifoType == FIFO_RX)
        {
            ((FIFO8L_TypeDef *)FIFOx)->CR.BIT.RXRDBITW = xBitWidth;
        }
        else if(xFifoType == FIFO_TX)
        {
            ((FIFO8L_TypeDef *)FIFOx)->CR.BIT.TXWRBITW = xBitWidth;
        }
    }
    
    return;
}


/**
  * @brief  Reset FIFO data,counter and point
  * @param  FIFOx: where x can be 0,1 or 2 to select the FIFO peripheral.
  * @param  xFifoType: specifie the fifo channel to be config
  *          This parameter can be one of the following values:
  *         @arg @b  FIFO_RX 
  *         @arg @b  FIFO_TX  
  *         structure which will be initialized.
  * @retval None
  */
void FIFO_Reset(void * FIFOx,FIFO_TypeDef xFifoType)
{
    assert_param(IS_FIFO_ALL_PERIPH(FIFOx));
    assert_param(IS_FIFO_TYPE(xFifoType));
    
    if(FIFOx == (void *)FIFO0  || FIFOx == (void *)FIFO1)
    {
        if(xFifoType == FIFO_RX)
        {
            ((FIFO4L_TypeDef *)FIFOx)->CR.BIT.RXFRST = 1;
        }
        else if(xFifoType == FIFO_TX)
        {
            ((FIFO4L_TypeDef *)FIFOx)->CR.BIT.TXFRST = 1;
        }
    }
    else if(FIFOx ==  (void *)FIFO2)
    {
        if(xFifoType == FIFO_RX)
        {
            ((FIFO8L_TypeDef *)FIFOx)->CR.BIT.RXFRST = 1;
        }
        else if(xFifoType == FIFO_TX)
        {
            ((FIFO8L_TypeDef *)FIFOx)->CR.BIT.TXFRST = 1;
        }
    }
    
}


/**
  * @brief  Check whether the specified FIFO flag is set or not.
  * @param  FIFOx: where x can be 0,1,2 or 3 to select the FIFO peripheral.
  * @param  xFifoType: specifie the fifo channel to be config
  *          This parameter can be one of the following values:
  *         @arg @b  FIFO_RX 
  *         @arg @b  FIFO_TX  
  * @param  xMapTo: specifie the fifo map peripheral
  *          This parameter can be one of the following values:
  *         @arg @b FIFO_MAP_TO_UART0 
  *         @arg @b FIFO_MAP_TO_UART1         
  *         @arg @b FIFO_MAP_TO_UART2       
  *         @arg @b FIFO_MAP_TO_SPI0  
  * @retval  None
  */
void FIFO_SetMapTo(void * FIFOx, FIFO_TypeDef xFifoType,FIFO_MapTo_TypeDef xMapTo)
{
    assert_param(IS_FIFO_ALL_PERIPH(FIFOx));
    assert_param(IS_FIFO_TYPE(xFifoType));
    assert_param(IS_FIFO_MAP_TO(xMapTo));
    
    if(FIFOx == (void *)FIFO0  || FIFOx == (void *)FIFO1)
    {
        if(xFifoType == FIFO_RX)
        {
            ((FIFO4L_TypeDef *)FIFOx)->CR.BIT.RXMAP = xMapTo;
        }
        else if(xFifoType == FIFO_TX)
        {
            ((FIFO4L_TypeDef *)FIFOx)->CR.BIT.TXMAP = xMapTo;
        }
    }
    else if(FIFOx ==  (void *)FIFO2)
    {
        if(xFifoType == FIFO_RX)
        {
            ((FIFO8L_TypeDef *)FIFOx)->CR.BIT.RXMAP = xMapTo;
        }
        else if(xFifoType == FIFO_TX)
        {
            ((FIFO8L_TypeDef *)FIFOx)->CR.BIT.TXMAP = xMapTo;
        }
    }
}



/**
  * @brief  Check whether the specified FIFO flag is set or not.
  * @param  FIFOx: where x can be 0,1,2 or 3 to select the FIFO peripheral.
  * @param  xRxMode: specifie the rx fifo mode 
  *          This parameter can be one of the following values:
  *         @arg @b FIFO_RX_MODE_8_BIT
  *         @arg @b FIFO_RX_MODE_8_N_BIT
  * @retval  None
  */
void FIFO_SetRxMode(void * FIFOx,FIFO_RX_Mode_TypeDef xRxMode)
{
    assert_param(IS_FIFO_ALL_PERIPH(FIFOx));
    assert_param(IS_FIFO_RX_MODE(xRxMode));
    
    if(FIFOx == (void *)FIFO0  || FIFOx == (void *)FIFO1)
    {
        ((FIFO4L_TypeDef *)FIFOx)->CR.BIT.RXMODE = xRxMode;
    }
    else if(FIFOx ==  (void *)FIFO2)
    {
        ((FIFO8L_TypeDef *)FIFOx)->CR.BIT.RXMODE = xRxMode;
    }
}
 


/**
  * @brief  Reset FIFO data,counter and point
  * @param  FIFOx: where x can be 0,1 or 2 to select the FIFO peripheral.
  *         structure which will be initialized.
  * @param  u32Data: specifie the data to be write to fifo
  * @retval None
  */
void FIFO_WriteData(void * FIFOx,uint32_t u32Data)
{
    assert_param(IS_FIFO_ALL_PERIPH(FIFOx));
    
    if(FIFOx == (void *)FIFO0  || FIFOx == (void *)FIFO1)
    {
       ((FIFO4L_TypeDef *)FIFOx)->TXDATA = u32Data;
    }
    else if(FIFOx ==  (void *)FIFO2)
    {
       ((FIFO8L_TypeDef *)FIFOx)->TXDATA = u32Data;
    }
}


/**
  * @brief  Reset FIFO data,counter and point
  * @param  FIFOx: where x can be 0,1 or 2 to select the FIFO peripheral.
  * @retval Read data from fifo
  */
uint32_t FIFO_ReadData(void * FIFOx)
{
    uint32_t u32ReadData  = 0x00;
    
    assert_param(IS_FIFO_ALL_PERIPH(FIFOx));
    
    if(FIFOx == (void *)FIFO0  || FIFOx == (void *)FIFO1)
    {
       u32ReadData = ((FIFO4L_TypeDef *)FIFOx)->RXDATA;
    }
    else if(FIFOx ==  (void *)FIFO2)
    {
       u32ReadData = ((FIFO8L_TypeDef *)FIFOx)->RXDATA;
    }
    
    return u32ReadData;
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
