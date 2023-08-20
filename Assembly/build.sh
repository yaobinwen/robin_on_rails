rm -f ./hello-world

nasm -f elf32 hello-world.asm

ld -m elf_i386 -s -o hello-world hello-world.o
