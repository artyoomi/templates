#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <stdbool.h>

#define ehdlr(msg) {perror(msg); exit(EXIT_FAILURE);}

int (*some_func)(int);

bool
init_library(const char *libname,
             const char *funcname)
{
    void *hdl;

    hdl = dlopen(libname, RTLD_LAZY);
    if (NULL == hdl)
        return false;
    
    some_func = (int (*)(int))dlsym(hdl, funcname);
    if (NULL == some_func)
        return false;

    return true;
}

int
main(int argc, char *argv[])
{
    if (4 != argc) {
        fprintf(stderr,
                "Usage: %s <libname.so> <function> <arg>\n", argv[0]);
        return EXIT_SUCCESS;
    }

    if (init_library(argv[1], argv[2]))
        printf("%d\n", some_func(atoi(argv[3])));
    else
        fprintf(stderr, "Unknown function: %s\n", argv[2]);
    
    return EXIT_SUCCESS;
}
