# Comet VM

Comet VM is an interpreted virtual machine for the Comet Instruction
Set Architecture (ISA).

Comet ISA is based on ULEB128 integer encoding for *all* fields of an
instruction. This means the encoding supports essentially infinite
numbers of opcodes, infinite numbers of general purpose or floating
point registers, full 64bit immediate constants, etc.

Comet is a RISC-like (upto 2 register sources and one register target)
register machine but because it supports essentially unlimited numbers
of registers, it can also serve as a compiler intermediate language -
one that can be executed with this VM.

## Status

2023-09-09T23:53:56-04:00

The basics have been worked out - there is an interpreter, a simple
built-in debugger (with some support to make testing easier), an
assembler, a disassembler, some basic amount of testing, a set of
conventions for the source code is worked out, a small support library
to make C bearable for "low-performance" cases is done, etc.

The current lines of source in *.[ch] is about 2257 lines (including
comment lines) though the ratio of documentation to code will probably
grow. I'm hoping that comet-vm will contains less than 5K lines of
code when it reaches version "1.0".

(BTW, use date --iso-8601=seconds to get a consistent format for
dates when updating the status).

## Why?

This is mostly a fun project, part of my "what if things were actually
simple" thesis (UTF-8 and "tar" archives have also been subject of my
scrutiny, spoiler, I think ULEB128 is a better encoding than UTF-8 but
that's another story).

The AMD 29K is an architecture that got shelved way too early. In
it's hayday, these chips powered laser printers, etc, as they were
fast and relatively inexpensive. [[BTW, AMD apparently reused some of
the design to produce the first commercially available mainstream
(certainly x86 anyways) chips to hit a 1 GHZ frequency even though
Intel had a clear advantage in silicon fabrication technology at the
time. The AMD 29K actually had "register windows" like SPARC but
better (Itanium, at least when I left the project, was going to have
something like regisgter windows...)]]

If the IBM PC ecosystem had not destroyed the enthusiast computer
market (in part because Motorola was pretty shit at chip design and
certainly inferior to Intel at fabrication and little-endian is
actually better than big-endian by a little bit) then maybe the AMD
29K would be still around in a meaningful way.

So this is a bit of a love letter to the AMD 29K.

The AMD 29K had 32 bit instructions and allowed 1 byte or 256 for
opcodes, and 1 byte for each source or destination register number (so
in theory could support 256 registers, it didn't in practice but had
way more architectural registers compared to it's
contemporaries). It's nearly obvious how to "assemble" an instruction
for the AMD 29K and even MIPS and POWERPC are not perfectly clean if
you look at their instruction encodings. Your clean looking assembly
code has bits going all over the place.

As long as an instruction is going to be at least 32 bits, we can free
our minds. ULEB128 is a byte-oriented technique for encoding large
unsigned numbers (even past 128 bits despite its name) which happens
to encode any number <= 127 (a seven bit number) in one byte.

So one byte opcode, one byte destination register, one byte for the
first source register and one byte for the second source register. It
kind of all makes sense to me and code density will be within a
constant factor of a current state of the art ISA. We won't win any
awards for code density this way (ULEB128 used in this format will
definitely "waste" 4 bits). Cramming as much "constants" into an
instruction generally is a win for code density but it always comes
with complexity in encoding/decoding and certainly versus a C (aka
byte based) instruction at a time emulator.

With an extremely minimal instruction set, 127 opcodes might be enough
(but it doesn't seems expansive). 127 integer registers and 127
floating-point registers seem like enough especially considering that
32 registers was deemed enough even by RISCV designers which is backed
up with lots of analysis before its design.

Why limit to only 127 opcodes or 127 registers or a 7 bit constant?
Fully embracing ULEB128 for each "field" of instruction has positive
and negative impacts. Increasing the opcode space beyound 127 opcodes
is easy with ULEB128 encoding though it does imply variable length
instructions (and byte aligned instructions start address instead of
nice 32bit aligned instruction start addresses). Once you get over
that, very large constants just fall out. And why not 10,000 or
1,000,000 registers?  Obviously no real machine will have 1,000,000
registers but having the flexibility to use that many registers is a
huge win for a *compiler intermediate language*. And if it's a "soft"
format using a JIT translator, maybe there is a win here and we might
unlock more extreme architectural decisions.

The instruction format for Comet is 1, 2, 3, or 4 ULEB constants. So
the minimum instruction length is 1 byte and the largest is super huge
(taken at face value of 128bits maximum for each field, which isn't a
technical limitation of ULEB128), we get about 19 * 4 or 74 bytes or
so).

## Pros and Cons

First the pros:

1) suitable as an intermediate format for a compiler - with an
infinite number of registers available, it is not necessary to perform
register allocation during code generation.

2) similar opcodes to wasm which is becoming quite popular

3) an intermediate format that can be easily turned into efficient
RISC-V or ARM instructions (when the program uses less than the number
of registers on those machines)

Now some cons:

1) variable length instructions, especially long ones like we support
because of large constants, are typically expensive to decode in
hardware so even with a restriction on the number of regsiters, this
may not be very efficient to turn into actual hardware.

2) not supported by gcc or llvm (though maybe wasm or other formats
can probably be converted to the Comet ISA easily).

Some neutral observations:

1) code density was not a primary concern in the design and therefore
is not likely to be great. OTOH, since instructions are variable
width, there are some cases where instructions will be shorter than on
a 32 bit RISC machine, especially register copies).

## RISCisms

gr0 is like other RISC machines in that it always reads as zero and
writes to gr0 thow away the result.

I'm not sure how RISC machines tend to treat fp0. If at least some
architectures follow the same convention, we will follow the same
convention.

## Instruction Format

The instruction format for Comet is 1, 2, 3, or 4 ULEB constants. (In
theory a future version of Comet could use even more ULEB constants.)

Only 4 instructions actually accept large constants (I'll probably add
several to deal with floating-point constants of various
formats). These instructions are always OPCODE, DESTINATION REGISTER,
CONSTANT.

Destination registers always proceed source registers (and technically
constants though see the above rule).

Except for details like minimizing instruction length (or rather,
leaving just enough space so the linker can insert the proper
constants), you can now assemble instructions for comet.

## Planned Features

1) User defined "microcode" (no recompilation necessary to have user
   defined opcodes)
2) in conjunction with #1, provide some number of instructions that
   are closer to common architectures.
3) time traveling debugger

## Intellectual Property

I'm by no means an expert in this area of law.

ULEB128 encoding has been used since at least 1995 for the DWARF debug
file format. It is also now used by WebAssembly. Google's protocol
buffer format essentially uses ULEB128 (we don't use SLEB128 nor does
Google since they use "zig-zag" for signed constants). As long as a
deep pocket like Google et. al has a vested interest in fighting
patent trolls, I think ULEB128 itself is IP safe.

The idea to have a variable length instructions based on one or more
ULEB128 fields may be very slightly novel strangely because it would
not be viewed as an efficient machine encoding. Despite this, and by
making this public disclosure on github, I am hoping to ward off
certain IP claims. I have posted the idea of forming an ISA by
sequencing ULEB128 constants to several Reddit groups including:

```
    r/publicdomain
    r/computerarchitecture
    r/ProgrammingLanguages
    r/scheme
    r/computer
    r/compiler
```

I have now publicly disclosed the idea of using sequences of ULEB
numbers to create an ISA. I have not nor will I apply for a patent
(nor should it be granted based on wide-spread dissemination of this
information).

# Developer Information

This will be moved into it's own file well before anyone else develops
for comet-vm.

## Style Guide

### C code formatting

All C source code is expected to be formatted with clang-format and
pass tests (just use make test though occasionally a breaking change
may be submitted since I am currently a solo developer). A
_clang-format file is provided along with a rule "make format" to
automatically apply this format. It's probably useful to first run
"make format" before editing any code to make sure no sweeping deltas
exist (may imply a different version of clang-format).

The current C style is based off of clang's LLVM style guide with a
few changes (including it's default line length, often a source of
style disagreement).

```
---
Language: Cpp
BasedOnStyle: LLVM
BreakBeforeBinaryOperators: All
PointerAlignment: Left
AlwaysBreakBeforeMultilineStrings: true
IndentWrappedFunctionNames: true
MaxEmptyLinesToKeep: 2
SpaceAfterCStyleCast: true
```

Some of these changes are consistent with WEBKIT's style guide so I'm
not the only one that likes code to look like this. I also like parts
of the WEBKIT style guide such as not using abbreviation.

The most noticeable deviation from LLVM style is breaking before
binary operators (even "=") instead of after. My justification is that
breaking after operators, while possibly removing a few lines from a
large code base, is less readable. My brain has been trained to look
at code from left to right and a line at a time AND also in chunks
scanning from left to right to understand at a glance the logic and
putting operators first means I can understand a chunk better
especially if there are chained "&&" and "||" which dramatically
effect control flow.

Pointer alignment just means I prefer "char*<SPACE>foo" to
"char<SPACE>*foo". Again this is based on my own brain, not science,
and there is a strong argument that with multiple variable
declarations in one statement, this is the wrong choice however I also
prefer to see each declaration as it's own statement so that advantage
doesn't fully apply. Each variable declaration should be on it's own
and also provide a default value (I wish C guaranteed that everything
was zeroed).

Adding a space after a C style cast also looks better to my eyes
though I feel less strongly about this than the above changes to LLVM
style.

### Naming

All names should be as desciptive as possible so abbreviations are
frowned upon. Most names should be *all lower case* (and obviously "_"
is meant to be used to seperate words, i.e., "snake case"). Sometimes
it feels like folks like the shortest names possible and the fewest
characters possible. While longer names probably have a minuscule
impact on executable size and possibly even execution time, like debug
information, it's very useful. In terms of execution speed, if the
length of an identifer changes the speed of a program once it has been
"linked", something is amiss.

All types we define in comet source code end in "_t" and additionally
"typedef" is used so that these types don't need the struct qualifier
(for self-recursive struct, say a next pointer, we use the suffix "_S"
and then use "struct foo_S" to handle it but the _S version should not
escape the typedef. Eliminating the struct qualifier makes it a bit
easier to abstract types without changing lines and lines of code. The
"_t" suffix I mostly use because of "<stdint.h>" which is usage is
very pervasive in comet (especially uint64_t) and I want as much
consistency as possible. BTW, the benefits of a short type name, say
u64 instead of uint64_t are dubious at best. While most programmers
(and large language models) will understand the shorter names, there
is only a slight gain in source code density and maybe even a slight
decrease in readability, at least for me. The solution to code not
fitting onto a small screen or something like that is to just refactor
the code into smaller functions. Both gcc and clang do many
optimizations that makes this almost a non-concern (for example, they
seem to inline small leaf procedures even without the inline
keyword). More importantly, small well named chunks of code are easier
to read.

Note that file names should use "-" instead of "_".

### Header Files

Header files which only need to be included once should be guarded by
something like this:

```C
#ifndef _FATAL_ERROR_
#define _FATAL_ERROR_

...
#endif /* _FATAL_ERROR_ */
```

This is so standard that if a header file has different meanings when
included multiple times (such as when folks use the C pre-processor
for generic programming and such), then this should be documented at
the top of the header file.

Most files will include from both the C standard library as well as
"self-includes" which should be grouped into two independent "blocks",
the "standard library includes" coming first, followed by a blank
line, followed by "self includes" and both should be independently
sorted (which clang-format does automatically).

### Documentation Comments

We are currently using javadoc style comments, which is luckily
understood by doxygen, for files and function (implementations). Since
defining more than one type in a header file is often a mistake though
in some case this is OK, we can document "types" in the implementation
file making header inclusion a bit faster. Mostly I consider header
files to be nuisance after programming in Java for so long and a big
header file is a bad code smell. javadoc style comments are probably a
bit longer than using "///" but after so much Java programming, my
brain likes the look of javadoc style comments and when I'm scanning
code, they help me identify "breaks". (One annoying thing about
clang-format is that it throws away page breaks, aka "CTRL-L" which I
actually kind of like (I learned this from a much better programmer
than me)).
