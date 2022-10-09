#pragma once

#include "../stddef.h"

// Initialize the screen
// It clears it too and sets the cursor to the top left corner
void init_screen();

// Clear the screen
void clear_screen();

// Change color
// color = ( byte low = character color, byte high = background color )
void set_color( uint8_t color );

// Put a character to the screen
void put_char( char character );