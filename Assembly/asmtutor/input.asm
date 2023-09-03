%include "func.asm"

section .data
  prompt db "Please enter your name: ", 0h
  msg_part1 db "Hello, ", 0h

section .bss
  ; Reserve 255 bytes in memory for the users input string.
  sinput resb 255

section .text
global _start

_start:

  ; Display the prompt.
  mov eax, prompt
  call sprint

  mov edx, 255        ; number of bytes to read
  mov ecx, sinput     ; reserved space to store our input (known as a buffer)
  mov ebx, 0          ; read from the STDIN file
  mov eax, 3          ; invoke sys_read (kernel opcode 3)
  int 80h

  mov eax, msg_part1
  call sprint

  mov eax, sinput
  call sprint

  call sys_exit
