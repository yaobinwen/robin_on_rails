# Assembly Programming (mainly NASM)

## 1. Overview

This folder has my notes about programming in assembly language.

## 2. Assemblers & Syntax

By some quick search you will see there are [many assemblers](https://en.wikipedia.org/wiki/Comparison_of_assemblers), such as:
- [Netwide Assembler (NASM)](https://www.nasm.us/)
- [GNU Assembler (GAS)](https://tldp.org/HOWTO/Assembly-HOWTO/gas.html)
- [Microsoft Macro Assembler (MASM)](https://learn.microsoft.com/en-us/cpp/assembler/masm/microsoft-macro-assembler-reference?view=msvc-170)
- [Flat Assembler (FASM)](https://flatassembler.net/)
- [Yet Another Assembler (YASM)](https://yasm.tortall.net/)

And it looks like different assemblers may use different syntax. For example, NASM and GAS use different syntax; MASM and FASM have their own syntax, too.

I will use NASM so I will learn NASM's syntax. I will use [NASM's documentation](https://www.nasm.us/xdoc/2.16.01/html/nasmdoc0.html) as my primary learning resource. The choice of NASM is mostly a coincidence: I realized the assembly code I've seen so far is mostly written in NASM syntax so it is the most familiar one to me. But whenever possible, I will also learn GAS syntax as much as needed.

## 3. The NASM Language

Main references:
- [NASM documentation](https://www.nasm.us/xdoc/2.16.01/html/nasmdoc3.html)
- [GitHub: netwide-assembler/nasm](https://github.com/netwide-assembler/nasm)

### 3.a Critical Expression

(TODO)

### 3.b Macros

(TODO)

### 3.1 Layout of a NASM Source Line

Most of the time, a NASM source line is in the following general form:

```
label: instruction operands   ; comment
```

#### 3.1.1 Labels (Identifiers)

**A label does not have to end with `:`**. As a result, it's possible that you want to code `lodsb` but mis-type it as `lodab` and NASM doesn't complain anything because `lodab` is silently treated as a label. NASM's command line option `-w+orphan-labels` can warn you if you define a label alone on a line without a trailing colon.

Valid _first_ character:
- letters
- `.` (with special meaning)
- `_`
- `?`

Valid subsequent characters:
- letters
- numbers
- `_`, `$`, `#`, `@`, `~`, `.`, and `?`.

`$` can prefix an identifier to indicate the identifier should not be interpreted as a reserved word. For example, if a module defines a symbol called `eax`, you can refer to `$eax` in NASM code to distinguish the symbol from the register.

Maximum length: 4095.

#### 3.1.2 Instructions

The instruction may be prefixed by `LOCK`, `REP`, `REPE/REPZ`, `REPNE/REPNZ`, `XACQUIRE/XRELEASE` or `BND/NOBND`, in the usual way.

Explicit address-size and operand-size prefixes A16, A32, A64, O16 and O32, O64 are provided.

You can also use the name of a segment register as an instruction prefix: coding es mov [bx],ax is equivalent to coding mov [es:bx],ax. We recommend the latter syntax, since it is consistent with other syntactic features of the language, but for instructions such as LODSB, which has no operands and yet can require a segment override, there is no clean syntactic way to proceed apart from es lodsb.

An instruction is not required to use a prefix: prefixes such as CS, A32, LOCK or REPE can appear on a line by themselves, and NASM will just generate the prefix bytes.

#### 3.1.3 Operands

An operand can be:
- A register, such as `ax`, `bp`, `ebx`, `cr0`.
- NASM does not use the GAS-style in which register names must be prefixed by a `%` sign.
- An effective address.
- A constant.
- An expression.

### 3.2 Pseudo-Instructions

#### 3.2.1 `Dx` for initialized data

The notation `Dx` represents any of the following pseudo-instructions that declare **initialized data** in the **output file**:
- DB
- DW
- DD
- DQ
- DT (does not accept integer numeric constants as operands)
- DO (does not accept integer numeric constants as operands)
- DY (does not accept integer numeric constants as operands)
- DZ (does not accept integer numeric constants as operands)

Examples (assuming on a 16-bit machine so a `word` consists of 2 bytes):

```x86asm
      db    0x55                ; just the byte 0x55
      db    0x55,0x56,0x57      ; three bytes in succession
      db    'a',0x55            ; character constants are OK
      db    'hello',13,10,'$'   ; so are string constants
      dw    0x1234              ; 0x34 0x12
      dw    'a'                 ; 0x61 0x00 (it's just a number)
      dw    'ab'                ; 0x61 0x62 (character constant)
      dw    'abc'               ; 0x61 0x62 0x63 0x00 (string)
      dd    0x12345678          ; 0x78 0x56 0x34 0x12
      dd    1.234567e20         ; floating-point constant
      dq    0x123456789abcdef0  ; eight byte constant
      dq    1.234567e20         ; double-precision float
      dt    1.234567e20         ; extended-precision float
```

#### 3.2.1 `RESx` for uninitialized data

The notation `RESx` represents any of the following pseudo-instructions used in the `BSS` section of a module to declare **uninitialized** storage space:
- RESB
- RESW
- RESD
- RESQ
- REST
- RESO
- RESY
- RESZ

Examples:

```x86asm
buffer:         resb    64              ; reserve 64 bytes
wordvar:        resw    1               ; reserve a word
realarray:      resq    10              ; array of ten reals
ymmval:         resy    1               ; one YMM register
zmmvals:        resz    32              ; 32 ZMM registers
```

The operand to a RESB–type pseudo-instruction would be a **critical expression**

#### 3.2.2 `INCBIN`: Including External Binary Files

`INCBIN` includes binary file data verbatim into the output file. This can be handy for (for example) including graphics and sound data directly into a game executable file. It can be called in one of these three ways:

```x86asm
    incbin  "file.dat"             ; include the whole file
    incbin  "file.dat",1024        ; skip the first 1024 bytes
    incbin  "file.dat",1024,512    ; skip the first 1024, and
                                   ; actually include at most 512
```

#### 3.2.3 `EQU`: Defining Constants

Syntax: `label EQU value`

For example:

```x86asm
message         db      'hello, world'
msglen          equ     $-message
```

The `label` is evaluated only **once** at the time of the definition, hence "defining constants".

#### 3.2.4 `TIMES`: Repeating Instructions or Data

`TIMES` can be used with both constants and variables, for example:

```x86asm
zerobuf:        times 64 db 0   ; followed by a constant (64)
buffer: db      'hello, world'
times 64-$+buffer db ' '        ; followed by a variable (64 - length of buffer)
                                ; This will store exactly enough spaces to make
                                ; the total length of buffer up to 64.
times 100 movsb                 ; repeats an instruction 100 times
```

Notes:
- `TIMES` is a critical expression.
- `TIMES` can't be applied to macros (because `TIMES` is processed after the macro phase).

### 3.3 Effective Addresses: [address]

An expression evaluating to the desired address, enclosed in square brackets. For example:

```x86asm
wordvar dw      123
        mov     ax,[wordvar]
        mov     ax,[wordvar+1]
        mov     ax,[es:wordvar+bx]
        mov     eax,[ebx*2+ecx+offset]
        mov     ax,[bp+di+8]
        mov     eax,[ebx*5]             ; assembles as [ebx*4+ebx]
        mov     eax,[label1*2-label2]   ; ie [label1+(label1-label2)]
```

Counter-examples: `es:wordvar[bx]`

Some keywords that affect addressing behaviors:
- BYTE, WORD, DWORD, NONSPLIT
- `ABS`, `REL`

A new form of split effective address syntax is also supported. This is mainly intended for mib operands as used by MPX instructions, but can be used for any memory reference. The basic concept of this form is splitting base and index.

```x86asm
     mov eax,[ebx+8,ecx*4]   ; ebx=base, ecx=index, 4=scale, 8=disp
```

For mib operands, there are several ways of writing effective address depending on the tools. NASM supports all currently possible ways of mib syntax:

```x86asm
     ; bndstx
     ; next 5 lines are parsed same
     ; base=rax, index=rbx, scale=1, displacement=3
     bndstx [rax+0x3,rbx], bnd0      ; NASM - split EA
     bndstx [rbx*1+rax+0x3], bnd0    ; GAS - '*1' indecates an index reg
     bndstx [rax+rbx+3], bnd0        ; GAS - without hints
     bndstx [rax+0x3], bnd0, rbx     ; ICC-1
     bndstx [rax+0x3], rbx, bnd0     ; ICC-2
```

When broadcasting decorator is used, the opsize keyword should match the size of each element.

```x86asm
     VDIVPS zmm4, zmm5, dword [rbx]{1to16}   ; single-precision float
     VDIVPS zmm4, zmm5, zword [rbx]          ; packed 512 bit memory
```

### 3.4 Constants

#### 3.4.1 Numeric Constants

Summary:

| Base    | Prefix    | Suffix |
|:-------:|:---------:|:------:|
| Bin(2)  | 0b, 0y    | B,Y    |
| Oct(8)  | 0o, 0q    | Q,O    |
| Dec(10) | 0d, 0t    | D,T    |
| Hex(16) | 0h, 0x, $ | H,X    |

Notes:
- A `0` prefix by itself does not imply an octal constant, e.g., `0200` is 200 in decimal.
- When using `$` to prefix a hexadecimal, `$` must be followed by a digit rather than a letter due to the dual-responsibility of `$`.

Examples:

```x86asm
        mov     ax,200          ; decimal
        mov     ax,0200         ; still decimal
        mov     ax,0200d        ; explicitly decimal
        mov     ax,0d200        ; also decimal
        mov     ax,0c8h         ; hex
        mov     ax,$0c8         ; hex again: the 0 is required
        mov     ax,0xc8         ; hex yet again
        mov     ax,0hc8         ; still hex
        mov     ax,310q         ; octal
        mov     ax,310o         ; octal again
        mov     ax,0o310        ; octal yet again
        mov     ax,0q310        ; octal yet again
        mov     ax,11001000b    ; binary
        mov     ax,1100_1000b   ; same binary constant
        mov     ax,1100_1000y   ; same binary constant once more
        mov     ax,0b1100_1000  ; same binary constant yet again
        mov     ax,0y1100_1000  ; same binary constant yet again

```

#### 3.4.2 Character Strings

Notes:
- Up to **eight** characters.
- Enclosed one of the following quotation marks:
  - single quotes ('...'): equivalent to `("...")`
  - double quotes ("..."): equivalent to `('...')`
  - backquotes (`...`): supports C-style `\`-escapes for special characters.
- Available escape sequences:
  - `\'`: single quote (')
  - `\"`: double quote (")
  - `\`: backquote (`)
  - `\\`: backslash (\)
  - `\?`: question mark (?)
  - `\a`: BEL (ASCII 7)
  - `\b`: BS  (ASCII 8)
  - `\t`: TAB (ASCII 9)
  - `\n`: LF  (ASCII 10)
  - `\v`: VT  (ASCII 11)
  - `\f`: FF  (ASCII 12)
  - `\r`: CR  (ASCII 13)
  - `\e`: ESC (ASCII 27)
  - `\377`: Up to 3 octal digits - literal byte
  - `\xFF`: Up to 2 hexadecimal digits - literal byte
  - `\u1234`: 4 hexadecimal digits - Unicode character, converted to UTF-8
  - `\U12345678`: 8 hexadecimal digits - Unicode character, converted to UTF-8
  - `\0`: meaning a NUL character (ASCII 0)

#### 3.4.3 Character Constants

A character constant consists of a string up to eight bytes long, used in an expression context. It is treated as if it was an integer.

A character constant with more than one byte will be arranged with **little-endian** order in mind: if you code

```x86asm
          mov eax,'abcd'
```

then the constant generated is **not** `0x61626364`, but `0x64636261`, so that if you were then to store the value into memory, it would read `abcd` rather than `dcba`. This is also the sense of character constants understood by the Pentium's `CPUID` instruction.

#### 3.4.4 String Constants

A string constant looks like a character constant, only longer. It is treated as a concatenation of maximum-size character constants for the conditions. So the following are equivalent:

```x86asm
      db    'hello'               ; string constant
      db    'h','e','l','l','o'   ; equivalent character constants
```

#### 3.4.5 Unicode Strings

The special operators __?utf16?__, __?utf16le?__, __?utf16be?__, __?utf32?__, __?utf32le?__ and __?utf32be?__ allows definition of Unicode strings. They take a string in UTF-8 format and converts it to UTF-16 or UTF-32, respectively. Unless the be forms are specified, the output is littleendian.

For example:

```x86asm
%define u(x) __?utf16?__(x)
%define w(x) __?utf32?__(x)

      dw u('C:\WINDOWS'), 0       ; Pathname in UTF-16
      dd w(`A + B = \u206a`), 0   ; String in UTF-32
```

#### 3.4.6 Floating-Point Constants

(TODO)

#### 3.4.7 Packed BCD Constants

x87-style packed BCD constants can be used in the same contexts as 80-bit floating-point numbers. They are suffixed with p or prefixed with 0p, and can include up to 18 decimal digits.

As with other numeric constants, underscores can be used to separate digits.

For example:

```x86asm
      dt 12_345_678_901_245_678p
      dt -12_345_678_901_245_678p
      dt +0p33
      dt 33p
```

### 3.5 Expressions

(TODO)

### 3.6 SEG and WRT

(TODO)

### 3.7 STRICT: Inhibiting Optimization

(TODO)

### 3.8 Critical Expressions

(TODO: Add to section 3.a)

### 3.9 Local Labels

A label beginning with a single period is treated as a **local label**, which means that it is **associated with the previous non-local label**.

For example:

```x86asm
label1  ; some code

.loop
        ; some more code

        jne     .loop
        ret

label2  ; some code

.loop
        ; some more code

        jne     .loop
        ret

label3  ; some more code
        ; and some more

        jmp label1.loop
```

Each `jne` jumps to the `.loop` line immediately before it. You can just to the **full label**, as `jmp label1.loop` does.
