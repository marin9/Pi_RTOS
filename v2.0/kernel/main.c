


void uart_print(char *s){
	volatile unsigned int *DR=(unsigned int*)(0x101f1000+0x00);
	volatile unsigned int *FR=(unsigned int*)(0x101f1000+0x00);
	while(*s){
		while(((*FR)&(1<<5)));
		*DR=(unsigned int)(*s++);
	}
}




void startup(){



}

/*



#define KMI_KB_BASE	            0x10006000	// keyboard
#define KMI_MS_BASE	            0x19000000	// mouse

typedef struct _KMI_MMIO {
	unsigned int		cr;		// control register (rw)
	unsigned int		stat;		// status register (r)
	unsigned int		data;		// data register (rw)
	unsigned int		clk;		// clock divisor register (rw)
	unsigned int		ir;		// interrupt control register (r)
} KMI_MMIO;

volatile KMI_MMIO *mmio;
unsigned int tmp;




void startup(){
	stdio_init();

	*(volatile unsigned int *)(0x1000001C) = 0x2CAC; /* timing magic for SVGA 800x600 */
	*(volatile unsigned int *)(0x10120000) = 0x1313A4C4;
	*(volatile unsigned int *)(0x10120004) = 0x0505F657;
	*(volatile unsigned int *)(0x10120008) = 0x071F1800;
	*(volatile unsigned int *)(0x10120010) = (1 * 1024 * 1024); /* base addr of frame buffer */
	*(volatile unsigned int *)(0x10120018) = 0x82b; /* control bits */
	int i;
	volatile unsigned int *p=*(volatile unsigned int *)(0x10120010);
	for(i=0;i<1*1024*1024;++i){
		p[i]=0x100010;
	}


	
	mmio=(KMI_MMIO*)KMI_KB_BASE;
mmio->cr = 0x14;
mmio->clk = 15;

	while(1){
		for(i=0;i<50000000;++i) asm volatile("");
		printf("data: %x\nstat: %x\n\n", mmio->data, mmio->stat);
	}
	while(1);	

*/
