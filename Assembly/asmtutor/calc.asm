%include "func.asm"

section .data
  msg_remainder db " remainder "

section .text
global _start

_start:

  ; Addition
  mov eax, 90
  mov ebx, 9
  add eax, ebx
  call int_print_lf

  ; Subtraction
  mov eax, 90
  mov ebx, 9
  sub eax, ebx
  call int_print_lf

  ; Multiplication
  mov eax, 90
  mov ebx, 9
  mul ebx
  call int_print_lf

  ; Division
  mov eax, 93
  mov ebx, 9
  div ebx
  call int_print
  mov eax, msg_remainder
  call sprint
  mov eax, edx
  call int_print_lf

  mov eax, 0
  call sys_exit
