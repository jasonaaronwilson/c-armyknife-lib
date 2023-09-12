# Comet VM Architecture

The Comet VM contains a "byte-code" interpreter which operatates on
the registers associated with a thread as well as with a paged memory
abstraction (multiple "address spaces" are supported for isolation
(aka sandboxing) if desired). In theory these can be provided as a
stand-alone shared library giving 3rd party programs the ability to be
embedded by this interpreter at a very low foot-print (especially low
considering that there is no JIT and when a JIT is available, the
interpreter only configuration will still be provided).

The next piece of the Comet VM are program loaders. While some of
these loader involve the scheme interpreter, there is a version of the
standalone vm, comet-vm-no-scheme, which avoids this entirely. This
version may be desireable to offer a more locked-down experience
(though technically, the comet-vm already contains a Turing complete
interpreter so adding a scheme interpreter doesn't radically change
things).

The final two pieces of the Comet VM are the assembler and
debugger. Both of these are dependent on a small scheme implementation
and the full power of scheme is exposed to give power users very
advanced capabilities and customization. This is similar to how emacs
contains the elisp interpreter and therefore instead of being just a
text editor, it can be highly customized to take on tasks that the
emacs authors did not anticipate. vi is a text editor but folks often
joke that emacs is an "operating system". Obviously it is possible to
add your own commands (such as custom data inspectors) that make the
debugger one of the most powerful that I am aware of.

While normally the assembler and debugger would be provided as
separate programs, the idea of a single executable, comet-vm, that
basically contains it's own binary utils is very appealing since
rather than simply "crashing", the VM can enter the debugger
automatically. Note that having a debugger inside of the VM isn't
really a new concept, for example the JVM essentially contains many
parts of the debugger though an external program like jdb (not very
powerful unfortunately) is necessary to provide a user interface.

