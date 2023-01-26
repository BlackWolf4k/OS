/* SOME INFORMATIONS */
/*
FileSystem name: ext0
ext0 is my reinterpretation of the ext1 / ext2 file system
It uses the basics concepts of the ext file systems but in a simpler way:
	-The superblock is reduces, there are less informations
	-The block groups are more compact:
	Each block group is composed by 8192 blocks:
		-The first 2 blocks are reserved for bitmaps ( blocks bitmap and inodes bitmap )
		-The following 32 blocks are reserved for the indodes
		-All the remaining are usable to store data
	-The inodes are pretty much the same
The size of a block is not changeble, it is of 1024 byte


SUPERBLOCK
The superblock is located 1024 bytes from the start of the partition and is 1024 bytes long
Superblock descriptor:
Offset	SIZE		Description
+0		4			Number of inodes in the file system
+4		4			Number of blocks in the file system
+8		4			Number of the block of the superblock
+12		4			Last mount timestamp
+16		4			Last written timestamp
+20		2			ext signature 0xef53 // TO REMOVE ?
+22		2			File system state
+24		12			Error handling methods
+36		4			Creator Operating System

FILE SYSTEM STATE
Value	State
1		Clean
2		Has Errors
3		Is damaged ( kernel panic )

ERROR HANDLING METHODS
Offset 	Description
0		Ignore
4		Correction
8		Kernel panic

CREATOR OPERATING SYSTEM ID
Value	Operating System
0		Linux
1		GNU HURD
2		MASIX
3		FreeBSD
4		Mine // Insert here name of os

BLOCK GROUP DESCRIPTOR
Contains the informations about the other blocks

Block group descriptor:
BLOCK OFFESET	BLOCK SIZE		Description
+0				1				Contains the bitmap of the blocks ( 0: unused, 1: used )
+1				1				Contains the address of each inode
+2				32				Contains the inodes
+34				8158			Block for data

INODE
Structure that reppresents data indirectly
It has a size of 128 bytes

Inode descriptor
Offset	SIZE		Description
+0		2			Type of permission
+2		2			User ID
+4		4			Lower 32 bits of file size in bytes
+8		4			Last acces timestamp
+12		4			Creation timestamp
+16		4			Last Modify timestamp
+20		4			Deletion timestamp
+24		2			Group ID
+26		2			Count of hard disk links
+28		2			Count of disks sectors
+32		4			Flags
+36		4			Removed
+40		4			Direct Block Pointer 0
+44		4			Direct Block Pointer 1
+48		4			Direct Block Pointer 2
+52		4			Direct Block Pointer 3
+56		4			Direct Block Pointer 4 
+60		4			Direct Block Pointer 5
+64		4			Direct Block Pointer 6
+68		4			Direct Block Pointer 7
+72		4			Direct Block Pointer 8
+76		4			Direct Block Pointer 9
+80		4			Direct Block Pointer 10
+84		4			Direct Block Pointer 11
+88		4			Single Indirect Block
+92		4			Double Indirect Block
+96		4			Triple Indirect Block
+100	4			Removed
+104	4			Reserved
+108	4			Reserved
+112	4			Block address of fragment // REMOVE
+116	12			Removed

Inode Type
Value		Description
0x1000		FIFO
0x2000		Character device
0x4000		Directory
0x6000		Block device
0x8000		Regular File
0xA000		Symbolic Link
0xC000		Unix Socket

Inode Permissions
Value		Description
0x001		Other - execute
0x002		Other - write
0x004		Other - read
0x008		Group - execute
0x010		Group - write
0x020		Group - read
0x040		User - execute
0x080		User - write
0x100		User - read
0x200		Removed
0x400		Removed // to remove??
0x800		Removed // to remove??

MATH
Number of blocks per partition
nb( ns, bs ) = ( ( ns - 2 ) * 512 ) / bs
Where:
	ns = number of sectors
	bs = block size
Each sector is 512 bytes long
The size of the superblock is already subtracted

Inodes per block
ib( bs ) = bs / 128
Where:
	bs = block size
Each inode is 128 bytes long
*/

#include "../include/file_system/ext.h"
#include "../include/kernel/memory.h"
#include "../include/kernel/debug.h"
#include "../include/string.h"
#include "../include/kernel/memory.h"

// The size of a block
#define BLOCK_SIZE 1024

typedef struct
{
	ptr_t ignore;
	ptr_t correction;
	ptr_t panic;
} error_handling_methods_t;

typedef struct
{
	uint32_t number_of_inodes;
	uint32_t number_of_blocks;
	uint32_t starting_block;
	uint32_t last_mount;
	uint32_t last_written;
	uint16_t ext2_signature;
	uint16_t file_system_state;
	error_handling_methods_t error_handling_methods;
	uint32_t operating_system_id;
} superblock_t;

enum
{
	clean = 1,
	errors = 2,
	panic_ = 3
} file_system_state_t;

enum
{
	linux = 0,
	gnu_hurd,
	masix,
	freebsd,
	mine // change the name
} creator_operating_system_ids_t;

typedef struct
{
	uint8_t bytes[ BLOCK_SIZE ];
} block_t;

typedef struct
{
	block_t blocks_bitmap; // Blocks bitmap
	block_t inodes_addresses;  // The address of the inodes
	block_t inodes_blocks[ 32 ]; // Store the inodes descriptors
	block_t data_blocks[ 8158 ]; // The blocks where to store the data
} block_group_t;

typedef struct
{
	uint16_t permission;
	uint16_t user_id;
	uint32_t lower_size;
	uint32_t last_access;
	uint32_t creation;
	uint32_t last_modify;
	uint32_t delete;
	uint16_t group_id;
	uint16_t hard_disk_links_count;
	uint32_t disk_sectors_count;
	uint32_t flags;
	uint32_t __removed0;
	ptr_t direct_block_pointer_0;
	ptr_t direct_block_pointer_1;
	ptr_t direct_block_pointer_2;
	ptr_t direct_block_pointer_3;
	ptr_t direct_block_pointer_4;
	ptr_t direct_block_pointer_5;
	ptr_t direct_block_pointer_6;
	ptr_t direct_block_pointer_7;
	ptr_t direct_block_pointer_8;
	ptr_t direct_block_pointer_9;
	ptr_t direct_block_pointer_10;
	ptr_t direct_block_pointer_11;
	ptr_t single_direct_block_pointer;
	ptr_t double_direct_block_pointer;
	ptr_t triple_direct_block_pointer;
	uint32_t __removed1;
	uint32_t __reserved0;
	uint32_t __reserved1;
	uint32_t fragment_block_address;
	uint32_t __removed2[3];
} inode_t;

typedef enum
{
	fifo = 0x1000,
	character_device = 0x2000,
	directory = 0x4000,
	block_device = 0x6000,
	regular_file = 0x8000,
	symbolic_link = 0xa000,
	unix_socket = 0xc000
} inode_type_t;

typedef enum
{
	other_execute = 0x001,
	other_write = 0x002,
	other_read = 0x004,
	group_execute = 0x008,
	group_write = 0x010,
	group_read = 0x020,
	user_execute = 0x040,
	user_write = 0x080,
	user_read = 0x100,
	__removed0 = 0x200,
	__removed1 = 0x400,
	__removed2 = 0x800
} inode_permissions_t;

/* PRIVATE FUNCTIONS */
/*
Write a block to the disk
ARGUMENTS: ( block_t*, uint32_t, superblock_t* )
	-block: the block to write
	-block_offset: how much to offset from the beginning of the partition
	-superblock: pointer to the superblock of the partition
RETURN: ( uint8_t )
	-0: Error
	-1: OK
*/
uint8_t write_block( block_t* block, uint32_t block_offset, superblock_t* superblock );

/* PUBLIC FUNCTIONS */

uint8_t make_ext( partition_table_descriptor_t partition_descriptor )
{
	// Allocate two blocks in the heap memory
	block_t* block = kmalloc_c( sizeof( block_t ) );
	block_t* super_block = kmalloc_c( sizeof( block_t ) );

	// Check that the allocation was sucessfull
	if ( block == NULL || super_block == NULL )
	{
		// Print some debug informations
		debugf( error, "file_system/ext.c -> make_ext()", "There was an error allocating the memory" );

		// Return a error code
		return 0;
	}

	// Set the superblock
	// Calcolate the number of blocks
	uint32_t number_of_blocks = partition_descriptor.total_sectors / ( BLOCK_SIZE / 512 );

	// Calcolate number of block groups
	uint32_t number_of_block_groups = number_of_blocks / ( BLOCK_SIZE * 8 );

	kprintf( "Number of blocks: %d\n", number_of_block_groups );

	// Calcolate number of inode
	uint32_t number_of_inodes = number_of_block_groups * ( BLOCK_SIZE / 4 );

	// Calculate number of blocks for the inodes
	uint32_t number_of_blocks_for_inodes = ( sizeof( inode_t ) * number_of_inodes ) / BLOCK_SIZE;

	// Set the values of the superblock
	( ( superblock_t* )super_block ) -> number_of_inodes = number_of_inodes;
	( ( superblock_t* )super_block ) -> number_of_blocks = number_of_blocks;
	( ( superblock_t* )super_block ) -> starting_block = partition_descriptor.relative_sector;
	( ( superblock_t* )super_block ) -> file_system_state = clean;
	( ( superblock_t* )super_block ) -> operating_system_id = 0x7777; // to change

	// Write the superblock
	write_block( super_block, 0, ( superblock_t* )super_block );

	// Clear the block buffer
	bzero( block, BLOCK_SIZE );

	// Set the default values of the block bitmap
	memset( block, 255, 4 );

	// Write the block groups block descriptors
	for ( uint32_t i = 0; i < number_of_block_groups; i++ )
		// Write the block bitmap
		write_block( block, i + 1, ( superblock_t* )super_block );
	
	// Clear the block buffer
	bzero( block, BLOCK_SIZE );
	
	// Write the block groups inodes' addresses block
	// for ( uint32_t i = 0; i < number_of_block_groups; i++ )
	// 	write_block( block, ( i * ( BLOCK_SIZE * 8 ) ) + 1, ( superblock_t* )super_block );
	
	// Clear the block buffer
	bzero( block, BLOCK_SIZE );

	// Create empty inode
	inode_t inode;

	// Fill a block with inode descriptors
	// for ( uint32_t i = 0; i < BLOCK_SIZE / sizeof( inode_t ); i++ )
	// 	( ( inode_t* )block )[i] = inode;

	// Write the inodes blocks
	// for ( uint32_t i = 0; i < number_of_block_groups; i++ )
	// 	for ( uint32_t j = 0; j < ( number_of_blocks_for_inodes ); j++ )
	// 		write_block( block, ( i * ( BLOCK_SIZE * 8 ) ) + 2, ( superblock_t* )super_block );

	// Check the creation

	// Free the memory spaces
	kfree( super_block );
	kfree( block );
}

/* PRIVATE FUNCTIONS */

uint8_t write_block( block_t* block, uint32_t block_offset, superblock_t* superblock )
{
	// Get the lba offset
	uint32_t lba = superblock -> starting_block + block_offset * ( BLOCK_SIZE / 512 ); // 512 = sector size

	// Write to the disk
	write_disk( ( ptr_t )block, BLOCK_SIZE, lba );

	// For now there is no control if the writing was sucessfull
	// CORRECT THIS
	return 1;
}