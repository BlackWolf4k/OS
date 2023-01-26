#pragma once

#include "../stddef.h"
#include "../kernel/disk.h"

/*
Create a ext file system
ARGUMENTS: ( partition_table_descriptor_t )
	-partition_descriptor: descriptor of the partition of the disk where to make the file system
RETURN: ( uint8_t )
	-0: Error
	-1: OK
*/
uint8_t make_ext( partition_table_descriptor_t partition_descriptor );