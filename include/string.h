#pragma once

#include "stddef.h"

// Set n bytes of memory space to a value
// source = source, value = value, length = number of bytes
void memset( void* source, int32_t value, uint32_t length );

// Copy n bytes from a source to a destination
// destination = destination, source = source, length = number of bytes to copy
void memcpy( void* destination, void* source, uint32_t length );

// Set n bytes to zero
// destination = destination, length = number of bytes
void bzero( void* source, uint32_t length );

// Copy a string into another
// destination = destionation, source = source
void strcpy( char* destination, char* source );

// Returns the length of a string
// string = string, return value = length ( uint32_t )
uint32_t strlen( char* string );

// Compare two strings
// first = first string to compare, second = second string to compare, return true if the strings are the same
bool_t strcmp( const char* first, const char* second );

// Search a string in a string
// string = string to search in, match = string to match
// return position of matched string ( if there is one, else return NULL), remember to free the address
char* strstr( const char* string, const char* match );

// Enumerate how many times a character repeats in a string
// string = string to look in, character = character to enumerate
uint32_t strenm( const char* string, char character );

// Convert a character to upper case
// character = character
void to_upper( char* character );