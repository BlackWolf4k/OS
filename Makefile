CC = x86_64-elf-gcc # gcc cross compiler for x86
CFLAGS = -Wall -ffreestanding -m32 -g -c # all the flags for compiling
LD = x86_64-elf-ld
LDFLAGS = -m elf_i386

C_SOURCES = $(wildcard 	kernel/*.c 					\
						kernel/interrupts/*.c		\
					   	libc/*.c 					\
					   	init/*.c					\
						file_system/*.c				\
						)

C_HEADERS = $(wildcard 	include/*.h 					\
						include/kernel/*.h				\
						include/kernel/interrupts/*.h 	\
						include/kernel/file_system/*.h 	\
					   	)

OBJ = 	${C_SOURCES:.c=.o 			\
			  kernel/interrupts/interrupts.o}

OS.bin: boot/bootloader.bin full_kernel.bin
	cat $^ > OS.bin

full_kernel.bin: boot/kernel_entry.o ${OBJ}
	${LD} ${LDFLAGS} -T linker.ld $^ --oformat binary -o $@

kernel.elf: boot/kernel_entry.o ${OBJ}
	${LD} ${LDFLAGS} -o $@ -Ttext 0x1000 $^ 

%.o: %.c ${C_HEADERS}
	${CC} ${CFLAGS} $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

# qemu-system-x86_64 -drive format=raw,file="iso/OS.bin",index=0,if=floppy
# qemu-system-i386 -fda OS.bin

first_run: OS.bin
	make disk ARGS="10G"
	mv hard_disk.img ../
	make run

# QEMU INFORMATIONS
# -m 2048 : memory size, -enable-kvm : enable virtualization, -smp 2 : 2 cores,
# -net -nic -net user : ( removable ) ethernet enable, -hda ../hard_drive.img : hard drive, -fda OS.bin : bootable disk ( floppy )
# qemu-system-x86_64 -m 2048 -enable-kvm -smp 2 -net nic -net user -hda ../hard_drive.img -fda OS.bin
# qemu-system-x86_64 -m 2048 -enable-kvm -smp 2 -net nic -net user -drive file=../hard_drive.img,format=raw -fda OS.bin
#	qemu-system-x86_64 -m 2048 -enable-kvm -smp 2 -net nic -net user -drive format=raw,file=../hard_drive.img -drive format=raw,file=OS.bin,index=0,if=floppy
# qemu-system-x86_64 -m 2048 -enable-kvm -smp 2 -net nic -net user -drive format=raw,file=./OS.bin,index=0 -drive format=raw,file=../hard_drive.img,index=1
run: OS.bin
	qemu-system-x86_64 -m 2048 -enable-kvm -smp 2 -net nic -net user -drive format=raw,file=./OS.bin,index=0,if=floppy -drive format=raw,file=../hard_drive.img -boot order=c -serial stdio
	make clean

#gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"
debug: OS.bin kernel.elf
	echo 'gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"'
	qemu-system-x86_64 -s -S -fda OS.bin
	make clean

# make disk ARGS="[size]"
disk:
	qemu-img create hard_disk.img $( ARGS )

compile: OS.bin
	make clean

clear_disk:
	shred -n 0 -z -v ../hard_drive.img

clean:
	$(RM) boot/*.o boot/*.bin
	$(RM) bootloader/*.o bootloader/*.bin
	$(RM) init/*.o
	$(RM) file_system/*.o
	$(RM) kernel/*.o
	$(RM) kernel/interrupts/*.o
	$(RM) libc/*.o
	$(RM) full_kernel.bin
	$(RM) OS.bin
	$(RM) kernel.elf