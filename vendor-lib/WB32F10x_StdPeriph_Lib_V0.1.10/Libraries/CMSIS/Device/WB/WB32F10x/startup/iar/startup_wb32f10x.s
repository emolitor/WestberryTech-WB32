;/**************************************************************************//**
; * @file     startup_wb32f10x.s
; * @brief    CMSIS Core Device Startup File for
; *           WB32F10x Device
; * @version  V0.1.10
; * @date     05-January-2023
; ******************************************************************************/
;/*
; * Copyright (c) 2020 - 2023 Westberry Technology (ChangZhou) Corp., Ltd. All rights reserved.
; *
; * SPDX-License-Identifier: Apache-2.0
; *
; * Licensed under the Apache License, Version 2.0 (the License); you may
; * not use this file except in compliance with the License.
; * You may obtain a copy of the License at
; *
; * www.apache.org/licenses/LICENSE-2.0
; *
; * Unless required by applicable law or agreed to in writing, software
; * distributed under the License is distributed on an AS IS BASIS, WITHOUT
; * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; * See the License for the specific language governing permissions and
; * limitations under the License.
; */

;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

                MODULE   ?cstartup

                ;; Forward declaration of sections.
                SECTION  CSTACK:DATA:NOROOT(3)

                SECTION  .intvec:CODE:NOROOT(2)

                EXTERN   __iar_program_start
                EXTERN   SystemInit
                PUBLIC   __vector_table
                PUBLIC   __vector_table_0x1c
                PUBLIC   __Vectors
                PUBLIC   __Vectors_End
                PUBLIC   __Vectors_Size

                DATA

__vector_table
                DCD      sfe(CSTACK)                         ;     Top of Stack
                DCD      Reset_Handler                       ;     Reset Handler
                DCD      NMI_Handler                         ; -14 NMI Handler
                DCD      HardFault_Handler                   ; -13 Hard Fault Handler
                DCD      MemManage_Handler                   ; -12 MPU Fault Handler
                DCD      BusFault_Handler                    ; -11 Bus Fault Handler
                DCD      UsageFault_Handler                  ; -10 Usage Fault Handler
__vector_table_0x1c
                DCD      0                                   ;     Reserved
                DCD      0                                   ;     Reserved
                DCD      0                                   ;     Reserved
                DCD      0                                   ;     Reserved
                DCD      SVC_Handler                         ;  -5 SVCall Handler
                DCD      DebugMon_Handler                    ;  -4 Debug Monitor Handler
                DCD      0                                   ;     Reserved
                DCD      PendSV_Handler                      ;  -2 PendSV Handler
                DCD      SysTick_Handler                     ;  -1 SysTick Handler

                ; Interrupts
                DCD     WWDG_IRQHandler                      ;  0  Window Watchdog
                DCD     PVD_IRQHandler                       ;  1  PVD through EXTI Line detect
                DCD     TAMPER_IRQHandler                    ;  2  Tamper
                DCD     RTC_IRQHandler                       ;  3  RTC
                DCD     FMC_IRQHandler                       ;  4  FMC
                DCD     RCC_IRQHandler                       ;  5  RCC
                DCD     EXTI0_IRQHandler                     ;  6  EXTI Line 0
                DCD     EXTI1_IRQHandler                     ;  7  EXTI Line 1
                DCD     EXTI2_IRQHandler                     ;  8  EXTI Line 2
                DCD     EXTI3_IRQHandler                     ;  9  EXTI Line 3
                DCD     EXTI4_IRQHandler                     ; 10  EXTI Line 4
                DCD     DMAC1_IRQHandler                     ; 11  DMAC1
                DCD     DMAC2_IRQHandler                     ; 12  DMAC2
                DCD     ADC_IRQHandler                       ; 13  ADC
                DCD     USB_IRQHandler                       ; 14  USB
                DCD     USB_DMA_IRQHandler                   ; 15  USB DMA
                DCD     EXTI9_5_IRQHandler                   ; 16  EXTI Line 9..5
                DCD     TIM1_BRK_IRQHandler                  ; 17  TIM1 Break
                DCD     TIM1_UP_IRQHandler                   ; 18  TIM1 Update
                DCD     TIM1_TRG_COM_IRQHandler              ; 19  TIM1 Trigger and Commutation
                DCD     TIM1_CC_IRQHandler                   ; 20  TIM1 Capture Compare
                DCD     TIM2_IRQHandler                      ; 21  TIM2
                DCD     TIM3_IRQHandler                      ; 22  TIM3
                DCD     TIM4_IRQHandler                      ; 23  TIM4
                DCD     I2C1_IRQHandler                      ; 24  I2C1
                DCD     I2C2_IRQHandler                      ; 25  I2C2
                DCD     QSPI_IRQHandler                      ; 26  QSPI
                DCD     SPIM2_IRQHandler                     ; 27  SPIM2
                DCD     SPIS1_IRQHandler                     ; 28  SPIS1
                DCD     SPIS2_IRQHandler                     ; 29  SPIS2
                DCD     UART1_IRQHandler                     ; 30  UART1
                DCD     UART2_IRQHandler                     ; 31  UART2
                DCD     UART3_IRQHandler                     ; 32  UART3
                DCD     EXTI15_10_IRQHandler                 ; 33  EXTI Line 15..10
                DCD     RTCAlarm_IRQHandler                  ; 34  RTC Alarm through EXTI Line
                DCD     USBP_WKUP_IRQHandler                 ; 35  USB PIN
                DCD     I2S_IRQHandler                       ; 36  I2S
                DCD     ISO_IRQHandler                       ; 37  ISO7816
__Vectors_End

__Vectors       EQU      __vector_table
__Vectors_Size  EQU      __Vectors_End - __Vectors


                THUMB

; Reset Handler

                PUBWEAK  Reset_Handler
                SECTION  .text:CODE:REORDER:NOROOT(2)
Reset_Handler
                LDR      R0, =SystemInit
                BLX      R0
                LDR      R0, =__iar_program_start
                BX       R0


                PUBWEAK NMI_Handler
                PUBWEAK HardFault_Handler
                PUBWEAK MemManage_Handler
                PUBWEAK BusFault_Handler
                PUBWEAK UsageFault_Handler
                PUBWEAK SVC_Handler
                PUBWEAK DebugMon_Handler
                PUBWEAK PendSV_Handler
                PUBWEAK SysTick_Handler

                PUBWEAK WWDG_IRQHandler
                PUBWEAK PVD_IRQHandler
                PUBWEAK TAMPER_IRQHandler
                PUBWEAK RTC_IRQHandler
                PUBWEAK FMC_IRQHandler
                PUBWEAK RCC_IRQHandler
                PUBWEAK EXTI0_IRQHandler
                PUBWEAK EXTI1_IRQHandler
                PUBWEAK EXTI2_IRQHandler
                PUBWEAK EXTI3_IRQHandler
                PUBWEAK EXTI4_IRQHandler
                PUBWEAK DMAC1_IRQHandler
                PUBWEAK DMAC2_IRQHandler
                PUBWEAK ADC_IRQHandler
                PUBWEAK USB_IRQHandler
                PUBWEAK USB_DMA_IRQHandler
                PUBWEAK EXTI9_5_IRQHandler
                PUBWEAK TIM1_BRK_IRQHandler
                PUBWEAK TIM1_UP_IRQHandler
                PUBWEAK TIM1_TRG_COM_IRQHandler
                PUBWEAK TIM1_CC_IRQHandler
                PUBWEAK TIM2_IRQHandler
                PUBWEAK TIM3_IRQHandler
                PUBWEAK TIM4_IRQHandler
                PUBWEAK I2C1_IRQHandler
                PUBWEAK I2C2_IRQHandler
                PUBWEAK QSPI_IRQHandler
                PUBWEAK SPIM2_IRQHandler
                PUBWEAK SPIS1_IRQHandler
                PUBWEAK SPIS2_IRQHandler
                PUBWEAK UART1_IRQHandler
                PUBWEAK UART2_IRQHandler
                PUBWEAK UART3_IRQHandler
                PUBWEAK EXTI15_10_IRQHandler
                PUBWEAK RTCAlarm_IRQHandler
                PUBWEAK USBP_WKUP_IRQHandler
                PUBWEAK I2S_IRQHandler
                PUBWEAK ISO_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
HardFault_Handler
MemManage_Handler
BusFault_Handler
UsageFault_Handler
SVC_Handler
DebugMon_Handler
PendSV_Handler
SysTick_Handler

WWDG_IRQHandler
PVD_IRQHandler
TAMPER_IRQHandler
RTC_IRQHandler
FMC_IRQHandler
RCC_IRQHandler
EXTI0_IRQHandler
EXTI1_IRQHandler
EXTI2_IRQHandler
EXTI3_IRQHandler
EXTI4_IRQHandler
DMAC1_IRQHandler
DMAC2_IRQHandler
ADC_IRQHandler
USB_IRQHandler
USB_DMA_IRQHandler
EXTI9_5_IRQHandler
TIM1_BRK_IRQHandler
TIM1_UP_IRQHandler
TIM1_TRG_COM_IRQHandler
TIM1_CC_IRQHandler
TIM2_IRQHandler
TIM3_IRQHandler
TIM4_IRQHandler
I2C1_IRQHandler
I2C2_IRQHandler
QSPI_IRQHandler
SPIM2_IRQHandler
SPIS1_IRQHandler
SPIS2_IRQHandler
UART1_IRQHandler
UART2_IRQHandler
UART3_IRQHandler
EXTI15_10_IRQHandler
RTCAlarm_IRQHandler
USBP_WKUP_IRQHandler
I2S_IRQHandler
ISO_IRQHandler
                B        .


                END
