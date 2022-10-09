#include "../include/stdio.h"

char* itoa( int32_t value, char* string, int32_t base )
{
	char* rc;
	char* ptr;
	char* low;

	if ( base < 2 || base > 36 )
	{
		*string = '\0';
		return string;
	}
	rc = ptr = string; 

	if ( value < 0 && base == 10 )
	{
		*ptr++ = '-';
	}
	low = ptr;
	do
	{
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[ 35 + value % base ];
		value /= base;
	} while( value );
	*ptr-- = '\0';
	while( low < ptr )
	{
		char tmp = *low;
		*low++ = *ptr;
		*ptr-- = tmp;
	}
	return rc;
}