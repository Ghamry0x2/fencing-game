#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

#include "nrf24l01.h"

#include "tm4c123gh6pm.h"
#include "delay.h"
#include "reg.h"


struct nrf24l01p nrf;
uint8_t addresses[][6] = {"1Node","2Node"};

#define player01        1u
#define player02        1u << 1

void running(){
    while(1) {
        if (nrf24l01p_available(&nrf)) {
            uint8_t payload;

            //get the payload
            while (nrf24l01p_available(&nrf)) {
                nrf24l01p_read(&nrf, &payload, sizeof(payload));
            }

            if(payload == 0x37){
                GPIO_PORTB_DATA_R &= ~(0x01);
                delayTimer(1);
                GPIO_PORTB_DATA_R |= 0x01;
                delayTimer(1);
            }
            else if(payload == 0x27){
                GPIO_PORTB_DATA_R &= ~(0x02);
                delayTimer(1);
                GPIO_PORTB_DATA_R |= 0x02;
                delayTimer(1);
            }

        }

        //give it a little space
        SysCtlDelay((SysCtlClockGet() >> 12) * 5);
    }
}

int main(void) {
	//
	// Enable lazy stacking for interrupt handlers.  This allows floating-point
	// instructions to be used within interrupt handlers, but at the expense of
	// extra stack usage.
	//
	FPULazyStackingEnable();
	FPUEnable();

	//
	// Set the clocking to run directly from the crystal.
	// Clock to 80MHZ
	//
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
	IntMasterEnable();

	//set up the radio
	if( !nrf24l01p_setup(&nrf, GPIO_PORTC_BASE, GPIO_PIN_4, SSI0_BASE) ){
		//we couldn't communicate with the radio
		return 1;
	}

	nrf24l01p_set_PA_level(&nrf, RF24_PA_MAX);

	nrf24l01p_open_reading_pipe(&nrf, 1, addresses[0]);
	nrf24l01p_open_reading_pipe(&nrf, 2, addresses[1]);

	nrf24l01p_start_listening(&nrf);

	setPortBGPIO(player01, player02);

	running();

	return 0;
}
