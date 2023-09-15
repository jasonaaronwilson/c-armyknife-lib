all: c-armyknife-lib

# CC = clang
CC = gcc

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
	tokenizer.h

SRC_H =  \
	boolean.h \
	ct-assert.h

generate-header-files: ${SRC_C}
	../c-single-source-file/generate-header-file ${SRC_C}

c-armyknife-lib: ${SRC_C} ${SRC_H} generate-header-files
	${CC} ${CC_FLAGS} -c -o allocate.o allocate.c
	${CC} ${CC_FLAGS} -c -o array.o array.c
	${CC} ${CC_FLAGS} -c -o byte-array.o byte-array.c
	${CC} ${CC_FLAGS} -c -o fatal-error.o fatal-error.c
	${CC} ${CC_FLAGS} -c -o io.o io.c
	${CC} ${CC_FLAGS} -c -o string-util.o string-util.c
	${CC} ${CC_FLAGS} -c -o tokenizer.o tokenizer.c
	ar -rcs libarmyknife.a allocate.o array.o byte-array.o fatal-error.o io.o string-util.o tokenizer.o
	stat --format=%s libarmyknife.a

format:
	clang-format -i ${SRC_C} ${SRC_H}

CLEAN_BINARIES = \
	a.out libarmyknife.a

clean:
	rm -rf *~ docs/*~ tests/*~ ${CLEAN_BINARIES} TAGS doxygen-docs *.o ${SRC_GENERATED_H}

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


