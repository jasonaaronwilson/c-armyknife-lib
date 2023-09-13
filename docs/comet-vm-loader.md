# Comet VM Loader

The commet VM loader can load new code into the current VM in several
formats.

The simplest format is to just load the raw contents of a file into
the VM at a given address with no additional processing.

Another format is akin to a traditional "object file" like ELF where
multiple relocatable "sections" can be defined. (We don't yet handle
ELF files but we can handle a simplified format based on an archive
file format called core-archive).

Usually the VM already contains an assembler, so another format, the
one used extensively for testing, is simply assembler source code.
