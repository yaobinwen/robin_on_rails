# Guide to Assembly Programming

## 1. What is this?

This is a guide to learning Assembly programming, but itself is not an actual tutorial.

I'm not making this into a tutorial because there are already many good ones, not to mention the official documents. But I've noticed that many tutorials are built on top of some background knowledge that are often not explained in the tutorials. However, the background knowledge can be important clues when the readers run into some difficulty or questions. The readers who read the tutorials are usually those who don't know very much about the subject of the tutorials (and that's why they want to read the tutorials in the first place). Without knowing enough background, the readers may be left clueless and don't know where to find the answers. I believe search engines (e.g., Google) and AI products (e.g., ChatGPT) can help a lot, but I also believe that, should the readers have learned enough background knowledge, it would be more efficient for them to find the answers.

Therefore, this document, instead of teaching specific knowledge of assembly programming, tries to show the readers the "big picture" of what to know in order to learn assembly programming. Once the readers know the big picture, it would be easier to know which part could go wrong and what to look for when they run into problems.

## 2. Who is this for?

This document is for the beginners of assembly programming.

## 3. License

This work is under the license [Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)](https://creativecommons.org/licenses/by-nc-sa/4.0/)

## 4. The big picture

To learn assembly programming, the learners actually need to be aware of the following items:
- The CPUs, which are the ultimate hardware that runs the instructions.
- The hardware accessories, such as the memory, BIOS, and the display.
- The programming context, such as "are you writing assembly code on an operating system?" or "are you writing assembly code on a bare-metal machine?"
- The assembler, which translates the assembly code into machine code.

I'll explain them with more details.

### 4.1 CPUs

The CPUs are the ultimate hardware that executes the programs that the you write. Therefore, you need to know what CPU your program targets and what instructions are available on this CPU.

To find such information, you will need to find the software development manual for your CPU model. Generally speaking, this consists of the following steps:
- 1). Find the CPU model information.
- 2). Find the software development manual on the vendor's website.
- 3). Verify the manual covers the CPU model that you are using.

There are many ways to find the CPU model of your computer. For example, on my Ubuntu, I can run `lscpu` to find it (and I believe other operating systems must have some equivalent graphical or non-graphical counterpart):

```
$ lscpu
Architecture:        x86_64
CPU op-mode(s):      32-bit, 64-bit
Byte Order:          Little Endian
CPU(s):              8
On-line CPU(s) list: 0-7
Thread(s) per core:  2
Core(s) per socket:  4
Socket(s):           1
NUMA node(s):        1
Vendor ID:           GenuineIntel
CPU family:          6
Model:               94
Model name:          Intel(R) Core(TM) i7-6920HQ CPU @ 2.90GHz
Stepping:            3
CPU MHz:             900.008
CPU max MHz:         3800.0000
CPU min MHz:         800.0000
BogoMIPS:            5799.77
Virtualization:      VT-x
L1d cache:           32K
L1i cache:           32K
L2 cache:            256K
L3 cache:            8192K
NUMA node0 CPU(s):   0-7
Flags:               fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp lm constant_tsc art arch_perfmon pebs bts rep_good nopl xtopology nonstop_tsc cpuid aperfmperf pni pclmulqdq dtes64 monitor ds_cpl vmx smx est tm2 ssse3 sdbg fma cx16 xtpr pdcm pcid sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand lahf_lm abm 3dnowprefetch cpuid_fault epb invpcid_single pti ssbd ibrs ibpb stibp tpr_shadow vnmi flexpriority ept vpid ept_ad fsgsbase tsc_adjust bmi1 hle avx2 smep bmi2 erms invpcid rtm mpx rdseed adx smap clflushopt intel_pt xsaveopt xsavec xgetbv1 xsaves dtherm ida arat pln pts hwp hwp_notify hwp_act_window hwp_epp md_clear flush_l1d arch_capabilities
```

The output tells at least two things:
- The model name is "Intel® Core™ i7-6920HQ".
- The vendor is Intel.

Then searching "intel cpu architecture software development manual" returned me the following link: [Intel® 64 and IA-32 Architectures Software Developer Manuals](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html). This is a huge document of 10 volumes (as of 2023-09-04). The section _1.1 INTEL® 64 AND IA-32 PROCESSORS COVERED IN THIS MANUAL_ in the first volume _Basic Architecture_ lists all the CPU models that are covered by this document, including "Intel® Core™ i7 processor".

Now I know that, if I have any questions regarding my CPU, I can use this document as the official reference.

AMD calls this document the "programmer's manual" and you can find them on their [documentation hub](https://www.amd.com/en/search/documentation/hub.html). For example, the [_AMD64 Architecture Programmer's Manual_](https://www.amd.com/content/dam/amd/en/documents/processor-tech-docs/programmer-references/40332.pdf).

Most of the time, you probably don't need to touch these documents at all, because the assembly programming tutorial that you are reading probably also teaches the syntax and semantics of the CPU instructions. However, if the tutorial doesn't provide enough details or you suspect the tutorial has a mistake, the CPU's software development manual is the ultimate source of truth to answer your questions.

#### 4.1.1 Assembler documentation vs assembly programming tutorial

(TODO)

#### 4.1.2 What about GPUs?

https://computergraphics.stackexchange.com/questions/7809/what-does-gpu-assembly-look-like

### 4.2 Hardware accessories, IVT, and system calls

My personal experience shows that they usually go hand in hand so I'll explain them together.

Although CPUs are the ultimate hardware that executes the instructions, a CPU itself does not make a full computer. Nowadays, most of the modern computers still follow the [von Neumann architecture](https://en.wikipedia.org/wiki/Von_Neumann_architecture) which also includes input device (e.g., keyboard, mouse), memory unit (e.g., random-access memory (RAM)), and output device (e.g., video display). Typically, the input device can be a keyboard or a mouse; the memory unit is the random-access memory (RAM); the output device is the display.

But there are more.

Reusing code is important in programming. Functions/procedures/subroutines are the code that your program can call to perform some function.

### 4.3 Assemblers

Not all the assemblers are made the same.
