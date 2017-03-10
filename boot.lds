SECTIONS {
	. = 0X 33FF80000
	. = text : {*(.text)}

	. = ALLGN (4);
	. rodata : {*(.rodata*)}

	. = ALLGN (4);
	. data : {*(data)}

	. = ALLGN (4);
	_bss_start = . ;
	. bss : {*(. bss) *(common)}
	_bss_end = . ;
}
