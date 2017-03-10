#define S3C2440_MPLL_200MHZ     ((0x5c<<12)|(0x01<<4)|(0x02))
#define MEM_CTL_BASE    0x48000000

.text
.global _start
_start:
	/*1.关看门狗*/
	ldr r0, = 0x53000000
	mov r1, #0x0     //?
str r1,[r0]
	/*2.设置时钟*/
	ldr r0, = 0x4C000014
	mov r1, #0x03  //fclk:hclk:pclk=1:2:4
	/*if HDIV!=0,cpu bus mode should from "fast bus mode"change"asynchromous"*/
	mcr	p15, 0, r1, c1, c0, 0 //read register
	orr r1, r1, #0xc0000000	           // set asynchromous bus mode
	mrc	p15, 0, r1, c1, c1, 0   //write register
	/*set cpu clock = 200mhz*/
	ldr r0, = 0x4C00000  //register addr
	ldr r1, = S3C2440_MPLL_200MHZ    //register value
	str r1, [r0]
	/*3.init sdram*/
	ldr r0, = MEM_CTL_BASE      //sdram first addr
	adr r1, sdram_config    //sdram now addr
	adr r3, r0, #(13*4)
1:
	ldr r2, [r1], #4
	str r2, [r0], #4
	cmp r10, r3
	bne 1b
	/*4.renew select place*/
	ldr sp, = 0x34000000  //?
	bl nand_init

	mov r0, #0       //source addr
	ldr r1, = _start  //destination
	ldr r2，= _bss_start   // ?
	sub r2, r2, r1      //len

	bl copy_code_to_sdram
	bl clear_bss
	/*5.execution main*/
	ldr lr, = halt
	ldr pc, = main
halt:
	b halt

sdram_config:
	.long 0x22011110	 //BWSCON
	.long 0x00000700	 //BANKCON0
	.long 0x00000700	 //BANKCON1
	.long 0x00000700	 //BANKCON2
	.long 0x00000700	 //BANKCON3  
	.long 0x00000700	 //BANKCON4
	.long 0x00000700	 //BANKCON5
	.long 0x00018005	 //BANKCON6
	.long 0x00018005	 //BANKCON7
	.long 0x008C04F4	 // REFRESH
	.long 0x000000B1	 //BANKSIZE
	.long 0x00000030	 //MRSRB6
	.long 0x00000030	 //MRSRB7