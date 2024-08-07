#include <ctype.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

// checks if string is number
int is_number(char *str)
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
