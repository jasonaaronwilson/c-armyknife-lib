#!/bin/bash

/home/jasonaaronwilson/src/comet-vm/comet-vm <<EOF
  assemble-file /home/jasonaaronwilson/src/comet-vm/test-data/count-down-loop.s
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

exec $(dirname "$0")/pass-fail.sh $0 $?

