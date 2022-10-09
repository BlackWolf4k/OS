GDT_Start:
	null:
		dd 0x0
		dd 0x0
		; everything is empty
	code:
		dw 0xffff ; first four bytes of the limit
		dw 0x0 ; first four bytes of the base
		db 0x0 ; byte of the base
		db 0b10011010 ; present(0), privilege(00), descriptor_type(1), executable(1), executed_from_lower_privilege(0), readable(1), accessed_from_cpu(0)
		db 0b11001111 ; limit_increased(1), is_for_32_bit_memory(1), things_for_long_mode(00), last_four_bits_of_limit(1111)
		db 0x0 ; last byte of the base
	data:
		dw 0xffff ; first four bytes of the limit
		dw 0x0 ; first four bytes of the base
		db 0x0 ; byte of the base
		db 0b10010010 ; present(0), privilege(00), descriptor_type(1), executable(0), grow_down_segment(0), writable(1), accessed_from_cpu(0)
		db 0b11001111 ; limit_increased(1), is_for_32_bit_memory(1), things_for_long_mode(00), last_four_bits_of_limit(1111)
		db 0x0 ; last byte of the base
GDT_End:

GDT:
	dw GDT_End - GDT_Start - 1 ; size of gdt
	dd GDT_Start ; start of the gdt

; specify values for code and data segments
CODE_SEGMENT equ code - GDT_Start ; constant value
DATA_SEGMENT equ data - GDT_Start ; constant value