#pragma once

#include <cstdarg>
#include <cstdio>

inline void log(const char *format, ...) {
#ifdef LOGGER
  char buf[BUFSIZ];
  va_list args;
  va_start(args, format);
  vsprintf(buf, format, args);
  fprintf(stderr, "%s", buf);
  va_end(args);
#endif
}