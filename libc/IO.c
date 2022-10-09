#include "../include/stdio.h"

void outb( uint16_t port, uint8_t value )
{
	asm( "out %%al, %%dx" : : "a" ( value ), "d" ( port ) );
}

uint8_t inb( uint16_t port )
{
	uint8_t value;
	asm( "in %%dx, %%al" : "=a" ( value ) : "d" ( port ) );
	return value;
}

void outw( uint16_t port, uint16_t value )
{
	asm( "out %%ax, %%dx" : : "a" ( value ), "d" ( port ) );
}

uint16_t inw( uint16_t port )
{
	uint16_t value;
	asm( "in %%dx, %%ax" : "=a" ( value ) : "d" ( port ) );
	return value;
}