#!/bin/bash

# Originally I was piping stderr and stdout to a file but these
# instruction level tests aren't that long...

/home/jasonaaronwilson/src/comet-vm/comet-vm <<EOF
assemble
#
# First set up a bunch of immediates in the lower registers.
#
imm r1,15
step 1
#
# Now peform a bunch of operations checking their results
#
add r32,r1,r1
step 1
expect-register r32 30
#
# Finally we are done. Note that the black line below is necessary to
# exit the assembler.

quit
EOF

EXIT_CODE=$?
exec $(dirname "$0")/pass-fail.sh $0 ${EXIT_CODE}

