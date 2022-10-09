#pragma once

/*
SERIAL PORT INFORMATIONS
COM1 0x3f8
COM2 0x2f8

COMX Base Address
+0: Data Register. Reading reads from Receive Buffer, Writing writes to Transmit Buffer
+1: Interrupt Enable Register ( 0 = disabled, 1 = enabled )
+2: FIFO Control Register
+3: Line Control Register
+4: Modem Control Register
+5: Line Status Register
+6: Modem Status Register
+7: Scratch Register
*/

// Define Address of COM1
#define COM1 0x3f8

// Define Address of COM2
#define COM2 0x2f8

// Entirely based on: https://wiki.osdev.org/Serial_Ports
// Initialize the serial ports ( COM1, COM2 )
bool_t init_serial( uint32_t port );

// Recive data from serial port
// buffer = buffer where to store data, port = port to listen
void read_serial( char* buffer, uint32_t port );

// Send data to serial port
// buffer = buffer with data to send, port = port into write
void write_serial( char* buffer, uint32_t port );

void put_serial( char buffer );