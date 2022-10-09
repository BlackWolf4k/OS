print:
	pusha

	.loop:
		lodsb
		or al, al
		jz .done
		mov ah, 0x0e
		mov bh, 0x00
		int 0x10
		jmp short .loop
	
	.done:
		popa
		ret