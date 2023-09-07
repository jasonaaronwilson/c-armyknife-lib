all: comet-vm

CC = clang

SRC_C = assembler.c \
	fatal-error.c \
	instruction-info.c \
	interpreter.c \
	main.c \
	paged-memory.c \
	printer.c \
	uleb128.c

SRC_H = assembler.h \
	cpu-thread-state.h \
	fatal-error.h \
	instruction-info.h \
	interpreter.h \
	opcodes.h \
	paged-memory.h \
	printer.h \
	uleb128.h

comet-vm: ${SRC_C} ${SRC_H}
	${CC} ${SRC_C}
	stat --format=%s a.out

format:
	clang-format -i ${SRC_C} ${SRC_H}

clean:
	rm -rf *~ a.out TAGS

diff: clean
	git difftool HEAD

how-big: clean
	cat ${SRC_C} ${SRC_H} | wc --lines

tags:
	etags ${SRC_C} ${SRC_H}
