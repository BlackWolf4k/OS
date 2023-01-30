#include "../include/stdio.h"
#include "../include/kernel/screen.h"
#include "../include/string.h"

#include "../include/stdarg.h"

#include "../include/kernel/memory.h"

#include "../include/kernel/serial.h"

#define PUT void ( *put )( char )

// Print a string to the screen
// str = string, ... = other arguments
void printk( PUT, const char* string );

// Print a pointed memory address to the screen
// pointer = pointer
void printkp( PUT, void* pointer );

// Print a signed integer
// integer = integer to print
void printkd( PUT, int32_t integer );

// Print a unsigned integer
// integer = integer to print
void printku( PUT, uint32_t integer );

// Print a character
// character = character to print
void printkc( PUT, char character );

// Print a floating point value
// value = floating point value to print
// NOT DONE YET
void printkf( PUT, float value );


/* Public Functions Declearation */
void kprintf( const char* string, ... )
{
	// Create a va_list of the arguments
	va_list arguments;
	va_start( arguments, string );

	// Call the vprintf, specifing the "PUT" function
	vprintf( put_char, string, arguments );

	va_end( arguments );
}

void lprintf( bool_t status, const char* string, ... )
{
	// Create a va_list of the arguments
	va_list arguments;
	va_start( arguments, string );

	// Write the status
	kprintf( "[" );

	// Change message bases on the status value
	if ( status )
	{
		set_color( 0x02 );
		kprintf( "OK" );
	}
	else
	{
		set_color( 0x04 );
		kprintf( "ERROR" );
	}

	set_color( 0x0f );
	kprintf( "] " );

	// Call the vprintf, specifing the "PUT" function
	vprintf( put_char, string, arguments );

	va_end( arguments );
}

// Print data to a buffer
void sprintf( const char* string, ... );

// Print data to a serial port
void cprintf( const char* string, ... )
{
	// Create a va_list of the arguments
	va_list arguments;
	va_start( arguments, string );

	// Call the vprintf, specifing the "PUT" function
	vprintf( put_serial, string, arguments );

	va_end( arguments );
}

// Format the data
void vprintf( PUT, const char* string, va_list arguments )
{
	// All the accepted formates
	// specifiers = "dufcsp";
	uint32_t i = 0;

	// Print characters of the string untill '\0' is reached
	while ( string[i] != '\0' )
	{
		if ( string[i] == '%' )
		{
			// What to print
			switch ( string[ i + 1 ] )
			{
				case '%':
				{
					put( '%' );
					break;
				}
				case 'd': // Integer ( 32 bit )
				{
					printkd( put, va_arg( arguments, int32_t ) );
					break;
				}
				case 'u': // Unsigned Integer ( 32 bit )
				{
					printku( put, va_arg( arguments, uint32_t ) );
					break;
				}
				case 'f': // Float
				{
					printkf( put, va_arg( arguments, float ) ); // check warning from float to double or something...
					break;
				}
				case 'c': // Character
				{
					printkc( put, va_arg( arguments, char* ) );
					break;
				}
				case 's': // String
				{
					printk( put, va_arg( arguments, char* ) );
					break;
				}
				case 'p': // Pointer ( 32 bit )
				{
					printkp( put, va_arg( arguments, ptr_t ) );
					break;
				}
				case 'v': // Void ( to call function )
				{
					break;
				}
				default:
				{
					vprintf( put, "%c NOT SUPPORTED\n", string[i] );
					return;
				}
			}
			// Need to increase one more time
			i++;
		}
		else
			put( string[i] );

		// Increase the counter
		i++;
	}

	va_end( arguments );
}

/* Private Functions Declearation */

void printk( PUT, const char* string )
{
	// Change with call to vprintf
	uint32_t i = 0;
	while ( string[i] != '\0' ) // print untill end of string
	{
		printkc( put, string[i] ); // put character to the screen
		i++;
	}

	return;
}

void printkp( PUT, void* pointer )
{
	// Create string to store memory address
	// Change this in malloc string allocation?
	char str[ 8 + 1 ]; // 8 of pointer size + 1 for '\0'
	bzero( str, 9 ); // clear the string

	// Convert pointer to string
	itoa( ( uint32_t )pointer, str, 16 );

	// Print the memory address
	printk( put, str );

	return;
}

void printkd( PUT, int32_t integer )
{
	// Create string to store integer
	// Change this in malloc string allocation?
	char str[ 10 + 1 ]; // 10 of integer size + 1 for '\0'
	bzero( str, 9 ); // clear the string

	// Convert integer to string
	itoa( integer, str, 10 );

	// Print the integer
	printk( put, str );

	return;
}

void printku( PUT, uint32_t integer )
{
	// Create string to store integer
	// Change this in malloc string allocation?
	char str[ 10 + 1 ]; // 10 of integer size + 1 for '\0'
	bzero( str, 9 ); // clear the string

	// Convert integer to string
	itoa( integer, str, 10 );

	// Print the integer
	printk( put, str );

	return;
}

void printkc( PUT, char character )
{
	put( character );
}

void printkf( PUT, float value )
{
	printk( put, "Not done yet" );
}