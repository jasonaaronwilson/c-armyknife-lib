# @file io.c

This contains routines to read the contents of a file or write a
new file.
 
## @function buffer_append_file_contents

Completely reads a file and appends the contents to the passed in
buffer. This is often much more convenient than streaming a file.
 
## @function buffer_write_file

Writes the contents of the buffer to the given file.
 
