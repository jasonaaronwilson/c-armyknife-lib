# Trivial count down loop.
_start: 
imm r1,5
imm r2,loop_header
imm r3,1
loop_header:
sub r1,r1,r3
brz r0,r1,r2
brk
