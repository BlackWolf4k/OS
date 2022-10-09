#pragma once

#include "isr.h"

/*
LIST OF ALL THE ISR
0	PIT
1	Keyboard Interrupt
2	Cascade ( never rised )
3	COM2 ( if enabled )
4	COM1 ( if enabled )
5	LPT2 ( if enabled )
6	Floppy disk
7	LPT1
8	CMOS Real time clock
9 	Free for peripherals
10 	Free for peripherals
11 	Free for peripherals
12 	PS2 Mouse
13 	FPU / Coprocessor
14 	Primary ATA HDD
15 	Secondary ATA HDD
*/

// Initialize all the IRQs
void init_irq();

// Function that handles the IRQs
// registers = CPU registers
void irq_handler( registers_t* registers );