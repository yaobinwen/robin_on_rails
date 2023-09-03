; Implement the fizz-buzz game.
;
; In the range of [1, 100], if the number can be divided only by 3, print
; "fizz"; if it can be divided only by 5, print "buzz"; if it can be divided by
; both 3 and 5, print "fizzbuzz".

%include "func.asm"

section .data
  fizz: db "fizz", 0h
  buzz: db "buzz", 0h
  lf: db 0Ah, 0h
  div3: dd 3
  div5: dd 5
  end: dd 101

section .bss
  neither: resb 1
  rem3: resb 1
  rem5: resb 1

section .text
global _start

_start:

; for (int i = 1; i < end; i++) {
;   int rem3 = i % 3;
;   int rem5 = i % 5;
;   int neither = 1;
;   if (rem3 == 0) {printf("fizz"); neither = 0;}
;   if (rem5 == 0) {printf("buzz"); neither = 0;}
;   if (neither == 0) goto print_lf;
;   printf("%d", i);
;print_lf:
;   printf("\n");
; }

  mov ecx, 1

.loop_begin:
  cmp ecx, dword [end]
  jz .loop_end

  ; Calculate rem3
  mov eax, ecx
  mov edx, 0
  idiv dword [div3]
  ; The remainder is stored in EDX but in our problem, a byte is large enough
  ; to store all the possible results (i.e., 0, 1, 2, 3, 4).
  mov byte [rem3], dl

  ; Calculate rem5
  mov eax, ecx
  mov edx, 0
  idiv dword [div5]
  mov byte [rem5], dl

  ; Reset `neither`.
  mov byte [neither], 1

.rem3:
  cmp byte [rem3], 0
  jnz .rem5
  mov eax, fizz
  call sprint
  mov byte [neither], 0

.rem5:
  cmp byte [rem5], 0
  jnz .check_neither
  mov eax, buzz
  call sprint
  mov byte [neither], 0

.check_neither:
  cmp byte [neither], 0
  jz .print_lf
  mov eax, ecx
  call int_print

.print_lf:
  mov eax, lf
  call sprint

  inc ecx
  jmp .loop_begin

.loop_end:

  call sys_exit
