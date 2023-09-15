all: c-armyknife-lib

CC = clang
#CC = gcc

# debug information
CC_FLAGS=-g -rdynamic

SRC_C = allocate.c \
	array.c \
	byte-array.c \
	debug-repl.c \
	environment.c \
	evaluator.c \
	fatal-error.c \
	io.c \
	paged-memory.c \
	string-util.c \
	tokenizer.c \
	uleb128.c

SRC_GENERATED_H = \
	allocate.h \
	array.h \
	byte-array.h \
	fatal-error.h \
	io.h \
	string-util.h

SRC_H =  \
	boolean.h \
	ct-assert.h \
	tagged-reference.h \
	optional.h \
	scheme-symbol.h \

comet-vm: ${SRC_C} ${SRC_H}
	${CC} ${CC_FLAGS} ${SRC_C} -o comet-vm
	stat --format=%s comet-vm

SYMBOL_HASH_SRC_C=string-util.c symbol-hash-main.c allocate.c fatal-error.c
SYMBOL_HASH_SRC_H=string-util.h

symbol-hash: ${SYMBOL_HASH_SRC_C} ${SYMBOL_HASH_SRC_H} 
	${CC} ${CC_FLAGS} ${SYMBOL_HASH_SRC_C} -o symbol-hash

format:
	clang-format -i ${SRC_C} ${SRC_H}

CLEAN_BINARIES = \
	a.out comet-vm symbol-hash

clean:
	rm -rf *~ docs/*~ tests/*~ scheme/*~ ${CLEAN_BINARIES} TAGS doxygen-docs 

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


