#pragma once

void sspi_init(uint sck, uint so, uint si);
void sspi_setbaud(uint br);
void sspi_ssinit(uint pin);
void sspi_start(uint ss);
void sspi_stop(uint ss);
void sspi_send(char byte);
char sspi_recv();

void ssd1306_init();
void ssd1306_clear(char color);
void ssd1306_setcursor(char page, char column);
void ssd1306_putc(char c);
void ssd1306_print(char *s);
void ssd1306_write(char x, char y, char byte);

char flash_rdstatus();
void flash_wrstatus(char s);
uint flash_read(uint addr, char *buff, uint len);
uint flash_write(uint addr, char *buff, uint len);

void lcd1602_control(char on, char cursor, char blink);
void lcd1602_cgram_addr(char adr);
void lcd1602_ddram_addr(char adr);
void lcd1602_ramwrite(char c);
void lcd1602_clear();
void lcd1602_putc(char c);
void lcd1602_print(char *s);
void lcd1602_setcursor(char line, char col);
void lcd1602_init(uint p_rs, uint p_rw, uint p_en,
					uint p_d7, uint p_d6, uint p_d5, 
					uint p_d4);
