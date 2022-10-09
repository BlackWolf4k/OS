#pragma once

#include "idt.h"
#include "../../stddef.h"

// Structure that contains the CPU registers data
typedef struct {
	uint32_t ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t interrupt_number, error_code;
	uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

// Initialize all the ISRs
void init_isr();

// Function that handles the ISRs
// registers = CPU registers
void isr_handler( registers_t* registers );

// Pointer to the function that the interrupt has to call
// As argument taxes the CPU registers
typedef void ( *isr_t )( registers_t* );

// Array of all the interrupts handlers
extern isr_t interrupt_handlers[IDT_ENTRIES];

// Add a new interrupt handler
void register_interrupt_handler( uint8_t number, isr_t handler );