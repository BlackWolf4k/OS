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

	kprintf( "Kernel End: '%p'", &kernel_end );

	char* b = kmalloc( 512 );
	char* a = kmalloc_c( 4096 );

	if ( a == NULL || b == NULL )
		kprintf( "NULL\n" );

	write_disk( a, 4096, 0x00 );


	write_disk( "Hello Disk World!\n", 19, 0x01 );
	create_partition( 0, create_partition_descriptor( 1, lba_to_chs( 0x02 ), 0x83, lba_to_chs( 0x4 ) ) );
	
	read_disk( a, 1, 0x0 );
	kprintf( "A:");
	for ( uint16_t i = 0; i < 512; i++ )
		kprintf( "%u", a[i] );

	// read_disk( b, 1, 0x1);
	// kprintf( b );

	kfree( b );
	kfree( a );

	kprintf( "Goodbye World!\n" );

	debug_heap_memory();

	return;
}