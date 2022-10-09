SECTIONS
{
	kernel_start = .;
	. = 0x1000;


	.text :
	{
		*(.text)
	}

	.data ALIGN ( 0x1000 ) :
	{
		*(.data)
	}

	.code ALIGN ( 0x1000 ) :
	{
		*(.code)
	}

	kernel_end = .;
}