all: c-armyknife-lib

CC = clang
#CC = gcc

# debug information
CC_FLAGS=-g -rdynamic

SRC_C = allocate.c \
	array.c \
	byte-array.c \
	fatal-error.c \
	io.c \
	string-util.c \
	tokenizer.c

SRC_GENERATED_H = \
	allocate.h \
	array.h \
	byte-array.h \
	fatal-error.h \
	io.h \
	string-util.h \
	tokenizer.c \

SRC_H =  \
	boolean.h \
	ct-assert.h

c-armyknife-lib: ${SRC_C} ${SRC_H}
	${CC} ${CC_FLAGS} ${SRC_C} -o c-armyknife-lib
	stat --format=%s c-armyknife-lib

format:
	clang-format -i ${SRC_C} ${SRC_H}

CLEAN_BINARIES = \
	a.out FIXME

clean:
	rm -rf *~ docs/*~ tests/*~ ${CLEAN_BINARIES} TAGS doxygen-docs 

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


