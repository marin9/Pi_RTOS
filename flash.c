#include "rpi.h"

#define CMD_WRSR	0x01
#define CMD_WRITE	0x02
#define CMD_READ	0x03
#define CMD_WRDI	0x04
#define CMD_RDSR	0x05
#define CMD_WREN	0x06

#define PAGE_LEN    256


uint flash_write(uint addr, char *buff, uint len) {
    char cmd[4];

    len = (len > PAGE_LEN) ? PAGE_LEN : len;

    // ready?
    do {
        cmd[0] = CMD_RDSR;
        spi_begin();
        spi_readwrite(cmd, 2);
        spi_end();
    } while (cmd[1] & 0x01);

    // write enable
    cmd[0] = CMD_WREN;
    spi_begin();
    spi_readwrite(cmd, 1);
    spi_end();

    // write data
    cmd[0] = CMD_WRITE;
    cmd[1] = (addr >> 16) & 0xff;
    cmd[2] = (addr >> 8) & 0xff;
    cmd[3] = (addr >> 0) & 0xff;
    spi_begin();
    spi_readwrite(cmd, 4);
    spi_readwrite(buff, len);
    spi_end();
    return len;
}

uint flash_read(uint addr, char *buff, uint len) {
    char cmd[4];

    len = (len > PAGE_LEN) ? PAGE_LEN : len;

    // ready ?
    do {
        cmd[0] = CMD_RDSR;
        spi_begin();
        spi_readwrite(cmd, 2);
        spi_end();
    } while (cmd[1] & 0x01);

    // read data
    cmd[0] = CMD_READ;
    cmd[1] = (addr >> 16) & 0xff;
    cmd[2] = (addr >> 8) & 0xff;
    cmd[3] = (addr >> 0) & 0xff;
    spi_begin();
    spi_readwrite(cmd, 4);
    spi_readwrite(buff, len);
    spi_end();
    return len;
}
