all: comet-vm

CC = clang

SRC_C = allocate.c \
	array.c \
	assembler.c \
	byte-array.c \
	debug-repl.c \
	fatal-error.c \
	instruction-info.c \
	interpreter.c \
	io.c \
	main.c \
	paged-memory.c \
	printer.c \
	string-util.c \
	symbol-table.c \
	tokenizer.c \
	uleb128.c

SRC_H = allocate.h \
	array.h \
	assembler.h \
	byte-array.h \
	debug-repl.h \
	cpu-thread-state.h \
	fatal-error.h \
	instruction-info.h \
	interpreter.h \
	io.h \
	opcodes.h \
	paged-memory.h \
	printer.h \
	string-util.h \
	symbol-table.h \
	tokenizer.h \
	uleb128.h

comet-vm: ${SRC_C} ${SRC_H}
	${CC} -g ${SRC_C} -o comet-vm
	stat --format=%s comet-vm

format:
	clang-format -i ${SRC_C} ${SRC_H}

clean:
	rm -rf *~ a.out TAGS test-data/*~ comet-vm doxygen-docs

diff: clean
	git difftool HEAD

how-big: clean
	cat ${SRC_C} ${SRC_H} | wc --lines

tags:
	etags ${SRC_C} ${SRC_H}

TESTS = ./test-data/nop-test.sh \
	./test-data/numbers-test.sh \
	./test-data/alignment.sh \
	./test-data/count-down-loop.sh

test: comet-vm
	./run-tests.sh ${TESTS}

docs:
	doxygen


