#!/bin/bash

## TODO(jawilson): 

/home/jasonaaronwilson/src/comet-vm/comet-vm <<EOF
  assemble-file /home/jasonaaronwilson/src/comet-vm/test-data/nop.s
  step
  quit
EOF

exec $(dirname "$0")/pass-fail.sh $0 $?

