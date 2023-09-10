#!/bin/bash

## TODO(jawilson): 

/home/jasonaaronwilson/src/comet-vm/comet-vm >/tmp/numbers.out 2>&1 <<EOF
  assemble-file /home/jasonaaronwilson/src/comet-vm/test-data/numbers.s
  step 6
  print-register r10
  quit
EOF

OUTPUT=`tail -n 2 /tmp/numbers.out | head -n 1`
EXPECTED='(debug) r10 00000000000000ff'
if [[ ${OUTPUT} == ${EXPECTED} ]] ; then
    echo PASS $0 
else
    echo FAIL $0 
    echo "--" expected ${EXPECTED} but got ${OUTPUT}
    exit 1
fi
