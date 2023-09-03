%include "func.asm"

section .data
  environ: dd 0h
  command: db "/bin/echo", 0h
  arg1: db "Hello, world!", 0h
  argv: dd command
        dd arg1
        dd 0h

section .text
global _start

_start:
  mov edx, environ
  mov ecx, argv
  mov ebx, command
  mov eax, 11
  int 80h

  call sys_exit
