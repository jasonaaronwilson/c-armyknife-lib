all: examples manual-smoke-test

examples:
	mkdir -p build/
	gcc -g -rdynamic -O2 -o build/unique-file-lines unique-file-lines.c
	gcc -g -rdynamic -O2 -o build/sort-lines sort-lines.c
	gcc -g -rdynamic -O2 -o build/line-histogram line-histogram.c
	gcc -g -rdynamic -o build/unique-line-generator unique-line-generator.c

manual-smoke-test: examples
	@echo "================================================================="
	@echo "            unique-file-lines                                    "
	@echo "================================================================="
	build/unique-file-lines testdata/sample.txt 
	@echo "================================================================="
	@echo "            sort-lines                                           "
	@echo "================================================================="
	build/sort-lines testdata/sample.txt 
	@echo "================================================================="
	@echo "            line-histogram                                       "
	@echo "================================================================="
	build/line-histogram testdata/sample.txt 
	@echo "================================================================="
	@echo "            unique-line-generator                                "
	@echo "================================================================="
	build/unique-line-generator --max-number=1000000 --number-of-lines=5

