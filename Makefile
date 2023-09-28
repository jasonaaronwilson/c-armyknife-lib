all: c-armyknife-lib

SRC_C = allocate.c \
	array.c \
	byte-array.c \
	fatal-error.c \
	io.c \
	reference.c \
	string-util.c \
	tokenizer.c \
	trace.c \
	tuple.c \
	type.c

SRC_GENERATED_H = \
	allocate.h \
	array.h \
	byte-array.h \
	fatal-error.h \
	io.h \
	reference.h \
	string-util.h \
	tokenizer.h \
	trace.h \
	tuple.h \
	type.h

SRC_H =  \
	boolean.h \
	ct-assert.h

ORDERED_H = \
	trace.h \
	boolean.h \
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
	rm -rf *~ docs/*~ tests/*~ ${CLEAN_BINARIES} TAGS doxygen-docs *.o ${SRC_GENERATED_H} tests/build/*

diff: clean
	git difftool HEAD

how-big: clean
	cat ${SRC_C} ${SRC_H} | wc --lines

tags:
	etags ${SRC_C} ${SRC_H}

TESTS= ./compile-test.sh ./array-test.sh ./string-util-test.sh ./tokenizer-test.sh

test:
	(cd tests && ../run-tests.sh ${TESTS})

docs:
	doxygen


