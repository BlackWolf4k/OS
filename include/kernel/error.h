// This is a panic function, after this function is called, the machine will have to be rebooted
// Something that should not have happened... happened! :o
// This function will print the message and halt the system
void panic_( const char* message );

// An error occurred in memory allocation
// Write a message of it
// THIS FUNCTION IS TO DO
void memory_error( const char* message );

// An error occurred with the disk
// Write a message of it
// THIS FUNCTION IS TO DO
void disk_error( const char* message );