all: c-armyknife-lib

SRC_C = allocate.c \
	array.c \
	byte-array.c \
	fatal-error.c \
	io.c \
	string-util.c \
	tokenizer.c \
	type.c

SRC_GENERATED_H = \
	allocate.h \
	array.h \
	byte-array.h \
	fatal-error.h \
	io.h \
	string-util.h \
	tokenizer.h \
	type.h

SRC_H =  \
	boolean.h \
	ct-assert.h

ORDERED_H = \
	boolean.h \
	ct-assert.h \
	type.h \
	fatal-error.h \
	allocate.h \
	array.h \
	byte-array.h \
	io.h \
	string-util.h \
	tokenizer.h

generate-header-files: ${SRC_C}
	../c-single-source-file/generate-header-file ${SRC_C}

c-armyknife-lib: ${SRC_C} ${SRC_H} generate-header-files
	cat header-comment.txt ${ORDERED_H} >c-armyknife-lib.h
	echo '#ifdef C_ARMYKNIFE_LIB_IMPL' >>c-armyknife-lib.h
	cat ${SRC_C} >>c-armyknife-lib.h
	echo '#endif /* C_ARMYKNIFE_LIB_IMPL */' >>c-armyknife-lib.h

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


