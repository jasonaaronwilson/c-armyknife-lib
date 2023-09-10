#!/bin/bash

/home/jasonaaronwilson/src/comet-vm/comet-vm >/tmp/numbers.out 2>&1 <<EOF
  assemble-file /home/jasonaaronwilson/src/comet-vm/test-data/numbers.s
  step 6
  expect-register r10 0xff
  quit
EOF

EXIT_CODE=$?
exec $(dirname "$0")/pass-fail.sh $0 ${EXIT_CODE}

