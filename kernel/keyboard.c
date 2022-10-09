#include "../include/stdio.h"
#include "../include/string.h"
#include "../include/kernel/keyboard.h"
#include "../include/kernel/interrupts/isr.h"

#include "../include/kernel/screen.h"

struct // CHANGE THIS STRUCT WITH CONTROL TO KEYBOARD LIGHTS
{
	bool_t capital;
	bool_t shift;
	bool_t number_lock;
} keyboard_informations_t;

// To change with real keyboard layout
static const char scancodes_ascii[] = { '\0', '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '\'', '^', '\b',
							'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', '\0',
							'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', '@', '#', '\\', '\0', '`',
							'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', '\0',
							'*', '\0', ' ', '\0',
							'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
							'\0', '\0', '\0'/*numpad starts here*/, '\0', '\0', '-', '\0', '\0', '\0', '+', '\0', '\0', '\0', '\0'/*numpad ends here*/, '\x18',
							'\0', '\0', '<', '\0', '\0' };

static const char upper_ascii[] = { '\0', '\0', '!', '\'', '$', '$', '%', '&', '/', '(', ')', '=', '?', '~', '\b',
							'\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', '\0',
							'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', '@', '°', '|', '\0', '`',
							'Z', 'X', 'C', 'V', 'B', 'N', 'M', ';', ':', '_', '\0',
							'*', '\0', ' ', '\0',
							'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
							'\0', '\0', '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.',
							'\0', '\0', '>', '\0', '\0' };

/*
const char* scancodes_name[] = { "\0", "ESC", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "\'", "^", "\b",
							"\t", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]", "\n", "CTRL",
							"a", "s", "d", "f", "g", "h", "j", "k", "l", "@", "#", "\\", "LSHIFT", "`",
							"z", "x", "c", "v", "b", "n", "m", ",", ".", "-", "RSHIFT",
							"*", "LALT", " ", "CAPITAL",
							"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10",
							"NUMLOCK", "SCROLL", "HOME", "UP", "PRIOR", "SUBTRA", "LEFT", "CLEAR", "RIGHT", "ADD", "END", "DOWN", "NEXT", "INSERT", "DELETE",
							"STAMP", "\0", "<", "F11", "F12", "\0", "\0", "LWIN", "RWIN", "APPS" };
*/

// Calls this function at each key pressed
// Only test, modify to return character instead of printing it
static void keyboard_callback()
{
	// Get the scancode from keyboard ( port 0x60 )
	uint8_t scancode = inb( 0x60 );

	// Check if the scan code is valide
	if ( ( ( scancode < 0x00 || scancode > 0x58 ) && ( scancode != 0x59 || scancode != 0x5a ) ) && ( scancode != 0xaa && scancode != 0xb6 ) )
		return;

	// Some keys start with 0xe0
	if ( scancode >> 8 == 0xe0 )
		scancode &= 0xff; // Wanted value

	// Check if capital was pressed ( maybe change with call to port 0x64 )
	if ( scancode == 0x3a )
		keyboard_informations_t.capital = !keyboard_informations_t.capital; // Toggle capital
	// Check if shift was pressed
	else if ( scancode == 0x2a || scancode == 0x36 )
		keyboard_informations_t.shift = true; // Shift pressed
	else if ( scancode == 0xaa || scancode == 0xb6 )
		keyboard_informations_t.shift = false; // Shift released
	else if ( scancode == 0x45 )
		keyboard_informations_t.number_lock = !keyboard_informations_t.number_lock; // Toggle number lock
	// Check if to print upper or lower case
	else if ( ( ( scancode > 1 && scancode < 55 ) || scancode == 86 ) && ( keyboard_informations_t.capital || keyboard_informations_t.shift ) )
		kprintf( "%c", upper_ascii[scancode] ); // Upper case
	else if ( ( scancode > 0x46 && scancode < 0x54 ) && keyboard_informations_t.number_lock )
		kprintf( "%c", upper_ascii[scancode] ); // Num lock
	else
		kprintf( "%c", scancodes_ascii[scancode] ); // Lower case

	return;
}

void init_keyboard()
{
	// For irq33 set keyboard_callback as handler
	register_interrupt_handler( 32 + 1, keyboard_callback );
}