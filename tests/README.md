# Comet VM test directory

A test is simply any program that requires zero arguments and returns
0 on success. Since the debug repl can assemble and execute comet
assembly language, many tests are simply bash scripts.

The script tests/integer-binary-operators.sh is a good example
where the entire test is driven by the debugger. Note we use the lispy
syntax for debugger commands so they are easy to spot which you can
read about in the debugger documentation.



