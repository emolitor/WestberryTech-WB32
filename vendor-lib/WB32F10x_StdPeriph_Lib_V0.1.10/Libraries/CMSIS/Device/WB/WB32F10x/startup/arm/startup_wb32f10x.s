;/**************************************************************************//**
; * @file     startup_wb32f10x.s
; * @brief    CMSIS Core Device Startup File for
; *           WB32F10x Device Series
; * @version  V0.1.10
; * @date     05-January-2023
; *
; * @note
; *
; ******************************************************************************/
;/* Copyright (c) 2020 - 2023 Westberry Technology (ChangZhou) Corp., Ltd. All rights reserved.
;  
;   SPDX-License-Identifier: Apache-2.0
;  
;   Licensed under the Apache License, Version 2.0 (the License); you may
;   not use this file except in compliance with the License.
;   You may obtain a copy of the License at
;  
;   www.apache.org/licenses/LICENSE-2.0
;  
;   Unless required by applicable law or agreed to in writing, software
;   distributed under the License is distributed on an AS IS BASIS, WITHOUT
;   WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
;   See the License for the specific language governing permissions and
;   limitations under the License.
;   ---------------------------------------------------------------------------*/
;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000100

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     WWDG_IRQHandler           ;  0: Window Watchdog
                DCD     PVD_IRQHandler            ;  1: PVD through EXTI Line detect
                DCD     TAMPER_IRQHandler         ;  2: Tamper
                DCD     RTC_IRQHandler            ;  3: RTC
                DCD     FMC_IRQHandler            ;  4: FMC
                DCD     RCC_IRQHandler            ;  5: RCC
                DCD     EXTI0_IRQHandler          ;  6: EXTI Line 0
                DCD     EXTI1_IRQHandler          ;  7: EXTI Line 1
                DCD     EXTI2_IRQHandler          ;  8: EXTI Line 2
                DCD     EXTI3_IRQHandler          ;  9: EXTI Line 3
                DCD     EXTI4_IRQHandler          ; 10: EXTI Line 4
                DCD     DMAC1_IRQHandler          ; 11: DMAC1
                DCD     DMAC2_IRQHandler          ; 12: DMAC2
                DCD     ADC_IRQHandler            ; 13: ADC
                DCD     USB_IRQHandler            ; 14: USB
                DCD     USB_DMA_IRQHandler        ; 15: USB DMA
                DCD     EXTI9_5_IRQHandler        ; 16: EXTI Line 9..5
                DCD     TIM1_BRK_IRQHandler       ; 17: TIM1 Break
                DCD     TIM1_UP_IRQHandler        ; 18: TIM1 Update
                DCD     TIM1_TRG_COM_IRQHandler   ; 19: TIM1 Trigger and Commutation
                DCD     TIM1_CC_IRQHandler        ; 20: TIM1 Capture Compare
                DCD     TIM2_IRQHandler           ; 21: TIM2
                DCD     TIM3_IRQHandler           ; 22: TIM3
                DCD     TIM4_IRQHandler           ; 23: TIM4
                DCD     I2C1_IRQHandler           ; 24: I2C1
                DCD     I2C2_IRQHandler           ; 25: I2C2
                DCD     QSPI_IRQHandler           ; 26: QSPI
                DCD     SPIM2_IRQHandler          ; 27: SPIM2
                DCD     SPIS1_IRQHandler          ; 28: SPIS1
                DCD     SPIS2_IRQHandler          ; 29: SPIS2
                DCD     UART1_IRQHandler          ; 30: UART1
                DCD     UART2_IRQHandler          ; 31: UART2
                DCD     UART3_IRQHandler          ; 32: UART3
                DCD     EXTI15_10_IRQHandler      ; 33: EXTI Line 15..10
                DCD     RTCAlarm_IRQHandler       ; 34: RTC Alarm through EXTI Line
                DCD     USBP_WKUP_IRQHandler      ; 35: USB PIN
                DCD     I2S_IRQHandler            ; 36: I2S
                DCD     ISO_IRQHandler            ; 37: ISO7816
__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler        [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler          [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  WWDG_IRQHandler           [WEAK]
                EXPORT  PVD_IRQHandler            [WEAK]
                EXPORT  TAMPER_IRQHandler         [WEAK]
                EXPORT  RTC_IRQHandler            [WEAK]
                EXPORT  FMC_IRQHandler            [WEAK]
                EXPORT  RCC_IRQHandler            [WEAK]
                EXPORT  EXTI0_IRQHandler          [WEAK]
                EXPORT  EXTI1_IRQHandler          [WEAK]
                EXPORT  EXTI2_IRQHandler          [WEAK]
                EXPORT  EXTI3_IRQHandler          [WEAK]
                EXPORT  EXTI4_IRQHandler          [WEAK]
                EXPORT  DMAC1_IRQHandler          [WEAK]
                EXPORT  DMAC2_IRQHandler          [WEAK]
                EXPORT  ADC_IRQHandler            [WEAK]
                EXPORT  USB_IRQHandler            [WEAK]
                EXPORT  USB_DMA_IRQHandler        [WEAK]
                EXPORT  EXTI9_5_IRQHandler        [WEAK]
                EXPORT  TIM1_BRK_IRQHandler       [WEAK]
                EXPORT  TIM1_UP_IRQHandler        [WEAK]
                EXPORT  TIM1_TRG_COM_IRQHandler   [WEAK]
                EXPORT  TIM1_CC_IRQHandler        [WEAK]
                EXPORT  TIM2_IRQHandler           [WEAK]
                EXPORT  TIM3_IRQHandler           [WEAK]
                EXPORT  TIM4_IRQHandler           [WEAK]
                EXPORT  I2C1_IRQHandler           [WEAK]
                EXPORT  I2C2_IRQHandler           [WEAK]
                EXPORT  QSPI_IRQHandler           [WEAK]
                EXPORT  SPIM2_IRQHandler          [WEAK]
                EXPORT  SPIS1_IRQHandler          [WEAK]
                EXPORT  SPIS2_IRQHandler          [WEAK]
                EXPORT  UART1_IRQHandler          [WEAK]
                EXPORT  UART2_IRQHandler          [WEAK]
                EXPORT  UART3_IRQHandler          [WEAK]
                EXPORT  EXTI15_10_IRQHandler      [WEAK]
                EXPORT  RTCAlarm_IRQHandler       [WEAK]
                EXPORT  USBP_WKUP_IRQHandler      [WEAK]
                EXPORT  I2S_IRQHandler            [WEAK]
                EXPORT  ISO_IRQHandler            [WEAK]

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

                B       .

                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap

__user_initial_stackheap PROC
                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR
                ENDP

                ALIGN

                ENDIF


                END
