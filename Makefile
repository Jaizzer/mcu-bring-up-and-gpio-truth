blinky:
	arm-none-eabi-gcc -c -g -mcpu=cortex-m4 -mthumb -ffreestanding main.o main.c
	arm-none-eabi-gcc -o application.elf -T linker.ld -nostdlib -nostartfiles main.o
	arm-none-eabi-objcopy -O binary application.elf application.bin