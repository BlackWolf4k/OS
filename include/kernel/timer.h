#pragma once

#include "../stddef.h"

// Initialize the timer
// Timer uses the IRQ0
// frequency = frequency of the timer
void init_timer( uint32_t frequency );

// Get the current tick count
uint32_t get_ticks();

// Get timestamp
// Starts from date: 05 / 10 / 2004
uint32_t get_timestamp();

void sleep( uint32_t seconds );