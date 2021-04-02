# Raspberry Pi Zero minimal RTOS

Simple real-time operating system for Raspberry Pi Zero.

**Features:**

OS:
- Priority preemptive scheduler
- Binary and counting semaphore
- Mailbox

Peripherals:
- GPIO
- UART
- SPI
- I2C
- PWM
- Timer

External devices:
- SPI Flash memory
- Display ssd1306


**Usage:**

    1) Write your code into main.c file
    2) OS configuration parameters are in os.h file
    3) make
    4) Copy to SD: bootcode.bin, start.elf and kernel.img
