#include "../../include/kernel/interrupts/isr.h"
#include "../../include/stdio.h"

isr_t interrupt_handlers[IDT_ENTRIES];

// Exeption messages in order of ISR
char *exception_messages[] = {
	"Division By Zero",
	"Debug",
	"Non Maskable Interrupt",
	"Breakpoint",
	"Into Detected Overflow",
	"Out of Bounds",
	"Invalid Opcode",
	"No Coprocessor",

	"Double Fault",
	"Coprocessor Segment Overrun",
	"Bad TSS",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Fault",
	"Page Fault",
	"Unknown Interrupt",

	"Coprocessor Fault",
	"Alignment Check",
	"Machine Check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",

	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};

void init_isr()
{
	for ( uint8_t i = 0; i < 32; i++ )
		idt_set_descriptor( i );
}

void isr_handler( registers_t* registers )
{
	kprintf( "Received interrupt: %d\n%s\n", registers -> interrupt_number, exception_messages[ registers -> interrupt_number ] );
	/*printk( "Received interrupt: " );
	char s[3];
	itoa( registers -> interrupt_number, s, 10 );
	printk( s );
	printk( "\n" );
	printk( exception_messages[ registers -> interrupt_number ] );
	printk( "\n" );*/
}

void register_interrupt_handler( uint8_t number, isr_t handler )
{
	interrupt_handlers[number] = handler;
}