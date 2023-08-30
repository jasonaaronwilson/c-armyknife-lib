all: comet-vm

comet-vm: opcodes.h interpreter.c main.c
	gcc interpreter.c main.c

format:

