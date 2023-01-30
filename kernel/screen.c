#include "../include/stdio.h"
#include "../include/kernel/screen.h"
#include "../include/string.h"

// Informations about the screen
struct
{
	vuint8_t* video_memory; // base address of the video memory
	vuint8_t* video_memory_buffer; // not used ( for now )
	uint8_t columns; // number of columns in the video
	uint8_t rows; // number of rows in the video
	uint8_t color; // color of the text ( byte low = text color, byte high = background color )
} screen_informations_t;

// Screen cursor registers informations
struct
{
	uint32_t control; // control register
	uint32_t data; // data register
} screen_registers_t;

/* Private functions declarations */
// Set the cursor position
// offset = offset from the beginning of the video memory
void set_cursor( uint32_t offset );

// Get the cursor position
// Return uint32_t with cursor position
uint32_t get_cursor();

// Set character to the screen
// __c = character, offset = offset in the video memory
void set_character( char __c, uint32_t offset );

// Scroll down of one line
// offset = how much to scroll down
uint32_t scroll( uint32_t offset );

// Calculate the offset
// column = column, row = row
uint32_t get_offset( uint32_t column, uint32_t row );

/* Public functions declaration */
void init_screen()
{
	screen_informations_t.video_memory = ( vuint8_t* )0xb8000; // base address for text mode // graphic mode address = 0xa0000
	screen_informations_t.columns = 80;
	screen_informations_t.rows = 25;
	set_color( 0x0f ); // black background and white text

	screen_registers_t.control = 0x3d4;
	screen_registers_t.data = 0x3d5;

	// Clear the screen from booting messages
	clear_screen();
	set_cursor( 0 );

	lprintf( 1, "Screen Initialized\n" );
}

void clear_screen()
{
	for( uint32_t i = 0; i < ( screen_informations_t.columns * screen_informations_t.rows ); i += 2 )
	{
		screen_informations_t.video_memory[ i ] = '\0';
		screen_informations_t.video_memory[ i + 1 ] = screen_informations_t.color;
	}
	set_cursor( 0 );
}

void set_color( uint8_t color )
{
	screen_informations_t.color = color;
}

void put_char( char character )
{
	// Check that the pointer points to charater not to a \0
	uint32_t offset = get_cursor();
	// Check for special characters
	switch ( character )
	{
		case '\n': // newline
		{
			offset = ( ( offset / ( 2 * screen_informations_t.columns ) ) + 1 ) * ( screen_informations_t.columns * 2 );
			break;
		}
		case '\b': // backspace
		{
			offset = ( offset >= 2 ) ? offset - 2 : 0;
			set_character( '\0', offset );
			// Make sure that cursor is at character
			while( offset >= 2 && screen_informations_t.video_memory[ offset - 2 ] == '\0' )
				offset -= 2;
			break;
		}
		case '\t': // tabulation
		{
			// Add spaces untill aligned with tabulation
			do
			{
				set_character( ' ', offset );
				offset += 2;
			} while ( ( ( offset % screen_informations_t.columns ) / 2 ) % 4 != 0 );
		}
		case '\x18': // cancel
		{
			set_character( '\0', offset );
			break;
		}
		default: // normal character
		{
			set_character( character, offset );
			offset += 2;
			break;
		}
	}
	if ( offset >= screen_informations_t.columns * screen_informations_t.rows * 2 )
		offset = scroll( offset );
	set_cursor( offset );
}

/* Private functions declaration */
void set_cursor( uint32_t offset )
{
	offset /= 2;
	outb( screen_registers_t.control, 0x0e );
	outb( screen_registers_t.data, ( uint8_t )( offset >> 8 ) );
	outb( screen_registers_t.control, 0x0f );
	outb( screen_registers_t.data, ( uint8_t )( offset & 0xff ) );
}

uint32_t get_cursor()
{
	uint32_t offset = 0;
	outb( screen_registers_t.control, 14 );
	offset = inb( screen_registers_t.data ) << 8;
	outb( screen_registers_t.control, 15 );
	offset += inb( screen_registers_t.data );
	return offset * 2;
}

uint32_t get_offset( uint32_t column, uint32_t row )
{
	return ( 2 * ( row * screen_informations_t.columns + column ) );
}

void set_character( char __c, uint32_t offset )
{
	screen_informations_t.video_memory[ offset ] = __c;
	screen_informations_t.video_memory[ offset + 1 ] = screen_informations_t.color;
}

uint32_t scroll( uint32_t offset )
{
	memcpy( ( uint8_t* )( get_offset( 0, 0 ) + screen_informations_t.video_memory ),
			( uint8_t* )( get_offset( 0, 1 ) + screen_informations_t.video_memory ),
			screen_informations_t.columns * ( screen_informations_t.rows - 1 ) * 2 );
	
	for ( uint32_t i = 0; i < screen_informations_t.columns * 2; i++ )
		set_character( '\0', get_offset( i, screen_informations_t.rows - 1 ) );
	return offset - 2 * screen_informations_t.columns;
}