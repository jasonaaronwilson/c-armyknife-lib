#!/bin/bash

/home/jasonaaronwilson/src/comet-vm/comet-vm >/tmp/integer-binary-operators.out 2>&1 <<EOF
assemble
imm r1,15
add r2,r1,r1

step 2
expect-register r2 30
quit
EOF

EXIT_CODE=$?
exec $(dirname "$0")/pass-fail.sh $0 ${EXIT_CODE}

