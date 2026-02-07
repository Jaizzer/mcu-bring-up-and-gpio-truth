#include <stdint.h>
#define PB3 3
#define PA0 0
#define PB10 10

#define GPIOA 0x40020000
#define GPIOB 0x40020400
#define GET_REGISTER(BASE, OFFSET) (*(volatile uint32_t *)(BASE + OFFSET))
#define GPIOA_MODER GET_REGISTER(GPIOA, 0x00)
#define GPIOA_AFRL GET_REGISTER(GPIOA, 0x20)
#define GPIOB_MODER GET_REGISTER(GPIOB, 0x00)
#define GPIOB_AFRL GET_REGISTER(GPIOB, 0x20)
#define GPIOB_AFRH GET_REGISTER(GPIOB, 0x24)

#define TIM2 0x40000000
#define TIM2_CR1 GET_REGISTER(TIM2, 0x00)
#define TIM2_CCMR1 GET_REGISTER(TIM2, 0x18)
#define TIM2_CCMR2 GET_REGISTER(TIM2, 0x1c)
#define TIM2_CCER GET_REGISTER(TIM2, 0x20)
#define TIM2_CCR1 GET_REGISTER(TIM2, 0x34)
#define TIM2_CCR2 GET_REGISTER(TIM2, 0x38)
#define TIM2_CCR3 GET_REGISTER(TIM2, 0x3c)
#define TIM2_PSC GET_REGISTER(TIM2, 0x28)
#define TIM2_CNT GET_REGISTER(TIM2, 0x24)
#define TIM2_ARR GET_REGISTER(TIM2, 0x2c)
#define TIM2_EGR GET_REGISTER(TIM2, 0x14)

#define RCC 0x40023800
#define RCC_CR GET_REGISTER(RCC, 0x00)
#define RCC_PLLCFGR GET_REGISTER(RCC, 0x04)
#define RCC_CFGR GET_REGISTER(RCC, 0x08)
#define RCC_AHB1ENR GET_REGISTER(RCC, 0x30)
#define RCC_APB1ENR GET_REGISTER(RCC, 0x40)

#define FLASH 0x40023C00
#define FLASH_ACR GET_REGISTER(FLASH, 0x00)

int main(void) {
    // Enable GPIOB and GPIO A for PA0, PB3, PB10
    RCC_AHB1ENR |= (1U << 0);
    RCC_AHB1ENR |= (1U << 1);

    // Turn on the HSE Clock
    RCC_CR |= (1U << 16);

    // Wait for the HSE to stabilize by checking if HSE RDY (bit 17 of RCC_CR) == 1
    while (!(RCC_CR & (1U << 17))) {
    }

    // Choose the HSE as the PLL clock source
    // Set PLLSRC to 1
    RCC_PLLCFGR &= ~(1U << 22);
    RCC_PLLCFGR |= (1U << 22);

    // Shrink the HSE signal to 1MHz by dividing 8MHz by 8 since HSE runs at 8MHz
    // Set the PLLM3 to 8 which is 1000 in binary
    RCC_PLLCFGR &= ~(63U << 0);
    RCC_PLLCFGR |= (8U << 0);

    // Multiply the signal by 192 to obtain 192MHz frequency
    // Set the PLLN value to 192 which is 1100 0000 in binary
    RCC_PLLCFGR &= ~(511U << 6);
    RCC_PLLCFGR |= (192U << 6);

    // Divide the signal by 2 to obtain 96MHz frequency for the CPU
    // Set the PLLP value to 2 which maps to 00 in PLLP table of values
    RCC_PLLCFGR &= ~(3U << 16);

    // Divide the signal by 4 to obtain 48MHz frequency for the USB
    // Set the PLLQ value to 4 which is 100 in binary
    RCC_PLLCFGR &= ~(15U << 24);
    RCC_PLLCFGR |= (4U << 24);

    // Turn on the PLL
    RCC_CR |= (1U << 24);

    // Wait for the PLL to stabilize by checking if PLL RDY (bit 25 of RCC_CR) == 1
    while (!(RCC_CR & (1U << 25))) {
    }

    // Add latency of 3 wait states before switching from 16Hz to 96Hz
    FLASH_ACR &= ~(7U << 0);
    FLASH_ACR |= (3U << 0);

    // Set the PLL as the system clock
    RCC_CFGR &= ~(3U << 0);
    RCC_CFGR |= 2U << 0;

    // Wait for the PLL clock to get set as the system clock
    while (!(RCC_CFGR & (2U << 2))) {
    }

    // Divide the clock by 2 to get 48Hz since APB1's maximum speed should 50Hz
    RCC_CFGR |= (4U << 10);

    // Enable TIMER 2
    RCC_APB1ENR |= (1U << 0);

    // Set PA0, PB3, PB10 to alternating function since they will be controlled by the Timer 2
    GPIOB_MODER &= ~((3U << PB3 * 2) | (3U << PB10 * 2));
    GPIOB_MODER |= (2U << PB3 * 2) | (2U << PB10 * 2);
    GPIOA_MODER &= ~((3U << PA0 * 2));
    GPIOA_MODER |= (2U << PA0 * 2);

    // Set the alternating functions of PA0, PB3, PB10 to AF1  which is the Timer 2
    GPIOA_AFRL &= ~(15U << 0);
    GPIOA_AFRL |= (1U << 0);
    GPIOB_AFRL &= ~(15U << 12);
    GPIOB_AFRL |= (1U << 12);
    GPIOB_AFRH &= ~(15U << 8);
    GPIOB_AFRH |= (1U << 8);

    // Slow down the clock to 10KHz by dividing 96MHz by (9599 + 1)
    TIM2_PSC = 9599U;

    // Reset the counter every 4 second by setting ARR to 39999 + 1
    TIM2_ARR = 39999U;

    // Set the CCR1 value to 2 second for PA0
    TIM2_CCR1 = 10000U;

    // Set the CCR2 value to 2 second for PB3
    TIM2_CCR2 = 20000U;

    // Set the CCR3 value to 3 second for PB10
    TIM2_CCR3 = 30000U;

    // Enable channel 1, 2, and 3 for PA0, PB3, and PB10
    TIM2_CCER |= (1U << 0) | (1U << 4) | (1U << 8);

    // Set channel 2, 3 and 4 to output mode
    TIM2_CCMR1 &= ~(3U << 8);
    TIM2_CCMR2 &= ~((3U << 0) | (3U << 8));

    // Turn off the signal once the timer exceeds the assigned time for that channel pin
    TIM2_CCMR1 &= ~((7U << 4) | (7U << 12));
    TIM2_CCMR1 |= (6U << 4) | (6U << 12);
    TIM2_CCMR2 &= ~(7U << 4);
    TIM2_CCMR2 |= (6U << 4);

    // Enable preload to prevent glitches
    TIM2_CCMR1 |= (1U << 11) | (1U << 3);
    ;
    TIM2_CCMR2 |= 1U << 3;

    // Force an update to load the PSC and ARR values immediately
    TIM2_EGR |= (1U << 0);

    // Enable the clock
    TIM2_CR1 |= (1U << 0);

    return 0;
}