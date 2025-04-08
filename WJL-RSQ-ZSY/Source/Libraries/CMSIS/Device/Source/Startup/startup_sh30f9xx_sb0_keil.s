;/***************************************************************************************/
;/* startup_sh30f9010_keil.s: Startup file for Sinowealth SH30F9xx_sa0 device series    */
;/***************************************************************************************/
;/* <<< Use Configuration Wizard in Context Menu >>>                                    */ 
;/***************************************************************************************/
;/* This file is part of the uVision/ARM development tools.                             */
;/* Copyright (c) 2017-2024 Sinowealth. All rights reserved.                            */
;/* This software may only be used under the terms of a valid, current,                 */
;/* end user licence from KEIL for a compatible version of KEIL software                */
;/* development tools. Nothing else gives you the right to use this software.           */
;/***************************************************************************************/
; *
; * SINOWEALTH IS SUPPLYING THIS SOFTWARE FOR USE EXCLUSIVELY ON SINOWEALTH'S 
; * MICROCONTROLLER PRODUCTS. IT IS PROTECTED UNDER APPLICABLE COPYRIGHT LAWS. 
; * THIS SOFTWARE IS FOR GUIDANCE IN ORDER TO SAVE TIME. AS A RESULT, SINOWEALTH 
; * SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES 
; * WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT OF SUCH FIRMWARE AND/OR
; * THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN 
; * CONNECTION WITH THEIR PRODUCTS.
; *
; * COPYRIGHT 2024 Sinowealth
; *
; ******************************************************************************
; */

;<e>Run Application in User Mode
USER_MODE_EN      EQU  0
;</e>

;// <h> Stack Configuration
;   <o0>System Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
;   <o1>User Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
;// </h> 
SystemStack_Size  EQU     0x00000400
UserStack_Size    EQU     0x00000000

          AREA    STACK, NOINIT, READWRITE, ALIGN=3
__stack_limit
System_Stack_Mem  SPACE   SystemStack_Size
system_stack_top
__user_stack_limit
User_Stack_Mem    SPACE   UserStack_Size
usr_stack_top
__initial_sp

;// <h> Heap Configuration
;//   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF>
;// </h>
Heap_Size        EQU     0x00000100

;/*****************************************************************************/
;/*-------- <<< end of configuration section >>> -----------------------------*/
;/*****************************************************************************/

                 AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem         SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB
; Vector Table Mapped to Address 0 at Reset
                         AREA     RESET, DATA, READONLY
                         EXPORT   __Vectors
                         EXPORT   __Vectors_End
                         EXPORT   __Vectors_Size
__Vectors
                         DCD      system_stack_top     ; -16 (000H) Top of Stack
                         DCD      Reset_Handler        ; -15 (004H) Reset Handler
                         DCD      NMI_Handler          ; -14 (008H) Non Maskable Interrupt
                         DCD      HardFault_Handler    ; -13 (00CH) Hard Fault Interrupt
                         DCD      Default_Handler      ; -12 (010H) Reserved
                         DCD      Default_Handler      ; -11 (014H) Reserved
                         DCD      Default_Handler      ; -10 (018H) Reserved
                         DCD      Default_Handler      ;  -9 (01CH) Reserved
                         DCD      Default_Handler      ;  -8 (020H) Reserved
                         DCD      Default_Handler      ;  -7 (024H) Reserved
                         DCD      Default_Handler      ;  -6 (028H) Reserved
                         DCD      SVC_Handler          ;  -5 (02CH) Cortex-M0+ SV Call Interrupt
                         DCD      Default_Handler      ;  -4 (030H) Reserved
                         DCD      Default_Handler      ;  -3 (034H) Reserved
                         DCD      PendSV_Handler       ;  -2 (038H) Cortex-M0+ Pend SV Interrupt
                         DCD      SysTick_Handler      ;  -1 (03CH) Cortex-M0+ System Tick Interrupt
                         DCD      WWDT_Handler         ;   0 (040H) Window WatchDog Interrupt
                         DCD      BOD_Handler          ;   1 (044H) PVD through EXTI Line detection Interrupt
                         DCD      RCC_Handler          ;   2 (048H) RCC global Interrupt
                         DCD      EXTI0_Handler        ;   3 (04CH) EXTI Line0 Interrupt
                         DCD      EXTI1_Handler        ;   4 (050H) EXTI Line1 Interrupt
                         DCD      EXTI2_Handler        ;   5 (054H) EXTI Line2 Interrupt
                         DCD      EXTI3_Handler        ;   6 (058H) EXTI Line3 Interrupt
                         DCD      ADC_Handler          ;   7 (05CH) ADC Interrupt
                         DCD      TWI0_Handler         ;   8 (060H) TWI0 Interrupt
                         DCD      TWI1_Handler         ;   9 (064H) TWI1 Interrupt
                         DCD      PCA0_Handler         ;  10 (068H) PCA0 Interrupt
                         DCD      PCA1_Handler         ;  11 (06CH) PCA1 Interrupt
                         DCD      PCA2_Handler         ;  12 (070H) PCA2 Interrupt
                         DCD      PCA3_Handler         ;  13 (074H) PCA3 Interrupt
                         DCD      EXTI9_4_Handler      ;  14 (078H) EXTI Line4 ~ Line9 global Interrupt
                         DCD      UART0_Handler        ;  15 (07CH) UART0 Interrupt
                         DCD      SPI0_Handler         ;  16 (080H) SPI0 Interrupt
                         DCD      SPI1_Handler         ;  17 (084H) SPI1 Interrupt
                         DCD      UART1_Handler        ;  18 (088H) UART1 Interrupt
                         DCD      UART2_Handler        ;  19 (08CH) UART2 Interrupt
                         DCD      UART3_Handler        ;  20 (090H) UART3 Interrupt
                         DCD      EXTI15_10_Handler    ;  21 (094H) EXTI Line10 ~ Line15 Interrupt
                         DCD      UART4_Handler        ;  22 (098H) UART4 Interrupt
                         DCD      UART5_Handler        ;  23 (09CH) UART5 Interrupt
                         DCD      PWM0_Handler         ;  24 (0A0H) PWM0 Interrupt
                         DCD      PWM1_Handler         ;  25 (0A4H) PWM1 Interrupt
                         DCD      PWM2_Handler         ;  26 (0A8H) PWM2 Interrupt
                         DCD      PWM3_Handler         ;  27 (0ACH) PWM3 Interrupt
                         DCD      TIM0_Handler         ;  28 (0B0H) TIM0 Interrupt
                         DCD      TIM1_FIFO0_Handler   ;  29 (0B4H) TIM1_FIFO0 Interrupt
                         DCD      TIM2_FIFO1_Handler   ;  30 (0B8H) TIM2_FIFO1 Interrupt
                         DCD      TIM3_FIFO2_Handler   ;  31 (0BCH) TIM3_FIFO2 Interrupt


__Vectors_End
__Vectors_Size           EQU      __Vectors_End - __Vectors

                         AREA     |.text|, CODE, READONLY
Reset_Handler            PROC
                         EXPORT   Reset_Handler        [WEAK]
                         IMPORT   __main
                         LDR      R0, =SystemInit
                         BLX      R0
               IF  USER_MODE_EN = 1
                         ;switch to user mode
                         LDR      R0, =usr_stack_top
                         MSR      PSP, R0                                      
                         MOVS     R0,  #2
                         MSR      CONTROL, R0         
               ENDIF                         
                         LDR      R0, =__main
                         BX       R0
                         ENDP    

NMI_Handler              PROC
                         EXPORT   NMI_Handler          [WEAK]
                         B        .
                         ENDP    

HardFault_Handler        PROC
                         EXPORT   HardFault_Handler    [WEAK]
                         B        .
                         ENDP    

SVC_Handler              PROC
                         EXPORT   SVC_Handler          [WEAK]
                 IF  USER_MODE_EN = 1
                         ;instruction svc push xPSR,PC,LR,R12,R3,R2,R1,R0
                         MOVS     R1,  #0x4
                         MOV      R2,  LR
                         TST      R2,  R1            ;test return addr bit 2
                         BNE      Procee_NE          ;if 0
                         MRS      R0,    MSP         ; use main stack
                         B        Procee_exit
                Procee_NE
                         MRS      R0,    PSP         ; use user stack
                Procee_exit
                         B        SVC_Call_Proc
                ELSE
                         B        .
                ENDIF
                         ENDP    
                IF  USER_MODE_EN = 1
SVC_Call_Proc            PROC
                         EXPORT   SVC_Call_Proc       [WEAK]
                         BX       LR
                         ENDP
                ENDIF

PendSV_Handler           PROC
                         EXPORT   PendSV_Handler       [WEAK]
                         B        .
                         ENDP    

SysTick_Handler          PROC
                         EXPORT   SysTick_Handler      [WEAK]
                         B        .
                         ENDP    

Default_Handler          PROC
                         EXPORT   WWDT_Handler         [WEAK]
                         EXPORT   BOD_Handler          [WEAK]
                         EXPORT   RCC_Handler          [WEAK]
                         EXPORT   EXTI0_Handler        [WEAK]
                         EXPORT   EXTI1_Handler        [WEAK]
                         EXPORT   EXTI2_Handler        [WEAK]
                         EXPORT   EXTI3_Handler        [WEAK]
                         EXPORT   ADC_Handler          [WEAK]
                         EXPORT   TWI0_Handler         [WEAK]
                         EXPORT   TWI1_Handler         [WEAK]
                         EXPORT   PCA0_Handler         [WEAK]
                         EXPORT   PCA1_Handler         [WEAK]
                         EXPORT   PCA2_Handler         [WEAK]
                         EXPORT   PCA3_Handler         [WEAK]
                         EXPORT   EXTI9_4_Handler      [WEAK]
                         EXPORT   UART0_Handler        [WEAK]
                         EXPORT   SPI0_Handler         [WEAK]
                         EXPORT   SPI1_Handler         [WEAK]
                         EXPORT   UART1_Handler        [WEAK]
                         EXPORT   UART2_Handler        [WEAK]
                         EXPORT   UART3_Handler        [WEAK]
                         EXPORT   EXTI15_10_Handler    [WEAK]
                         EXPORT   UART4_Handler        [WEAK]
                         EXPORT   UART5_Handler        [WEAK]
                         EXPORT   PWM0_Handler         [WEAK]
                         EXPORT   PWM1_Handler         [WEAK]
                         EXPORT   PWM2_Handler         [WEAK]
                         EXPORT   PWM3_Handler         [WEAK]
                         EXPORT   TIM0_Handler         [WEAK]
                         EXPORT   TIM1_FIFO0_Handler   [WEAK]
                         EXPORT   TIM2_FIFO1_Handler   [WEAK]
                         EXPORT   TIM3_FIFO2_Handler   [WEAK]
WWDT_Handler
BOD_Handler
RCC_Handler
EXTI0_Handler
EXTI1_Handler
EXTI2_Handler
EXTI3_Handler
ADC_Handler
TWI0_Handler
TWI1_Handler
PCA0_Handler
PCA1_Handler
PCA2_Handler
PCA3_Handler
EXTI9_4_Handler
UART0_Handler
SPI0_Handler
SPI1_Handler
UART1_Handler
UART2_Handler
UART3_Handler
EXTI15_10_Handler
UART4_Handler
UART5_Handler
PWM0_Handler
PWM1_Handler
PWM2_Handler
PWM3_Handler
TIM0_Handler
TIM1_FIFO0_Handler
TIM2_FIFO1_Handler
TIM3_FIFO2_Handler
                         B        .
                         ENDP    

                         
SystemInit  PROC
                EXPORT  SystemInit  [WEAK]
                BX LR      
                ENDP
                    
                ALIGN
                
; User setup Stack & Heap

                EXPORT   __stack_limit
                EXPORT   __initial_sp
                IF       Heap_Size != 0                      ; Heap is provided
                EXPORT   __heap_base
                EXPORT   __heap_limit
                ENDIF

                END

;*********************** (C) COPYRIGHT SINOWEALTH *** END OF FILE*****   
