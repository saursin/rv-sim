#pragma once

#include <cstdarg>
#include <cstdio>
#include <stdint.h>
#include <string>

extern uint64_t sim_cycles;

#ifdef LOGGER_ALL
#define LOGGER_INFO
#define LOGGER_DEBUG
#define LOGGER_CORE
#endif


#define TERMCOLOR_Red       "\u001b[31m"
#define TERMCOLOR_Green     "\u001b[32m"
#define TERMCOLOR_Yellow    "\u001b[33m"
#define TERMCOLOR_Blue      "\u001b[34m"
#define TERMCOLOR_Magenta   "\u001b[35m"
#define TERMCOLOR_Cyan      "\u001b[36m"
#define TERMCOLOR_White     "\u001b[37m"
#define TERMCOLOR_Reset     "\u001b[0m"


#define __LOGFN_BODY                \
    char buf[BUFSIZ];               \
    va_list args;                   \
    va_start(args, format);         \
    vsprintf(buf, format, args);    \
    va_end(args);                   \
    fprintf(stderr, "%s%08ld %s", TERMCOLOR_Cyan, sim_cycles, TERMCOLOR_Reset); \


inline void log_info(const char *format, ...) {
#ifdef LOGGER_INFO
    __LOGFN_BODY
    fprintf(stderr, "[INFO] ");
    fprintf(stderr, "%s", buf);
#endif
}

inline void log_debug(const char *format, ...) {
#ifdef LOGGER_DEBUG
    __LOGFN_BODY
    fprintf(stderr, "%s[DEBUG] %s", TERMCOLOR_Green, TERMCOLOR_Reset);
    fprintf(stderr, "%s", buf);
#endif
}

inline void log_error(const char *format, ...) {
#ifdef LOGGER_DEBUG
    __LOGFN_BODY
    fprintf(stderr, "%s[ERROR] %s", TERMCOLOR_Red, TERMCOLOR_Reset);
    fprintf(stderr, "%s", buf);
#endif
}

inline void log_core(unsigned core_id, const char *format, ...) {
#ifdef LOGGER_CORE
    __LOGFN_BODY
    fprintf(stderr, "[Core % 2d] ", core_id);
    fprintf(stderr, "%s", buf);
#endif
}
