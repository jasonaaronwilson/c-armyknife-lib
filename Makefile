all: c-armyknife-lib

SRC_C = allocate.c \
	array.c \
	boolean.c \
	byte-array.c \
	ct-assert.c \
	fatal-error.c \
	io.c \
	reference.c \
	string-util.c \
	test.c \
	tokenizer.c \
	trace.c \
	tuple.c \
	type.c

ORDERED_H = \
	ct-assert.h \
	boolean.h \
	trace.h \
	ct-assert.h \
	type.h \
	fatal-error.h \
	reference.h \
	tuple.h \
	allocate.h \
	array.h \
	byte-array.h \
	io.h \
	string-util.h \
	tokenizer.h \
	test.h

generate-header-files: ${SRC_C}
	../c-single-source-file/generate-header-file ${SRC_C}

c-armyknife-lib: ${SRC_C} generate-header-files
	cat header-comment.txt ${ORDERED_H} >c-armyknife-lib.h
	echo '#ifdef C_ARMYKNIFE_LIB_IMPL' >>c-armyknife-lib.h
	cat ${SRC_C} >>c-armyknife-lib.h
	echo '#endif /* C_ARMYKNIFE_LIB_IMPL */' >>c-armyknife-lib.h

format:
	clang-format -i ${SRC_C} ${SRC_H}

clean:
	rm -rf *~ docs/*~ tests/*~ TAGS doxygen-docs *.o ${ORDERED_H} tests/build/*

diff: clean
	git difftool HEAD

how-big: clean
	cat ${SRC_C} ${SRC_H} | wc --lines

tags:
	etags ${SRC_C} ${SRC_H}

TESTS= ./compile-test.sh  ./tuple-test.sh ./array-test.sh ./string-util-test.sh ./tokenizer-test.sh

test:
	(cd tests && ../run-tests.sh ${TESTS})

docs:
	doxygen

cfd:	clean format diff
