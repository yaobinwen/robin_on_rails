; -----------------------------------------------------------------------------
; Calculate the length of the given string.
;
; Args:
; - eax: The address of the string.
;
; Returns:
; - eax: The length of the string.
slen:
    ; push the value in EBX onto the stack to preserve it while we use EBX in
    ; this function
    push    ebx

    mov     ebx, eax        ; move the address of our message string into EBX
 
.nextchar:
    ; NOTE(ywen):
    ; - `[]` dereferences the address to fetch the content.
    ; - `byte` specifies the width of the content to fetch (i.e., in the size of a byte).
    cmp     byte [eax], 0   ; compare the byte pointed to by EAX at this address against zero (Zero is an end of string delimiter)
    jz      .finished       ; jump (if the zero flagged has been set) to the point in the code labeled 'finished'
    inc     eax             ; increment the address in EAX by one byte (if the zero flagged has NOT been set)
    jmp     .nextchar       ; jump to the point in the code labeled 'nextchar'
 
.finished:
    sub     eax, ebx        ; subtract the address in EBX from the address in EAX
                            ; remember both registers started pointing to the same address (see line 15)
                            ; but EAX has been incremented one byte for each character in the message string
                            ; when you subtract one memory address from another of the same type
                            ; the result is number of segments between them - in this case the number of bytes

    ; Resume the original value of EBX.
    pop     ebx

    ; Return to where the function was called.
    ret


; -----------------------------------------------------------------------------
; Print the given string to stdout.
;
; Args:
; - eax: The address of the string.
;
; Returns: N/A
sprint:

    ; Save the current EBX, ECX, and EDX because we need to modify them in this
    ; function.
    push    edx
    push    ecx
    push    ebx

    ; At this moment, EAX is the address of the string. We need to save it to
    ; the stack for future use, because we are about to call `slen` to
    ; calculate the string length, and after `slen` returns, EAX will be
    ; updated to store the string length. If we don't save EAX now, we will
    ; lose the address of the string.
    push    eax

    ; Call `slen` to calculate string length.
    call    slen

    ; When `slen` returns, EAX has the string length.
    ; Move EAX to EDX because `sys_write` call needs EDX to store the length.
    mov     edx, eax

    ; Restore the earlier value of EAX which was the address of the string.
    pop     eax
    ; Move EAX to ECX because `sys_write` call needs ECX to be the address of
    ; the string.
    mov     ecx, eax

    call    sys_print_stdout

    ; Restore the earlier values of EBX, ECX, and EDX.
    pop     ebx
    pop     ecx
    pop     edx

    ret


; -----------------------------------------------------------------------------
; Print the given string to stdout, with a new-line at the end.
;
; Args:
; - eax: The address of the string.
;
; Returns: N/A
sprint_lf:
    ; `sprint` will handle the saving and restoring of the registers so we
    ; don't need to worry about that.

    ; `sprint` uses the same input registers so we can call it directly to
    ; print the original string.
    call  sprint

    ; We are going to use EAX so we need to save its current value.
    push  eax

    mov   eax, 0Ah
    ; Now we push 0Ah to the top of the stack which is pointed to by ESP.
    push  eax
    mov   eax, esp
    call  sprint
    pop   eax

    ; Pop the original value of EAX.
    pop   eax

    ret


; -----------------------------------------------------------------------------
; Print an integer.
;
; Args:
; - eax: The number to print.
;
; Returns: N/A
int_print:
  ; Save the current values of the registers that we need to use in this
  ; function.
  push eax
  push ecx
  push edx
  push esi

  ; ECX counts how many digits there are in the number.
  mov ecx, 0

  mov esi, 10

.divideLoop:
  ; We need to set EDX as 0 because `idiv` treats EDX:EAX as the dividend.
  mov edx, 0

  ; Divide EDX:EAX by ESI. The quotient is in EAX and the remainder is in EDX.
  idiv esi

  ; Convert the number in EDX into its corresponding ASCII representation.
  add edx, 48
  push edx

  ; We did one loop of calculation, so increase the counter.
  inc ecx

  cmp eax, 0
  jnz .divideLoop

.printLoop:
  mov eax, esp
  call sprint
  pop eax

  dec ecx
  cmp ecx, 0
  jnz .printLoop

  pop esi
  pop edx
  pop ecx
  pop eax

  ret


; -----------------------------------------------------------------------------
; Print an integer with a line feed at the end.
;
; Args:
; - eax: The number to print.
;
; Returns: N/A
int_print_lf:

    ; `int_print` uses the same input registers so we can call it directly to
    ; print the original string.
    call  int_print

    ; We are going to use EAX so we need to save its current value.
    push  eax

    mov   eax, 0Ah
    ; Now we push 0Ah to the top of the stack which is pointed to by ESP.
    push  eax
    mov   eax, esp
    call  sprint
    pop   eax

    ; Pop the original value of EAX.
    pop   eax

    ret

; -----------------------------------------------------------------------------
; Print the string to stdout
;
; Args:
; - ecx: address of the string
; - edx: length of the string
;
; Returns: N/A
sys_print_stdout:
    push    ebx
    push    eax

    mov     ebx, 1
    mov     eax, 4
    int     80h

    pop     eax
    pop     ebx

    ret

; -----------------------------------------------------------------------------
; Exit program and restore resources
sys_exit:
    ; This function causes the program to exit, so we don't really have to save
    ; the registers because we are quitting anyway, but it's a good habbit to
    ; remember to save and restore the registers in a function to minimize the
    ; side effects.
    push    ebx
    push    eax

    ; return 0 status on exit - 'No Errors'
    mov     ebx, 0
    ; invoke SYS_EXIT (kernel opcode 1)
    mov     eax, 1
    int     80h

    pop     eax
    pop     ebx

    ret
