#pragma once

#include "../stddef.h"
#include "../stdio.h"

/*
DISK STATUS INFORMATIONS
ERR: An error occured, send a new command to clear it
IDX: Index always to zero
CORR: Corrected data, always to zero
DRQ: Read to accept PIO data
SRV: Overleaped mode service request
DF: Driver Fault Error ( doesn't set ERR )
RDY: Ready to recive data
BSY: Preparing to send / recive commands

REGISTERS INFORMATIONS
0x1F0: Base address				SIZE
+0: Data register				16 bits
+1: ( R ): Error register, 		8 bits
+1: ( W ): Features register, 	8 bits
+2: Sector count register, 		8 bits
+3: Sector number register, 	8 bits
+4: Cylinder low register, 		8 bits
+5: Cylinder high register, 	8 bits
+6: Drive/Head register, 		8 bits
+7: ( R ): Status register, 		8 bits
+7: ( W ): Command register, 	8 bits

PARTITIONING INFORMATIONS ( MBR )
Partition n		OFFSET
Partition 1: 	0x01be
Partition 2: 	0x01ce
Partition 3: 	0x01de
Partition 4: 	0x01ee

Descriptor
Offset	SIZE		Description
+0		8 bits		Boot indicator flag ( 0 = no, 0x80 = bootable )
+1		8 bits		Starting head
+2		6 bits		Starting sector ( bits 6 - 7 are the upper of the Starting Cylinder )
+3		10 bits		Starting Cylinder
+4		8 bits		System ID ( 82 = Linux Swap, 83 = Linux Native Partition ( Microsoft Standard ) )
+5		8 bits		Ending Head
+6		6 bits		Ending Sector ( bits 6 - 7 are the upper of Ending Cylinder )
+7		10 bits		Ending Cylinder
+8		32 bits		Relative Sector ( LBA value )
+12		32 bits		Total Sectors
*/

// Describes a CHS tuple
typedef struct
{
	uint8_t sector; // maximum of 63
	uint8_t head; // maximum of 15
	uint32_t cylinder; // no additional limits
} chs_t;

// Describe a partition table element
typedef struct
{
	uint8_t bootable;
	uint8_t starting_head;
	uint8_t	starting_sector;
	uint8_t starting_cylinder;
	uint8_t system_id;
	uint8_t ending_head;
	uint8_t ending_sector;
	uint8_t ending_cylinder;
	uint32_t relative_sector;
	uint32_t total_sectors;
} partition_table_descriptor_t;

// Create a partition descriptor
partition_table_descriptor_t create_partition_descriptor( bool_t bootable, chs_t chs_start, uint8_t partition_id, chs_t chs_end );

// Creates a partition in the disk
void create_partition( uint8_t partition_number, partition_table_descriptor_t partition_informations );

// Removes a partition from the disk
void remove_parition();

// CHS to LBA
// LBA = ( C * HPC + H ) * SPT + ( S - 1 )
// HPC = usualy is 16, SPT = usualy 63
uint32_t chs_to_lba( chs_t chs );

// LBA to CHS
// C = LBA / ( HPC * SPT )
// H = ( LBA / SPT ) % HPC
// S = ( LBA % SPT ) + 1
// HPC = usualy is 16, SPT = usualy 63
chs_t lba_to_chs( uint32_t lba );


// Reads from the disk n sectors at addres LBA and stores them in buffer
// buffer = buffer to store data, sectors = sector to read, LBA = logical block address
void read_disk( uint32_t buffer, uint8_t sectors, uint32_t LBA );

// Wrote to the disk n sectors at addres LBA from buffer
// buffer = buffer to read data, length = number of bytes, LBA = logical block address
void write_disk( uint32_t buffer, int32_t length, uint32_t LBA );