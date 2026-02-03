GPIO_MODER, set bit 11 to 0 and bit 10 to 1
GPIO_OTYPER, set bit 5 to 0 
GPIO_OSPEEDR, set bit 11 to 0 and bit 10 to 1
GPIO_ODR, set bit 5 to 1
GPIO BSRR, set bit 5 to 1 and bit 21 to 1

Goal: Light the LED light for 1 second and turn it off for 3 seconds 
RCC_CR will enable HSE and turn on PLL
RCC_PLLCFGR
    PLLM will shrink the signal to 1MHz
    PLLN will multiply the signal to 100 to Gain 100MHz
    PLLQ wil divide the 100MHz by 2 to gain 50Hz for the USB
    PLLP will divide the 100MHz by 1 to retain 100MHz
    PLLSRC will use HSE

RCC_CFGR
    SW will use PLL as the clock source
    HPRE will divide by 1 to keep the 100MHz signal for the CPU and RAM
    PPRE1 will divide by 2 to gain 50MHz for APB1 from crashing
    PPRE2 divide by 1 so that the timer will still receive the 100MHz


TIMER Input: 1,000,000Hz
PSC: 1,000,000Hz / (9999 + 1) = 100Hz, PSC = 9999
ARR: 100 x 400 = 4, ARR = 399
CCR1: 100 x 1 = 100, CCR1 = 99


MUCH BETTER Approach: Use CCMR and CCER instead of IF statements
FOR(;;) {
    IF (CNT == 0) {
        TURN ON PA5
    }

    IF (CNT == CRCR) {
        TURN OFF PA5
    }
}
