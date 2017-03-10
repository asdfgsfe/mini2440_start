void copy_code_to_sdram(unsigned char *src,unsigned char *dest,unsigned char *len)
{
	int i = 0;
	/*if is nor boot*/
	if(isBootFromNorFlash())
		{
			while(i < len)
				{
					dest[i] = src[i];
					i++;
				}
			else
				{
					nand_read(src, dest, len);
				}
	}
}

int isBootFromNorFlash (void)
{
	volatile int *p = (volatile int *) 0;
	int val;
	val = *p;
	*p = 0x12345678;
	if (*p = 0x1234578)
		{
			/*write vactory nand boot*/
			*p = val;
			return 0;
		}
	else
		{
			/*nor boot*/
			return 1;
		}
}

void nand_read(unsigned int src,unsigned char *buf,unsigned int len)
{
	int col = src / 2048;
	int i = 0;
	/*1.chinp*/
	nand_select();
	while(i < len)
		{		
			/*2.issue read cmd 0xooh*/
			nand_cmd(0x00h);
			/*3.issue addr*/
	 		nand_addr(addr);
			/*4.issue read cmd 0x3oh*/
			nand_cmd(0x30h);	
			/*5.judgement status*/
	 		nand_wait_ready();		
			/*6.read data*/
			for(; (col < 2048) && (i<len);i++)
				{
					buf[i] = nand_data();
					i++;
					src++;
				}
			col = 0;
		}
	nand_disselect();
}

void nand_cmd (unsigned char cmd)
{
	volatile int i;
	NFCMD = cmd;
	for (i = 0; i < 10; i++);
}

void nand_addr (unsigned int addr)
{
	unsigned int col = addr / 2018;
	unsigned int page = addr % 2048;
	int i;
	
	NFADDR = col & 0x0ff;
	for (i = 0; i < 10; i++);
	NFADDR = (col >> 8) & 0xff;
	for (i = 0; i < 10; i++);

	NFADDR = page & 0xff;
	for (i = 0; i < 10; i++);
	NFADDR = (page >> 8) & 0xff;
	for (i = 0; i < 10; i++);
	NFADDR = (page >> 16) & 0xff;
}

void nand_wait_ready ()
{
	while (!CNFSTAT & 1);
}

unsigned char nand_data (void)
{
	return NFDATA;
}

void nand_select (void)
{
	NFCONT & = ~(1 << 1);
}

void nand_disselect (void)
{
	NFCONT | = ~(1 << 1);
}


