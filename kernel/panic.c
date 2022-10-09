/*CHANGE THIS FILE NAME*/

#include "../include/stdio.h"
#include "../include/kernel/kernel.h"
#include "../include/kernel/screen.h"

void panic_( const char* message )
{
	// Change the color of the text
	set_color( 0x04 );

	// Print the message and what to do
	kprintf( "\n%s\nSystem Halted\nReboot your system\nMemory Debug:", message );

	debug_heap_memory();

	// Disable interrupts
	asm volatile( "cli" );

	// Halt the system
	asm volatile( "hlt" );
}

void memory_error( const char* message )
{
	// Change this function
	kprintf( message );
}

void disk_error( const char* message )
{
	panic_( message );
}