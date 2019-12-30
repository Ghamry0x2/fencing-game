#define EN 1 << 4
#define RW 1 << 3
#define RS 1 << 2

//input
#define COLS            0x0f
//output
#define ROWS            0xf0

#define rightSwitch     0x01
#define leftSwitch      1u << 4

#define redLed          1u << 1
#define blueLed         1u << 2
#define greenLed        1u << 3

void switchSetup(){
    //port f
    SYSCTL_RCGCGPIO_R |= (0x20);
    uint32_t dummy = SYSCTL_RCGCGPIO_R;

    GPIO_PORTF_LOCK_R = 0x4c4f434b;
    GPIO_PORTF_CR_R = 0x01f;

    GPIO_PORTF_DEN_R |= (leftSwitch | rightSwitch);
    GPIO_PORTF_DIR_R &= ~(leftSwitch | rightSwitch);
    GPIO_PORTF_PUR_R |= (leftSwitch | rightSwitch);

}

int leftSwitchPressed(){
    return !(GPIO_PORTF_DATA_R & leftSwitch);
}

int rightSwitchPressed(){
    return !(GPIO_PORTF_DATA_R & rightSwitch);
}

void LCDSetup(){
    //port b a
    SYSCTL_RCGCGPIO_R |= (0x03);
    uint32_t dummy = SYSCTL_RCGCGPIO_R;

    GPIO_PORTB_DIR_R |= 0xff;
    GPIO_PORTA_DIR_R |= (EN | RW | RS);


    GPIO_PORTB_DEN_R |= 0xff;
    GPIO_PORTA_DEN_R |= (EN | RW | RS);
}

void onBoardLedsSetup(int r, int g, int b){
    r = r << 1;
    b = b << 2;
    g = g << 3;

    //port f
    SYSCTL_RCGCGPIO_R |= (0x20);
    uint32_t dummy = SYSCTL_RCGCGPIO_R;

    GPIO_PORTF_LOCK_R = 0x4c4f434b;
    GPIO_PORTF_CR_R = 0x01f;

    GPIO_PORTF_DEN_R |= (r | g | b);
    GPIO_PORTF_DIR_R |= (r | g | b);
}

void startTimer() {
    TIMER0_CTL_R = 0x1;

    while((TIMER0_RIS_R & 0x1) == 0);
    TIMER0_ICR_R = 0x1;

    TIMER0_CTL_R = 0x0;
}

void timerSetup(int t){
    //timer setup
    SYSCTL_RCGCTIMER_R |= 0x1;

    TIMER0_CTL_R = 0x0;

    TIMER0_CFG_R = 0x0;
    TIMER0_TAMR_R = 0x1;
    TIMER0_TAILR_R = 16000 * t;

}

void keypadSetup(){
    //port c e
    SYSCTL_RCGCGPIO_R |= (0x14);
    uint32_t dummy = SYSCTL_RCGCGPIO_R;

    GPIO_PORTC_DIR_R |= ROWS;
    GPIO_PORTE_DIR_R &= ~COLS;

    GPIO_PORTE_PUR_R |= COLS;

    GPIO_PORTC_ODR_R |= ROWS;

    GPIO_PORTC_DEN_R |= ROWS;
    GPIO_PORTE_DEN_R |= COLS;
}

void setPortBGPIO(int pin0, int pin1){
    //port b
    SYSCTL_RCGCGPIO_R |= 0x02;
    uint32_t dummy = SYSCTL_RCGCGPIO_R;

    GPIO_PORTB_DEN_R |= ( pin0 | pin1 );
    GPIO_PORTB_DIR_R |= ( pin0 | pin1 );

    GPIO_PORTB_DATA_R = 0x03;
}

// Assumes a 16 MHz bus clock, creates 115200 baud rate
void setUART(){
  SYSCTL_RCGCUART_R |= 0x02;        // activate UART1
  SYSCTL_RCGCGPIO_R |= 0x02;        // activate port B
  UART1_CTL_R &= ~0x0001;           // disable UART1

  //IBRD=int(16000000/(16*115,200)) = int(8.6805555556)
  UART1_IBRD_R = 8;

  //FBRD = round(0.6805555556 * 64) = 44
  UART1_FBRD_R = 44;

  UART1_LCRH_R = 0x0070;        // 8-bit length, enable FIFO
  UART1_CTL_R = 0x0301;         // enable RXE, TXE and UART
  GPIO_PORTB_AFSEL_R |= 0x03;   // alt funct on PB1-0
  GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R&0xFFFFFF00)+0x00000011;
  GPIO_PORTB_DEN_R |= 0x03;   // digital I/O on PB1-0
  GPIO_PORTB_AMSEL_R &= ~0x03; // No analog on PB1-0
}

uint8_t UARTGetBW() {
  while((UART1_FR_R & 0x0010) != 0);
  // wait until RXFE is 0
  return((uint8_t)(UART1_DR_R&0xFF));
}

void UARTSendBW(uint8_t data) {
  while((UART1_FR_R & 0x0020) != 0);
  // wait until TXFF is 0
  UART1_DR_R = data;
}



