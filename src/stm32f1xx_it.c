/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/

#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "stm32f1xx_it.h"
#include "hal/hal_uart.h"

/* External functions --------------------------------------------------------*/


/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
 * @brief This function handles Hard fault interrupt.
 */
void prvGetRegistersFromStack(uint32_t * pulFaultStackAddress)
{
  console_printf("R0 = 0x%08x\n", pulFaultStackAddress[0]);
  console_printf("R1 = 0x%08x\n", pulFaultStackAddress[1]);
  console_printf("R2 = 0x%08x\n", pulFaultStackAddress[2]);
  console_printf("R3 = 0x%08x\n", pulFaultStackAddress[3]);

  console_printf("R12 = 0x%08x\n", pulFaultStackAddress[4]);
  console_printf("LR = 0x%08x\n", pulFaultStackAddress[5]);
  console_printf("PC = 0x%08x\n", pulFaultStackAddress[6]);
  console_printf("PSR = 0x%08x\n", pulFaultStackAddress[7]);

  assert(0);
}

/**
 * The fault handler implementation calls a function called
 * prvGetRegistersFromStack().
 */
void HardFault_Handler(void)
{
  __asm volatile
  (
    " tst lr, #4                                                \n"
    " ite eq                                                    \n"
    " mrseq r0, msp                                             \n"
    " mrsne r0, psp                                             \n"
    " ldr r1, [r0, #24]                                         \n"
    " ldr r2, handler2_address_const                            \n"
    " bx r2                                                     \n"
    " handler2_address_const: .word prvGetRegistersFromStack    \n"
  );
}

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler(void)
{
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
 * @brief This function handles Prefetch fault, memory access fault.
 */
void BusFault_Handler(void)
{
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void)
{
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
 * @brief This function handles Debug monitor.
 */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */
  /* USER CODE END DebugMonitor_IRQn 0 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
  * @brief  This function handles USART interrupt request.
  * @param  None
  * @retval None
  */
void USART3_IRQHandler(void)
{
  HAL_UART_IRQHandler(&uarts[0].u_regs);
}

void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&uarts[1].u_regs);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
