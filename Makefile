all: c-armyknife-lib

SRC_C = allocate.c \
	boolean.c \
	buffer.c \
	command-line-parser.c \
	fatal-error.c \
	io.c \
	random.c \
	string-alist.c \
	string-hashtable.c \
	string-tree.c \
	string-util.c \
	test.c \
	tokenizer.c \
	trace.c \
	value.c \
	value-array.c

ORDERED_H = \
	fatal-error.h \
	boolean.h \
	value.h \
	trace.h \
	allocate.h \
	string-util.h \
	buffer.h \
	value-array.h \
	string-alist.h \
	string-hashtable.h \
	string-tree.h \
	command-line-parser.h \
	io.h \
	tokenizer.h \
	random.h \
	test.h

generate-header-files: ${SRC_C}
	../c-single-source-file/generate-header-file ${SRC_C}
	chmod -w ${ORDERED_H}

c-armyknife-lib: ${SRC_C} generate-header-files
	cat header-comment.txt ${ORDERED_H} >c-armyknife-lib.h
	echo '#ifdef C_ARMYKNIFE_LIB_IMPL' >>c-armyknife-lib.h
	cat ${SRC_C} >>c-armyknife-lib.h
	echo '#endif /* C_ARMYKNIFE_LIB_IMPL */' >>c-armyknife-lib.h

format:
	clang-format -i ${SRC_C} ${SRC_H} tests/*.c

clean:
	rm -rf *~ docs/*~ tests/*~ TAGS doxygen-docs *.o ${ORDERED_H} tests/build/*

diff: clean
	git difftool HEAD

how-big: clean
	cat ${SRC_C} ${SRC_H} | wc --lines

tags:
	etags ${SRC_C} ${SRC_H}

TESTS= \
	./compile-test.sh \
	./string-util-test.sh \
	./buffer-test.sh \
	./string-alist-test.sh \
	./string-hashtable-test.sh \
	./command-line-parser-test.sh \
	./tokenizer-test.sh \
	./io-test.sh \
	./value-array-test.sh \
	./random-test.sh \
	./string-tree-test.sh

test:
	(cd tests && ../run-tests.sh ${TESTS})

docs:
	doxygen

cfd:	clean format diff

cf:	clean format
