# Comet VM

Comet VM is an interpreted virtual machine for the Comet Instruction
Set Architecture (ISA).

Comet ISA is based on LEB integer encoding for all fields of an
instruction. This means the encoding supports essentially infinite
numbers of opcodes and registers, and full 64bit immediate constants
if necessary.

Comet is a RISC-like (little endian) register machine but because it
supports essetially unlimited numbers of registers, it can also serve
as a compiler intermediate language - one that can be executed with
this VM.

## Pros and Cons

First the pros:

1) suitable as an intermediate format for a compiler. with an infinite
number of registers available, it is not necessary to perform register
allocation.

2) similar opcodes to wasm which is quite popular and otherwise
compromises like only allowing immediates with the IMM and FIMM.

3) realistic intermediate format that can be easily turned into
efficient RISC-V or ARM instructions (when the program uses less than
32 registers).

Now some cons:

1) variable length instructions, especially long ones like we support
because of large constants, are typically expensive to decode in
hardware so even with a restriction on the number of regsiters, this
may not be very efficient to turn into actual hardware.

2) not supported by gcc or llvm (though maybe wasm can be converted to
the Comet ISA easily).

## Instruction Format

All instructions are an (unsigned) LEB encoded integer followed by up
to 3 additional additional LEB encoded instruction fields. By design,
only a few instructions take constants (not even branches take
immediates).

All opcodes above 4096 are available as user defined opcodes. The
bottom N bits of the opcode then specifies one of the formats so that
the instruction can be decoded even if it's name or semantics are
unknown.
