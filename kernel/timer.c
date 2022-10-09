#include "../include/stdio.h"
#include "../include/kernel/timer.h"
#include "../include/kernel/interrupts/isr.h"

// Structure to represent a date

uint32_t tick = 0; // At 1 tick per second it would take about 136 years to overflow this variable
static void timer_callback( registers_t *registers )
{
	tick++;
}

void init_timer( uint32_t frequency )
{
	register_interrupt_handler( 32 + 0, timer_callback );

	// Set PIT value
	uint32_t value = 1193180 / frequency;
	uint8_t low = ( uint8_t )( value & 0xff );
	uint8_t high = ( uint8_t )( ( value >> 8 ) & 0xff );
	
	// Send to port
	outb( 0x43, 0x36 ); // Set command port
	outb( 0x40, low );
	outb( 0x40, high );
}

uint32_t get_ticks()
{
	return tick;
}

uint32_t get_timestamp()
{
	return tick;
}

void sleep( uint32_t seconds )
{
	// CORRECT THIS FUNCTION
	// Scale to system seconds
	seconds *= 100; // pretty fine scale

	seconds += tick;

	// Wait untill the sleep time is passed
	while ( seconds >= tick );
}