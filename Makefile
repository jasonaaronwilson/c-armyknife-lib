all: comet-vm

comet-vm: opcodes.h interpreter.h interpreter.c main.c
	gcc interpreter.c main.c
	stat --format=%s a.out

format:
	clang-format -i opcodes.h interpreter.c main.c

diff:
	git difftool HEAD



