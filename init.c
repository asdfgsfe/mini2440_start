/*nand 控制器*/

#define  HW_GPMI_CTRL0           (*((volatile unsigned long *)0x8000_C000))
#define  HW_GPMI_COMPARE         (*((volatile unsigned long *)0x8000_C010))
#define  HW_GPMI_ECCCTRL         (*((volatile unsigned long *)0x8000_C020))
#define  HW_GPMI_ECCCOUNT        (*((volatile unsigned long *)0x8000_C030))
#define  HW_GPMI_PAYLOAD         (*((volatile unsigned long *)0x8000_C040))
#define  HW_GPMI_AUXILIARY       (*((volatile unsigned long *)0x8000_C050))
#define  HW_GPMI_CTRL1           (*((volatile unsigned long *)0x8000_C060))
#define  HW_GPMI_TIMING0         (*((volatile unsigned long *)0x8000_C070))
#define  HW_GPMI_TIMING1         (*((volatile unsigned long *)0x8000_C080))
#define  HW_GPMI_DATA            (*((volatile unsigned long *)0x8000_C0A0))
#define  HW_GPMI_STAT            (*((volatile unsigned long *)0x8000_C0B0))
#define  HW_GPMI_DEBUG           (*((volatile unsigned long *)0x8000_C0C0))
#define  HW_GPMI_VERSION         (*((volatile unsigned long *)0x8000_C0D0))

/*debug uart register*/

#define    HW_UARTDBG_DR          (*((volatile unsigned char *)0x8007_4000))
#define    HW_UARTDBG_EC          (*((volatile unsigned long *)0x8007_4004))
#define    HW_UARTDBG_FR          (*((volatile unsigned long *)0x8007_4018))
#define    HW_UARTDBG_ILPR        (*((volatile unsigned long *)0x8007_4020))
#define    HW_UARTDBG_IBRD        (*((volatile unsigned long *)0x8007_4024))
#define    HW_UARTDBG_FBRD        (*((volatile unsigned long *)0x8007_4028))
#define    HW_UARTDBG_H           (*((volatile unsigned long *)0x8007_402C))
#define    HW_UARTDBG_CR          (*((volatile unsigned long *)0x8007_4030))
//#define    HW_UARTDBG_IFLS        (*((volatile unsigned long *)0x8007_4034))
//#define    HW_UARTDBG_IMSC        (*((volatile unsigned long *)0x8007_4038))
//#define    HW_UARTDBG_RIS         (*((volatile unsigned long *)0x8007_403C))
//#define    HW_UARTDBG_MIS         (*((volatile unsigned long *)0x8007_4040))
//#define    HW_UARTDBG_ICR         (*((volatile unsigned long *)0x8007_4044))
//#define    HW_UARTDBG_DMACR       (*((volatile unsigned long *)0x8007_4048))

/*GPIO*/


void nand_init(void)
{
	#define TACLS      0
	#define TWRPH0     1
	#define TWRPH1     0
	/*set clock*/
	NFCONF = (TACLS<<12) | (TWRPH0<<8) | (TWRPH1<<4);
	/*enable nand flash,init_ecc,disable chip*/
	NFCONF = (1<<4) | (1<<1) | (1<<0);
}

#define PCLK            50000000    // init.c中的clock_init函数设置PCLK为50MHz
#define UART_CLK        PCLK        //  UART0的时钟源设为PCLK
#define UART_BAUD_RATE  115200      // 波特率
#define UART_BRD        ((UART_CLK  / (UART_BAUD_RATE * 16)) - 1)

/*
 * 初始化UART0
 * 115200,8N1,无流控
 */
void uart0_init(void)
{
    GPHCON  |= 0xa0;    // GPH2,GPH3用作TXD0,RXD0
    GPHUP   = 0x0c;     // GPH2,GPH3内部上拉

    HW_UARTDBG_H  = 0x060;     // 8N1(8个数据位，无较验，1个停止位)
    HW_UARTDBG_CR = 0x0181;    //qi yong txd rxd
    UCON0   = 0x05;     // 查询方式，UART时钟源为PCLK
    //UFCON0  = 0x00;     // 不使用FIFO
    //UMCON0  = 0x00;     // 不使用流控
    /*波特率为115200*/
    HW_UARTDBG_FBRD = 0x02A
    HW_UARTDBG_IBRD = 0x0D
}

/*
 * 发送一个字符
 */
void putc(unsigned char c)
{
    /* 等待，直到发送缓冲区中的数据已经全部发送出去 */
    while (!(UTRSTAT0 & TXD0READY));
    
    /* 向UTXH0寄存器中写入数据，UART即自动将它发送出去 */
    HW_UARTDBG_DR = c;
}

void puts (char *src)
{
	int i = 0;
	while (src[i])
		{
			putc(src[i]);
			i++;
		}
}




