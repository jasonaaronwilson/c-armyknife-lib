all: c-armyknife-lib

install: c-armyknife-lib
	sudo install -m 755 c-armyknife-lib.h /usr/local/include/
	sudo install -m 755 c-armyknife-lib-no-lines.h /usr/local/include/

SRC_C = allocate.c \
	boolean.c \
	buffer.c \
	cdl-printer.c \
	compound-literal.c \
	fn.c \
	flag.c \
	fatal-error.c \
	io.c \
	leb128.c \
	logger.c \
	min-max.c \
	random.c \
	string-alist.c \
	string-hashtable.c \
	string-tree.c \
	string-util.c \
	terminal.c \
	test.c \
	tokenizer.c \
	uint64.c \
	utf8-decoder.c \
	value.c \
	value-alist.c \
	value-array.c \
	value-hashtable.c \
	value-tree.c

ORDERED_H = \
	min-max.h \
	boolean.h \
	compound-literal.h \
	fn.h \
	leb128.h \
	fatal-error.h \
	value.h \
	allocate.h \
	uint64.h \
	string-util.h \
	logger.h \
	utf8-decoder.h \
	buffer.h \
	value-array.h \
	value-alist.h \
	string-alist.h \
	value-hashtable.h \
	string-hashtable.h \
	value-tree.h \
	string-tree.h \
	flag.h \
	io.h \
	terminal.h \
	tokenizer.h \
	random.h \
	cdl-printer.h \
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

# value.c 
SRC_C_FILTERED := $(filter-out logger.c value-hashtable.c, $(SRC_C))

# This doesn't work yet but this is the goal 
c-armyknife-lib-omni-c:
	../omni-c/src/build/bin/omni-c generate-library --output-file=c-armyknife-lib-omni-c.c ${SRC_C_FILTERED}
	../omni-c/src/build/bin/omni-c generate-header-file --output-file=c-armyknife-lib-omni-c.h ${SRC_C_FILTERED}
	gcc -o /tmp/armyknife-lib c-armyknife-lib-omni-c.c

format:
	clang-format -i ${SRC_C} ${SRC_H} tests/*.c examples/*.c

clean:
	rm -rf *~ docs/*~ tests/*~ examples/*~ TAGS doxygen-docs *.o ${ORDERED_H} tests/build/* examples/build/*

# These are generated files that I happen to check-in to make it
# easier for people to use this as a "single source file" but seem to
# diverge a bit which should be investigated (maybe as simple as I
# started using lazygit and I didn't include them like git add --all
# would do?)
very-clean: clean
	git checkout c-armyknife-lib-no-lines.h c-armyknife-lib.h	

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
	./value-alist-test.sh \
	./string-alist-test.sh \
	./value-hashtable-test.sh \
	./string-hashtable-test.sh \
	./flag-test.sh \
	./tokenizer-test.sh \
	./io-test.sh \
	./value-array-test.sh \
	./random-test.sh \
	./value-tree-test.sh \
	./string-tree-test.sh \
	./logger-test.sh \
	./terminal-test.sh \
	./cdl-printer-test.sh

test:
	(cd tests && mkdir -p build && ../run-tests.sh ${TESTS})

doc:
	c-markdown-extractor --output-dir=./src-doc/ *.c

cfd:	clean format diff

cf:	clean format
