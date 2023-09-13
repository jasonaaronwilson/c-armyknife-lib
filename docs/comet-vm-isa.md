# Comet VM ISA

The comet VM runs a simple binary encoded insruction set. This
document describes the instruction format and then lists the supported
instruction.

## Comet Insruction Binary Encoding

Comet ISA is based on ULEB128 integer encoding for *all* fields of an
instruction. This means the encoding supports essentially unlimited
numbers of opcodes, unlimited numbers of general purpose or floating
point registers, full 64bit immediate constants, etc.

Comet is a RISC-like (upto 2 register sources and one register target)
register machine but because it supports essentially unlimited numbers
of registers, it can also serve as a compiler intermediate language -
one that can be executed with this VM. Additionally r0 works like the
zero register on typical RISC ISAs - it always reads as zero and
writes to r0 are effectively thrown away.

## Examples

## Currently Supported Opocdes

(fixme)
