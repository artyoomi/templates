// prints count of processes with necessary name
// "genenv" in this case

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>

#define MAXFNAME_LEN 255
#define MAX_PATH_LEN 4096

enum {TRUE=1, FALSE=0};

// checks if string is number
int is_number(const char *str)
{
    size_t str_len = strlen(str);
    size_t numi = 0;
    int in_digit = FALSE;
    
    // skip white spaces
    for (; isspace(str[numi]); numi++)
        ;

    // check string
    for (; numi < str_len; numi++) {
        char c = str[numi];
        if (!isspace(c) && !isdigit(c))
            return FALSE;

        if (isspace(c))
            break;

        in_digit = TRUE;
    }

    // check on digits after number end
    for (; numi < str_len; numi++) {
        if (!isspace(str[numi]))
            return FALSE;
    }

    // check what there was at least one digit in string
    if (!in_digit)
        return FALSE;
    return TRUE;
}

int filter(const struct dirent* de)
{
    if (de->d_type != DT_DIR || !is_number(de->d_name))
        return 0;
    
    char* path = (char*)malloc(MAX_PATH_LEN * sizeof(char));
    snprintf(path, MAX_PATH_LEN, "/proc/%s/comm", de->d_name);

    FILE* fd = fopen(path, "r");
    if (!fd) {
        perror(path);
        exit(EXIT_FAILURE);
    }

    char fname[MAXFNAME_LEN + 1];
    fgets(fname, MAXFNAME_LEN + 1, fd);
    if (fname[strlen(fname) - 1] == '\n')
        fname[strlen(fname) - 1] = '\0';

    if (!strcmp(fname, "genenv"))
        return 1;
    else
        return 0;

    fclose(fd);
}

int main()
{
    struct dirent **namelist;
    int n;

    n = scandir("/proc", &namelist, filter, alphasort);
    if (n == -1) {
        perror("scandir");
        return EXIT_FAILURE;
    }

    printf("%d\n", n);

    // free memory 
    while (n--)
        free(namelist[n]);
    free(namelist);
    
    return EXIT_SUCCESS;
}
