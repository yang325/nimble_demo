/* Includes ------------------------------------------------------------------*/

#include <stdarg.h>
#include "stm32f1xx_hal.h"

/* Private define ------------------------------------------------------------*/

#define CONSOLE_BUFFER_LEN                             128

/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef console_handle;

/* Exported functions --------------------------------------------------------*/


/* Console init function */
void console_init(void)
{
  console_handle.Instance = USART1;
  console_handle.Init.BaudRate = 115200;
  console_handle.Init.WordLength = UART_WORDLENGTH_8B;
  console_handle.Init.StopBits = UART_STOPBITS_1;
  console_handle.Init.Parity = UART_PARITY_NONE;
  console_handle.Init.Mode = UART_MODE_TX_RX;
  console_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  console_handle.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&console_handle) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * Prints the specified format string to the console.
 *
 * @return                      The number of characters that would have been
 *                                  printed if the console buffer were
 *                                  unlimited.  This return value is analogous
 *                                  to that of snprintf.
 */
int console_printf(const char *fmt, ...)
{
    va_list args;
    int num_chars;
    char buffer[CONSOLE_BUFFER_LEN];

    va_start(args, fmt);
    num_chars = vsnprintf(buffer, CONSOLE_BUFFER_LEN, fmt, args);
    va_end(args);

    if (num_chars > 0)
    {
      num_chars = (num_chars > CONSOLE_BUFFER_LEN) ? CONSOLE_BUFFER_LEN : num_chars;
    }

    return num_chars;
}

