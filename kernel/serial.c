#include "../include/stddef.h"
#include "../include/kernel/serial.h"
#include "../include/stdio.h"

bool_t init_serial( uint32_t port )
{
	// Disable all interrupts
	outb( port + 1, 0x00 );

	// Set baud rate divisor ( 38400baud )
	outb( port + 3, 0x80 );

	// baud divisor high byte
	outb( port + 0, 0x03 );

	// baud divisor low byte
	outb( port + 1, 0x00 );

	// No parity
	outb( port + 3, 0x03 );

	// Enable FIFO
	outb( port + 2, 0xc7 );

	// Enable IRQs
	outb( port + 4, 0x0b );

	// Set Loopback move
	outb( port + 4, 0x1e );

	// Send 0xea to test if returns the same
	outb( port + 0, 0xea );

	// Check if serial works
	// Check if return same byte that was sended
	if ( inb( port + 0 ) != 0xea )
		return 0; // There is a problem with the serial, return error value

	// Serial is not faulty, set normal mode
	outb( port + 4, 0x0f );

	// Return success value
	return 1;
}

void read_serial( char* buffer, uint32_t port )
{
	// Check the line status register
	while( inb( port + 5 ) & 1 == 0 );

	uint32_t recived_byte = 0;

	// Recive bytes from serial untill a 0 is recived
	do
	{
		recived_byte = inb( port );
	} while ( recived_byte != 0 );
}

void write_serial( char* buffer, uint32_t port )
{
	// Check the line status register
	while ( inb( port + 5 ) & 0x20  == 0 );

	// Send all the bytes in the buffer
	for ( uint32_t i = 0; buffer[i] != '\0'; i++ )
		outb( port, buffer[i] );
}

void put_serial( char buffer )
{
	// Check the line status register
	while ( inb( COM1 + 5 ) & 0x20  == 0 );

	// Send all the bytes in the buffer
	outb( COM1, buffer );
}