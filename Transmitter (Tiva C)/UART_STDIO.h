#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"


void UART_Init(uint32_t clk_rate);
void UART_printf(const char *pcString);
void UART_printInt(int num);
