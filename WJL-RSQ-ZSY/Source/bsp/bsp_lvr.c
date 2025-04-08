/**
 * *****************************************************************************
 * @file     bsp_lvr.c
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
void    bsp_lvr_init(void)
{
    RCC_AHBPeriphClockOnOff(RCC_AHB_SYSCFG, SH_ON); /* System Clock Config Enable */

    SYSCFG_VLVRInit(SYSCFG_VLVR_Level_3V1);         // LVR配置为2.3V
    SYSCFG_VLVRConfig(SH_ENABLE);                   // 使能LVR
}

/*----------------------------------------- E.N.D -----------------------------------------*/
