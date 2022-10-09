#include "../include/stddef.h"
#include "../include/kernel/debug.h"
#include "../include/stdio.h"

static const char* colors[] = { "\e[m", "\e[97;100m", "\e[97;44m", "\e[31;103m", "\e[1;30;41m" };

void debugf( uint8_t level, const char* caller, const char* informations )
{
	cprintf( "%s[%s]: %s\e[m\n", colors[ ( level % DEBUG_LEVELS ) ], caller, informations );
}

void log_status( const char* service, bool_t status )
{
	if ( status )
		cprintf( "%s: [\e[32mOK\e[m]]\n", service );
	else
		cprintf( "%s: [\e[31mKO\e[m]]\n", service );
}