# Comet Assembler

The comet VM can include a built-in assembler.

At first glance the assembler syntax looks very similar to other
assemblers.

This is a valid assembly language fragment:

```
    _start:
    add r0,r2,r3

    ; this is a comment that extends to the end of the line

    ;;;
    ;;; This is also a comment but it stands out more.
    ;;;

    ;;; There are some familar assembler directives
    .align 8

    ; commas between fields are optional and hex formatted numbers are
    ; supported.
    imm r5 0x5555

    ; binary numbers are also useful. Let's me know if you think we 
    ; should add octal number support.
    imm r6, 0b101111

    ; like _start: above, we have "labels"
    another_label:
    multiple_labels_at_the_same_address_is_supported:
    add r19,r0,r0

    ; Like other assemblers, you can have different "sections" for
    ; code and data so that code and read-only data can be write
    ; protected or marked for no execution, etc.
    .data
    my_object:
    .byte 10 12 34

    ; But wait, there is more. You can embed scheme code!

    (define %tmp0 r1)
    (define %tmp1 r2)
    (define %tmp2 r3)
    (define %tmp3 r4)

    (define (clear-scratch-registers)
      (list (list mov %tmp0 r0)
            (list mov %tmp1 r0)
            (list mov %tmp2 r0)
            (list mov %tmp3 r0)))

    some_routine:
    (clear-scratch-registers)
    add %tmp1 r12 r19
    imm %tmp2 0xf0f0
    xor %tmp1 %tmp1 %tmp2
```

However this assembler includes the ability to evaluate comet-vm
scheme expressions during assembly which is how computed fields are
handled. So where a traditional assembler would use an expression like
'foo+10', our syntax for the same thing looks like '(+ (address "foo")
10)'.

In fact, arbitrary scheme code can be executed which means that very
powerful "assembler macros" can be written in scheme! You can even use
things like a random number generator in your scheme code to say
generate labels names, etc. In this case, the scheme code invoked must
return one of the following values:

1. an empty list
2. a single integer (that will be encoded in ULEB128)
3. a byte-vector
4. a label marker (which causes an entry to be added to the symbol
   table)
5. a relocation expression
6. a list of these items (including other lists)

The scheme code can also have side-effects like directly entering a
new label into the assemblers symbol table, incrementing a global
variable containing say a counter, etc.

In order to perform the equivalent of "branch tensioning", the
assembler will assemble the code multiple times until all labels
settle and this means running your code multiple times. This means
that your code scheme code must be idempotent. To make this easier, a
fresh environment is created each time the assembler runs so state
like the random number generator, are reset automatically between
iterations.
