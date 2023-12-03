# @file io.c

This contains routines to read the contents of a file or write a
new file.
 
## @function buffer_append_file_contents

Completely reads a file and appends the contents to the passed in
buffer. This is often much more convenient than streaming a file.
 
## @function buffer_read_until

Read from a FILE* until either the end of file is reached or a
particular "end-of-line" character is read. Every character except
the end_of_line character is appended to the buffer.

This can be used to read normal Unix "lines" or another use is to
read "lines" that end with NUL (U+0000) or until say a seperator
like "," is encountered.

This function (and string_tokenize) should take a character set
which can unify their interface and allow U+0000 to be a member of
the set (unlike using a C string (aka char*) as the character set).

TODO(jawilson): write a proper test!
 
## @function buffer_write_file

Writes the contents of the buffer to the given file.
 
