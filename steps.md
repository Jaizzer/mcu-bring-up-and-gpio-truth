GPIO_MODER, set bit 11 to 0 and bit 10 to 1
GPIO_OTYPER, set bit 5 to 0 
GPIO_OSPEEDR, set bit 11 to 0 and bit 10 to 1
GPIO_ODR, set bit 5 to 1
GPIO BSRR, set bit 5 to 1 and bit 21 to 1

Goal: Light the LED light for 1 second and turn it off for 3 seconds 

TIMER Input: 1,000,000Hz
PSC: 1,000,000Hz / (9999 + 1) = 100Hz, PSC = 9999
ARR: 100 x 400 = 4, ARR = 399
CCR1: 100 x 1 = 100, CCR1 = 99