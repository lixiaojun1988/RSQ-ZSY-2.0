/**
 * *****************************************************************************
 * @file     bsp_systick.c
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
void    bsp_systick_init(void)
{
    uint32_t    period;
    RCC_Clocks_TypeDef rcc_clocks;

	RCC_GetClocksFreq(&rcc_clocks);                 /* get current system clocks */
//    period = rcc_clocks.hclkFreq / 1000000 * 100;   // 100us
    period = rcc_clocks.hclkFreq / 1000000 * 1000;  // 1ms
		SysTick_Config(period);                         /* set systick interrupt */
}

/*----------------------------------------- E.N.D -----------------------------------------*/
