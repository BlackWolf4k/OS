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
	// Initialize the screen
	init_screen();

	// Initialize the heap memory
	init_heap();

	// Initialize interrupts
	init_idt();

	// Initialize paging
	init_paging();

	// Initialize serial ports
	if ( init_serial( COM1 ) )
		lprintf( 1, "Serial %d Initialized\n", COM1 );
	else
		lprintf( 0, "Serial %d not Initialized\n", COM1 );

	if ( init_serial( COM2 ) )
		lprintf( 1, "Serial %d Initialized\n", COM2 );
	else
		lprintf( 0, "Serial %d not Initialized\n", COM2 );

	// Initialize timer
	init_timer( 60 );

	// Initialize keyboard
	init_keyboard();
}

// Kernel start and kernel end are in linker.ld

// Kernel main function
void kmain()
{
	/* Initialize the kernel */
	kernel_early_main();

	for ( uint16_t i = 0; i <= 256; i += 16 )
	{
		set_color( ( uint8_t )i );
		kprintf( " " );
	}

	set_color( 0x0f );

	kprintf( "\nHello World!\n" );

	kprintf( "Kernel End: '%p'\n", &kernel_end );

	partition_table_descriptor_t partition_descriptor = create_partition_descriptor( 0, lba_to_chs( 2 ), 0x83, lba_to_chs( 1 * 1024 * 1024 ) );

	create_partition( 0, partition_descriptor );

	kprintf( "Created partition 0x%p of size: %d bytes\n", 0x83, 1024 * 1024 * 511 );

	make_file_system( partition_descriptor, 0 );
	kprintf( "Created ext file system for partition 0x%p\n", 0x83 );

	kprintf( "Goodbye World!\n" );

	debug_heap_memory();

	return;
}