#include "UART_STDIO.h"


void UART_Init(uint32_t clk_rate){
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)){}
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0)){}
  GPIOPinConfigure(GPIO_PA0_U0RX);
  GPIOPinConfigure(GPIO_PA1_U0TX);
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
  
  UARTConfigSetExpClk(UART0_BASE,               // Configuring UART0
                     SysCtlClockGet(), clk_rate,  // Setting UART0 Baudrate based on the bus clock available
                     (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE)); // 8-bits, one stop bit, no parity bit
}

void UART_printf(const char *pcString){
  uint32_t ui32Idx;
     for(ui32Idx = 0;(pcString[ui32Idx] != '\0'); ui32Idx++)
        {
          UARTCharPut(UART0_BASE,pcString[ui32Idx]);
        }
}

void UART_printInt(int num){
    int counter = 0;
    while(num >= 16){
        num -= 16;
        counter++;
    }

    UARTCharPut(UART0_BASE, (char)(counter+48));
    UARTCharPut(UART0_BASE, (char)(num+48));
}

