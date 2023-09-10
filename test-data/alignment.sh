#!/bin/bash

/home/jasonaaronwilson/src/comet-vm/comet-vm <<EOF
  assemble-file /home/jasonaaronwilson/src/comet-vm/test-data/alignment.s
  disassemble
  quit
EOF

exec $(dirname "$0")/pass-fail.sh $0 $?

