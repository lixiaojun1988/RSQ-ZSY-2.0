/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_uart.c
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-5-21    
  * @brief   This file provides firmware functions to manage the UART module
  *         
  @verbatim
  ==============================================================================
                         How to use this driver
  ==============================================================================
     (#) Enable UART Clock by calling RCC_APB1PeriphClockOnOff() function

     (#) Fill UART UART_InitTypeDef type structure variable's member as default use UART_StructInit() function

     (#) Config UART Pin
        (++) Enable Pin Clock calling RCC_AHBPeriphClockOnOff() function 
        
        (++) Call GPIO_Init()function for config UART pin(input or output ,pull up or no-pull etc ...),if not use all member for UART 
            pin config ,makesure call GPIO_StructInit() function for GPIO_Init function  paramter set default value

     (++) Call GPIO_PinAFConfig() for config pin as UART function

     (#) Config uart mode,baud rate,stop check etc ... use UART_Init() function

     (#) Use UART interrupt by following step

        (++) Config UART Interrupt use UART_INTConfig() function
        
        (++) Enable Kernel UART's Global interrupt by calling NVIC_EnableIRQ() function
        
        (++) reference startup asm file for FIFO interrupt function name and rewrite this function 	

     (#) When timer interrupt happen, use UART_GetFlagStatus() function to get UART interrupt flag

     (#) Call UART_ClearFlag() function for clear UART interrupt flag

     (#) Call UART_SendData() for send data

     (#) Call UART_ReceiveData() for receive  data
  * @endverbatim
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
#include "sh30f9xx_sb0_uart.h"

/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */

/** @defgroup UART_MODULE UART 
  * @brief SH30F9XX SB0 UART module driver library,reference this topic macro and function for drive UART
  * @{
  */


  
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup UART_Exported_Functions  UART Exported Functions 
  *
  * @brief    This is UART  exported function code group
  * @{
  */
  
  
/**
  * @brief  Fills each UART_InitStruct member with its default value.
  * @param  UART_InitStruct : pointer to a @ref UART_InitTypeDef
  *         structure which will be initialized.
  * @retval None
  */
void UART_StructInit(UART_InitTypeDef *UART_InitStruct)
{
    UART_InitStruct->UART_Mode = UART_Mode_0;
    UART_InitStruct->UART_BaudRate = 0x00;
    UART_InitStruct->UART_StopBits = UART_StopBits_1;
    UART_InitStruct->UART_Parity = UART_SoftWare_Custom;
    UART_InitStruct->UART_Enable = 0;
}

/**
  * @brief  Initialize the UART peripheral according to the specified parameters
  *         in the UART_InitStruct.
  * @param  UARTx: where x can be 0,1,2 or 3 to select the UART peripheral.
  * @param  UART_InitStruct: pointer to a @ref UART_InitTypeDef structure that contains
  *         the configuration information for the specified UART peripheral.
  * @retval None
  */
void UART_Init(UART_TypeDef *UARTx, UART_InitTypeDef *UART_InitStruct)
{
    uint32_t temp = 0x00;
    uint32_t baudRate = 0;
    RCC_Clocks_TypeDef RCC_ClocksStatus;

    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_MODE(UART_InitStruct->UART_Mode));

    /* Set UART mode */
    UARTx->CR.V32 &= ~UART_CR_SM_Msk;
    UARTx->CR.V32 |= UART_InitStruct->UART_Mode;

    /* UART baud rate configuration */
    if ((UART_InitStruct->UART_Mode == UART_Mode_1) || (UART_InitStruct->UART_Mode == UART_Mode_3))
    {
        RCC_GetClocksFreq(&RCC_ClocksStatus);
        baudRate = RCC_ClocksStatus.pclk1Freq / 16 / UART_InitStruct->UART_BaudRate - 1;
        temp = RCC_ClocksStatus.pclk1Freq / UART_InitStruct->UART_BaudRate - 16 * (baudRate + 1);
        baudRate += temp << 16;
    }

    if (UART_InitStruct->UART_Mode == UART_Mode_0)
    {
        assert_param(IS_UART_MODE_0_BAUDRATE(UART_InitStruct->UART_BaudRate));

        /* Set the UART baud rate */
        UARTx->CR.V32 &= ~UART_CR_SMOD0_Msk;
        UARTx->CR.V32 |= ((UART_InitStruct->UART_BaudRate >> 23) & UART_CR_SMOD0_Msk);
    }
    else if (UART_InitStruct->UART_Mode == UART_Mode_1)
    {
        assert_param(IS_UART_MODE_13_BAUDRATE(UART_InitStruct->UART_BaudRate));
        assert_param(IS_UART_MODE_123_STOP_BITS(UART_InitStruct->UART_StopBits));

        UARTx->BRT.V32 = baudRate;
        UARTx->CR.V32 |= UART_CR_SBRTEN_Msk;

        UARTx->CR.V32 &= ~UART_CR_STOP_Msk;
        UARTx->CR.V32 |= UART_InitStruct->UART_StopBits - 1;
    }
    else if (UART_InitStruct->UART_Mode == UART_Mode_2)
    {
        assert_param(IS_UART_MODE_2_BAUDRATE(UART_InitStruct->UART_BaudRate));
        assert_param(IS_UART_MODE_123_STOP_BITS(UART_InitStruct->UART_StopBits));
        assert_param(IS_UART_MODE_23_PARITY(UART_InitStruct->UART_Parity));

        UARTx->CR.V32 &= ~UART_CR_SMOD2_Msk;
        UARTx->CR.V32 |= ((UART_InitStruct->UART_BaudRate >> 27) & UART_CR_SMOD2_Msk);

        UARTx->CR.V32 &= ~UART_CR_STOP_Msk;
        UARTx->CR.V32 |= UART_InitStruct->UART_StopBits - 1;

        /* Set the Parity bit */
        UARTx->CR.V32 &= (~(UART_CR_MULTIE_Msk | UART_CR_PS_Msk));
        UARTx->CR.V32 |= UART_InitStruct->UART_Parity;
    }
    else
    {
        assert_param(IS_UART_MODE_13_BAUDRATE(UART_InitStruct->UART_BaudRate));
        assert_param(IS_UART_MODE_123_STOP_BITS(UART_InitStruct->UART_StopBits));
        assert_param(IS_UART_MODE_23_PARITY(UART_InitStruct->UART_Parity));

        UARTx->BRT.V32 = baudRate;

        UARTx->CR.V32 |= UART_CR_SBRTEN_Msk;

        UARTx->CR.V32 &= ~UART_CR_STOP_Msk;
        UARTx->CR.V32 |= UART_InitStruct->UART_StopBits - 1;

        /* Set the Parity bit */
        UARTx->CR.V32 &= (~(UART_CR_MULTIE_Msk | UART_CR_PS_Msk));
        UARTx->CR.V32 |= UART_InitStruct->UART_Parity;
    }

    UARTx->CR.V32 &= ~(UART_CR_TEN_Msk | UART_CR_REN_Msk);
    /* Enable UART transmit or receive */
    UARTx->CR.V32 |= UART_InitStruct->UART_Enable;
    
    
}

/**
  * @brief  Check whether the specified UART interrupt is enable or disnable.
  * @param  UARTx: where x can be 0,1,2 or 3 to select the UART peripheral.
  * @param  UART_INTEnable: specifie the interrupt enable bit.
  *          This parameter can be one of the following values:
  *            @arg @b UART_INT_RX 
  *            @arg @b UART_INT_TX 
  *            @arg @b UART_INT_TC 
  *            @arg @b UART_INT_LBD
  * @retval The new state of UART enable(enable or disnable).
  */
FunctionalState UART_GetINTStatus(UART_TypeDef *UARTx, UART_INT_TypeDef UART_INTEnable)
{
    FunctionalState bitStatus;

    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_INT(UART_INTEnable));

    if ((UARTx->CR.V32 & UART_INTEnable) != SH_RESET)
    {
        bitStatus = SH_ENABLE;
    }
    else
    {
        bitStatus = SH_DISABLE;
    }
    return bitStatus;
}

/**
  * @brief  Check whether the specified UART flag is set or not.
  * @param  UARTx: where x can be 0,1,2 or 3 to select the UART peripheral.
  * @param  UART_Flag: specifie the flag to check.
  *          This parameter can be one of the following values:
  *            @arg @b UART_FLAG_RI
  *            @arg @b UART_FLAG_TI
  *            @arg @b UART_FLAG_TC
  *            @arg @b UART_FLAG_TXCOL
  *            @arg @b UART_FLAG_RXOV
  *            @arg @b UART_FLAG_FE
  *            @arg @b UART_FLAG_PE
  *            @arg @b UART_FLAG_LBD  
  * @retval The new state of UART_Flag(SH_SET or SH_RESET).
  */
FlagStatus UART_GetFlagStatus(UART_TypeDef *UARTx, UART_FLAG_TypeDef UART_Flag)
{
    FlagStatus bitStatus;

    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_GET_ONE_FLAG(UART_Flag));

    if ((UARTx->FR.V32 & UART_Flag) != SH_RESET)
    {
        bitStatus = SH_SET;
    }
    else
    {
        bitStatus = SH_RESET;
    }
    return bitStatus;
}

/**
  * @brief  Clear the UART's pending flags.
  * @param  UARTx: where x can be 0,1,2 or 3 to select the UART peripheral.
  * @param  UART_Flag: Specifie the flag to clear.
  *         This parameter can be any combination of the following values:
  *            @arg @b UART_FLAG_TC
  *            @arg @b UART_FLAG_TXCOL
  *            @arg @b UART_FLAG_RXOV
  *            @arg @b UART_FLAG_FE
  *            @arg @b UART_FLAG_PE
  *            @arg @b UART_FLAG_LBD
  * @note RI Flag can only clear by read RDR register and TI Flag can only clear by write TDR register
  * @retval None
  */
void UART_ClearFlag(UART_TypeDef *UARTx, UART_FLAG_TypeDef UART_Flag)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_FLAG(UART_Flag));

    UARTx->FR.V32 |= ((uint32_t)UART_Flag << 16);
}

/**
  * @brief  Enable or disable the specified UART's interrupts.
  * @param  UARTx: where x can be 0,1,2 or 3 to select the UART peripheral.
  * @param  UART_INT: specifie the UART interrupt sources to be enabled or disabled.
  *          This parameter can be any combination of the following values:
  *            @arg @b UART_INT_RX
  *            @arg @b UART_INT_TX
  *            @arg @b UART_INT_TC
  *            @arg @b UART_INT_LBD  
  * @param  NewState: New state of UART interrupt.
  *          This parameter can be one of the following values:
  *            @arg @b SH_ENABLE
  *            @arg @b SH_DISABLE
  * @retval None
  */
void UART_INTConfig(UART_TypeDef *UARTx, UART_INT_TypeDef UART_INT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_INT(UART_INT));
    assert_param(IS_FUNCTION_STATE(NewState));

    if (NewState != SH_DISABLE)
    {
        UARTx->CR.V32 |= UART_INT;
    }
    else
    {
        UARTx->CR.V32 &= ~UART_INT;
    }
}


/**
  * @brief  Transmit single data through the UARTx peripheral.
  * @param  UARTx: where x can be 0,1,2 or 3 to select the UART peripheral.
  * @param  SendData: the data to transmit.
  * @retval None
  */
void UART_SendData(UART_TypeDef *UARTx, uint16_t SendData)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));

    /* Transmit data */
    UARTx->TDR.V32 = SendData;
}

/**
  * @brief  Return the most recent received data by the UARTx peripheral.
  * @param  UARTx: where x can be 0,1,2 or 3 to select the UART peripheral.
  * @retval The received data.
  */
uint16_t UART_ReceiveData(UART_TypeDef *UARTx)
{
    uint16_t tempData = 0;
    uint16_t tempReg = 0;

    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));

    tempReg = (uint16_t)(UARTx->CR.V32 & UART_CR_SM_Msk);

    if ((tempReg == UART_Mode_0) || (tempReg == UART_Mode_1))
    {
        tempData = UARTx->RDR.BIT.RDR;
    }
    else
    {
        tempData = UARTx->RDR.V32;
    }
    return tempData;
}

/**
  * @brief  Synchronization interval detection threshold length.
  * @param  UARTx: where x can be 1, 2, 3 to select the UART peripheral.
  * @param  UART_LINDetectLength: specifie the LIN break detect length.
  *          This parameter can be one of the following values:
  *            @arg @b UART_LINDetectLength_10b
  *            @arg @b UART_LINDetectLength_11b
  * @retval None
  */
void UART_LINDetectLengthConfig(UART_TypeDef *UARTx, UART_LINDetectLength_TypeDef UART_LINDetectLength)
{
    uint32_t temp = 0;

    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_LIN_DETECT_LENGTH(UART_LINDetectLength));

    temp = UARTx->CR.V32;
    temp &= ~UART_CR_LBDL_Msk;
    temp |= UART_LINDetectLength;
    UARTx->CR.V32 = temp;
}

/**
  * @brief  Enable or Disable the UART's LIN mode.
  * @param  UARTx: where x can be 1, 2, 3 to select the UART peripheral.
  * @param  NewState: status.
  *          This parameter can be one of the following values:
  *            @arg @b SH_DISABLE
  *            @arg @b SH_ENABLE 
  * @retval None
  */
void UART_Mode1_LIN_OnOff(UART_TypeDef *UARTx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_FUNCTION_STATE(NewState));

    UARTx->CR.BIT.LINEN = NewState;
}

/**
  * @brief  LIN synchronization interval enable bit.
  * @param  UARTx: where x can be 1, 2, 3 to select the UART peripheral.
  * @param  NewState: status.
  *          This parameter can be one of the following values:
  *            @arg @b SH_DISABLE
  *            @arg @b SH_ENABLE 
  * @retval None
  */
void UART_Mode1_LIN_SBK(UART_TypeDef *UARTx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_FUNCTION_STATE(NewState));

    UARTx->CR.BIT.SBK = NewState;
}

/**
  * @brief  Enable or disable the UART's frame error receive.
  * @param  UARTx: where x can be 0,1,2 or 3 to select the UART peripheral.
  * @param  NewState: new state of the frame error.
  *          This parameter can be one of the following values:
  *            @arg @b SH_ENABLE
  *            @arg @b SH_DISABLE
  * @retval None
  */
void UART_Frame_Error_Rev(UART_TypeDef *UARTx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_FUNCTION_STATE(NewState));

    UARTx->CR.BIT.FER = NewState;
}

/**
  * @brief  Enable or disable the UART's send polar.
  * @param  UARTx: where x can be 0, 1,2 or 3 to select the UART peripheral.
  * @param  NewState: new state of the frame error.
  *          This parameter can be one of the following values:
  *            @arg @b SH_ENABLE
  *            @arg @b SH_DISABLE
  * @retval None
  */
void UART_TX_Polar(UART_TypeDef *UARTx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_FUNCTION_STATE(NewState));

    UARTx->CR.BIT.TNEG = NewState;
}

/**
  * @brief  Enable or disable the UART's send polar.
  * @param  UARTx: where x can be 0, 1,2 or 3 to select the UART peripheral.
  * @param  NewState: new state of the frame error.
  *          This parameter can be one of the following values:
  *            @arg @b SH_ENABLE
  *            @arg @b SH_DISABLE
  * @retval None
  */
void UART_RX_Polar(UART_TypeDef *UARTx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_FUNCTION_STATE(NewState));

    UARTx->CR.BIT.RNEG = NewState;
}

/**
  * @brief  Turn on or off the UART clock.
  * @param  UARTx: where x can be 0, 1,2 or 3 to select the UART peripheral.
  * @param  OnOffState: new state of the frame error.
  *          This parameter can be one of the following values:
  *            @arg @b SH_ON
  *            @arg @b SH_OFF
  * @retval None
  */
void UART_ClockOnOff(UART_TypeDef *UARTx, CmdState OnOffState)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    if (OnOffState == SH_ON)
    {
        if (UARTx == UART0)
        {
            RCC_APB1PeriphClockOnOff(RCC_APB1_UART0, SH_ON);
        }
        else if (UARTx == UART1)
        {
            RCC_APB1PeriphClockOnOff(RCC_APB1_UART1, SH_ON);
        }
        else if (UARTx == UART2)
        {
            RCC_APB1PeriphClockOnOff(RCC_APB1_UART2, SH_ON);
        }
        else if (UARTx == UART3)
        {
            RCC_APB1PeriphClockOnOff(RCC_APB1_UART3, SH_ON);
        }
        else if (UARTx == UART4)
        {
            RCC_APB1PeriphClockOnOff(RCC_APB1_UART4, SH_ON);
        }
        else
        {
            RCC_APB1PeriphClockOnOff(RCC_APB1_UART5, SH_ON);
        }
    }
    else
    {
        if (UARTx == UART0)
        {
            RCC_APB1PeriphClockOnOff(RCC_APB1_UART0, SH_OFF);
        }
        else if (UARTx == UART1)
        {
            RCC_APB1PeriphClockOnOff(RCC_APB1_UART1, SH_OFF);
        }
        else if (UARTx == UART2)
        {
            RCC_APB1PeriphClockOnOff(RCC_APB1_UART2, SH_OFF);
        }
        else if (UARTx == UART3)
        {
            RCC_APB1PeriphClockOnOff(RCC_APB1_UART3, SH_OFF);
        }
        else if (UARTx == UART4)
        {
            RCC_APB1PeriphClockOnOff(RCC_APB1_UART4, SH_OFF);
        }
        else
        {
            RCC_APB1PeriphClockOnOff(RCC_APB1_UART5, SH_OFF);
        }
    }
}



/**
  * @brief  Turn on or off the UART clock.
  * @param  UARTx: where x can be 0, 1,2 or 3 to select the UART peripheral.
  * @param  FifoChannel: specifies witch channel will be config
  *         This parameter can be one of the following values:
  *            @arg @b UART_FIFO_CH_TX
  *            @arg @b UART_FIFO_CH_RX
  * @param  NewState: new state of the fifo channel
  *            @arg @b SH_ON
  *            @arg @b SH_OFF
  * @retval None
  */
void UART_FIFOCofig(UART_TypeDef *UARTx,UART_FIFO_Channel_TypeDef FifoChannel, FunctionalState NewState)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_FIFO_CHANNEL(FifoChannel));
    assert_param(IS_FUNCTION_STATE(NewState));
    
    if(FifoChannel == UART_FIFO_CH_TX)
    {
        UARTx->CR.BIT.UATXFIFO =  (uint8_t)NewState;
    }
    else if(FifoChannel == UART_FIFO_CH_RX)
    {
        UARTx->CR.BIT.UARXFIFO =  (uint8_t)NewState;
    }
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
