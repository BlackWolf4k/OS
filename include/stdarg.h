#pragma once

// This library uses GCC va_list implementation
// https://forum.osdev.org/viewtopic.php?f=13&t=24645

typedef __builtin_va_list va_list;
#define va_arg( list, type ) ( __builtin_va_arg( list, type ) )
#define va_copy( destination, source ) ( __builtin_va_copy( destination, source ) )
#define va_end( list ) ( __builtin_va_end( list ) )
#define va_start( list, number ) ( __builtin_va_start( list, number ) )