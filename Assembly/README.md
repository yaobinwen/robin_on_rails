# Assembly Programming (mainly NASM)

## 1. Overview

This folder has my notes about programming in assembly language.

## 2. The Big Picture

You need to understand what "assembly programming" invovles in order to have a clear mind of what you are doing and where you can look into when they run into problems.

Assembly programming includes the following components:
- The CPU model that the assembly code will run on. The CPU model is the ultimate source of truth of how the instructions work.
- The hardware accessories, such as the memroy, the BIOS, the display. To write a program that does something useful, you will need to deal with input and output.
  - An important part of this is the interrupt vector table (IVT) which provides callable functions that you can use. This also emphasizes the importance of understanding your programming environment: If you are writing a boot loader for an operating system, you probably only have the IVT in BIOS; if you are writing an assembly program under an operating system (e.g., Linux), the OS may provide system calls that you can use and maybe BIOS's IVT is not available. You just need to know what you have.
  - Knowing the CPU model is important because the IVT of an OS may be different for different models. For example, as shown by [Linux System Call Table](https://chromium.googlesource.com/chromiumos/docs/+/master/constants/syscalls.md), the system call `write` has different call numbers:
    - `x86_64` (64-bit): 1
    - `x86` (32-bit): 4
    - `arm64` (64-bit): 64
    - `arm` (32-bit): 4
- The assembler which you use to write assembly code. Assemblers translate the assembly code into the machine code that CPUs can run. Different assemblers may slightly differ in the syntax. For example, one assembler may require `%` to refer to a register while another may not require `%`.

I realized many tutorials fail to mention all of the components above clearly. This can cause a lot of confusion because the tutorial readers may run into problems that are caused by the programming environment.

## 3. Assemblers & Syntax

By some quick search you will see there are [many assemblers](https://en.wikipedia.org/wiki/Comparison_of_assemblers), such as:
- [Netwide Assembler (NASM)](https://www.nasm.us/)
- [GNU Assembler (GAS)](https://tldp.org/HOWTO/Assembly-HOWTO/gas.html)
- [Microsoft Macro Assembler (MASM)](https://learn.microsoft.com/en-us/cpp/assembler/masm/microsoft-macro-assembler-reference?view=msvc-170)
- [Flat Assembler (FASM)](https://flatassembler.net/)
- [Yet Another Assembler (YASM)](https://yasm.tortall.net/)

And it looks like different assemblers may use different syntax. For example, NASM and GAS use different syntax; MASM and FASM have their own syntax, too.

I will use NASM so I will learn NASM's syntax. I will use [NASM's documentation](https://www.nasm.us/xdoc/2.16.01/html/nasmdoc0.html) as my primary learning resource. The choice of NASM is mostly a coincidence: I realized the assembly code I've seen so far is mostly written in NASM syntax so it is the most familiar one to me. But whenever possible, I will also learn GAS syntax as much as needed.

## 3. Guide

See [Guide.md](./Guide.md).
