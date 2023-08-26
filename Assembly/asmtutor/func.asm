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
; Print the string to stdout
;
; Args:
; - ecx: address of the string
; - edx: length of the string
;
; Returns: N/A
sys_print_stdout:
    mov     ebx, 1
    mov     eax, 4
    int     80h

    ret

; -----------------------------------------------------------------------------
; Exit program and restore resources
sys_exit:
    ; return 0 status on exit - 'No Errors'
    mov     ebx, 0
    ; invoke SYS_EXIT (kernel opcode 1)
    mov     eax, 1
    int     80h

    ret
