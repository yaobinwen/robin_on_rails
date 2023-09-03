%include "func.asm"

section .data
  parent_msg: db "This is the parent process.", 0h
  child_msg: db "This is the child process.", 0h

section .text
global _start

_start:
  ; Call SYS_FORK.
  mov eax, 2
  int 80h

  cmp eax, 0
  jz child

parent:
  mov eax, parent_msg
  call sprint_lf

  jmp finished

child:
  mov eax, child_msg
  call sprint_lf

  jmp finished

finished:

  call sys_exit
