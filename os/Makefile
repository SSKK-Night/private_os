all: mykernel.elf

mykernel.elf: boot.o kernel.o
	ld -n -T linker.ld -o mykernel.elf boot.o kernel.o

boot.o: boot.s
	nasm -f elf64 boot.s -o boot.o

kernel.o: kernel.c
	gcc -ffreestanding -m64 -c kernel.c -o kernel.o

clean:
	rm -f *.o *.elf
