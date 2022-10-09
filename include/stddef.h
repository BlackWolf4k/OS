/* DATA TYPES */

typedef char int8_t;
typedef unsigned char uint8_t;
typedef volatile unsigned char vuint8_t;

typedef short int int16_t;
typedef unsigned short int uint16_t;
typedef volatile unsigned short int vuint16_t;

typedef int int32_t;
typedef unsigned int uint32_t;

typedef uint32_t* ptr_t;

typedef long long int int64_t;
typedef unsigned long long int uint64_t;

typedef uint8_t bool_t;
// typedef unsigned long size_t;

#define NULL ( ( void* )0 )

#define false 0
#define true 1

#define SIZE( array ) ( sizeof( array ) / sizeof( array[0] ) )