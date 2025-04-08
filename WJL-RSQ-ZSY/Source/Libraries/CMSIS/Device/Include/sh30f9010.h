/**
 * @file      sh30f9010.h
 * @brief     
 * @author    sinowealth software group ()
 * @date      2024-06-18
 * @version   V1.0
 * @copyright Copyright (c) 1994 - 2024 SINOWEALTH Electronic Ltd.
 * 
 * @attention
 * SINOWEALTH IS SUPPLYING THIS SOFTWARE FOR USE EXCLUSIVELY ON SINOWEALTH'S
 * MICROCONTROLLER PRODUCTS. IT IS PROTECTED UNDER APPLICABLE COPYRIGHT LAWS. 
 * THIS SOFTWARE IS FOR GUIDANCE IN ORDER TO SAVE TIME. AS A RESULT, SINOWEALTH 
 * SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES 
 * WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT OF SUCH FIRMWARE AND/OR
 * THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN 
 * CONNECTION WITH THEIR PRODUCTS.
 * @par Change log:
 * <table>
 * <tr><th>Date        <th>Version  <th>Author    <th>Description
 * <tr><td>2024-06-18  <td>1.0      <td>bruce     <td>Create initial version
 * </table>
 */

#ifndef __SH30F9010_H__
#define __SH30F9010_H__

#include "sh30f9xx_sb0.h"
#include "system_sh30f9xx_sb0.h"
#include "sh30f9xx_sb0_libcfg.h"




/***************************************/
#define ROM_SIZE   0x40000
#define SRAM_SIZE  0x4000
#define ROM_BASE   0x0
#define SRAM_BASE  0x20000000
/***************************************/

#define IS_IN_SRAM(addr) ((((uint32_t)(addr)) >= SRAM_BASE) && (((uint32_t)(addr)) <= (SRAM_BASE+SRAM_SIZE)))
#define IS_IN_ROM(addr)  ((((uint32_t)(addr)) >= ROM_BASE)  && (((uint32_t)(addr)) <= (ROM_BASE+ROM_SIZE)))

#endif /*__SH30F9010_H__*/
