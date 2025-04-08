/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_flash.c
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-5-21    
  * @brief   This file provides firmware functions to manage the FLASH module
  *
    @verbatim
  ==============================================================================
                         How to use this driver
  ==============================================================================
 [..]
   (#) User call can set flash latency use  FLASH_SetLatency() function 

   (#) User can On/Off  flash data and instruction prefetch use FLASH_PrefetchOnOff function or reset prefetch cache use FLASH_CacheReset()
         function

   (#) Use can erase main flash sector :
       (++) Unlock main flash use FLASH_Main_Unlock function
       (++) call FLASH_Main_EraseSector function to erase sector witch user code want to erase or 
            call FLASH_Main_EraseAllSectors for all main flash sector 

       (++) lock main flash use FLASH_Main_Lock function

   (#) User can program flash by:
       (++) Unlock main flash use FLASH_Main_Unlock function
       (++) call FLASH_Main_ProgramWord function to program a word size data into main flash
       (++) lock main flash use FLASH_Main_Lock function
       
   (#) User can use following code for read flash context:(whitch FLASH_READ_ADDR is flash address for read)
       (++) (*((volatile uint8_t *)(FLASH_READ_ADDR)))  8 bit read 
       (++) (*((volatile uint16_t *)(FLASH_READ_ADDR)))  16 bit read 
       (++) (*((volatile uint32_t *)(FLASH_READ_ADDR)))  32 bit read 


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

/* Includes ------------------------------------------------------------------*/
#include "sh30f9xx_sb0_flash.h"

/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */

/** @addtogroup FLASH_MODULE FLASH
  * @brief SH30F9XX SB0 FLASH module driver library,reference this topic macro and function for drive FLASH
  * @{
  */

/** @defgroup FLASH_Exported_Functions  FLASH Exported Functions 
  * 
  * @brief    This is FLASH  exported function code group
  * @{
  */
  
/**
  * @brief  Sets the code latency value.
  * @param  FLASH_Latency: specifies the FLASH Latency value.
  *         This parameter can be one of the following values:
  *            @arg @b FLASH_Latency_0: FLASH Zero Latency cycle
  *            @arg @b FLASH_Latency_1: FLASH One Latency cycle
  *            @arg @b FLASH_Latency_2: FLASH Two Latency cycles
  *            @arg @b FLASH_Latency_3: FLASH Three Latency cycles
  *            @arg @b FLASH_Latency_4: FLASH Four Latency cycles
  *            @arg @b FLASH_Latency_5: FLASH Five Latency cycles
  *            @arg @b FLASH_Latency_6: FLASH Six Latency cycles
  *            @arg @b FLASH_Latency_7: FLASH Seven Latency cycles  
* @retval None
*/
void FLASH_SetLatency(uint32_t FLASH_Latency)
{
    uint32_t tmpreg = 0;

    /* Check the parameters */
    assert_param(IS_FLASH_LATENCY(FLASH_Latency));

    /* Clear FLASH_Latency */
    tmpreg = FLASH->ACR.V32 & (~FLASH_ACR_LATENCY_Msk);
    /* Set the Latency cycles according to the new value */
    tmpreg |= ((0x5AA5 << FLASH_ACR_LOCK_Pos) | (FLASH_Latency << FLASH_ACR_LATENCY_Pos));
    /* Store the new value */
    FLASH->ACR.V32 = tmpreg;
}

/**
  * @brief  Enables or disables the Prefetch feature(Data Prefetch or Instruction Prefetch).
  * @param  PrefetchType: specifies the FLASH Prefetch Type.
  *            @arg @b FLASH_Prefetch_Data: FLASH Data Pefetch Buf
  *            @arg @b FLASH_Prefetch_Ins: FLASH Instruction Pefetch Buf
  * @param  NewState: new state of the Prefetch.
  *          This parameter can be: SH_ON or SH_OFF.
  * @retval None
  */
void FLASH_PrefetchOnOff(Flash_Prefetch_TypeDef PrefetchType,CmdState NewState)
{
    uint32_t tmpreg = 0;

    //Check the parameters
    assert_param(IS_FLASH_PREFETCH(PrefetchType));
    assert_param(IS_CMD_STATE(NewState));


    if(PrefetchType == FLASH_Prefetch_Data)
    {
        //Clear DBEN bit
        tmpreg = FLASH->ACR.V32 & (~FLASH_ACR_DBEN_Msk);
        //Set DBEN bit according to the New State value
        tmpreg |= ((0x5AA5 << FLASH_ACR_LOCK_Pos) | (NewState << FLASH_ACR_DBEN_Pos));
        //Store the new value
        FLASH->ACR.V32 = tmpreg;
    }
    else if(PrefetchType == FLASH_Prefetch_Ins)
    {
        //Clear IBEN bit
        tmpreg = FLASH->ACR.V32 & (~FLASH_ACR_IBEN_Msk);
        //Set IBEN bit according to the New State value
        tmpreg |= ((0x5AA5 << FLASH_ACR_LOCK_Pos) | (NewState << FLASH_ACR_IBEN_Pos));
        //Store the new value
        FLASH->ACR.V32 = tmpreg; 
    }

}

/**
  * @brief  Resets the Instruction Cache and the Data Cache.
  * @retval None
  */
void FLASH_CacheReset(void)
{
    uint32_t tmpreg = FLASH->ACR.V32;
    /* Set CRST bit to Reset Cache  */
    FLASH->ACR.V32 |= ((0x5AA5 << FLASH_ACR_LOCK_Pos) | (SH_SET << FLASH_ACR_CRST_Pos));
    FLASH->ACR.V32 = (tmpreg | (0x5AA5 << FLASH_ACR_LOCK_Pos));
}

/**
  * @brief  Sets the programming window timer values.
  * @param  CounterInitValue: specifies the watchdog counter initial value.
  * @param  UpperCounterValue: specifies the upper limit value.
  * @retval None
  */
void FLASH_Set_PGMWindow(uint32_t CounterInitValue, uint32_t UpperCounterValue)
{
    /* Sets the initial value of counter */
    FLASH->CNTR = CounterInitValue;

    /* Sets the upper limit value of counter */
    FLASH->UPCNTR = UpperCounterValue;

    /* Enable the downcounter */
    FLASH->CNTCR = SH_SET;
}

/**
  * @brief  Clears the programming window timer values.
  * @retval None
  */
void FLASH_Clear_PGMWindow(void)
{
    /* Clears the window counters */
    FLASH->CNTR = 0x0;
    FLASH->UPCNTR = 0x0;
    FLASH->CNTCR = SH_RESET;
}

/**
  * @brief  Calculates the programming window counter value according the window time.
  * @param  us100Delay: the time of programming window.
  * @retval CounterValue: The counter value of the window time
  */
uint32_t FLASH_Calculate_WinCounterVal(uint32_t us100Delay)
{
    uint32_t CounterValue = 0;
    RCC_Clocks_TypeDef curClk;

    /* Get current system's clock frequency */
    RCC_GetClocksFreq(&curClk);

    /* Calculates the window counter value according the window time */
    CounterValue = curClk.sysFreq / 10000 * us100Delay;

    return CounterValue;
}

/**
  * @brief  Locks the FLASH control register access of main block.
  * @retval None
  */
void FLASH_Main_Lock(void)
{
    /* Set the LOCK Bit to lock the FLASH Registers access */
    FLASH->CR.BIT.MNLCK = SH_SET;
}

/**
  * @brief  Unlocks the FLASH control register access of main block.
  * @param  OperationType: The single operation unlock or the multiple operations unlock
  *         This parameter can be one of the following values:
  *          @arg @b SINGLE_OPERATION: The single operation unlock
  *          @arg @b MULTI_OPERATION: The multiple operations unlock
  * @retval None
  */
void FLASH_Main_Unlock(uint8_t OperationType)
{
    if (FLASH->CR.BIT.E2LCK == SH_RESET)
    {
        FLASH->CR.BIT.E2LCK = SH_SET;
    }
    if (FLASH->CR.BIT.INFLCK == SH_RESET)
    {
        FLASH->CR.BIT.INFLCK = SH_SET;
    }
    if (FLASH->CR.BIT.MNLCK != SH_RESET)
    {
        /* Authorize the FLASH Registers access */
        FLASH->MKYR = FLASH_MAIN_KEY;

        /* The single unlock key unlocks the flash interface for one write or erase operation. */
        if (OperationType == SINGLE_OPERATION)
            FLASH->MKYR = FLASH_SINGLE_OP_KEY;

        /* The multiple unlock key unlocks the flash interface for write or 
                                              erase operations until the block is locked. */
        else if (OperationType == MULTI_OPERATION)
            FLASH->MKYR = FLASH_MULTI_OP_KEY;
    }
}

/**
  * @brief  Locks the FLASH Info. block control register access
  * @retval None
*/
void FLASH_Info_Lock(void)
{
    /* Set the INFLCK Bit to lock the FLASH Registers access */
    FLASH->CR.BIT.INFLCK = SH_SET;
}

/**
  * @brief  Unlocks the FLASH Info. block control register access
  * @param  OperationType: The single operation unlock or the multiple operations unlock
  *         This parameter can be one of the following values:
  *            @arg @b SINGLE_OPERATION: The single operation unlock
  *            @arg @b MULTI_OPERATION: The multiple operations unlock
  * @retval None
*/
void FLASH_Info_Unlock(uint8_t OperationType)
{
    if (FLASH->CR.BIT.MNLCK == SH_RESET)
        FLASH->CR.BIT.MNLCK = SH_SET;

    if (FLASH->CR.BIT.E2LCK == SH_RESET)
        FLASH->CR.BIT.E2LCK = SH_SET;

    if (FLASH->CR.BIT.INFLCK != SH_RESET)
    {
        /* Authorize the FLASH Registers access */
        FLASH->IKYR = FLASH_INFO_KEY;

        /* The single unlock key unlocks the flash interface for one write or erase operation. */
        if (OperationType == SINGLE_OPERATION)
            FLASH->IKYR = FLASH_SINGLE_OP_KEY;

        /* The multiple unlock key unlocks the flash interface for write or  
        erase operations until the block is locked. */
        else if (OperationType == MULTI_OPERATION)
            FLASH->IKYR = FLASH_MULTI_OP_KEY;
    }
}



/**
  * @brief  Checks whether the specified FLASH flag is set or not.
  * @param  FLASH_FLAG: specifies the FLASH flag to check.
  *         This parameter can be one of the following values:
  *            @arg @b FLASH_FLAG_EOP: FLASH End of Operation flag 
  *            @arg @b FLASH_FLAG_OPERR: FLASH operation Error flag 
  *            @arg @b FLASH_FLAG_FLSERR: FLASH hardware Error flag 
  *            @arg @b FLASH_FLAG_WRPERR: FLASH Write protected error flag 
  *            @arg @b FLASH_FLAG_PGPERR: FLASH Programming one time error flag
  *            @arg @b FLASH_FLAG_PGWERR: FLASH Programming Window counter error flag
  *            @arg @b FLASH_FLAG_STAERR: FLASH Programming State error flag
  *            @arg @b FLASH_FLAG_BSY: FLASH Busy flag
  * @retval The new state of FLASH_FLAG (SH_SET or SH_RESET).
*/
FlagStatus FLASH_GetFlagStatus(uint32_t FLASH_FLAG)
{
    FlagStatus bitstatus = SH_RESET;
    /* Check the parameters */
    assert_param(IS_FLASH_GET_FLAG(FLASH_FLAG));

    if ((FLASH->SR.V32 & FLASH_FLAG) != (uint32_t)SH_RESET)
    {
        bitstatus = SH_SET;
    }
    else
    {
        bitstatus = SH_RESET;
    }
    /* Return the new state of FLASH_FLAG (SH_SET or SH_RESET) */
    return bitstatus;
}
/**
* @brief  Clears the FLASH's pending flags.
* @param  FLASH_FLAG: specifies the FLASH flags to clear.
*          This parameter can be any combination of the following values:
*            @arg @b FLASH_FLAG_EOP: FLASH End of Operation flag 
*            @arg @b FLASH_FLAG_OPERR: FLASH operation Error flag 
*            @arg @b FLASH_FLAG_FLSERR: FLASH hardware Error flag 
*            @arg @b FLASH_FLAG_WRPERR: FLASH Write protected error flag 
*            @arg @b FLASH_FLAG_PGPERR: FLASH Programming one time error flag
*            @arg @b FLASH_FLAG_PGWERR: FLASH Programming Window counter error flag
*            @arg @b FLASH_FLAG_STAERR: FLASH Programming State error flag
* @retval None
*/
void FLASH_ClearFlag(uint32_t FLASH_FLAG)
{
    /* Check the parameters */
    assert_param(IS_FLASH_CLEAR_FLAG(FLASH_FLAG));

    /* Clear the flags */
    FLASH->SR.V32 |= (FLASH_FLAG << 16);
}

/**
* @brief  Returns the FLASH Status.
* @retval FLASH_Status: The returned value can be: 
*            FLASH_BUSY,FLASH_ERROR_STA,FLASH_ERROR_PGW,FLASH_ERROR_PGP,FLASH_ERROR_WRP,
*            FLASH_ERROR_FLS,FLASH_ERROR_OPERATION,FLASH_ERROR_ERASE,FLASH_ERROR_PROGRAM,
*            FLASH_ERROR_BLANK,FLASH_ERROR_VERIFY,FLASH_COMPLETE          
*/
FLASH_Status FLASH_GetStatus(void)
{
    FLASH_Status flashstatus = FLASH_COMPLETE;

    if ((FLASH->SR.V32 & FLASH_FLAG_BSY) == FLASH_FLAG_BSY)
    {
        flashstatus = FLASH_BUSY;
    }
    else
    {
        if ((FLASH->SR.V32 & FLASH_FLAG_WRPERR) != (uint32_t)0x00)
        {
            flashstatus = FLASH_ERROR_WRP;
        }
        else
        {
            if ((FLASH->SR.V32 & FLASH_FLAG_FLSERR) != (uint32_t)0x00)
            {
                flashstatus = FLASH_ERROR_FLS;
            }
            else
            {
                if ((FLASH->SR.V32 & FLASH_FLAG_PGPERR) != (uint32_t)0x00)
                {
                    flashstatus = FLASH_ERROR_PGP;
                }
                else
                {
                    if ((FLASH->SR.V32 & FLASH_FLAG_PGWERR) != (uint32_t)0x00)
                    {
                        flashstatus = FLASH_ERROR_PGW;
                    }
                    else
                    {
                        if ((FLASH->SR.V32 & FLASH_FLAG_STAERR) != (uint32_t)0x00)
                        {
                            flashstatus = FLASH_ERROR_STA;
                        }
                        else
                        {
                            if ((FLASH->SR.V32 & FLASH_FLAG_OPERR) != (uint32_t)0x00)
                            {
                                flashstatus = FLASH_ERROR_OPERATION;
                            }
                            else
                            {
                                flashstatus = FLASH_COMPLETE;
                                FLASH_ClearFlag(FLASH_FLAG_EOP);
                            }
                        }
                    }
                }
            }
        }
    }
    /* Return the FLASH Status */
    return flashstatus;
}

/**
* @brief  Waits for a FLASH operation to complete.
* @retval FLASH_Status: The returned value can be: 
*         FLASH_BUSY,FLASH_ERROR_STA,FLASH_ERROR_PGW,FLASH_ERROR_PGP,FLASH_ERROR_WRP,
*         FLASH_ERROR_FLS,FLASH_ERROR_OPERATION,FLASH_ERROR_ERASE,FLASH_ERROR_PROGRAM,
*         FLASH_ERROR_BLANK,FLASH_ERROR_VERIFY,FLASH_COMPLETE  
*/
FLASH_Status FLASH_WaitForLastOperation(void)
{
    __IO FLASH_Status status = FLASH_COMPLETE;

    /* Check for the FLASH Status */
    status = FLASH_GetStatus();

    /* Wait for the FLASH operation to complete by polling on BUSY flag to be reset.
    Even if the FLASH operation fails, the BUSY flag will be reset and an error
    flag will be set */
    while (status == FLASH_BUSY)
    {
        status = FLASH_GetStatus();
    }
    /* Return the operation status */
    return status;
}

/**
* @brief  Erases a specified FLASH Sector.
*   
* @param  FLASH_Sector: The Sector number to be erased.
*         This parameter can be a value between 0 and 127
*    
* @retval FLASH_Status: The returned value can be: 
*           FLASH_BUSY,FLASH_ERROR_STA,FLASH_ERROR_PGW,FLASH_ERROR_PGP,FLASH_ERROR_WRP,
*           FLASH_ERROR_FLS,FLASH_ERROR_OPERATION,FLASH_ERROR_ERASE,FLASH_ERROR_PROGRAM,
*           FLASH_ERROR_BLANK,FLASH_ERROR_VERIFY,FLASH_COMPLETE  
*/
FLASH_Status FLASH_Main_EraseSector(uint32_t FLASH_Sector)
{
    FLASH_Status status = FLASH_COMPLETE;

    /* Check the parameters */
    assert_param(IS_FLASH_SECTOR(FLASH_Sector));

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation();

    if (status == FLASH_COMPLETE)
    {
        /* Sets the programming window timer values */
        FLASH_Set_PGMWindow(SE_ERS_TIME, SE_ERS_TIME);

        /* if the previous operation is completed, proceed to erase the sector */
        FLASH->CR1 = FLASH_Sector;
        FLASH->CR.V32 = (FLASH_CR_CMD_MSE << FLASH_CR_CMD_Pos) | (SH_SET << FLASH_CR_STRT_Pos);

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation();
        if (status == FLASH_COMPLETE)
        {
            /* if the erase operation is completed, disable the operation command */
            FLASH->CR.V32 = 0;
        }

        /* Clears the programming window timer values */
        FLASH_Clear_PGMWindow();
    }
    /* Cache reset */
    FLASH_CacheReset();
    /* Return the Erase Status */
    return status;
}

/**
* @brief  Erase All Flash Secotrs
* @retval FLASH_Status: The returned value can be: 
*           FLASH_BUSY,FLASH_ERROR_STA,FLASH_ERROR_PGW,FLASH_ERROR_PGP,FLASH_ERROR_WRP,
*           FLASH_ERROR_FLS,FLASH_ERROR_OPERATION,FLASH_ERROR_ERASE,FLASH_ERROR_PROGRAM,
*           FLASH_ERROR_BLANK,FLASH_ERROR_VERIFY,FLASH_COMPLETE  
*/
FLASH_Status FLASH_Main_EraseAllSectors(void)
{
    FLASH_Status status = FLASH_COMPLETE;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation();

    if (status == FLASH_COMPLETE)
    {
        /* Sets the programming window timer values */
        FLASH_Set_PGMWindow(SE_ERS_TIME, SE_ERS_TIME);
 
        /* if the previous operation is completed, proceed to erase all sectors */
        FLASH->CR.V32 = (FLASH_CR_CMD_ME0 << FLASH_CR_CMD_Pos) | (SH_SET << FLASH_CR_STRT_Pos);

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation();

        /* if the erase operation is completed, disable the operation command */
        FLASH->CR.V32 = 0;
        /* Clears the programming window timer values */
        FLASH_Clear_PGMWindow();
    }
    /* Cache reset */
    FLASH_CacheReset();
    /* Return the Erase Status */
    return status;
}

/**
* @brief  Programs a word (32-bit) at a specified address of the main block.
* @param  Address: specifies the address to be programmed.
*         This parameter can be any address in Program memory zone.  
* @param  Data: specifies the data to be programmed.
* @retval FLASH_Status: The returned value can be: 
*                FLASH_BUSY,FLASH_ERROR_STA,FLASH_ERROR_PGW,FLASH_ERROR_PGP,FLASH_ERROR_WRP,
*                FLASH_ERROR_FLS,FLASH_ERROR_OPERATION,FLASH_ERROR_ERASE,FLASH_ERROR_PROGRAM,
*                FLASH_ERROR_BLANK,FLASH_ERROR_VERIFY,FLASH_COMPLETE  
*/
FLASH_Status FLASH_Main_ProgramWord(uint32_t Address, uint32_t Data)
{
    FLASH_Status status = FLASH_COMPLETE;

    uint32_t tmpreg = FLASH->ACR.V32;

    FLASH->ACR.V32 = ((0x5AA5 << FLASH_ACR_LOCK_Pos) | (tmpreg & FLASH_ACR_LATENCY_Msk));

    /* Check the parameters */
    assert_param(IS_FLASH_MAIN_BLOCK_ADDRESS(Address));

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation();

    if (status == FLASH_COMPLETE)
    {
        /* Sets the programming window timer values */
        FLASH_Set_PGMWindow(SE_ERS_TIME, SE_ERS_TIME);
 
        /* if the previous operation is completed, proceed to program the new data */
        FLASH->CR.V32 = ((FLASH_CR_PSIZE_WD << FLASH_CR_PSIZE_Pos) \
                        | (FLASH_CR_CMD_MPG << FLASH_CR_CMD_Pos)   \
                        | (SH_SET << FLASH_CR_STRT_Pos));

        *(__IO uint32_t *)Address = Data;

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation();

        /* if the program operation is completed, disable the operation command */
        FLASH->CR.V32 = 0;
 
        /* Clears the programming window timer values */
        FLASH_Clear_PGMWindow();
    }
    tmpreg = ((0x5AA5 << FLASH_ACR_LOCK_Pos) | tmpreg);
 
    FLASH->ACR.V32 = tmpreg;
    
    /* Cache reset */
    FLASH_CacheReset();
 
    /* Return the Program Status */
    return status;
}

/**
* @brief  Programs a half word (16-bit) at a specified address of the main block.           
* @param  Address: specifies the address to be programmed.
*         This parameter can be any address in Program memory zone.  
* @param  Data: specifies the data to be programmed.
* @retval FLASH_Status: The returned value can be: 
*              FLASH_BUSY,FLASH_ERROR_STA,FLASH_ERROR_PGW,FLASH_ERROR_PGP,FLASH_ERROR_WRP,
*              FLASH_ERROR_FLS,FLASH_ERROR_OPERATION,FLASH_ERROR_ERASE,FLASH_ERROR_PROGRAM,
*              FLASH_ERROR_BLANK,FLASH_ERROR_VERIFY,FLASH_COMPLETE  
*/
FLASH_Status FLASH_Main_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
    FLASH_Status status = FLASH_COMPLETE;

    uint32_t tmpreg = FLASH->ACR.V32;

    FLASH->ACR.V32 = ((0x5AA5 << FLASH_ACR_LOCK_Pos) | (tmpreg & FLASH_ACR_LATENCY_Msk));

    /* Check the parameters */
    assert_param(IS_FLASH_MAIN_BLOCK_ADDRESS(Address));

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation();

    if (status == FLASH_COMPLETE)
    {
        /* Sets the programming window timer values */
        FLASH_Set_PGMWindow(SE_ERS_TIME, SE_ERS_TIME);
 
        /* if the previous operation is completed, proceed to program the new data */
        FLASH->CR.V32 = ((FLASH_CR_PSIZE_HALFWD << FLASH_CR_PSIZE_Pos) \
                        | (FLASH_CR_CMD_MPG << FLASH_CR_CMD_Pos) \
                        | (SH_SET << FLASH_CR_STRT_Pos));

        *(__IO uint16_t *)Address = Data;

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation();

        /* if the program operation is completed, disable the operation command */
        FLASH->CR.V32 = 0;
 
        /* Clears the programming window timer values */
        FLASH_Clear_PGMWindow();
    }
    tmpreg = ((0x5AA5 << FLASH_ACR_LOCK_Pos) | tmpreg);
 
    FLASH->ACR.V32 = tmpreg;
 
    /* Cache reset */
    FLASH_CacheReset();
 
    /* Return the Program Status */
    return status;
}

/**
  * @brief  Erases the Sector of Customer block.
  * @retval FLASH_Status: The returned value can be: 
  *                  FLASH_BUSY,FLASH_ERROR_STA,FLASH_ERROR_PGW,FLASH_ERROR_PGP,FLASH_ERROR_WRP,
  *                  FLASH_ERROR_FLS,FLASH_ERROR_OPERATION,FLASH_ERROR_ERASE,FLASH_ERROR_PROGRAM,
  *                  FLASH_ERROR_BLANK,FLASH_ERROR_VERIFY,FLASH_COMPLETE  
*/

FLASH_Status FLASH_CB_EraseSector(void)
{
    FLASH_Status status = FLASH_COMPLETE;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation();

    if (status == FLASH_COMPLETE)
    {
        /* Sets the programming window timer values */
        FLASH_Set_PGMWindow(SE_ERS_TIME, SE_ERS_TIME);
 
        /* if the previous operation is completed, proceed to erase the sector */
        FLASH->CR.V32 = ((FLASH_CR_CMD_CBSE << FLASH_CR_CMD_Pos) \
                        | (SH_SET << FLASH_CR_STRT_Pos));

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation();

        /* if the erase operation is completed, disable the operation command */
        FLASH->CR.V32 = 0;

        /* Clears the programming window timer values */
        FLASH_Clear_PGMWindow();
    }
    /* Cache reset */
    FLASH_CacheReset();
    /* Return the Erase Status */
    return status;
}

/**
* @brief  Programs a word (32-bit) at a specified address of the Customer block.
* @param  Address: specifies the address to be programmed.
*         This parameter can be any address in Program memory zone.  
* @param  Data: specifies the data to be programmed.
* @retval FLASH_Status: The returned value can be: 
*                     FLASH_BUSY,FLASH_ERROR_STA,FLASH_ERROR_PGW,FLASH_ERROR_PGP,FLASH_ERROR_WRP,
*                     FLASH_ERROR_FLS,FLASH_ERROR_OPERATION,FLASH_ERROR_ERASE,FLASH_ERROR_PROGRAM,
*                     FLASH_ERROR_BLANK,FLASH_ERROR_VERIFY,FLASH_COMPLETE  
*/

FLASH_Status FLASH_CB_ProgramWord(uint32_t Address, uint32_t Data)
{
    FLASH_Status status = FLASH_COMPLETE;

    uint32_t tmpreg = FLASH->ACR.V32;

    FLASH->ACR.V32 = ((0x5AA5 << FLASH_ACR_LOCK_Pos) | (tmpreg & FLASH_ACR_LATENCY_Msk));

    /* Check the parameters */
    assert_param(IS_CUSTOMER_BLOCK_ADDRESS(Address));

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation();

    if (status == FLASH_COMPLETE)
    {
        /* Sets the programming window timer values */
        FLASH_Set_PGMWindow(SE_ERS_TIME, SE_ERS_TIME);

        /* if the previous operation is completed, proceed to program the new data */
        FLASH->CR.V32 = ((FLASH_CR_PSIZE_WD << FLASH_CR_PSIZE_Pos) \
                        | (FLASH_CR_CMD_CBPG << FLASH_CR_CMD_Pos)  \
                        | (SH_SET << FLASH_CR_STRT_Pos));

        *(__IO uint32_t *)Address = Data;

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation();

        /* if the program operation is completed, disable the operation command  */
        FLASH->CR.V32 = 0;
 
        /* Clears the programming window timer values */
        FLASH_Clear_PGMWindow();
    }
    tmpreg = ((0x5AA5 << FLASH_ACR_LOCK_Pos) | tmpreg);

    FLASH->ACR.V32 = tmpreg;

    /* Cache reset */
    FLASH_CacheReset();
 
    /* Return the Program Status */
    return status;
}

/**
* @brief  Programs a half word (16-bit) at a specified address of the Customer block.              
* @param  Address: specifies the address to be programmed.
*         This parameter can be any address in Program memory zone.  
* @param  Data: specifies the data to be programmed.
* @retval FLASH_Status: The returned value can be: 
*                 FLASH_BUSY,FLASH_ERROR_STA,FLASH_ERROR_PGW,FLASH_ERROR_PGP,FLASH_ERROR_WRP,
*                 FLASH_ERROR_FLS,FLASH_ERROR_OPERATION,FLASH_ERROR_ERASE,FLASH_ERROR_PROGRAM,
*                 FLASH_ERROR_BLANK,FLASH_ERROR_VERIFY,FLASH_COMPLETE  
*/

FLASH_Status FLASH_CB_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
    FLASH_Status status = FLASH_COMPLETE;

    uint32_t tmpreg = FLASH->ACR.V32;

    FLASH->ACR.V32 = ((0x5AA5 << FLASH_ACR_LOCK_Pos) | (tmpreg & FLASH_ACR_LATENCY_Msk));

    /* Check the parameters */
    assert_param(IS_CUSTOMER_BLOCK_ADDRESS(Address));

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation();

    if (status == FLASH_COMPLETE)
    {
        /* Sets the programming window timer values */
        FLASH_Set_PGMWindow(SE_ERS_TIME, SE_ERS_TIME);
 
        /* if the previous operation is completed, proceed to program the new data */
        FLASH->CR.V32 = ((FLASH_CR_PSIZE_HALFWD << FLASH_CR_PSIZE_Pos) \
                        | (FLASH_CR_CMD_CBPG << FLASH_CR_CMD_Pos) \
                        | (SH_SET << FLASH_CR_STRT_Pos));

        *(__IO uint16_t *)Address = Data;

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation();

        /* if the program operation is completed, disable the operation command */
        FLASH->CR.V32 = 0;
 
        /* Clears the programming window timer values */
        FLASH_Clear_PGMWindow();
    }
    tmpreg = ((0x5AA5 << FLASH_ACR_LOCK_Pos) | tmpreg);

    FLASH->ACR.V32 = tmpreg;

    /* Cache reset */
    FLASH_CacheReset();
 
    /* Return the Program Status */
    return status;
}

/**
* @brief  Programs a word (32-bit) at a specified address of the OTP block.
* @param  Address: specifies the address to be programmed.
*         This parameter can be any address in Program memory zone.  
* @param  Data: specifies the data to be programmed.
* @retval FLASH_Status: The returned value can be: 
*               FLASH_BUSY,FLASH_ERROR_STA,FLASH_ERROR_PGW,FLASH_ERROR_PGP,FLASH_ERROR_WRP,
*               FLASH_ERROR_FLS,FLASH_ERROR_OPERATION,FLASH_ERROR_ERASE,FLASH_ERROR_PROGRAM,
*               FLASH_ERROR_BLANK,FLASH_ERROR_VERIFY,FLASH_COMPLETE  
*/
FLASH_Status FLASH_OTP_ProgramWord(uint32_t Address, uint32_t Data)
{
    FLASH_Status status = FLASH_COMPLETE;

    uint32_t tmpreg = FLASH->ACR.V32;

    FLASH->ACR.V32 = ((0x5AA5 << FLASH_ACR_LOCK_Pos) | (tmpreg & FLASH_ACR_LATENCY_Msk));

    /* Check the parameters */
    assert_param(IS_OTP_BLOCK_ADDRESS(Address));

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation();

    if (status == FLASH_COMPLETE)
    {
        /* Sets the programming window timer values */
        FLASH_Set_PGMWindow(SE_ERS_TIME, SE_ERS_TIME);

        /* if the previous operation is completed, proceed to program the new data */
        FLASH->CR.V32 = ((FLASH_CR_PSIZE_WD << FLASH_CR_PSIZE_Pos) \
                        | (FLASH_CR_CMD_OPG << FLASH_CR_CMD_Pos) \
                        | (SH_SET << FLASH_CR_STRT_Pos));

        *(__IO uint32_t *)Address = Data;

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation();

        /* if the program operation is completed, disable the operation command */
        FLASH->CR.V32 = 0;

        /* Clears the programming window timer values */
        FLASH_Clear_PGMWindow();
    }
    tmpreg = ((0x5AA5 << FLASH_ACR_LOCK_Pos) | tmpreg);

    FLASH->ACR.V32 = tmpreg;

    /* Cache reset */
    FLASH_CacheReset();
    /* Return the Program Status */
    return status;
}

/**
* @brief  Programs a half word (16-bit) at a specified address of the OTP block(0x0FFFF800 - 0x0FFFFFFF).             
* @param  Address: specifies the address to be programmed.
*         This parameter can be any address in Program memory zone.  
* @param  Data: specifies the data to be programmed.
* @retval FLASH_Status: The returned value can be: 
*                FLASH_BUSY,FLASH_ERROR_STA,FLASH_ERROR_PGW,FLASH_ERROR_PGP,FLASH_ERROR_WRP,
*                FLASH_ERROR_FLS,FLASH_ERROR_OPERATION,FLASH_ERROR_ERASE,FLASH_ERROR_PROGRAM,
*                FLASH_ERROR_BLANK,FLASH_ERROR_VERIFY,FLASH_COMPLETE  
*/
FLASH_Status FLASH_OTP_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
    FLASH_Status status = FLASH_COMPLETE;

    uint32_t tmpreg = FLASH->ACR.V32;

    FLASH->ACR.V32 = ((0x5AA5 << FLASH_ACR_LOCK_Pos) | (tmpreg & FLASH_ACR_LATENCY_Msk));

    /* Check the parameters */
    assert_param(IS_OTP_BLOCK_ADDRESS(Address));

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation();

    if (status == FLASH_COMPLETE)
    {
        /* Sets the programming window timer values */
        FLASH_Set_PGMWindow(SE_ERS_TIME, SE_ERS_TIME);

        /* if the previous operation is completed, proceed to program the new data */
        FLASH->CR.V32 = ((FLASH_CR_PSIZE_HALFWD << FLASH_CR_PSIZE_Pos) \
                        | (FLASH_CR_CMD_OPG << FLASH_CR_CMD_Pos) \
                        | (SH_SET << FLASH_CR_STRT_Pos));

        *(__IO uint16_t *)Address = Data;

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation();

        /* if the program operation is completed, disable the operation command */
        FLASH->CR.V32 = 0;

        /* Clears the programming window timer values */
        FLASH_Clear_PGMWindow();
    }
    tmpreg = ((0x5AA5 << FLASH_ACR_LOCK_Pos) | tmpreg);

    FLASH->ACR.V32 = tmpreg;
    /* Cache reset */
    FLASH_CacheReset();
    /* Return the Program Status */
    return status;
}

/**
* @brief  Programs a word (32-bit) at a specified address of the Protect block.
* @param  Address: specifies the address to be programmed.
*         This parameter can be any address in Program memory zone.  
* @param  Data: specifies the data to be programmed.
* @retval FLASH_Status: The returned value can be: 
*              FLASH_BUSY,FLASH_ERROR_STA,FLASH_ERROR_PGW,FLASH_ERROR_PGP,FLASH_ERROR_WRP,
*              FLASH_ERROR_FLS,FLASH_ERROR_OPERATION,FLASH_ERROR_ERASE,FLASH_ERROR_PROGRAM,
*              FLASH_ERROR_BLANK,FLASH_ERROR_VERIFY,FLASH_COMPLETE  
*/
FLASH_Status FLASH_PRTB_ProgramWord(uint32_t Address, uint32_t Data)
{
    FLASH_Status status = FLASH_COMPLETE;

    uint32_t tmpreg = FLASH->ACR.V32;

    FLASH->ACR.V32 = ((0x5AA5 << FLASH_ACR_LOCK_Pos) | (tmpreg & FLASH_ACR_LATENCY_Msk));

    /* Check the parameters */
    assert_param(IS_PROTECT_BLOCK_ADDRESS(Address));

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation();

    if (status == FLASH_COMPLETE)
    {
        /* Sets the programming window timer values */
        FLASH_Set_PGMWindow(SE_ERS_TIME, SE_ERS_TIME);

        /* if the previous operation is completed, proceed to program the new data */
        FLASH->CR.V32 = ((FLASH_CR_PSIZE_WD << FLASH_CR_PSIZE_Pos) \
                        | (FLASH_CR_CMD_SPG << FLASH_CR_CMD_Pos) \
                        | (SH_SET << FLASH_CR_STRT_Pos));

        *(__IO uint32_t *)Address = Data;

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation();

        /* if the program operation is completed, disable the operation command */
        FLASH->CR.V32 = 0;

        /* Clears the programming window timer values */
        FLASH_Clear_PGMWindow();
    }
    tmpreg = ((0x5AA5 << FLASH_ACR_LOCK_Pos) | tmpreg);

    FLASH->ACR.V32 = tmpreg;
    /* Cache reset */
    FLASH_CacheReset();
    /* Return the Program Status */
    return status;
}

/**
* @brief  Programs a half word (16-bit) at a specified address of the Protect block. 
* @param  Address: specifies the address to be programmed.
*         This parameter can be any address in Program memory zone.  
* @param  Data: specifies the data to be programmed.
* @retval FLASH_Status: The returned value can be: 
*                    FLASH_BUSY,FLASH_ERROR_STA,FLASH_ERROR_PGW,FLASH_ERROR_PGP,FLASH_ERROR_WRP,
*                    FLASH_ERROR_FLS,FLASH_ERROR_OPERATION,FLASH_ERROR_ERASE,FLASH_ERROR_PROGRAM,
*                    FLASH_ERROR_BLANK,FLASH_ERROR_VERIFY,FLASH_COMPLETE  
*/

FLASH_Status FLASH_PRTB_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
    FLASH_Status status = FLASH_COMPLETE;

    uint32_t tmpreg = FLASH->ACR.V32;

    FLASH->ACR.V32 = ((0x5AA5 << FLASH_ACR_LOCK_Pos) | (tmpreg & FLASH_ACR_LATENCY_Msk));

    /* Check the parameters */
    assert_param(IS_PROTECT_BLOCK_ADDRESS(Address));

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation();

    if (status == FLASH_COMPLETE)
    {
        /* Sets the programming window timer values */
        FLASH_Set_PGMWindow(SE_ERS_TIME, SE_ERS_TIME);
 
        /* if the previous operation is completed, proceed to program the new data */
        FLASH->CR.V32 = ((FLASH_CR_PSIZE_HALFWD << FLASH_CR_PSIZE_Pos) \
                        | (FLASH_CR_CMD_SPG << FLASH_CR_CMD_Pos) \
                        | (SH_SET << FLASH_CR_STRT_Pos));

        *(__IO uint16_t *)Address = Data;

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation();

        /* if the program operation is completed, disable the operation command */
        FLASH->CR.V32 = 0;
 
        /* Clears the programming window timer values */
        FLASH_Clear_PGMWindow();
    }
    tmpreg = ((0x5AA5 << FLASH_ACR_LOCK_Pos) | tmpreg);

    FLASH->ACR.V32 = tmpreg;
    /* Cache reset */
    FLASH_CacheReset();
    /* Return the Program Status */
    return status;
}


/**
  * @brief  Verify a page at a specified address of Flash. 
  * @param  StartAddr: The start Address                       
  * @note       Must be aligned to a BYTE(1 bytes) for each opertation. 
  * @param  Length: the length of verify operation
  * @param  Buf: the sourse data of programming operation
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_VERIFY or FLASH_COMPLETE.
  */
FLASH_Status FLASH_Verify_Byte(uint32_t StartAddr, uint32_t Length, uint8_t *Buf)
{
    FLASH_Status status = FLASH_COMPLETE;
    uint32_t tmp_len = 0;
    uint32_t tmp_addr = 0;

    /* Check the parameters */
    assert_param(IS_LENGTH_OK(Length));

    tmp_addr = StartAddr;
    while (tmp_len < Length)
    {
        if (*(__IO uint8_t *)tmp_addr != *((uint8_t *)(Buf + tmp_len)))
        {
            status = FLASH_ERROR_VERIFY;
            return status;
        }
        tmp_addr++;
        tmp_len++;
    }

    return status;
}

/**
  * @brief  Verify a page at a specified address of Flash. 
  * @param  StartAddr: The start Address                       
  * @note       Must be aligned to a HALFWORD(2 bytes) for each opertation. 
  * @param  Length: the length of verify operation
  * @note       Must be divisible by 2.  
  * @param  Buf: the sourse data of programming operation
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_VERIFY or FLASH_COMPLETE.
  */
FLASH_Status FLASH_Verify_HalfWord(uint32_t StartAddr, uint32_t Length, uint8_t *Buf)
{
    FLASH_Status status = FLASH_COMPLETE;
    uint32_t tmp_len = 0;
    uint32_t tmp_addr = 0;

    /* Check the parameters */
    assert_param(IS_LENGTH_OK(Length));

    tmp_addr = StartAddr;
    while (tmp_len < Length)
    {
        if (*(__IO uint16_t *)tmp_addr != *((uint16_t *)(Buf + tmp_len)))
        {
            status = FLASH_ERROR_VERIFY;
            return status;
        }
        tmp_addr += 2;
        tmp_len += 2;
    }

    return status;
}

/**
  * @brief  Verify a page at a specified address of Flash. 
  * @param  StartAddr: The start Address                       
  * @note       Must be aligned to a WORD(4 bytes) for each opertation. 
  * @param  Length: the length of verify operation
  * @note       Must be divisible by 4. 
  * @param  Buf: the sourse data of programming operation
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_VERIFY or FLASH_COMPLETE.
  */
FLASH_Status FLASH_VerifyPage(uint32_t StartAddr, uint32_t Length, uint8_t *Buf)
{
    FLASH_Status status = FLASH_COMPLETE;
    uint32_t tmp_len = 0;
    uint32_t tmp_addr = 0;

    /* Check the parameters */
    assert_param(IS_LENGTH_OK(Length));

    tmp_addr = StartAddr;
    while (tmp_len < Length)
    {
        if (*(__IO uint32_t *)tmp_addr != *((uint32_t *)(Buf + tmp_len)))
        {
            status = FLASH_ERROR_VERIFY;
            return status;
        }
        tmp_addr += 4;
        tmp_len += 4;
    }

    return status;
}

/**
  * @brief  Programs a page at a specified address of the main block. 
  * @param  StartAddr: The start Address                       
  * @note       Must be aligned to a WORD(4 bytes) for each programming. 
  * @param  Length: the length of programming operation
  * @note       Must be divisible by 4. 
  * @param  Buf: the sourse data of programming operation
  * @retval FLASH_Status: The returned value can be: 
  *                 FLASH_BUSY,FLASH_ERROR_STA,FLASH_ERROR_PGW,FLASH_ERROR_PGP,FLASH_ERROR_WRP,
  *                 FLASH_ERROR_FLS,FLASH_ERROR_OPERATION,FLASH_ERROR_ERASE,FLASH_ERROR_PROGRAM,
  *                 FLASH_ERROR_BLANK,FLASH_ERROR_VERIFY,FLASH_COMPLETE 
  */
FLASH_Status FLASH_Main_ProgramPage(uint32_t StartAddr, uint32_t Length, uint8_t *Buf)
{
    FLASH_Status status = FLASH_COMPLETE;
    uint32_t tmp_len = 0;
    uint32_t tmp_addr = 0;
    uint32_t temp_value = 0;

    /* Check the parameters */
    assert_param(IS_LENGTH_OK(Length));

    /* Clear pending flags (if any) */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR \
                    | FLASH_FLAG_STAERR | FLASH_FLAG_PGPERR \
                    | FLASH_FLAG_PGWERR | FLASH_FLAG_FLSERR);

    tmp_addr = StartAddr;
    while (tmp_len < Length)
    {
        /* Programming a Word to the special address */
        temp_value = (uint32_t)Buf[tmp_len+3] << 24 | Buf[tmp_len+2] << 16 | Buf[tmp_len+1] << 8 | Buf[tmp_len];
        status = FLASH_Main_ProgramWord(tmp_addr, temp_value);
        if (status == FLASH_COMPLETE)
        {
            tmp_addr += 4;
            tmp_len += 4;
        }
        else
        {
            break;
        }
    }

    if (status == FLASH_COMPLETE)
    {
        /* Verify the page */
        status = FLASH_Verify_Byte(StartAddr, Length, Buf);
    }

    return status;
}

/**
  * @brief  Programs a page at a specified address of the OTP block. 
  * @param  StartAddr: The start Address                       
  * @note       Must be aligned to a WORD(4 bytes) for each programming. 
  * @param  Length: the length of programming operation
  * @note       Must be divisible by 4. 
  * @param  Buf: the sourse data of programming operation
  * @retval FLASH_Status: The returned value can be: 
  *              FLASH_BUSY,FLASH_ERROR_STA,FLASH_ERROR_PGW,FLASH_ERROR_PGP,FLASH_ERROR_WRP,
  *              FLASH_ERROR_FLS,FLASH_ERROR_OPERATION,FLASH_ERROR_ERASE,FLASH_ERROR_PROGRAM,
  *              FLASH_ERROR_BLANK,FLASH_ERROR_VERIFY,FLASH_COMPLETE 
  */
FLASH_Status FLASH_OTP_ProgramPage(uint32_t StartAddr, uint32_t Length, uint8_t *Buf)
{
    FLASH_Status status = FLASH_COMPLETE;
    uint32_t tmp_len = 0;
    uint32_t tmp_addr = 0;
    uint32_t temp_value = 0;

    /* Check the parameters */
    assert_param(IS_LENGTH_OK(Length));

    /* Clear pending flags (if any) */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR \
                    | FLASH_FLAG_STAERR | FLASH_FLAG_PGPERR \
                    | FLASH_FLAG_PGWERR | FLASH_FLAG_FLSERR);

    tmp_addr = StartAddr;
    while (tmp_len < Length)
    {
        /* Programming a Word to the special address */
        temp_value = (uint32_t)Buf[tmp_len+3] << 24 | Buf[tmp_len+2] << 16 | Buf[tmp_len+1] << 8 | Buf[tmp_len];
        status = FLASH_OTP_ProgramWord(tmp_addr, temp_value);
        if (status == FLASH_COMPLETE)
        {
            tmp_addr += 4;
            tmp_len += 4;
        }
        else
        {
            break;
        }
    }

    if (status == FLASH_COMPLETE)
    {
        /* Verify the page */
        status = FLASH_Verify_Byte(StartAddr, Length, Buf);
    }

    return status;
}

/**
  * @brief  Enables or disables the write protection of the desired sectors
  * @param  WRT_PRT: specifies the sector(s) to be write protected or unprotected.
  *          This parameter can be one of the following values or combination value:
  *          @arg @b WRT_PRT_S0_S7    : Enable Write protection of Sector0   - Sector7        
  *          @arg @b WRT_PRT_S8_S15   : Enable Write protection of Sector8   - Sector15        
  *          @arg @b WRT_PRT_S16_S23  : Enable Write protection of Sector16  - Sector23      
  *          @arg @b ......                                                                  
  *          @arg @b WRT_PRT_S224_S231: Enable Write protection of Sector224 - Sector231
  *          @arg @b WRT_PRT_S232_S239: Enable Write protection of Sector232 - Sector239
  *          @arg @b WRT_PRT_S240_S247: Enable Write protection of Sector240 - Sector247
  *          @arg @b WRT_PRT_S248_S255: Enable Write protection of Sector248 - Sector255
  * @retval FLASH_Status: The returned value can be: 
  *                     FLASH_BUSY,FLASH_ERROR_STA,FLASH_ERROR_PGW,FLASH_ERROR_PGP,FLASH_ERROR_WRP,
  *                     FLASH_ERROR_FLS,FLASH_ERROR_OPERATION,FLASH_ERROR_ERASE,FLASH_ERROR_PROGRAM,
  *                     FLASH_ERROR_BLANK,FLASH_ERROR_VERIFY,FLASH_COMPLETE 
  */
FLASH_Status FLASH_SetWriteProtect(uint32_t WRT_PRT)
{
    FLASH_Status status = FLASH_COMPLETE;
    uint32_t tmpreg = 0;

    /* Clear pending flags (if any) */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR \
                    | FLASH_FLAG_STAERR | FLASH_FLAG_PGPERR \
                    | FLASH_FLAG_PGWERR | FLASH_FLAG_FLSERR);

    /* Programs the write protection values */
    tmpreg = 0xFF00FF00;
    tmpreg |= ((WRT_PRT & 0x000000FF)) | ((WRT_PRT & 0x0000FF00) << 8);
    status = FLASH_PRTB_ProgramWord(WRITE_PROTECT_ADDRESS, tmpreg);
    if (status == FLASH_COMPLETE)
    {
        tmpreg = 0xFF00FF00;
        tmpreg |= ((WRT_PRT & 0x00FF0000) >> 16) | ((WRT_PRT & 0xFF000000) >> 8);
        status = FLASH_PRTB_ProgramWord(WRITE_PROTECT_ADDRESS + 0x4, tmpreg);
    }

    /* Verify the write protection values  */
    if (FLASH_GetWriteProtect() != WRT_PRT)
    {
        status = FLASH_ERROR_VERIFY;
    }

    return status;
}

/**
  * @brief  Sets the read protection level.
  * @param  RD_PRT: specifies the read protection level.
  *          This parameter can be one of the following values:
  *            @arg @b 0x5AA5: No protection
  *            @arg @b 0xAAAA: The Low Level Read protection of the memory
  *            @arg @b 0xFFFF: The High Level Read protection of the memory (Debug Interface Disable)
  *   
  * @note      When enabling The High Level Read protection ,the debug interface is disable 
  *                                     and it's no more possible to go back to level 1 or 0
  *    
  * @retval FLASH_Status: The returned value can be:  
  *             FLASH_BUSY,FLASH_ERROR_STA,FLASH_ERROR_PGW,FLASH_ERROR_PGP,FLASH_ERROR_WRP,
  *             FLASH_ERROR_FLS,FLASH_ERROR_OPERATION,FLASH_ERROR_ERASE,FLASH_ERROR_PROGRAM,
  *             FLASH_ERROR_BLANK,FLASH_ERROR_VERIFY,FLASH_COMPLETE 
  */
FLASH_Status FLASH_SetReadProtect(uint16_t RD_PRT)
{
    FLASH_Status status = FLASH_COMPLETE;

    /* Check the parameters */
    assert_param(IS_RD_PRT(RD_PRT));

    /* Clear pending flags (if any) */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR \
                    | FLASH_FLAG_STAERR | FLASH_FLAG_PGPERR \
                    | FLASH_FLAG_PGWERR | FLASH_FLAG_FLSERR);

    /* Programs the read protection values */
    status = FLASH_PRTB_ProgramHalfWord(READ_PROTECT_ADDRESS, RD_PRT);

    /* Verify the read protection values  */
    if (FLASH->RPR != RD_PRT)
    {
        status = FLASH_ERROR_VERIFY;
    }

    return status;
}

/**
  * @brief  Returns the FLASH Write Protection Option Bytes value.
  * @retval The FLASH Write Protection  Option Bytes value
  */
uint32_t FLASH_GetWriteProtect(void)
{
    /* Return the FLASH write protection Register value */
    return (FLASH->WRPR);
}

/**
  * @brief  Returns the FLASH Read Protection level.
  * @retval FLASH ReadOut Protection Status:
  *           - SH_SET, when RD_PRT_HIGH_LEVEL or RD_PRT_LOW_LEVEL is set
  *           - SH_RESET, when RD_PRT_NO_PROTECT is set
  */
FlagStatus FLASH_GetReadProtect(void)
{
    FlagStatus readstatus = SH_RESET;

    if ((FLASH->RPR != (uint8_t)RD_PRT_NO_PROTECT))
    {
        readstatus = SH_SET;
    }
    else
    {
        readstatus = SH_RESET;
    }
    return readstatus;
}

/**
  * @brief  Sets the customer option
  * @param  OPT: Customer Option0.
  *         This parameter can be one of the following values:
  *         @arg  @b FLASH_NODIV_MAIN :  Flash is not divided into blocks, PC pointer addresses from Main Memory 
  *         @arg  @b FLASH_DIV_MAIN :    Flash is divided into two blocks, Main Memory and Backup Memory,PC pointer addresses from Main Memory 
  *         @arg  @b FLASH_DIV_BACKUP :  Flash is divided into two blocks, PC pointer addresses from Backup Memory  
  * @retval FLASH_Status: The returned value can be: FLASH_BUSY,FLASH_ERROR_STA,FLASH_ERROR_PGW,FLASH_ERROR_PGP,FLASH_ERROR_WRP,
  *                                                  FLASH_ERROR_FLS,FLASH_ERROR_OPERATION,FLASH_ERROR_ERASE,FLASH_ERROR_PROGRAM,
  *                                                  FLASH_ERROR_BLANK,FLASH_ERROR_VERIFY,FLASH_COMPLETE 
  */
FLASH_Status FLASH_SetCustomerOption(uint32_t OPT)
{
    FLASH_Status status = FLASH_COMPLETE;

    /* Clear pending flags (if any) */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR \
                    | FLASH_FLAG_STAERR | FLASH_FLAG_PGPERR \
                    | FLASH_FLAG_PGWERR | FLASH_FLAG_FLSERR);

    /* Programs the customer option0 */
    status = FLASH_CB_ProgramWord(OP_CUST1_ADDRESS, OPT);

    if (status == FLASH_COMPLETE)
    {
        /* Verify the Customer Option0 */
        if (*(__IO uint32_t *)OP_CUST1_ADDRESS != OPT)
        {
            status = FLASH_ERROR_VERIFY;
        }
    }
    return status;
}

/**
  * @brief  Returns the Customer Option0.
  * @retval The Customer Option0 Bytes value
  */
//uint32_t FLASH_GetCustomerOPT0(void)
//{
//    /* Return the Customer Option0 Bytes value */
//    return (FLASH->OPR.V32);
//}

/**
  * @brief  Returns the Customer Option0.
  * @retval The Customer Option0 Bytes value
  */
//uint32_t FLASH_GetCustomerOPT1(void)
//{
//    /* Return the Customer Option1 Bytes value */
//    return (FLASH->OPR_CUST1.V32);
//}

/**
  * @brief  Sets the customer security
  * @param  CS0: Customer Security0 .
  * @param  CS1: Customer Security1 .
  * @retval FLASH_Status: The returned value can be: 
  *               FLASH_BUSY,FLASH_ERROR_STA,FLASH_ERROR_PGW,FLASH_ERROR_PGP,FLASH_ERROR_WRP,
  *               FLASH_ERROR_FLS,FLASH_ERROR_OPERATION,FLASH_ERROR_ERASE,FLASH_ERROR_PROGRAM,
  *               FLASH_ERROR_BLANK,FLASH_ERROR_VERIFY,FLASH_COMPLETE 
  */
FLASH_Status FLASH_SetCustomerSecurity(uint32_t CS0, uint32_t CS1)
{
    FLASH_Status status = FLASH_COMPLETE;

    /* Sets the programming window timer values */
    FLASH_Set_PGMWindow(FLASH_Calculate_WinCounterVal(2 * PGM_WORD_TIME),
                        FLASH_Calculate_WinCounterVal(2 * PGM_WORD_TIME));

    /* Clear pending flags (if any) */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR \
                    | FLASH_FLAG_STAERR | FLASH_FLAG_PGPERR \
                    | FLASH_FLAG_PGWERR | FLASH_FLAG_FLSERR);

    /* Programs the read protection values */
    status = FLASH_CB_ProgramWord(CUSTOM_SECURITY_ADDRESS, CS0);
    if (status == FLASH_COMPLETE)
    {
        status = FLASH_CB_ProgramWord(CUSTOM_SECURITY_ADDRESS + 4, CS1);
    }

    if (status == FLASH_COMPLETE)
    {
        /* Verify the Customer Option0 */
        if (*(__IO uint32_t *)CUSTOM_SECURITY_ADDRESS != CS0)
        {
            status = FLASH_ERROR_VERIFY;
        }
        else
        {
            /* Verify the Customer Option1 */
            if (*(__IO uint32_t *)(CUSTOM_SECURITY_ADDRESS + 4) != CS1)
            {
                status = FLASH_ERROR_VERIFY;
            }
        }
    }
    return status;
}

/**
  * @brief  Returns the Unique Device ID.
  * @param  Buf: The point of Unique ID Buffer.
  * @retval The unique device ID(96bit)
  */
void FLASH_GetUniqueID(uint8_t *Buf)
{
    uint32_t tmp_i = 0;
    /* Return the Customer Option1 Bytes value */
    for (tmp_i = 0; tmp_i < 3; tmp_i++)
    {
        *((uint32_t *)(Buf + tmp_i * 4)) = *((uint32_t *)(UNIQUE_ID_ADDRESS + tmp_i * 4));
    }
}

/**
  * @brief  Returns the Agent ID.
  * @param  Buf: The buffer of Agent ID Buffer.
  * @retval None
  */
void FLASH_GetAgentID(uint8_t *Buf)
{
    uint32_t tmp_i = 0;
    /* Return the Customer Option1 Bytes value */
    for (tmp_i = 0; tmp_i < 4; tmp_i++)
    {
        *((uint32_t *)(Buf + tmp_i * 4)) = *((uint32_t *)(AGENT_ID_ADDRESS + tmp_i * 4));
    }
}

/**                 
  * @brief  This function writes a data buffer in flash (data are 32-bit aligned).
  * @note   After writing data buffer, the flash content is checked.
  * @param  FlashAddress: start address for writing data buffer
  * @param  Data: pointer on data buffer
  * @param  DataLength: length of data buffer (unit is 32-bit word)   
  * @param  file_size: total length of file size(unit is byte)  
  * @retval 0: Data successfully written to Flash memory
  *         1: Error occurred while writing data in Flash memory
  *         2: Written Data in flash memory is different from expected one
  */
uint32_t FLASH_Buf_Write(__IO uint32_t *FlashAddress, uint8_t *Data,
                        uint16_t DataLength, uint32_t file_size)
{
    FLASH_Status status = FLASH_COMPLETE;

    uint32_t total = 0;
    uint32_t tmp_len = 0;
    uint32_t tmp_addr = 0;
    uint32_t temp_value = 0;

    /* Clear pending flags (if any) */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR \
                    | FLASH_FLAG_STAERR | FLASH_FLAG_PGPERR \
                    | FLASH_FLAG_PGWERR | FLASH_FLAG_FLSERR);

    tmp_addr = *FlashAddress;
    while (tmp_len < DataLength)
    {
        /* Programming a Word to the special address */
        temp_value = (uint32_t)Data[tmp_len] << 24 | Data[tmp_len+1] << 16 | Data[tmp_len+2] << 8 | Data[tmp_len+3];
        status = FLASH_Main_ProgramWord(tmp_addr, temp_value);
        if (status == FLASH_COMPLETE)
        {
            tmp_addr += 4;
            tmp_len += 4;
            total++;
            if (total * 4 >= file_size)
            {
                total = 0;
                break;
            }
        }
        else
        {
            break;
        }
    }

    return status;
}
/**************************************************************************
                        EEPROM
 **************************************************************************/

/**
  * @brief  Locks the EEPRom block control register access
  * @retval None
*/
void FLASH_E2_Lock(void)
{
    /* Set the E2LCK Bit to lock the EEPRom block control Registers access */
    FLASH->CR.BIT.E2LCK = SH_SET;
}

/**
  * @brief  Unlocks the EEPRom block control register access
  * @param  OperationType: The single operation unlock or the multiple operations unlock
  *          This parameter can be one of the following values:
  *            @arg @b SINGLE_OPERATION: The single operation unlock
  *            @arg @b MULTI_OPERATION: The multiple operations unlock
  * @retval None
* 
*/
void FLASH_E2_Unlock(uint8_t OperationType)
{
    if (FLASH->CR.BIT.MNLCK == SH_RESET)
    {
        FLASH->CR.BIT.MNLCK = SH_SET;
    }
    if (FLASH->CR.BIT.INFLCK == SH_RESET)
    {
        FLASH->CR.BIT.INFLCK = SH_SET;
    }

    if (FLASH->CR.BIT.E2LCK != SH_RESET)
    {
        /* Authorize the FLASH Registers access */
        FLASH->E2KYR = FLASH_E2_KEY;

        /* The single unlock key unlocks the flash interface for one write or erase operation. */
        if (OperationType == SINGLE_OPERATION)
            FLASH->E2KYR = FLASH_SINGLE_OP_KEY;

        /* The multiple unlock key unlocks the flash interface for 
                                write or erase operations until the block is locked. */
        else if (OperationType == MULTI_OPERATION)
            FLASH->E2KYR = FLASH_MULTI_OP_KEY;
    }
}

/**
  * @brief  Reading EEPROM data. 
  * @param  e2psector: The Sector number to be read.
  *         This parameter can be a value of @ref E2_SECTOR_Num_Type.
  * @param  StartAddr: The start Address for each sector                      
  * @param  Length: the length of programming operation
  * @param  pBuf: Target address saved by read data
  * @retval None
  */
void FLASH_E2_ReadByte(E2_SECTOR_Num_TypeDef e2psector, uint32_t StartAddr,
                       uint32_t Length, uint8_t *pBuf)
{
    uint32_t tmp_len = 0;
    while (tmp_len < Length)
    {
        *pBuf++ = *((uint8_t *)(E2_BLOCK_ADDRESS + (e2psector << 10) + StartAddr + tmp_len));
        tmp_len++;
    }
}

/**
  * @brief  Erases a specified EEPROM block Sector.
  * @param  E2_Sector: The Sector number to be erased.
  *         This parameter can be a value of @ref E2_SECTOR_Num_Type.
  * @retval FLASH_Status: The returned value can be: 
  *                FLASH_BUSY,FLASH_ERROR_STA,FLASH_ERROR_PGW,FLASH_ERROR_PGP,FLASH_ERROR_WRP,
  *                FLASH_ERROR_FLS,FLASH_ERROR_OPERATION,FLASH_ERROR_ERASE,FLASH_ERROR_PROGRAM,
  *                FLASH_ERROR_BLANK,FLASH_ERROR_VERIFY,FLASH_COMPLETE 
**/
FLASH_Status FLASH_E2_EraseSector(E2_SECTOR_Num_TypeDef E2_Sector)
{
    FLASH_Status status = FLASH_COMPLETE;

    /* Check the parameters */
    assert_param(IS_E2_BLOCK_SECTOR(E2_Sector));

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation();

    if (status == FLASH_COMPLETE)
    {
        /* Sets the programming window timer values */
        FLASH_Set_PGMWindow(SE_ERS_TIME, SE_ERS_TIME);

        /* if the previous operation is completed, proceed to erase the sector */
        FLASH->CR1 = E2_Sector;
        FLASH->CR.V32 = ((FLASH_CR_CMD_E2SE << FLASH_CR_CMD_Pos) | (SH_SET << FLASH_CR_STRT_Pos));

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation();

        /* if the erase operation is completed, disable the operation command */
        FLASH->CR.V32 = 0;

        /* Clears the programming window timer values */
        FLASH_Clear_PGMWindow();
    }
    /* Cache reset */
    FLASH_CacheReset();
    /* Return the Erase Status */
    return status;
}

/**
  * @brief  Programs a word (32-bit) at a specified address of the EEPROM block.
  * @param  Address: specifies the address to be programmed.
  *         This parameter can be any address in Program memory zone.  
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH_Status: The returned value can be:
  *                 FLASH_BUSY,FLASH_ERROR_STA,FLASH_ERROR_PGW,FLASH_ERROR_PGP,FLASH_ERROR_WRP,
  *                 FLASH_ERROR_FLS,FLASH_ERROR_OPERATION,FLASH_ERROR_ERASE,FLASH_ERROR_PROGRAM,
  *                 FLASH_ERROR_BLANK,FLASH_ERROR_VERIFY,FLASH_COMPLETE 
*/
FLASH_Status FLASH_E2_ProgramWord(uint32_t Address, uint32_t Data)
{
    FLASH_Status status = FLASH_COMPLETE;

    /* Check the parameters */
    assert_param(IS_E2_BLOCK_ADDRESS(Address));

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation();

    if (status == FLASH_COMPLETE)
    {
        /* Sets the programming window timer values */
        FLASH_Set_PGMWindow(SE_ERS_TIME, SE_ERS_TIME);

        /* if the previous operation is completed, proceed to program the new data */
        FLASH->CR.V32 = ((FLASH_CR_PSIZE_WD << FLASH_CR_PSIZE_Pos) \
                        | (FLASH_CR_CMD_E2PG << FLASH_CR_CMD_Pos) \
                        | (SH_SET << FLASH_CR_STRT_Pos));

        *(__IO uint32_t *)Address = Data;

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation();

        /* if the program operation is completed, disable the operation command */
        FLASH->CR.V32 = 0;

        /* Clears the programming window timer values */
        FLASH_Clear_PGMWindow();
    }
    /* Cache reset */
    FLASH_CacheReset();
    /* Return the Program Status */
    return status;
}

/**
  * @brief  Programs a half word (16-bit) at a specified address of the EEPROM block.           
  * @param  Address: specifies the address to be programmed.
  *         This parameter can be any address in Program memory zone.  
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH_Status: The returned value can be: 
  *                FLASH_BUSY,FLASH_ERROR_STA,FLASH_ERROR_PGW,FLASH_ERROR_PGP,FLASH_ERROR_WRP,
  *                FLASH_ERROR_FLS,FLASH_ERROR_OPERATION,FLASH_ERROR_ERASE,FLASH_ERROR_PROGRAM,
  *                FLASH_ERROR_BLANK,FLASH_ERROR_VERIFY,FLASH_COMPLETE 
*/
FLASH_Status FLASH_E2_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
    FLASH_Status status = FLASH_COMPLETE;

    /* Check the parameters */
    assert_param(IS_E2_BLOCK_ADDRESS(Address));

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation();

    if (status == FLASH_COMPLETE)
    {
        /* Sets the programming window timer values */
        FLASH_Set_PGMWindow(SE_ERS_TIME, SE_ERS_TIME);

        /* if the previous operation is completed, proceed to program the new data */
        FLASH->CR.V32 = ((FLASH_CR_PSIZE_HALFWD << FLASH_CR_PSIZE_Pos) \
                        | (FLASH_CR_CMD_E2PG << FLASH_CR_CMD_Pos) \
                        | (SH_SET << FLASH_CR_STRT_Pos));

        *(__IO uint16_t *)Address = Data;

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation();

        /* if the program operation is completed, disable the operation command */
        FLASH->CR.V32 = 0;

        /* Clears the programming window timer values */
        FLASH_Clear_PGMWindow();
    }
    /* Cache reset */
    FLASH_CacheReset();
    /* Return the Program Status */
    return status;
}

/**
  * @brief  Programs a page at a specified address of the EEPRom block. 
  * @param  E2Sector: The EEPROM Sector Number 
  * @param  StartAddr: The start Address                       
  * @note       Must be aligned to a HalfWORD(2 bytes) for each programming. 
  * @param  Length: the length of programming operation
  * @note       Must be divisible by 2. 
  * @param  Buf: the sourse data of programming operation
  * @retval FLASH_Status: The returned value can be: 
  *              FLASH_BUSY,FLASH_ERROR_STA,FLASH_ERROR_PGW,FLASH_ERROR_PGP,FLASH_ERROR_WRP,
  *              FLASH_ERROR_FLS,FLASH_ERROR_OPERATION,FLASH_ERROR_ERASE,FLASH_ERROR_PROGRAM,
  *              FLASH_ERROR_BLANK,FLASH_ERROR_VERIFY,FLASH_COMPLETE 
  */
FLASH_Status FLASH_E2_ProgramPage_HalfWord(E2_SECTOR_Num_TypeDef E2Sector,
                                           uint32_t StartAddr, uint32_t Length, uint8_t *Buf)
{
    FLASH_Status status = FLASH_COMPLETE;
    uint32_t tmp_len = 0;
    uint32_t tmp_addr = 0;

    /* Check the parameters */
    assert_param(IS_LENGTH_OK(Length));

    /* Clear pending flags (if any) */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR \
                    | FLASH_FLAG_STAERR | FLASH_FLAG_PGPERR \
                    | FLASH_FLAG_PGWERR | FLASH_FLAG_FLSERR);

    tmp_addr = E2_BLOCK_ADDRESS + E2Sector * 1024 + StartAddr;
    while (tmp_len < Length)
    {
        /* Programming a Word to the special address */
        status = FLASH_E2_ProgramHalfWord(tmp_addr, *((uint16_t *)(Buf + tmp_len)));
        if (status == FLASH_COMPLETE)
        {
            tmp_addr += 2;
            tmp_len += 2;
        }
        else
        {
            break;
        }
    }

    tmp_addr = E2_BLOCK_ADDRESS + E2Sector * 1024 + StartAddr;

    if (status == FLASH_COMPLETE)
    {
        /* Verify the page */
        status = FLASH_Verify_Byte(tmp_addr, Length, Buf);
    }

    return status;
}

/**
  * @brief  Programs a page at a specified address of the EEPRom block. 
  * @param  E2Sector: The EEPROM Sector Number    
  * @param  StartAddr: The start Address                       
  * @note       Must be aligned to a WORD(4 bytes) for each programming. 
  * @param  Length: the length of programming operation
  * @note       Must be divisible by 4. 
  * @param  Buf: the sourse data of programming operation
  * @retval FLASH_Status: The returned value can be: 
  *                 FLASH_BUSY,FLASH_ERROR_STA,FLASH_ERROR_PGW,FLASH_ERROR_PGP,FLASH_ERROR_WRP,
  *                 FLASH_ERROR_FLS,FLASH_ERROR_OPERATION,FLASH_ERROR_ERASE,FLASH_ERROR_PROGRAM,
  *                 FLASH_ERROR_BLANK,FLASH_ERROR_VERIFY,FLASH_COMPLETE 
  */
FLASH_Status FLASH_E2_ProgramPage(E2_SECTOR_Num_TypeDef E2Sector, uint32_t StartAddr,
                                  uint32_t Length, uint8_t *Buf)
{
    FLASH_Status status = FLASH_COMPLETE;
    uint32_t tmp_len = 0;
    uint32_t tmp_addr = 0;

    /* Check the parameters */
    assert_param(IS_LENGTH_OK(Length));

    /* Clear pending flags (if any) */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR \
                    | FLASH_FLAG_STAERR | FLASH_FLAG_PGPERR \
                    | FLASH_FLAG_PGWERR | FLASH_FLAG_FLSERR);

    tmp_addr = E2_BLOCK_ADDRESS + E2Sector * 1024 + StartAddr;
    while (tmp_len < Length)
    {
        /* Programming a Word to the special address */
        status = FLASH_E2_ProgramWord(tmp_addr, *((uint32_t *)(Buf + tmp_len)));
        if (status == FLASH_COMPLETE)
        {
            tmp_addr += 4;
            tmp_len += 4;
        }
        else
        {
            break;
        }
    }

    tmp_addr = E2_BLOCK_ADDRESS + E2Sector * 1024 + StartAddr;

    if (status == FLASH_COMPLETE)
    {
        /* Verify the page */
        status = FLASH_Verify_Byte(tmp_addr, Length, Buf);
    }

    return status;
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
