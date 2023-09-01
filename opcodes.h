#ifndef _OPCODES_H_

// break instruction. on a posix system, this is likely to cause a
// signal, perhps sigil, to be raised but the emulator doesn't support
// this yet.
#define BRK 0

// perform no operation. this is used for padding which may
// occasionally be desired.
#define NOP 1

// read a register and if it is zero, perform a relative indirect
// branch and write the link address to the target register. This is a
// very powerful branch instruction since using the zero register as
// the predicate register makes the branch unconditional, and setting
// the link register to the zero register throws away a result that
// isn't needed. The surprising thing is that the branch is always
// indirect which is because we have IMM and PCIMM to set up the
// target address (and an assumption that we'll have lots of registers
// makes naming the target address not contribute to register
// pressure).
//
// This universal branch instruction probably has downsides for
// hardware implementations but reduces complexity by making things
// more regular.
#define BRZ 2

// Load a constant to an integer register
#define IMM 3

// Load the logical NOT of a constant to an integer register. This
// eliminates the need for another decoding strategy (for example SLEB
// or "zig-zag" could make sense)
#define NIMM 4

// Load an immediate plus the PC address of the begining of this
// instruction into an integer register. This instruction will be most
// used for forward jumps in PIC code.
#define PCIMM 5

// Load the logical NOT of an immediate plus the PC address of the
// begining of this instruction into an integer register. This
// instruction will be most used for backward jumps in PIC code.
#define NPCIMM 6

// Load a floating point immediate
#define FIMM 7

// move the value in one integer register to another integer register
#define MOV 8

// move the value in one fp register to another fp register
#define FMOV 9

/// Small gap here in case we missed critial opcodes...

// load 8 bits from the address in the source register and clear all
// other bits
#define LD_U8 16

// load 16 bits from the address in the source register and clear all
// other bits
#define LD_U16 17

// load 32 bits from the address in the source register and clear all
// other bits
#define LD_U32 18

// load 64 bits from the address in the source register
#define LD_U64 19

// load 8 bits from the address in the source register and sign extend
// to form the other bits
#define LD_S8 20

// load 16 bits from the address in the source register and sign extend
// to form the other bits
#define LD_S16 21

// load 32 bits from the address in the source register and sign extend
// to form the other bits
#define LD_S32 22

// NOTE: load signed 64 doesn't exist because sign extension doesn't
// make sense when the load already has the maximum amount of bits.

// undefined opcode (for now)
#define UNDEF_23 23

// Store the lowest 8 bits of a register to memory
#define ST_8 24

// Store the lowest 16 bits of a register to memory
#define ST_16 25

// Store the lowest 32 bits of a register to memory
#define ST_32 26

// Store all the bits of a register to memory
#define ST_64 27

#define FLD_8 28
#define FLD_16 29
#define FLD_32 30
#define FLD_64 31

#define FST_8 32
#define FST_16 33
#define FST_32 34
#define FST_64 35

// Count leading zeros
#define CLZ 48

// Count trailing zeros
#define CTZ 49

// Count number of bits set to 1
#define POPCNT 50

// Two's complement addition of the two source registers
#define ADD 51

// Two's complement subtraction of the two source registers
#define SUB 52

// Two's complement multiplication of the two source registers
#define MUL 53

#define DIV_U 54
#define DIV_S 55
#define REM_U 56
#define REM_S 57

// Logical AND of two source registers
#define AND 58

// Logical OR of two source registers
#define OR 59

// Logical XOR of two source registers
#define XOR 60

// Shift a value left by n bits
#define SHL 61

// Shift right (shifting zeros into the upper bits)
#define SHR_U 62

// Shift right (keeping the sign bit)
#define SHR_S 63

#define ROTL_8 64
#define ROTL_16 65
#define ROTL_32 66
#define ROTL_64 57

// rotate all 64 bits right
#define ROTR_64 58

// rotate lower 32 bits bits right
#define ROTR_32 59

// rotate lower 16 bits bits right
#define ROTR_16 60

// rotate lower 8 bits bits right
#define ROTR_8 61

#define LAST_OPCODE 256

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
