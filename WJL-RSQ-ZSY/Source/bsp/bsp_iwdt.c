/**
 * *****************************************************************************
 * @file     bsp_iwdt.c
 * @brief
 * @author   sinowealth
 * @version  1.0
 * @date     2024-08-07
 *
 * *****************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "sh30f9010.h"



/**
 * @brief
 *
 */
void    bsp_iwdt_init(void)
{
    //IWDT_Enable(IWDT_Prescaler_128, 0x0FFF);    // IWDT设置为4秒：128*4096/128KHz = 4096ms = 4.096s
	HWDT_ConfigPeriod(HWDT_ReloadPeriod_2048MS);
}

/*----------------------------------------- E.N.D -----------------------------------------*/
