#include <stdint.h>
#define RCC_BASE 0x40023800
#define RCC_AHB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define GPIO_AEN (1 << 0)
#define GPIOA 0x40020000
#define PIN 5
#define GPIO_MODER(x) (*(volatile uint32_t *)(x + 0x00))
#define GPIO_MODER_MASK(x) (1 << (x * 2))
#define GPIO_OSPEEDR(x) (*(volatile uint32_t *)(x + 0x08))
#define GPIO_OSPEEDR_MASK(x) (0x01 << (x * 2))
#define GPIO_BSRR(x) (*(volatile uint32_t *)(x + 0x18))

int main(void) {
    // Enable clock to GPIO A
    RCC_AHB1ENR |= GPIO_AEN;

    // Set bit 11 and 10 to 01 to enable output mode on pin 5
    GPIO_MODER(GPIOA) |= GPIO_MODER_MASK(PIN);

    // Set the slew rate to medium
    GPIO_OSPEEDR(GPIOA) |= GPIO_OSPEEDR_MASK(PIN);

    for (;;) {
        GPIO_BSRR(GPIOA) = (1 << PIN);
        for (uint32_t i = 0; i < 400000; i++) {
            __asm__ volatile("nop");
        }
        GPIO_BSRR(GPIOA) = (1 << PIN + 16);
        for (uint32_t i = 0; i < 50000; i++) {
            __asm__ volatile("nop");
        }
    }

    return 0;
}