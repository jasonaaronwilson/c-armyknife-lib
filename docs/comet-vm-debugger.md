# Comet VM Debugger

The Comet VM is itself a powerful instruction level debugger. The
instruction level debugger is actually a tiny scheme repl which makes
many tasks including automated testing much easier. Eventually a
breakpoint could use a script to automatically continue making them
conditional or even collect information at each break-point such as a
histogram of a particular registers value. They can obviously inject
values into registers (or memory). We eventually hope to have
time-traveling capabilities, aka reverse execution, to be the best
debugging experience for power users and is probably unprecedented in
a low-level instruction debugger.

Even though the debugger repl is a scheme interpreter, a simple trick
allows you to write simply "quit" instead of "(quit)" to invoke the
debugger command "quit" so you may not even realize that scheme is
being used to interpret your commands when you first use the
debugger. The debugger repl should feel like using gdb until you use
it's advanced capabilities.

## Common Debugger Commands

These commands are what is typically provided by a debugger like gdb:

* quit
* breakpoint
* etc.

### Supporting the gdb Simple Style Command Syntax

A very simple trick turns what look like gdb commands into scheme so
that it we can do read(), eval(), print() on it. If a line contains no
parens and one or more tokens, it is automatically turned into a
function call.

So if you type:

```
  help
```

This is converted, by textual means, to:

```
  (help)
```

before reading and evaluation. This and the fact that the interactive
command processor knows how to read multiple lines of input when the
current input is not complete (unclosed parens), makes the debugger
easy to use for beginners (since the syntax looks like gdb) and
therefore more convenient for interactive use.

## Differences From Standard Scheme

### In a nutshell

* extremely simplfied syntax (for example no dotted pairs)
* no quasi quatation (yet?)
* far fewer special forms
* fewer data-types
* only 64bit integers (and eventually double) so no real numeric tower
* no macros (though we do support "eval" so...)
* symbols are not interned
* no garbage collection (yet!) so large scripts could run out of
  memory
* tail recursion is basically not guaranteed though when gcc -O or
  clang (at any optimization level), tail recursion will probably work
  as expected.

## Additional Functions (and Data) Available In the Debug Repl

* *thread-state*
* continue
* breakpoint
* etc.
