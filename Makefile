all: c-armyknife-lib

install: c-armyknife-lib
	sudo install -m 755 c-armyknife-lib.h /usr/local/include/
	sudo install -m 755 c-armyknife-lib-no-lines.h /usr/local/include/

# Define the ordered list of source files
SRC_C = \
	min-max.c \
	boolean.c \
	compound-literal.c \
	fn.c \
	leb128.c \
	fatal-error.c \
	value.c \
	allocate.c \
	uint64.c \
	string-util.c \
	logger.c \
	utf8-decoder.c \
	buffer.c \
	value-array.c \
	value-alist.c \
	string-alist.c \
	value-hashtable.c \
	string-hashtable.c \
	value-tree.c \
	string-tree.c \
	flag.c \
	io.c \
	terminal.c \
	tokenizer.c \
	random.c \
	cdl-printer.c \
	test.c

ORDERED_H = $(SRC_C:.c=.h)

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
SRC_C_FILTERED := $(SRC_C)
# SRC_C_FILTERED := $(filter-out logger.c, $(SRC_C))

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
