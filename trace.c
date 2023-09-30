#line 2 "trace.c"
#ifndef _TRACE_H_
#define _TRACE_H_

#include <stdio.h>

#define TRACE()                                                                \
  do {                                                                         \
    fprintf(stderr, "TRACE file=%s line=%d\n", __FILE__, __LINE__);            \
  } while (0)

#define WARN(msg)                                                              \
  do {                                                                         \
    fprintf(stderr, "WARN file=%s line=%d '%s'\n", __FILE__, __LINE__, msg);   \
  } while (0)

#endif /* _TRACE_H_ */
