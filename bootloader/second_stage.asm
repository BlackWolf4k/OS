init_second_stage:
	; Set the stack pointer
	mov sp, 0x7c00

	; Get lower memory size
	; Get upper memory size
	; Enable a20

	ret

switch_to_protected_mode:
	; Disable interrupts
	cli

	; Enable a20
	call enable_a20

	; Load the GDT
	call load_gdt

	; Go in protected mode
	call go_protected_mode

enable_a20:
	ret

load_gdt:
	lgdt[GDT]
	ret

go_protected_mode:
	mov eax, cr0
	or eax, 1
	mov cr0, eax

	ret

start_protected_mode:
	; Load each segment register with data segment
	mov ax, DATA_SEGMENT
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	; Set the stack pointer to the protected mode stack
	;mov esp, 0xf000
	mov ebp, 0x90000
	mov esp, ebp

	jmp $

read_kernel:
	; Select subfunction 0x02
	mov ah, 0x02

	; Set the number of sectors to read
	mov al, 30

	; Set the chs
	mov dh, 0x00 ; Head number
	mov ch, 0x00 ; Cylinder number
	mov ah, 0x02 ; Sector number

	; Choosed device to read from
	mov dl, 0
	
	; Buffer where to store the readed data
	mov bx, 0xf000

	; Call bios int 0x13 with subfunction 0x13
	int 0x13

	ret

second_stage_flow:
	mov si, msg
	;call print
	call init_second_stage
	call switch_to_protected_mode
	jmp CODE_SEGMENT:start_protected_mode

%include "bootloader/gdt.asm"
;%include "bootloader/utilities.asm"

msg: db "Second Stage Started\0"

jmp second_stage_flow