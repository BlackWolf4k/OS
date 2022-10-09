/*
PAGES INFORMATIONS
P:		( present )page is in memory			1 bit
R/W		is 1 is writable, if 0 is read-only		1 bit
U/S		if 1 is user-mod, if 0 kernel-mod		1 bit
A		Accessed ( set by the cpu )				1 bit
D		Dirty ( been written )					1 bit
Res		Reserved for the CPU					4 bits
AVAIL	Unused, for kernel use					3 bits
Frame	Page frame address ( << 12 )			20 bits
*/

#include "../include/kernel/paging.h"

// Sizes of paging and of how much to move the heap starting position
#define PAGING_SIZE ( sizeof( page_table_t ) * 4 * 1024 ) // 16MB
#define PAGING_DESCRIPTORS_SIZES ( 4 * 1024 * 1024 + 4 * 1024 ) // around 4MB

// The page itself
// Page size descriptor is 32bits size
typedef struct
{
	uint16_t present : 1;
	uint16_t writable : 1;
	uint16_t user : 1;
	uint16_t accessed : 1;
	uint16_t dirty : 1;
	uint16_t reserved : 4;
	uint16_t unused : 3;
	uint32_t frame : 20;
} page_t;

typedef struct
{
	// The pages inside a page table
	page_t pages[1024];
} page_table_t;

typedef struct
{
	// The page tables
	// The page directory does not contain the pages, it points to them
	page_table_t* page_tables[1024];

	// Physical address of the page tables
	ptr_t page_tables_physical_address[1024];
} page_directory_t;

// Private Functions
// Alloc a page, returns the address to it
// There will be no pfree()
ptr_t palloc();

void init_paging()
{
	kprintf( "-%d-", sizeof( page_table_t ) );
}

ptr_t palloc()
{
}