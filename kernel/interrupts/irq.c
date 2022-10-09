#include "../../include/kernel/interrupts/isr.h"
#include "../../include/kernel/interrupts/idt.h"
#include "../../include/stdio.h"

/* Private Functions */
// Send to PIC iformations to remap IRQ
void irq_remap();

// Send to PIC message of end of interrupt
// interrupt number = number of IRQ who called
void irq_send_eoi( uint32_t interrupt_number );

void init_irq()
{
	// Map the irq
	irq_remap();
	
	// Load all the irq
	for ( uint8_t i = 32; i < 48; i++ )
		idt_set_descriptor( i );
}

void irq_handler( registers_t* registers ) // Clear this code
{
	if ( interrupt_handlers[registers -> interrupt_number] != 0 )
	{
		isr_t handler = interrupt_handlers[registers -> interrupt_number];
		handler( registers );
	}
	irq_send_eoi( registers -> interrupt_number );
}

void irq_remap()
{
	// Change with PIC names
	outb( 0x20, 0x11 );
	outb( 0xA0, 0x11 );
	outb( 0x21, 0x20 );
	outb( 0xA1, 0x28 );
	outb( 0x21, 0x04 );
	outb( 0xA1, 0x02 );
	outb( 0x21, 0x01 );
	outb( 0xA1, 0x01 );
	outb( 0x21, 0x00 );
	outb( 0xA1, 0x00 );
}

void irq_send_eoi( uint32_t interrupt_number )
{
	// Check if slave
	if ( interrupt_number >= 40 )
		outb( 0xA0, 0x20 ); // is slave
	outb( 0x20, 0x20 );
}