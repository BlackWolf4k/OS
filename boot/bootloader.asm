section .text
[org 0x7c00]
KERNEL_LOCATION equ 0x1000 ; constant kernel position

; Read from disk the kernel
BOOT_DISK: db ; 6 <- for hard disk
mov [BOOT_DISK], dl

xor ax, ax
mov es, ax
mov ds, ax
mov bp, 0x9000 ; stack away from bootloader
mov sp, bp

mov bx, KERNEL_LOCATION
mov dh, 31 ; <- for size problems increase this

mov ah, 0x02 ;
mov al, dh ; number of sectors to read
mov ch, 0x00 ; cylinder number
mov dh, 0x00 ; head number
mov cl, 0x02 ; sector number
mov dl, [BOOT_DISK] ; disk number
int 0x13

; Load video mode
;mov ah, 0x0
;mov al, 0x50 ; VGA 320*200 256 color

; TO START VESA ( mod 11b ) WITH CONTROLS
; mov ax, 0x4f02
; mov bx, 0x11b
; int 0x10
; mov ax, 0x4f01
; mov ax, 0x4f05
; mov bx, 0x0
; mov dx, 0x4

; TO START VESA ( mod 103 )
; mov ax, 0x4f02
; mov bx, 0x4103
;int 0x10

; Switch to protected mode
call switch_to_protected_mode

jmp $

%include "boot/gdt.asm"
%include "boot/start_protected_mode.asm"

times 510-($-$$) db 0
dw 0xaa55 ; magin numbers for boot segment