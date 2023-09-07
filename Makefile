all: comet-vm

CC = clang

SRC_C = assembler.c \
	debug-repl.c \
	fatal-error.c \
	instruction-info.c \
	interpreter.c \
	main.c \
	paged-memory.c \
	printer.c \
	string-util.c \
	symbol-table.c \
	tokenizer.c \
	uleb128.c

SRC_H = assembler.h \
	debug-repl.h \
	cpu-thread-state.h \
	fatal-error.h \
	instruction-info.h \
	interpreter.h \
	opcodes.h \
	paged-memory.h \
	printer.h \
	string-util.h \
	symbol-table.h \
	tokenizer.h \
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
