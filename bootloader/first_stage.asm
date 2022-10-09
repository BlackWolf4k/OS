[org 0x7c00]
[bits 16]

[section .text]

DISK_NUMBER: db 0 ; Store the number of the booted device
SECOND_STAGE_ADDRESS equ 0x1000 ; The location of the second stage of the bootloader is constant

global _start
_start:
	jmp short first_stage_flow
	nop

init_first_stage:
	; Disable interrupts
	cli

	; Store the bootdisk number
	mov [DISK_NUMBER], dl

	; Load segment registers
	; Set segment registers to zero
	mov ax, 0x00
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	; Set the stack pointer
	mov sp, 0x7c00

	; Enable the interrupts
	sti

	ret

reset_disk:
	; Select driver to reset
	mov dl, [DISK_NUMBER]

	; Select subfunction 0x00
	mov ax, 0

	; Call bios int 0x13 with subfunction 0x00
	int 0x13

	; Jump in case there was an error
	; jc floppy_error

	ret

read_disk:
	; Select subfunction 0x02
	mov ah, 0x02

	; Set the number of sectors to read
	mov al, 2

	; Set the chs
	mov dh, 0x00 ; Head number
	mov ch, 0x00 ; Cylinder number
	mov ah, 0x02 ; Sector number

	; Choosed device to read from
	mov dl, [DISK_NUMBER]
	
	; Buffer where to store the readed data
	mov bx, SECOND_STAGE_ADDRESS

	; Call bios int 0x13 with subfunction 0x13
	int 0x13

	ret

start_second_stage:
	jmp SECOND_STAGE_ADDRESS:0

	ret

first_stage_flow:
	mov si, a
	call print
	call init_first_stage
	call reset_disk
	call read_disk
	call start_second_stage

%include "bootloader/second_stage.asm"
%include "bootloader/utilities.asm"

; Error handlers

floppy_error:
	mov si, floppy_error_message
	call print
	ret

; Messages
floppy_error_message: db "There was an error reading the bootloader\0"
a: db "Ciao\0"

times 510-($-$$) db 0
dw 0xaa55 ; Magic numbers for boot segment