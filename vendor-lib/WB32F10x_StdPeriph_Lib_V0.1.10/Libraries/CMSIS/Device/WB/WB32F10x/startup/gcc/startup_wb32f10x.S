/**************************************************************************//**
 * @file     startup_wb32f10x.S
 * @brief    CMSIS Core Device Startup File for
 *           WB32F10x Device Series
 * @version  V0.1.10
 * @date     05-January-2023
 ******************************************************************************/
/*
 * Copyright (c) 2020 - 2023 Westberry Technology (ChangZhou) Corp., Ltd. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

                .syntax  unified
                .arch    armv7-m

                .section .vectors
                .align   2
                .globl   __Vectors
                .globl   __Vectors_End
                .globl   __Vectors_Size
__Vectors:
                .long    __StackTop                         /*     Top of Stack */
                .long    Reset_Handler                      /*     Reset Handler */
                .long    NMI_Handler                        /* -14 NMI Handler */
                .long    HardFault_Handler                  /* -13 Hard Fault Handler */
                .long    MemManage_Handler                  /* -12 MPU Fault Handler */
                .long    BusFault_Handler                   /* -11 Bus Fault Handler */
                .long    UsageFault_Handler                 /* -10 Usage Fault Handler */
                .long    0                                  /*     Reserved */
                .long    0                                  /*     Reserved */
                .long    0                                  /*     Reserved */
                .long    0                                  /*     Reserved */
                .long    SVC_Handler                        /*  -5 SVCall Handler */
                .long    DebugMon_Handler                   /*  -4 Debug Monitor Handler */
                .long    0                                  /*     Reserved */
                .long    PendSV_Handler                     /*  -2 PendSV Handler */
                .long    SysTick_Handler                    /*  -1 SysTick Handler */

                /* Interrupts */
                .long    WWDG_IRQHandler                    /*   0: Window Watchdog */
                .long    PVD_IRQHandler                     /*   1: PVD through EXTI Line detect */
                .long    TAMPER_IRQHandler                  /*   2: Tamper */
                .long    RTC_IRQHandler                     /*   3: RTC */
                .long    FMC_IRQHandler                     /*   4: FMC */
                .long    RCC_IRQHandler                     /*   5: RCC */
                .long    EXTI0_IRQHandler                   /*   6: EXTI Line 0 */
                .long    EXTI1_IRQHandler                   /*   7: EXTI Line 1 */
                .long    EXTI2_IRQHandler                   /*   8: EXTI Line 2 */
                .long    EXTI3_IRQHandler                   /*   9: EXTI Line 3 */
                .long    EXTI4_IRQHandler                   /*  10: EXTI Line 4 */
                .long    DMAC1_IRQHandler                   /*  11: DMAC1 */
                .long    DMAC2_IRQHandler                   /*  12: DMAC2 */
                .long    ADC_IRQHandler                     /*  13: ADC */
                .long    USB_IRQHandler                     /*  14: USB */
                .long    USB_DMA_IRQHandler                 /*  15: USB DMA */
                .long    EXTI9_5_IRQHandler                 /*  16: EXTI Line 9..5 */
                .long    TIM1_BRK_IRQHandler                /*  17: TIM1 Break */
                .long    TIM1_UP_IRQHandler                 /*  18: TIM1 Update */
                .long    TIM1_TRG_COM_IRQHandler            /*  19: TIM1 Trigger and Commutation */
                .long    TIM1_CC_IRQHandler                 /*  20: TIM1 Capture Compare */
                .long    TIM2_IRQHandler                    /*  21: TIM2 */
                .long    TIM3_IRQHandler                    /*  22: TIM3 */
                .long    TIM4_IRQHandler                    /*  23: TIM4 */
                .long    I2C1_IRQHandler                    /*  24: I2C1 */
                .long    I2C2_IRQHandler                    /*  25: I2C2 */
                .long    QSPI_IRQHandler                    /*  26: QSPI */
                .long    SPIM2_IRQHandler                   /*  27: SPIM2 */
                .long    SPIS1_IRQHandler                   /*  28: SPIS1 */
                .long    SPIS2_IRQHandler                   /*  29: SPIS2 */
                .long    UART1_IRQHandler                   /*  30: UART1 */
                .long    UART2_IRQHandler                   /*  31: UART2 */
                .long    UART3_IRQHandler                   /*  32: UART3 */
                .long    EXTI15_10_IRQHandler               /*  33: EXTI Line 15..10 */
                .long    RTCAlarm_IRQHandler                /*  34: RTC Alarm through EXTI Line */
                .long    USBP_WKUP_IRQHandler               /*  35: USB PIN */
                .long    I2S_IRQHandler                     /*  36: I2S */
                .long    ISO_IRQHandler                     /*  37: ISO7816 */
__Vectors_End:
                .equ     __Vectors_Size, __Vectors_End - __Vectors
                .size    __Vectors, . - __Vectors


                .thumb
                .section .text
                .align   2

                .thumb_func
                .type    Reset_Handler, %function
                .globl   Reset_Handler
Reset_Handler:
                bl       SystemInit

                ldr      r4, =__copy_table_start__
                ldr      r5, =__copy_table_end__

.L_loop0:
                cmp      r4, r5
                bge      .L_loop0_done
                ldr      r1, [r4]
                ldr      r2, [r4, #4]
                ldr      r3, [r4, #8]

.L_loop0_0:
                subs     r3, #4
                ittt     ge
                ldrge    r0, [r1, r3]
                strge    r0, [r2, r3]
                bge      .L_loop0_0

                adds     r4, #12
                b        .L_loop0
.L_loop0_done:

                ldr      r3, =__zero_table_start__
                ldr      r4, =__zero_table_end__

.L_loop2:
                cmp      r3, r4
                bge      .L_loop2_done
                ldr      r1, [r3]
                ldr      r2, [r3, #4]
                movs     r0, 0

.L_loop2_0:
                subs     r2, #4
                itt      ge
                strge    r0, [r1, r2]
                bge      .L_loop2_0

                adds     r3, #8
                b        .L_loop2
.L_loop2_done:

                bl       _start

                .size    Reset_Handler, . - Reset_Handler


                .thumb_func
                .type    Default_Handler, %function
                .weak    Default_Handler
Default_Handler:
                b        .
                .size    Default_Handler, . - Default_Handler

/* Macro to define default exception/interrupt handlers.
 * Default handler are weak symbols with an endless loop.
 * They can be overwritten by real handlers.
 */
                .macro   Set_Default_Handler  Handler_Name
                .weak    \Handler_Name
                .set     \Handler_Name, Default_Handler
                .endm


/* Default exception/interrupt handler */

                Set_Default_Handler  NMI_Handler
                Set_Default_Handler  HardFault_Handler
                Set_Default_Handler  MemManage_Handler
                Set_Default_Handler  BusFault_Handler
                Set_Default_Handler  UsageFault_Handler
                Set_Default_Handler  SVC_Handler
                Set_Default_Handler  DebugMon_Handler
                Set_Default_Handler  PendSV_Handler
                Set_Default_Handler  SysTick_Handler

                Set_Default_Handler  WWDG_IRQHandler
                Set_Default_Handler  PVD_IRQHandler
                Set_Default_Handler  TAMPER_IRQHandler
                Set_Default_Handler  RTC_IRQHandler
                Set_Default_Handler  FMC_IRQHandler
                Set_Default_Handler  RCC_IRQHandler
                Set_Default_Handler  EXTI0_IRQHandler
                Set_Default_Handler  EXTI1_IRQHandler
                Set_Default_Handler  EXTI2_IRQHandler
                Set_Default_Handler  EXTI3_IRQHandler
                Set_Default_Handler  EXTI4_IRQHandler
                Set_Default_Handler  DMAC1_IRQHandler
                Set_Default_Handler  DMAC2_IRQHandler
                Set_Default_Handler  ADC_IRQHandler
                Set_Default_Handler  USB_IRQHandler
                Set_Default_Handler  USB_DMA_IRQHandler
                Set_Default_Handler  EXTI9_5_IRQHandler
                Set_Default_Handler  TIM1_BRK_IRQHandler
                Set_Default_Handler  TIM1_UP_IRQHandler
                Set_Default_Handler  TIM1_TRG_COM_IRQHandler
                Set_Default_Handler  TIM1_CC_IRQHandler
                Set_Default_Handler  TIM2_IRQHandler
                Set_Default_Handler  TIM3_IRQHandler
                Set_Default_Handler  TIM4_IRQHandler
                Set_Default_Handler  I2C1_IRQHandler
                Set_Default_Handler  I2C2_IRQHandler
                Set_Default_Handler  QSPI_IRQHandler
                Set_Default_Handler  SPIM2_IRQHandler
                Set_Default_Handler  SPIS1_IRQHandler
                Set_Default_Handler  SPIS2_IRQHandler
                Set_Default_Handler  UART1_IRQHandler
                Set_Default_Handler  UART2_IRQHandler
                Set_Default_Handler  UART3_IRQHandler
                Set_Default_Handler  EXTI15_10_IRQHandler
                Set_Default_Handler  RTCAlarm_IRQHandler
                Set_Default_Handler  USBP_WKUP_IRQHandler
                Set_Default_Handler  I2S_IRQHandler
                Set_Default_Handler  ISO_IRQHandler


                .end
