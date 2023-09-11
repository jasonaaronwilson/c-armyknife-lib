#!/bin/bash

/home/jasonaaronwilson/src/comet-vm/comet-vm <<EOF
  assemble-file /home/jasonaaronwilson/src/comet-vm/tests/count-down-loop.s
  disassemble
  step
  step
  step
  step
  step
  step
  step
  step
  step
  step
  step
  step
  step
  step
  quit
EOF

EXIT_CODE=$?
exec $(dirname "$0")/pass-fail.sh $0 ${EXIT_CODE}

