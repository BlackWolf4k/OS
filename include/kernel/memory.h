#pragma once

#include "../stddef.h"

// Initialize the dynamic memory ( heap )
void init_heap();

// Allocate a memory block
// If block is found returns the address, else returns NULL
// The memory is not cleared
// size = size of the memory block
void* kmalloc( uint32_t size );

// Allocate a memory block
// If block is found returns the address, else returns NULL
// The memory is cleared
// size = size of the memory block
void* kmalloc_c( uint32_t size );

// Reallocate a memory block
// If block is found returns the address and copy the content, else returns NULL
// The memory is not cleared
// old_address = old heap block, size = size of the new block
void* krealloc( void* old_address, uint32_t size );

// Free a memory block
// address = address to free
void kfree( void* address );

// Return the size of a block
// address = address wich size is wanted
uint32_t block_size( void* address );

// REMOVE THIS FUNCTION?
// Debugs the whole heap memory
void debug_heap_memory();