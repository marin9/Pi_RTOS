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

#define GPIO_IN			0
#define GPIO_OUT		1
#define GPIO_FN0		4
#define GPIO_FN1		5
#define GPIO_FN2		6
#define GPIO_FN3		7
#define GPIO_FN4		3
#define GPIO_FN5		2
#define GPIO_PULLOFF	(0 << 4)
#define GPIO_PULLDOWN	(1 << 4)
#define GPIO_PULLUP		(2 << 4)
#define GPIO_NOEDG      0
#define GPIO_RSEDG		1
#define GPIO_FLEDG		2
#define GPIO_HGEDG	    3
#define GPIO_LWEDG		4
#define GPIO_COUNT 		60

#define IRQ_TMR1		1
#define IRQ_TMR3 		3
#define IRQ_GPIO0  		49
#define IRQ_GPIO1  		50
#define IRQ_GPIO2  		51
#define IRQ_GPIO3  		52
#define IRQ_COUNT		72

#define CPSR_MODE_FIQ	0x11
#define CPSR_MODE_IRQ	0x12
#define CPSR_MODE_SVC	0x13
#define CPSR_MODE_ABT	0x17
#define CPSR_MODE_UND	0x1b
#define CPSR_MODE_SYS	0x1f
#define CPSR_FLAG_T		0x20
#define CPSR_FLAG_F		0x40
#define CPSR_FLAG_I		0x80


typedef unsigned int uint;

void cpu_switch(void *prev, void *next);

void pic_init();
void pic_register(uint irq, void (*handler)());
void pic_enable(uint irq);
void pic_disable(uint irq);
uint pic_block();
void pic_unblock();

void timer_init();
uint timer_get();
void timer_set(uint us);
void timer_delay(uint us);

void uart_init(uint br);
uint uart_getc();
void uart_putc(char c);
void uart_print(char *s);
void uart_read(char* buff, uint len);
void uart_write(char* buff, uint len);
void uart_hex(uint n);
void uart_flush();

void gpio_mode(uint pin, int mode);
void gpio_write(uint pin, int val);
uint gpio_read(uint pin);
void gpio_attachintr(uint pin, void (*handler)(), int edge);
void gpio_irq_handler();

void i2c_init(uint freq);
int i2c_read(uint id, char *buff, uint len);
int i2c_write(uint id, char *buff, uint len);

void spi_init(uint freq);
void spi_begin();
void spi_end();
void spi_readwrite(char *buff, uint len);
