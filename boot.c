#include"setup.h"

static struct tag *params;

void setup_start_tag (void)
{
	params = (struct tag *) bd->bi_boot_params; //0x30000100

	params->hdr.tag = ATAG_CORE; //0x54410001
	params->hdr.size = tag_size (tag_core);

	params->u.core.flags = 0;
	params->u.core.pagesize = 0;
	params->u.core.rootdev = 0;

	params = tag_next (params);
}

void setup_memory_tag (void)
{
		params->hdr.tag = ATAG_MEM; //0x54410002
		params->hdr.size = tag_size (tag_mem32);

		params->u.mem.start = bd->bi_dram[i].start; //0x30000000
		params->u.mem.size = bd->bi_dram[i].size;   //0x4000000 64*1024*1024

		params = tag_next (params);
}

int strlen (char *str)
{
	int i = 0;
	while (str[i])
		{
			i++;
		}
	return 1;
}

void strcpy (char *dest, char *src)
{
	char *tmp = dest;
	while((*dest++ = *src++) != '\o')
}

void setup_commandline_tag ( char *commandline)
{
	int len = strlen(commandline) + 1;
	params->hdr.tag = ATAG_CMDLINE; //ox54410009
	params->hdr.size =
		(sizeof (struct tag_header) + len  + 3) >> 2;

	strcpy (params->u.cmdline.cmdline, p);

	params = tag_next (params);	
}

void setup_end_tag (void)
{
	params->hdr.tag = ATAG_NONE;
	params->hdr.size = 0;
}


void main (void)
{
	void (*theKernel)(int zero , int arch, unsigned int params)

	/*0.setup uart0*/
	uart0_init();
	
	/*1.the kernel from nand to sdram*/
	puts("copy kernel from nand\n\r ");
	nand_read(unsigned int src,unsigned char * buf,unsigned int len);

	/*2.set up params*/
	puts("set boot params\n\r ");
	setup_start_tag();
	setup_memory_tag();
	setup_commandline_tag("noinitrd root = /dev/mtdblock3 init = /linuxrc console = ttySAC0");
	setup_end_tag();

	/*3.loop */
	puts("boot kernel\n\r ");
	theKernel = (void (*) (int, int, unsigned int)) 0x3000800;
	theKernel(0, 362, 0x30000000);
	/*
	 *mov r0, #0
	 *ldr r1, = 362
	 *ldr r2, = 0x30000000
	 *mov pc, #0x30008000
	*/
	/*if all ok, do ont exit this*/
	puts("Erro\n\r ");
}
