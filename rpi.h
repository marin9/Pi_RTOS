#pragma once

#define PERIPH_BASE		0x20000000
#define SYSTMR_BASE		(PERIPH_BASE + 0x003000)
#define IRQ_BASE		(PERIPH_BASE + 0x00B000)
#define MBOX_BASE		(PERIPH_BASE + 0x00B800)
#define GPIO_BASE		(PERIPH_BASE + 0x200000)
#define UART0_BASE		(PERIPH_BASE + 0x201000)
#define SPI_BASE 		(PERIPH_BASE + 0x204000)
#define I2C_BASE 		(PERIPH_BASE + 0x205000)
#define PWM_BASE		(PERIPH_BASE + 0x20C000)

// GPIO
#define GPIO_IN			    0
#define GPIO_OUT			1
#define GPIO_FN0			4
#define GPIO_FN1			5
#define GPIO_FN2			6
#define GPIO_FN3			7
#define GPIO_FN4			3
#define GPIO_FN5			2
#define GPIO_PULLOFF		(0 << 4)
#define GPIO_PULLDOWN		(1 << 4)
#define GPIO_PULLUP		    (2 << 4)
#define GPIO_NO_EDGE        0
#define GPIO_RISING_EDGE	1
#define GPIO_FALLING_EDGE	2
#define GPIO_HIGH_EDGE	    3
#define GPIO_LOW_EDGE		4

// Interrupts
#define SYSTMR1_IRQ		1
#define SYSTMR3_IRQ 	3
#define GPIO_IRQ_0  	49
#define GPIO_IRQ_1  	50
#define GPIO_IRQ_2  	51
#define GPIO_IRQ_3  	52
#define IRQ_COUNT		72

// Processor
#define CPSR_MODE_FIQ	0x11
#define CPSR_MODE_IRQ	0x12
#define CPSR_MODE_SVC	0x13
#define CPSR_MODE_ABT	0x17
#define CPSR_MODE_UND	0x1b
#define CPSR_MODE_SYS	0x1f
#define CPSR_FLAG_T		0x20
#define CPSR_FLAG_F		0x40
#define CPSR_FLAG_I		0x80

#define ENABLE_INTR()	cpu_interrupts(0)
#define DISABLE_INTR()	cpu_interrupts(1)

typedef unsigned int uint;


uint cpu_interrupts(uint eif);
void cpu_switch(void *prev, void *next);

void pic_init();
void pic_register(uint irqn, void (*handler)());
void pic_unregister(uint irqn);

void timer_init();
uint timer_get();
void timer_set(uint us);

void uart_init();
uint uart_getc();
void uart_putc(char c);
void uart_print(char *s);
void uart_hex(uint n);
void uart_flush();

void spi_init();
void spi_begin();
void spi_end();
void spi_readwrite(char *buff, uint len);

void i2c_init();
int i2c_read(uint id, char *buff, uint len);
int i2c_write(uint id, char *buff, uint len);

void pwm_init();
int pwm_write(char *buff, uint len);

void gpio_mode(uint pin, int mode);
void gpio_write(uint pin, int val);
uint gpio_read(uint pin);
void gpio_enint(uint pin, void (*h)(), int edge);
void gpio_irq_handler();


uint flash_read(uint addr, char *buff, uint len);
uint flash_write(uint addr, char *buff, uint len);
