#!/bin/bash

# Originally I was piping stderr and stdout to a file but these
# instruction level tests aren't that long...

/home/jasonaaronwilson/src/comet-vm/comet-vm <<EOF
assemble
# First set up a bunch of immediates in the lower registers.
imm r1,15
# FIXME putting something illegal like just "imm" crashes stuff...
# Now 
add r32,r1,r1
#
#
# The blank line below exits the assembler back the the debugger's repl.

step 2
expect-register r32 30
quit
EOF

EXIT_CODE=$?
exec $(dirname "$0")/pass-fail.sh $0 ${EXIT_CODE}

