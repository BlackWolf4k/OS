#include "../include/string.h"

void memset( void* source, int32_t value, uint32_t length )
{
	for ( uint32_t i = 0; i < length; i++ )
		( ( uint8_t* )source )[i] = ( uint8_t )value;
}

void memcpy( void* destination, void* string, uint32_t length )
{
	for ( uint32_t i = 0; i < length; i++ )
		( ( uint8_t* )destination )[i] = ( ( uint8_t* )string )[i];
}

void bzero( void* source, uint32_t length )
{
	memset( source, 0, length );
}

void strcpy( char* destination, char* string )
{
	memcpy( destination, string, strlen( string ) );
}

uint32_t strlen( char* string )
{
	uint32_t length = 0;
	while ( string[length] != '\0' )
		length++;
	return length;
}

bool_t strcmp( const char* first, const char* second )
{
	uint32_t first_length = strlen( first ); // Only declared to save some computations

	// Compare the length of the two string, if it is difference they must be different
	if ( first_length != strlen( second ) )
		return false;
	
	// Compare each character of the two strings
	for ( uint32_t i = 0; i < first_length; i++ )
		if ( first[i] != second[i] )
			return false; // the characters are different so the two string are different
	
	// If execution arrived here string must be the same
	return true;
}

char* strstr( const char* string, const char* match )
{
	// Lengths of the strings, declared only to save some computations
	uint32_t string_length = strlen( string );
	uint32_t match_length = strlen( match );

	// Count how many same consecutives caracters were found
	uint32_t same_characters_found = 0;

	// The start of the match string
	// This variable will be the one returned
	char* match_start = NULL;

	// Check all the characters
	for ( uint32_t i = 0, j = 0; i < string_length - match_length + 1; i++ )
	{
		// Check if same character
		if ( string[i] == match[j] )
		{
			// It is the same character
			if ( same_characters_found == 0 )
				match_start = string; // Set where the string starts
			// Increare same_characters_found
			same_characters_found += 1;
		}
		else
		{
			// To zero all variables
			same_characters_found = 0;
			match_start = NULL;
		}
	}

	// If execution reached this point, match was found in string
	return match_start;
}

uint32_t strenm( const char* string, char character )
{
	uint32_t repetitions = 0; // To store the repetitions
	uint32_t string_length = strlen( string ); // Only decleared to save some computations

	// Check all string characters
	for ( uint32_t i = 0; i < string_length; i++ )
		// Compare the string character to the character searched
		if ( string[i] == character )
			repetitions += 1; // They are the same so increase repetitions
	
	// Return the total repetitions
	return repetitions;
}

void to_upper( char* character )
{
	if ( *character >= 97 && *character <= 122 )
		*character -= 32;
}