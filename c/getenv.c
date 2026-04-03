/*
Function to override standard getenv version.

Build: gcc -shared -fPIC hook.c -o getenv_hook.so -ldl
Usage: LD_PRELOAD=./getenv_hook.so <command>
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

char *getenv(const char *name) {
    static char *(*real_getenv)(const char*) = NULL;

    if (!real_getenv)
        real_getenv = dlsym(RTLD_NEXT, "getenv");

    fprintf(stderr, "getenv called: %s\n", name);
    return real_getenv(name);
}
