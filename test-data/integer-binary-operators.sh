#!/bin/bash

/home/jasonaaronwilson/src/comet-vm/comet-vm <<EOF


=====================================================================
========  # Put common immediates in the lower registers.
========  
========  setup:
========  imm r1,1
========  imm r2 19
========  brk
=====================================================================


=====================================================================
========  test-add:
========  add r32,r2,r2
========  brk
=====================================================================
    execute setup test-add
    expect-register r32 38


=====================================================================
========  test_unsigned_sub:
========  sub r32,r2,r2
========  nop
========  brk
=====================================================================
    execute setup test_unsigned_sub
    expect-register r32 38
    expect-register r32 38
    expect-register r32 38


======================================================================
========  test_add:
========  add r32,r2,r2
========  brk
=====================================================================
    execute setup test_add
    expect-register r32 38


quit
EOF

EXIT_CODE=$?
exec $(dirname "$0")/pass-fail.sh $0 ${EXIT_CODE}

