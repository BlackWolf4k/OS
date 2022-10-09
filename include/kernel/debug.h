#pragma once

#define DEBUG_LEVELS 5

enum debug_level
{
	log = 0,
	debug,
	info,
	error,
	panic
};

void debugf( uint8_t level, const char* caller, const char* informations );