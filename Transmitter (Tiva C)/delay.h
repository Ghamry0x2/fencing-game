#ifndef DELAY_H_
#define DELAY_H_

void delayTimer(int t){
    timerSetup(t);
    startTimer();
}

void delayMs(int t){
    NVIC_ST_CTRL_R = 0;
    NVIC_ST_RELOAD_R = 16000 * t - 1;
    NVIC_ST_CTRL_R = 5;

    while((NVIC_ST_CTRL_R & 0x10000) == 0);
}

void delayUs(int t){
    NVIC_ST_CTRL_R = 0;
    NVIC_ST_RELOAD_R = 16 * t - 1;
    NVIC_ST_CTRL_R = 5;

    while((NVIC_ST_CTRL_R & 0x10000) == 0);
}

void delay(int t){
    int sec = t/1000;
    int rem = t%1000;

    int i;
    for(i=0; i<sec; i++)
        delayMs(1000);

    delayMs(rem);

}

#endif /* DELAY_H_ */
