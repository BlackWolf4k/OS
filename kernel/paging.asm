.global load_page_directory
load_page_directory:
	; Get the argument
	push %ebp
	mov %esp %ebp

	; Copy page directory to %cr3
	mov 8( %esp ), %eax
	mov %eax, %cr3

	; Clear
	mov %ebp, %esp
	pop %ebp

	ret

.global enable_paging
enable_paging:
	; No arguments to get
	push %ebp
	mov %esp %ebp

	; To enable paging the 32th bit of CR0 has to be set
	mov %cr0, %eax
	or $0x80000000, %eax
	mov %eax, %cr0
	
	; Clear
	mov %ebp, %esp
	mov %ebp

	ret