; Hello World Program - asmtutor.com
; Compile with: nasm -f elf helloworld.asm
; Link with (64 bit systems require elf_i386 option): ld -m elf_i386 helloworld.o -o helloworld
; Run with: ./helloworld

SECTION .data
; NOTE(ywen): This is really not "assigning a variable". `msg` is actually just
; the starting address of the string.
msg     db      'Hello, brave brand new World!', 0Ah     ; assign msg variable with your message string

SECTION .text
global  _start

_start:

    mov     eax, msg    ; move the address of our message string into EAX
    call    strlen      ; call our function to calculate the length of the string

    mov     edx, eax    ; EAX now equals the number of bytes in our string
    mov     ecx, msg    ; move the memory address of our message string into ecx
    mov     ebx, 1      ; write to the STDOUT file
    mov     eax, 4      ; invoke SYS_WRITE (kernel opcode 4)
    int     80h

    mov     ebx, 0      ; return 0 status on exit - 'No Errors'
    mov     eax, 1      ; invoke SYS_EXIT (kernel opcode 1)
    int     80h

; Calculate the length of the given string.
;
; Args:
; - eax: The address of the string.
;
; Returns:
; - eax: The length of the string.
strlen:
    ; push the value in EBX onto the stack to preserve it while we use EBX in
    ; this function
    push    ebx

    mov     ebx, eax        ; move the address of our message string into EBX
 
nextchar:
    ; NOTE(ywen):
    ; - `[]` dereferences the address to fetch the content.
    ; - `byte` specifies the width of the content to fetch (i.e., in the size of a byte).
    cmp     byte [eax], 0   ; compare the byte pointed to by EAX at this address against zero (Zero is an end of string delimiter)
    jz      finished        ; jump (if the zero flagged has been set) to the point in the code labeled 'finished'
    inc     eax             ; increment the address in EAX by one byte (if the zero flagged has NOT been set)
    jmp     nextchar        ; jump to the point in the code labeled 'nextchar'
 
finished:
    sub     eax, ebx        ; subtract the address in EBX from the address in EAX
                            ; remember both registers started pointing to the same address (see line 15)
                            ; but EAX has been incremented one byte for each character in the message string
                            ; when you subtract one memory address from another of the same type
                            ; the result is number of segments between them - in this case the number of bytes

    ; Resume the original value of EBX.
    pop     ebx

    ; Return to where the function was called.
    ret
