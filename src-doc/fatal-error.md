## @constants error_code_t
 
# @file: fatal-error.c

The intent is that everything but a normal program exit will end up
here. (Currently, we don't catch any signals so this is definitely
not true.)

In this case C's macros are paying off as the file and line number
are easy to obtain.

TODO(jawilson): command line flag to be quieter...
 
## @macro fatal_error

Terminates the program with a fatal error.
 
