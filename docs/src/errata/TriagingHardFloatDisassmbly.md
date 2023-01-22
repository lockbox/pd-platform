# Triaging Hard Float Disassembly

Anytime anyone tells you "I had to look at [arbitrary compiler] source today",
that's a bad sign. It's like if someone told you they pooped today, what is
so wrong that there was a need to notify you that they pooped???

Well, I had to look at `gcc` source, and it's because disassembler's don't
support ARM FPv5 (note that gdb libopcode [seem's to](https://github.com/bminor/binutils-gdb/blob/master/opcodes/arm-dis.c#L1237)
albeit associates this FPU version with ARMv8 - but at least there's that).

# What is ARM FPv5 and why does this matter
For the STM32F746IE MCU (our target processor), the Playdate uses FPv5
single-precision floating point support, therefore there are fancy
instructions that aren't supported that ruin all our disassembly.
## Hard vs Soft Float
They do the same math, except Hard float is implemented in _Hard_ ware,
and Soft float is implemented in _Soft_ ware. TLDR; Hard float is orders
of magnitude faster, at the cost of higher power consumption and a more
expensive MCU etc.

## FPv5 Specification

[Architecture Reference Manual](https://developer.arm.com/documentation/ddi0403/latest)
    - chapter 6 is **The Floating-point Instruction Set Encoding**

[Floating Point Instructions](https://developer.arm.com/documentation/dui0646/c/The-Cortex-M7-Instruction-Set/Floating-point-instructions)

# WTF are they doing at ARM (and can I have some)

Primer: [ARM mode vs. Thumb Mode](https://stackoverflow.com/questions/28669905/what-is-the-difference-between-the-arm-thumb-and-thumb-2-instruction-encodings)

Primer: [What are FPU, VFP, ASE, NEON, MPE, SVE, SME, MVE, and VPU?](https://developer.arm.com/documentation/ka005091/1-0/?lang=en)

simd + neon + ase + mpe + vfp(v1 -> v5) + vfp11 + mve can u just not, plz and ty.

# Straight to the (gcc) source
**"why not just use the docs + libopcodes if they already support it"**

Because sister, I have seen the beyond and I'm sick of the docs being different
from real life. That and an incorrect lifter is worse than an absent lifter.

So, I'm going to use the thing that emits the actual code to add `FPv5` support.

For science.

And because I hate myself.

## Overview of gcc source tree



[Source tree](https://github.com/gcc-mirror/gcc)
- Top Level docs: [link](https://gcc.gnu.org/onlinedocs/gccint/Top-Level.html#Top-Level)
- What we care about: [link](https://gcc.gnu.org/onlinedocs/gccint/gcc-Directory.html#gcc-Directory)
    - specifically we want to know how a `gcc` backend is created (ie ARM)
    - note that a frontend is something like the C programming language, while the
    backend is a platform / mcu architecture

## Overview of a gcc backend
[GCC Backend docs](https://gcc.gnu.org/onlinedocs/gccint/Back-End.html#Back-End)

Of note:
>A directory machine under gcc/config, containing a machine description machine.md file (see [Machine Descriptions](https://gcc.gnu.org/onlinedocs/gccint/Machine-Desc.html#Machine-Desc)), header files machine.h and machine-protos.h and a source file machine.c (see [Target Description Macros and Functions](https://gcc.gnu.org/onlinedocs/gccint/Target-Macros.html#Target-Macros)), possibly a target Makefile fragment t-machine (see [The Target Makefile Fragment](https://gcc.gnu.org/onlinedocs/gccint/Target-Fragment.html#Target-Fragment)), and maybe some other files. The names of these files may be changed from the defaults given by explicit specifications in config.gcc.

We're mostly concerned with how instructions are emitted and what they look like, so we want to look at
the machine descriptions.

## Overview of ARM gcc backend


## How does GCC do ARM FPv5


Next: [FPv5 meet World](./FPv5MeetWorld.md)