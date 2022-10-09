#include "../stddef.h"

#define INDEX_FROM_BIT( address ) ( address / ( 8 * 4 ) )
#define OFFSET_FROM_BIT( address ) ( address % ( 8 * 4 ) )

void init_paging();

// void change_page_directory( page_directory_t* page_directory );

// Handler for page fault ( int 14 )
void page_fault();