global _start
[bits 32]

_start:
	[extern kmain]
	call kmain
	jmp $

;section .text
;	[bits 32]
;	[extern kmain]
;	call kmain
;	jmp $