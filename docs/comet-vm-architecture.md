# Comet VM Architecture

## Byte Code Interpreter

The Comet VM contains a "byte-code" interpreter (and will eventually
support multiple threads). The byte-code interpreter operates on
either per thread registers (aka, machine state) or a 64bit address
space paged memory abstraction (multiple "address spaces" are
supported for isolation (aka sandboxing) if desired). This part of
comet is provided as a stand-alone shared library giving 3rd party
programs the ability to be embedded by this interpreter at an
extremely low foot-print (especially low considering that there is no
JIT and when a JIT is available, the interpreter only configuration
will still be provided).

## Program Loaders

The next piece of the Comet VM are program loaders. While some of
these loader involve the scheme interpreter, there is a version of the
standalone vm, comet-vm-no-scheme, which avoids this entirely. This
version may be desireable to offer a more locked-down experience
(though technically, the comet-vm already contains a Turing complete
interpreter so adding a scheme interpreter doesn't radically change
things though in fairness every line of code add an attack surface).

## The Debugger and Assembler

The final two pieces of the Comet VM are the assembler and
debugger. Both of these are dependent on a small scheme implementation
and the full power of scheme is exposed to give power users very
advanced capabilities and customization. This is similar to how emacs
contains the elisp interpreter and therefore instead of being just a
text editor, it can be highly customized to take on tasks that the
emacs authors did not anticipate. vi is a text editor but folks often
joke that emacs is really an "operating system". Obviously it is
possible to add your own commands (such as custom data inspectors)
that make the debugger one of the most powerful that I am aware of.

While normally the assembler and debugger would be provided as
separate programs, the idea of a single executable, comet-vm, that
basically contains it's own binary utils is very appealing since
rather than simply "crashing", the VM can enter the debugger
automatically. Note that having a debugger inside of the VM isn't
really a new concept, for example the JVM essentially contains many
parts of the debugger though an external program like jdb (not very
powerful unfortunately) is necessary to provide a user interface.

## Comet VM Libraries

comet-vm may come with scheme libraries to add new capabilities. For
example a library to convert x86-64 assembly (or ARM or RISC-V) to
equivalent comet assembly code (as well as debug routines to present
the register state as though it was actually running on one of those
processors) is almost like having a virtual machine for those other
architectures.

Users are encouraged to create there own personal debug
libraries. While it is impossible for me to create your personal ideal
debugger, you may be able to customize things to your liking.

## Lines of Code

While LOC is a terrible metric, we use it anyways as a metric for
complexity so that we can compare the comet-vm with other virtual
machines.

### TODO - start putting some LOC summaries here

### LOC comparisons with other VMs

# Conclusion

The comet VM is meant to be a serious tool as well as a possible
educational tool. Having essentially unlimited registers means that
comet can represent partially optimized code like what might be seen
in a compiler's intermediate representation where register allocation
has not occurred which I think will be a sweet spot for the comet VM
as compiler optimizer errors can be isolated to a particular copiler
phase. Lastly anyone looking for a very low foot-print VM without the
constraints of other VMs may find the comet VM very useful.

