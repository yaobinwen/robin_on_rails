; Echo the command-line arguments.

%include "func.asm"

SECTION .text
global _start

_start:

  ; When the program starts, the top element in the stack is the number of
  ; arguments.
  pop ecx

next_arg:

  ; Compare the counter and zero.
  cmp ecx, 0h
  ; If the result is 0 (i.e., ECX == 0), jump out of the loop.
  jz no_more_args

  ; The CLI arguments are all strings so they certainly cannot be stored in the
  ; stack directly. The stack stores the addresses to these strings. Therefore,
  ; when we pop the current top element into EAX, we actually pop the address
  ; of the argument string into EAX, and then we can call `sprint_lf` directly
  ; because `sprint_lf` assumes EAX has the address of the string.
  pop eax
  call sprint_lf

  dec ecx
  jmp next_arg

no_more_args:

  call sys_exit
