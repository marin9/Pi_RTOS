#pragma once

#define KEY_PRESSED				1
#define KEY_PRESSED_AND_HELD	2
#define KEY_RELEASED			3

int kb_init();
int kb_read(char *key, char *stat, char *caps);
int kb_setbacklight(char b);

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
