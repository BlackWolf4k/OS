#include "../include/kernel/disk.h"

#include "../include/string.h"

typedef enum
{
	err = 0x01,
	drq = 0x08,
	df = 0x20,
	bsy = 0x80,
} status_register_values_t;

/* Private Functions */
// Wait that the disk returns a certain status
void wait_status( uint8_t status );

partition_table_descriptor_t create_partition_descriptor( bool_t bootable, chs_t chs_start, uint8_t partition_id, chs_t chs_end )
{
	// Descriptor to return
	partition_table_descriptor_t descriptor;

	// Set if partition is bootable
	descriptor.bootable = ( bootable ) ? 0x80 : 0;

	// Describe the start of partition
	descriptor.starting_head = chs_start.head;
	descriptor.starting_sector = ( ( ( chs_start.cylinder >> 6 ) & 0x3 ) << 6 ) + chs_start.sector & 0x3f; // CLEAR THIS
	descriptor.starting_cylinder = chs_start.cylinder & 0xff;

	descriptor.system_id = partition_id;

	descriptor.ending_head = chs_end.head;
	descriptor.ending_sector = ( ( ( chs_end.cylinder >> 6 ) & 0x3 ) << 6 ) + chs_end.sector & 0x3f; // CLEAR THIS
	descriptor.ending_cylinder = chs_end.cylinder & 0xff;

	descriptor.relative_sector = chs_to_lba( chs_start );

	descriptor.total_sectors = chs_to_lba( chs_end ) - chs_to_lba( chs_start );

	return descriptor;
}

void create_partition( uint8_t partition_number, partition_table_descriptor_t partition_informations )
{
	// Check that the partition number is valid
	if ( partition_number < 0 || partition_number > 3 )
	{
		// Invalid partition number
		// Give error and exit the execution
		disk_error( "Partition Number is not recognized" );
		return;
	}

	// Partition offsets ( described in the header )
	uint16_t partitions[4] = { 0x01be, 0x01ce, 0x01de, 0x01ee };

	// Check that the partition isn't used
	// check_partition()

	// Write informations to the disk
	char* informations = kmalloc_c( 512 );
	// informations[ 510 ] = 0x55;
	// informations[ 511 ] = 0xff;

	partition_table_descriptor_t* pt = &partition_informations;

	memcpy( informations + partitions[ partition_number ], pt, sizeof( partition_table_descriptor_t ) );

	//for ( uint32_t i = 0; i < sizeof( partition_table_descriptor_t ); i++ )
	//	informations[ partitions[ partition_number ] + i ] = ( ( uint8_t* )pt )[i];

	write_disk( informations, 511, 0x00 );

	kfree( informations );
	// kfree( pt );
}

void remove_parition()
{}

uint32_t chs_to_lba( chs_t chs )
{
	// Conversion costants
	uint32_t HPC = 16;
	uint32_t SPT = 63;

	// Make the conversion and return the result
	return ( chs.cylinder * HPC + chs.head ) * SPT + ( chs.sector - 1 );
}

chs_t lba_to_chs( uint32_t lba )
{
	// Conversion costants
	uint32_t HPC = 16;
	uint32_t SPT = 63;

	// Value to return
	chs_t chs;

	// Calculate the cylinder
	chs.cylinder = lba / ( HPC * SPT );

	// Calculate the head
	chs.head = ( lba / SPT ) % HPC;

	// Calculate the sector
	chs.sector = ( lba % SPT ) + 1;

	// Return the chs
	return chs;
}

void read_disk( ptr_t buffer, uint8_t sectors, uint32_t LBA )
{
	// Wait the disk is ready
	wait_status( bsy );

	outb( 0x1f6, 0xe0 | ( ( LBA >> 24 ) & 0xf ) );

	// Send the number of sectors to read
	outb( 0x1f2, sectors );

	// Send the lowest 8 bits of the LBA
	outb( 0x1f3, LBA );

	// Send the middle 8 bits of the LBA
	outb( 0x1f4, LBA >> 8 );

	// Send the highest 8 bits of the LBA
	outb( 0x1f5, LBA >> 16 );

	// Send the read command
	outb( 0x1f7, 0x20 );

	uint16_t* new_buffer = ( uint16_t* )buffer;

	// Read the data
	// CHECK ALL THIS
	for ( int i = 0; i < sectors; i++ )
	{
		// Wait that the disk is ready
		wait_status( bsy );
		wait_status( drq );

		// Read the data
		for ( int j = 0; j < 256; j++ )
		{
			*( new_buffer + j ) = inw( 0x1f0 );
		}
		new_buffer += 256;
	}
}

void write_disk( ptr_t buffer, int32_t length, uint32_t LBA )
{
	// The length translated in sectors
	uint32_t sectors = length / 512 + 1;
 
	// Wait the disk is ready
	wait_status( bsy );

	outb( 0x1f6, 0xe0 | ( ( LBA >> 24 ) & 0xf ) );

	// Send the number of sectors to write
	outb( 0x1f2, ( uint8_t )sectors ); // <- CHECK THIS

	// Send the lowest 8 bits of the LBA
	outb( 0x1f3, LBA );

	// Send the middle 8 bits of the LBA
	outb( 0x1f4, LBA >> 8 );

	// Send the highest 8 bits of the LBA
	outb( 0x1f5, LBA >> 16 );

	// Send the read command
	outb( 0x1f7, 0x30 );

	// Write the data
	// Block length is 256 becouse a int16bit is written
	// CHECK ALL THIS
	uint16_t* new_buffer = ( uint16_t* )buffer;
	for ( int i = 0; i < sectors; i++ ) // <- CHECK THIS
	{
		// Wait that the disk is ready
		wait_status( bsy );
		wait_status( drq );

		// Write the data
		// 2 bytes at time are written
		for ( int j = 0; j < 256; j++, length -= 2 ) // CHANGE THIS
		{
			// If length left is one, make the second written byte to 0
			if ( length == 1 )
				outw( 0x1f0, *( new_buffer + j ) & 0x00 );
			// If the length is bigger or equal to zero and is not one, keep writing data
			else if ( length >= 0 )
				outw( 0x1f0, *( new_buffer + j ) );
			// No more data to write, fill with 0s
			else
				outw( 0x1f0, 0 );
		}
		new_buffer += 256;
	}
}

/* Private Functions Decleartion */
void wait_status( uint8_t status )
{
	if ( status == bsy )
		while ( ( inb( 0x1f7 ) & status ) );
	else if ( status == drq )
		while( !( inb( 0x1f7 ) & status ) );
}