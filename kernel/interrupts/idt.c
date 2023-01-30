#include "../../include/kernel/interrupts/idt.h"
#include "../../include/kernel/interrupts/isr.h"
#include "../../include/kernel/interrupts/irq.h"
#include "../../include/stdio.h"

// Register for idts
idt_register_t idt_register;

// Entry of each idt
idt_entry_t idt[IDT_ENTRIES];

// Location of assembly isr / irq
extern uint32_t interrupts_vector[]; // Contained in interrupts.asm

void idt_set_descriptor( uint8_t number )
{
	idt[number].offset_low = ( uint16_t )( ( ( uint32_t )( interrupts_vector[number] ) ) & 0xffff );
	idt[number].kernel_cs = KERNEL_CS;
	idt[number].reserved = 0;
	idt[number].attributes = 0x8e; // <- check this
	idt[number].offset_high = ( uint16_t )( ( ( ( uint32_t )( interrupts_vector[number] ) ) >> 16 ) & 0xffff );
}

void init_idt()
{
	// Load all the isr
	init_isr();
	
	// Load the irq
	init_irq();

	// Load the idt register
	load_idt();

	lprintf( 1, "Interrupts Initialized\n" );
}

void load_idt()
{
	// Load the idt register
	idt_register.base = ( uint32_t )&idt;
	idt_register.limit = ( sizeof( idt_entry_t ) * IDT_ENTRIES - 1 );

	// Tell assembly to load the idt
	asm volatile( "lidt (%0)" : : "r" ( &idt_register ) );
	// Set the interrupts
	asm volatile ( "sti" );
}