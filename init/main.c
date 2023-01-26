#include "../include/stdio.h"
#include "../include/string.h"
#include "../include/kernel/kernel.h"
// First phase headers
#include "../include/kernel/screen.h"
#include "../include/kernel/debug.h"
#include "../include/kernel/interrupts/idt.h"
#include "../include/kernel/interrupts/isr.h"
#include "../include/kernel/timer.h"
#include "../include/kernel/keyboard.h"
// Second phase headers
#include "../include/kernel/memory.h"
#include "../include/kernel/disk.h"
#include "../include/kernel/serial.h"
// Third phase headers
#include "../include/file_system/file_system.h"
#include "../include/kernel/paging.h"

extern ptr_t kernel_start;
extern ptr_t kernel_end;

// Start the basic parts of the kernel
void kernel_early_main()
{
	// Second phase initialization
	init_serial( COM1 );
	init_serial( COM2 );
	init_heap();

	// First phase initialization
	init_screen();
	init_idt();
	init_timer( 60 );
	init_keyboard();
	init_paging();

	// Third phase initialization
}

// Kernel start and kernel end are in linker.ld

// Kernel main function
void kmain()
{
	/* Initialize the kernel */
	kernel_early_main();

	kprintf( "Hello World!\n" );

	kprintf( "Kernel End: '%p'\n", &kernel_end );

	partition_table_descriptor_t partition_descriptor = create_partition_descriptor( 0, lba_to_chs( 2 ), 0x83, lba_to_chs( 1 * 1024 * 1024 ) );

	create_partition( 0, partition_descriptor );

	make_file_system( partition_descriptor, 0 );

	kprintf( "Goodbye World!\n" );

	debug_heap_memory();

	return;
}