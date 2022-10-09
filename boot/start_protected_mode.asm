[bits 16] ; specify 16bit mode
switch_to_protected_mode:
	cli ; Disable interrupts
	lgdt [GDT] ; Load gdt
	; Change last bit of cr0 to 1
	mov eax, cr0 ; can't access directly cr0
	or eax, 1
	mov cr0, eax
	jmp CODE_SEGMENT:start_protected_mode

[bits 32] ; specify 32bit mode
start_protected_mode:
	; set all the registers
	mov ax, DATA_SEGMENT
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	mov ebp, 0x90000
	mov esp, ebp

	; far jump to the kernel
	jmp KERNEL_LOCATION