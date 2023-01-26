#include "../include/file_system/file_system.h"
#include "../include/file_system/ext.h"

/* PUBLIC FUNCTION */
void make_file_system( partition_table_descriptor_t partition_descriptor, uint8_t file_system )
{
	make_ext( partition_descriptor );
}