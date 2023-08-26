; Hello World Program - asmtutor.com
; Compile with: nasm -f elf helloworld.asm
; Link with (64 bit systems require elf_i386 option): ld -m elf_i386 helloworld.o -o helloworld
; Run with: ./helloworld

%include "func.asm"

SECTION .data
; NOTE(ywen): This is really not "assigning a variable". `msg` is actually just
; the starting address of the string.
msg1    db      'Hello, brave brand new World!', 0Ah, 0h        ; assign msg variable with your message string
msg2    db      'This is how we recycle in NASM.', 0Ah, 0h      ; our second message string

SECTION .text
global  _start

_start:

    mov     eax, msg1   ; move the address of our message string into EAX
    call    sprint      ; call our function to calculate the length of the string

    mov     eax, msg2   ; move the address of our message string into EAX
    call    sprint      ; call our function to calculate the length of the string

    call    sys_exit
