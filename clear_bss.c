void clear_bss (void)
{
	extern int _bss_start , _bss_end;
	int *p = &_bss_start;
	for (; p < &_bss_end; p++)
	*p = 0;
}
