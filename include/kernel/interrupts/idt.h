#pragma once

#include "../../stddef.h"

// Kernel code segment
#define KERNEL_CS 0x08

// Max number of idt entryes for a 32 bit system
#define IDT_ENTRIES 256

// Structure for register of all the idts
typedef struct
{
	uint16_t limit;
	uint32_t base;
} __attribute__( ( packed ) ) idt_register_t;

// Structure for each isr / irq
typedef struct
{
	uint16_t offset_low; // offset 0 - 15
	uint16_t kernel_cs;
	uint8_t reserved; // always 0
	uint8_t attributes;
	uint16_t offset_high; // offset 16 - 31
} __attribute__( ( packed ) ) idt_entry_t;

// Set a descriptor in IDT, given it's number
// number = number of the descriptor
void idt_set_descriptor( uint8_t number );

// Initialize the IDT, ISRs and IRQs
void init_idt();

// Load the IDT
void load_idt();