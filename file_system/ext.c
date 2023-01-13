/* SOME INFORMATIONS */
/*
SUPERBLOCK
The superblock is located 1024 bytes from the start of the partition and is 1024 bytes long
Superblock descriptor:
Offset	SIZE		Description
+0		4			Number of inodes in the file system
+4		4			Number of blocks in the file system
+8		4			Number of blocks reserved for the superuser
+12		4			Number of unlocated blocks
+16		4			Number of unlocated inodes
...

FILE SYSTEM STATE
ERROR HANDLING METHODS
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
Offset	SIZE		Description
+0		4			Block address of block usage bitmap
+4		4			Block address of inode usage bitmap
+8		4			Starting block of inode table
+12		2			Number of unllocated blocks
+14		2			Number of unllocated inodes
+16		2			Number of directories in the group
+18		x			Unused

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
+24		4			Group ID
+26		2			Count of hard disk links
+28		2			Count of disks sectors
+32		4			Flags
+36		4			Operating System Specific Value #1 // TO REMOVE
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
+100	4			Generation Number // TO REMOVE
+104	4			Reserved
+108	4			Reserved
+112	4			Block address of fragment
+116	12			Operating System Specific Value #2 // TO REMOVE

Inode Type and Permissions
Value		Description
0x1000		FIFO
0x2000		Character device
0x4000		Directory
0x6000		Block device
0x8000		Regular File
0xA000		Symbolic Link
0xC000		Unix Socket
*/