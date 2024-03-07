all: c-armyknife-lib

install: c-armyknife-lib
	sudo install -m 755 c-armyknife-lib.h /usr/local/include/
	sudo install -m 755 c-armyknife-lib-no-lines.h /usr/local/include/

SRC_C = allocate.c \
	boolean.c \
	buffer.c \
	command-line-parser.c \
	flag.c \
	fatal-error.c \
	io.c \
	logger.c \
	min-max.c \
	random.c \
	string-alist.c \
	string-hashtable.c \
	string-tree.c \
	string-util.c \
	test.c \
	tokenizer.c \
	uint64.c \
	value.c \
	value-alist.c \
	value-array.c

ORDERED_H = \
	min-max.h \
	boolean.h \
	fatal-error.h \
	value.h \
	allocate.h \
	uint64.h \
	string-util.h \
	logger.h \
	buffer.h \
	value-array.h \
	value-alist.h \
	string-alist.h \
	string-hashtable.h \
	string-tree.h \
	flag.h \
	command-line-parser.h \
	io.h \
	tokenizer.h \
	random.h \
	test.h

generate-header-files: ${SRC_C}
	/usr/local/bin/generate-header-file ${SRC_C}
	chmod -w ${ORDERED_H}

c-armyknife-lib: ${SRC_C} generate-header-files
	cat header-comment.txt ${ORDERED_H} >c-armyknife-lib.h
	echo '#ifdef C_ARMYKNIFE_LIB_IMPL' >>c-armyknife-lib.h
	cat ${SRC_C} >>c-armyknife-lib.h
	echo '#endif /* C_ARMYKNIFE_LIB_IMPL */' >>c-armyknife-lib.h
	cat c-armyknife-lib.h | grep -v "#line" >c-armyknife-lib-no-lines.h

format:
	clang-format -i ${SRC_C} ${SRC_H} tests/*.c examples/*.c

clean:
	rm -rf *~ docs/*~ tests/*~ examples/*~ TAGS doxygen-docs *.o ${ORDERED_H} tests/build/* examples/build/*

diff: clean
	git difftool HEAD

how-big: clean
	cat ${SRC_C} ${SRC_H} | wc --lines

tags:
	etags ${SRC_C} ${SRC_H}

TESTS= \
	./compile-test.sh \
	./uint64-test.sh \
	./string-util-test.sh \
	./buffer-test.sh \
	./string-alist-test.sh \
	./string-hashtable-test.sh \
	./command-line-parser-test.sh \
	./flag-test.sh \
	./tokenizer-test.sh \
	./io-test.sh \
	./value-array-test.sh \
	./random-test.sh \
	./string-tree-test.sh \
	./logger-test.sh

test:
	(cd tests && mkdir -p build && ../run-tests.sh ${TESTS})

doc:
	c-markdown-extractor --output-dir=./src-doc/ *.c

cfd:	clean format diff

cf:	clean format
