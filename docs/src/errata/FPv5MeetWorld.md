# FPv5 meet world
# Opcode patterns from the docs

Going through the [ARMv7M spec](https://developer.arm.com/documentation/ddi0403/latest)
there are a few things we need to keep in mind:

### Instruction alignment / byte ordering:

![insn_alignment.png](./insn_alignment.png)

The important bits are the **"byte at address `A + n`"**

### When reading the instruction decodings from the manual:

![fpu_register_formats.png](./fpu_register_formats.png)

### Rude "syntax flexibilities" to make things oh so much clearer:

![syntax_flexiblities](./syntax_flexibilities.png)

### FPU register encoding

![fpu_register_encoding](./fpu_register_encoding.png)

# Opcode patterns from gcc

# FPv5 meet Binary Ninja
# FPv5 meet Ghidra
# FPv5 meet objdump