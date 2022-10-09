#pragma once

#include "stddef.h"

// Send a byte to a port
// port = port, value = byte
void outb( uint16_t port, uint8_t value );

// Recive a byte from a port
// port = port
uint8_t inb( uint16_t port );

// Send a word ( 2 bytes ) to a port
// port = port, value = word
void outw( uint16_t port, uint16_t value );

// Recive a word ( 2 bytes ) from a port
// port = port
uint16_t inw( uint16_t port );

// From int to ascii
// value = value, string = string, base = base
char* itoa( int32_t value, char* string, int32_t base );

// Based on: https://cplusplus.com/reference/cstdio/printf/
void kprintf( const char* string, ... );

// Print data to a buffer
void sprintf( const char* string, ... );

void cprintf( const char* string, ... );