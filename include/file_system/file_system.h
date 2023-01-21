#include "../kernel/disk.h"

// Handle the file system
// There is one file system now: ext, wich is my review of the ext2 file system

// Enumerate the file systems
typedef enum
{
	ext
} file_system_t;

// Create a file system for a partition on the disk
// Requires as argument the descriptor of the partition and the type of file_system required
// Returns a status code with the result of the creation ( 1: everything was fine, 0: error )
void make_file_system( partition_table_descriptor_t partition_descriptor, file_system_t file_system );