all: comet-vm

CC = clang
#CC = gcc

# debug information
CC_FLAGS=-g -rdynamic

SRC_C = allocate.c \
	array.c \
	assembler.c \
	byte-array.c \
	debug-repl.c \
	environment.c \
	evaluator.c \
	fatal-error.c \
	global-environment.c \
	instruction-info.c \
	interpreter.c \
	io.c \
	main.c \
	paged-memory.c \
	pair.c \
	primitive.c \
	printer.c \
	reader.c \
	string-util.c \
	symbol-table.c \
	tokenizer.c \
	uleb128.c

SRC_H = allocate.h \
	array.h \
	assembler.h \
	boolean.h \
	byte-array.h \
	cpu-thread-state.h \
	debug-repl.h \
	environment.h \
	evaluator.h \
	fatal-error.h \
	global-environment.h \
	instruction-info.h \
	interpreter.h \
	io.h \
	opcodes.h \
	optional.h \
	paged-memory.h \
	pair.h \
	primitive.h \
	printer.h \
	reader.c \
	string-util.h \
	symbol-table.h \
	tagged-reference.h \
	tokenizer.h \
	uleb128.h

comet-vm: ${SRC_C} ${SRC_H}
	${CC} ${CC_FLAGS} ${SRC_C} -o comet-vm
	stat --format=%s comet-vm

SYMBOL_HASH_SRC_C=string-util.c symbol-hash-main.c allocate.c fatal-error.c
SYMBOL_HASH_SRC_H=string-util.h

symbol-hash: ${SYMBOL_HASH_SRC_C} ${SYMBOL_HASH_SRC_H} 
	${CC} ${CC_FLAGS} ${SYMBOL_HASH_SRC_C} -o symbol-hash

format:
	clang-format -i ${SRC_C} ${SRC_H}

clean:
	rm -rf *~ a.out TAGS tests/*~ test-data/*~ comet-vm doxygen-docs symbol-hash

diff: clean
	git difftool HEAD

how-big: clean
	cat ${SRC_C} ${SRC_H} | wc --lines

tags:
	etags ${SRC_C} ${SRC_H}

TESTS = ./tests/nop-test.sh \
	./tests/numbers-test.sh \
	./tests/alignment.sh \
	./tests/count-down-loop.sh

#
# I'm trying to find a sensible test strategy. Tests should look
# pretty simple and run fast.
#	./tests/integer-binary-operators.sh
#

test: comet-vm
	./run-tests.sh ${TESTS}

docs:
	doxygen


