#include "../include/string.h"
#include "../include/kernel/error.h"
#include "../include/kernel/memory.h"
#include "../include/stdio.h"
#include "../include/kernel/debug.h"

// Structure that containes information about a memory space
typedef struct
{
	bool_t used; // is the node used
	uint32_t size; // size of node
	ptr_t previous; // previous node
	ptr_t next; // next node
} heap_node_t;

// Where the non mepped memory starts
// Based on: https://wiki.osdev.org/Memory_Map_(x86)
// #define FREE_MEMORY_START 0xfffff;

// The size of the heap
// TO CHANGE BASED ON THE SIZE OF THE MEMORY
// Tge 4mb are for kernel general purpose
#define HEAP_SIZE 1024 * 1024 * 64

// Get the end of the kernel from "linker.ld"
extern ptr_t kernel_end;

// Size of a heap_node_t
#define NODE_SIZE sizeof( heap_node_t )

// Pointer to the heap start
// Heap starts at the very end of the kernel
// Moved of the size of the paging from kernel end
static uint8_t* heap = ( uint8_t* )( &kernel_end );

/* Private Functions */
// If two unused nodes are sequential merge them in one
void merge( heap_node_t* first, heap_node_t* second );

// Split a node in two
// The first node will be of the specified size
// The second one will be have the size of the remaining space
void split( heap_node_t* node, uint32_t size );

// Search all nodes to one of specified size
// If it is not found, split the smallest
// size = size of the block to look for
heap_node_t* find_best_block( uint32_t size );

// Checks if a address is in the heap address space
// address = address to check
bool_t check_address( void* address );

// Given an address locates at wich node it appartains
// Returns NULL if the node was not found
// address = address for the node to look for
heap_node_t* find_node( void* address );

// Set up the default value of a note ( node constructor )
void initialize_node( heap_node_t* node );

void init_heap()
{
	// Get the first node in the heap
	heap_node_t* heap_head = ( heap_node_t* )heap;

	// Get the list node in the heap
	heap_node_t* heap_tail = ( heap_node_t* )( heap + HEAP_SIZE - NODE_SIZE );

	// Instantiate the first heap node ( heap_head )
	heap_head -> used = false;
	heap_head -> size = HEAP_SIZE - 2 * NODE_SIZE; // subtract the first and the last node of the heap
	heap_head -> previous = NULL;
	heap_head -> next = ( ptr_t )heap_tail;

	// Instantiate the last heap node ( heap_tail )
	heap_tail -> used = true; // THIS ?
	heap_tail -> size = 0; // the tail has no size
	heap_tail -> previous = ( ptr_t )heap_head;
	heap_tail -> next = NULL; // has no next node

	lprintf( 1, "Heap Memory Initialized\n" );
}

void* kmalloc( uint32_t size )
{
	// Look for the best block for the specified size
	heap_node_t* node = find_best_block( size );

	// Check that a node was returned
	if ( node == NULL )
		return NULL; // No node returned, return an error value
	
	// Set that the block is beeing used
	node -> used = true;

	// Return the address to the block( skip the node size )
	return ( void* )( ( ( char* )node ) + NODE_SIZE );
}

void* kmalloc_c( uint32_t size )
{
	// Allocate a block of specified size
	void* block = kmalloc( size );

	// Kmalloc failed
	if ( block == NULL )
		return NULL; // Return a fail status

	// Clear the block
	bzero( block, size );

	// Return the address to the block
	return block;
}

void* krealloc( void* old_address, uint32_t size )
{
	// Check that the old address exists
	if ( !check_address( old_address ) )
	{
		// Not a heap memeory address
		debugf( error, "memory.c -> krealloc()", "Not a Heap Memory Block" );
		memory_error( "Not a Heap Memory Block" );
		return NULL;
	}

	// Create the new block of specified size ( is create clean )
	void* block = kmalloc_c( size );

	// Check that the kmalloc returned sucessfully
	if ( block == NULL )
		return NULL; // There was a problem with the kmalloc
	
	// This variable is needed only to save some computations
	uint32_t old_block_length = block_size( old_address );

	// Copy the value form the old address to the new one
	memcpy( block, old_address, ( old_block_length > size ) ? size : old_block_length );

	// Free the old block
	kfree( old_address );

	// Return the address to the new block
	return block;
}

void kfree( void* address )
{
	// Get the node that describe the block wich the address is in
	heap_node_t* node = find_node( address );

	// Check if a node was returned
	if ( node == NULL || node -> used == false )
	{
		// No node returned, not a heap memeory address
		debugf( error, "memory.c -> kfree()", "Not a Heap Memory Address or Address already Freed" );
		memory_error( "Not a Heap Memory Address or Address already Freed" );
		return;
	}

	// Change the variables
	node -> used = false;

	// Check if possible to merge with other blocks
	merge( ( heap_node_t* )node -> previous, node ); // Check block before
	merge( ( heap_node_t* )( node -> next ), node ); // Check block next
}

uint32_t block_size( void* address )
{
	// Get the node that describe the block wich the address is in
	heap_node_t* node = find_node( address );

	// Check if a node was returned
	if ( node == NULL )
	{
		// No node returned, not a heap memeory address
		debugf( error, "memory.c -> block_size()", "Argument is not a Heap Memory Block" );
		return 0;
	}

	// Return the size
	return node -> size;
}

void debug_heap_memory()
{
	// Get the first node
	heap_node_t* node = ( heap_node_t* )heap;

	// Print where the heap starts and ends
	cprintf( "\nHeap Start: %p", heap );
	cprintf( "\nHeap End: %p", heap + HEAP_SIZE - NODE_SIZE );

	cprintf( "\nAddress\t\tUsed\t\tSize\t\tPrevious\tNext\n");

	// Print all node informations untill the heap tail is reached
	while ( node != NULL )
	{
		cprintf( "%p", node );
		cprintf( "\t\t%d", node -> used );
		cprintf( "\t\t%u", node -> size );
		cprintf( "\t\t%p", node -> previous );
		cprintf( "\t\t%p\n", node -> next );

		// Go to the next node
		node = ( heap_node_t* )( node -> next );
	}
}

/* Private Functions Declaration */
void merge( heap_node_t* first, heap_node_t* second )
{
	// First of all check if the nodes exists
	if ( !( check_address( first ) && check_address( second ) ) )
	{
		debugf( error, "memory.c -> merge()", "Argument is not a Heap Memory Block" );
		return;
	}
	
	// Check that the blocks are in memory order
	if ( second < first )
	{
		// Not in order so order them
		heap_node_t* temp = first;
		first = second;
		second = temp;
	}

	// Check if the two node ar both free
	if ( first -> used == 0 && second -> used == 0 )
	{
		// Change nodes informations
		first -> next = second -> next;
		first -> size += second -> size + NODE_SIZE;

		if ( second -> next != NULL ) // CHECK THIS
			( ( heap_node_t* )( second -> next ) ) -> previous = ( ptr_t )first;

		// Make sure the node no longer exists
		bzero( second, NODE_SIZE );
	}
}

void split( heap_node_t* node, uint32_t size )
{
	// First of all check if the nodes exists
	if ( !check_address( node ) )
		debugf( panic, "memory.c -> split()", "Kernel Heap Corruption" ); // maybe change this?
	
	// Check that the node isn't used and is of correct size
	if ( node -> used == true || node -> size + 2 * NODE_SIZE < size )
	{
		debugf( error, "memory.c -> split()", "Heap Memory Allocation Error" );
		return;
	}

	// Create the second node
	heap_node_t* second_node = ( heap_node_t* )( ( char* )node + size + NODE_SIZE );
	initialize_node( second_node );

	// Split the memory between the nodes
	second_node -> size = node -> size - size - NODE_SIZE;
	( ( heap_node_t* )( node -> next ) ) -> previous = ( ptr_t )second_node;

	node -> size = size;
	second_node -> next = node -> next;

	node -> next = ( ptr_t )second_node;
	second_node -> previous = ( ptr_t )node;
}

heap_node_t* find_best_block( uint32_t size )
{
	// Check if the size is legitimate
	if ( size >= HEAP_SIZE - 2 * NODE_SIZE )
		debugf( error, "memory.c -> find_best_block()", "Heap Memory Allocation Error" ); // The size was to big

	// Needed variables
	heap_node_t* node = ( heap_node_t* )heap; // The actualy analizied node
	heap_node_t* previous = NULL; // The node before the node analized
	heap_node_t* smallest = ( heap_node_t* )heap; // Keeps track of the smalles node found

	// Look for a node enough big
	// Stop if the next node is null ( only the tail should have this property )
	while ( node -> next != NULL )
	{
		// Check if the node is big enough
		if ( node -> size >= size + NODE_SIZE && node -> size < size * 2 && node -> used == false )
			return node; // The size of the node is correct, so returns it
		
		// The node was not found
		previous = node;
		node = ( heap_node_t* )( node -> next );

		// Check if the smallest node is bigger than the requested size
		if ( smallest -> size > size + NODE_SIZE )
		{
			// The smalles is the one with the size most near to the one requested
			// The smallest size must must bigger than the one requested
			if ( ( previous -> size < smallest -> size && previous -> size >= size + NODE_SIZE ) || smallest -> used == true )
				smallest = previous;
		}
		// This case means that the smallest node was smaller than the requested size
		else
			smallest = previous;
		
		// Check that the actual node isn't null
		if ( node -> next != NULL )
		{
			// Check if possible to merge two nodes
			merge( previous, node );

			// node = previous; // This will make to check again the memory address
		}
	}

	if ( smallest != NULL )
	{
		split( smallest, size );
		return smallest;
	}

	debugf( error, "memory.c -> find_best_block()", "Not enough memory" );

	return NULL;
}

bool_t check_address( void* address )
{
	// Check if the address is in the heap range
	return ( ( uint8_t* )address >= heap && ( uint8_t* )address <= heap + HEAP_SIZE );
}

heap_node_t* find_node( void* address )
{
	// First of all check if the nodes exists
	if ( !check_address( address ) )
		return NULL;

	// Get the heap head
	heap_node_t* node = ( heap_node_t* )heap;

	// Look for the node
	// Stop if the next node is null ( only the tail should have this property )
	while ( node -> next != NULL )
	{
		// Check the range of address to determinate if the node was found
		if ( ( uint8_t* )node < ( uint8_t* )address && ( uint8_t* )address < ( uint8_t* )( node -> next ) )
			return node; // The node was found so returns it
		
		node = ( heap_node_t* )( node -> next );
	}

	// This line should never be executed
	return NULL;
}

void initialize_node( heap_node_t* node )
{
	node -> used = false;
	node -> size = 0;
	node -> previous = NULL;
	node -> next = NULL;
}