// This ext file system is my review of the ext2 file system

// Create the super block of a partition
// Requires as input the descriptor of the partition
void ext_create_super_block( partition_table_descriptor_t partition_descriptor );

//