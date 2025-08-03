#ifndef MACRO_H
#define MACRO_H

#include <stdio.h>
#include <string.h>
#include <errno.h>

// Size in bytes of shared memory region
#define REGION_SIZE 1000

#define ANSI_COLOR_RED "\033[31m"
#define ANSI_BOLD      "\033[1m"
#define ANSI_RESET     "\033[0m"


/*
 * Prints error, for example:
 * helpers.c: line 10: main(): error in func() (404): No such file or directory
 *
 * Requires included: stdio.h, errno,h, string.h
 */

// Macro function to print errors by errno value
#define PRINT_ERRNO_MSG(func_name) \
        fprintf(stderr, ANSI_BOLD \
                        "%s: throw on line %d:" \
                        ANSI_RESET \
                        " %s:" \
                        ANSI_COLOR_RED \
                        " error" \
                        ANSI_RESET \
                        " in %s (%d): %s\n", \
                __FILE__, \
                __LINE__, \
                __FUNCTION__, \
                func_name, \
                errno, \
                strerror(errno))

// Macro function to print custom error messages (example: for scanf func)
#define PRINT_ERRMSG(func_name, return_value, msg) \
        fprintf(stderr, ANSI_BOLD \
                        "%s: throw on line %d:" \
                        ANSI_RESET \
                        " %s:" \
                        ANSI_COLOR_RED \
                        " error" \
                        ANSI_RESET \
                        " in %s (%d): %s\n", \
                __FILE__, \
                __LINE__, \
                __FUNCTION__, \
                func_name, \
                return_value, \
                msg)

#endif
