#ifndef _OPCODES_H_

// break instruction.
//
// On a posix system, this is likely to cause a signal, perhps sigil,
// to be raised but the emulator doesn't support this yet.
//
// In the very olden times, debuggers would physically write a BREAK
// instruction to perform break-points but we'll provide a better way.
#define BRK 0

// Do nothing except advance the PC.
//
// This is used for padding which may occasionally be desired. While
// larger NOP instructions could be composed by say "mov r0,r0" (3
// bytes), or say "IMM r0,X" (where X could can be be as small as 1
// byte or as large as 64/7 bytes (9 bytes)), just using more NOPs is
// the preferred way of padding for clarity and performance.
#define NOP 1

// read a register and if it is zero, perform an indirect branch.
//
// In the taken case, write the link address to the target register
// (which is often r0 to throw the link address away).
//
// This is a very powerful branch instruction since using the zero
// register as the predicate register makes the branch unconditional,
// and sending the link address to r0 makes the branch more like
// "jmp" than a "call" instruction.
//
// This universal branch instruction probably has downsides for
// hardware implementations but reduces complexity by making things
// more regular for "software".
//
// Let's use an example:
//
// brz r0,r12,r19
//
// Using r0 as the target register makes this more like a "branch"
// than a "call". The "CPU" will read r12 and compare it with
// zero. So if r12 is zero, we branch to the address in r19, and if
// not, obviously we just keep executing instructions and possibly
// some IMM/PCIMM instructions were not needed. (Shrug Emoji)
//
// Note: the destination register is ALWAYS written even if the branch
// isn't taken (which is why r0 will often be the written
// register).
//
// In most cases, PCIMM/SPCIMM is superior to an intentionally not
// taken branch if your code needs to figure out relative addresses.
#define OPCODE_BRZ 2

// Load a constant to an integer register from the instruction stream.
#define OPCODE_IMM 3

// Load the negative (two's complement) of a constant to an integer
// register. This eliminates the need for another decoding strategy
// (for example SLEB or "zig-zag")
#define OPCODE_SIMM 4

// Load an immediate plus the PC address of the begining of this
// instruction into an integer register. This instruction will be most
// used for forward jumps in PIC code.
#define OPCODE_PCIMM 5

// Load the negative (two's complement) of an immediate plus the PC
// address of the begining of this instruction into an integer
// register. This instruction will be most used for backward jumps in
// PIC code.
#define OPCODE_SPCIMM 6

// Load a floating point immediate
#define OPCODE_FIMM 7

// move the value in one integer register to another integer register
#define OPCODE_MOV 8

// move the value in one fp register to another fp register
#define OPCODE_FMOV 9

/// Small gap here in case we missed critial opcodes...

// load 8 bits from the address in the source register and clear all
// other bits
#define OPCODE_LD_U8 16

// load 16 bits from the address in the source register and clear all
// other bits
#define OPCODE_LD_U16 17

// load 32 bits from the address in the source register and clear all
// other bits
#define OPCODE_LD_U32 18

// load 64 bits from the address in the source register
#define OPCODE_LD_U64 19

// load 8 bits from the address in the source register and sign extend
// to form the other bits
#define OPCODE_LD_S8 20

// load 16 bits from the address in the source register and sign extend
// to form the other bits
#define OPCODE_LD_S16 21

// load 32 bits from the address in the source register and sign extend
// to form the other bits
#define OPCODE_LD_S32 22

// NOTE: load signed 64 doesn't exist because sign extension doesn't
// make sense when the load already has the maximum amount of bits.

// undefined opcode (for now)
#define OPCODE_UNDEF_23 23

// Store the lowest 8 bits of a register to memory
#define OPCODE_ST_8 24

// Store the lowest 16 bits of a register to memory
#define OPCODE_ST_16 25

// Store the lowest 32 bits of a register to memory
#define OPCODE_ST_32 26

// Store all the bits of a register to memory
#define OPCODE_ST_64 27

#define OPCODE_FLD_8 28
#define OPCODE_FLD_16 29
#define OPCODE_FLD_32 30
#define OPCODE_FLD_64 31

#define OPCODE_FST_8 32
#define OPCODE_FST_16 33
#define OPCODE_FST_32 34
#define OPCODE_FST_64 35

// Count leading zeros
#define OPCODE_CLZ 48

// Count trailing zeros
#define OPCODE_CTZ 49

// Count number of bits set to 1
#define OPCODE_POPCNT 50

// Two's complement addition of the two source registers
#define OPCODE_ADD 51

// Two's complement subtraction of the two source registers
#define OPCODE_SUB 52

// Two's complement multiplication of the two source registers
#define OPCODE_MUL 53

#define OPCODE_DIV_U 54
#define OPCODE_DIV_S 55
#define OPCODE_REM_U 56
#define OPCODE_REM_S 57

// Logical AND of two source registers
#define OPCODE_AND 58

// Logical OR of two source registers
#define OPCODE_OR 59

// Logical XOR of two source registers
#define OPCODE_XOR 60

// Shift a value left by n bits
#define OPCODE_SHL 61

// Shift right (shifting zeros into the upper bits)
#define OPCODE_SHR_U 62

// Shift right (keeping the sign bit)
#define OPCODE_SHR_S 63

#define OPCODE_ROTL_8 64
#define OPCODE_ROTL_16 65
#define OPCODE_ROTL_32 66
#define OPCODE_ROTL_64 57

// rotate all 64 bits right
#define OPCODE_ROTR_64 58

// rotate lower 32 bits bits right
#define OPCODE_ROTR_32 59

// rotate lower 16 bits bits right
#define OPCODE_ROTR_16 60

// rotate lower 8 bits bits right
#define OPCODE_ROTR_8 61

#define OPCODE_LAST_OPCODE 256

// Absolutely unnecessary definitions that may make it easier to read
// some source code.

#define R0 0
#define R1 1
#define R2 2
#define R3 3
#define R4 4
#define R5 5
#define R6 6
#define R7 7
#define R8 8
#define R9 9
#define R10 10
#define R11 11
#define R12 12
#define R13 13
#define R14 14
#define R15 15

#define FP0 0
#define FP1 1
#define FP2 2
#define FP3 3
#define FP4 4
#define FP5 5
#define FP6 6
#define FP7 7
#define FP8 8
#define FP9 9
#define FP10 10
#define FP11 11
#define FP12 12
#define FP13 13
#define FP14 14
#define FP15 15

#endif /* _OPCODES_H_ */

// TODO: parity?

/*
                FABS
                FNEG
                FSQRT
                FCEIL
                FFLOOR
                FTRUNC
                FNEAREST
                FADD
                FSUB
                FMUL
                FDIV
                FMIN
                FMAX
                FCOPYSIGN

*/
