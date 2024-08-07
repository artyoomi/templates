#include <stdio.h>

// powers n to decimal degree m
double power(double n, int m)
{
    double result = 1;

    // calculations are not necessary when n equal to 1
    if ((n - 1 <= 0.0001 && n > 0) || (1 - n <= 0.0001 && n < 0))
        result = 1;
    // degree equal to 0
    else if (m == 0) {
        if ((n - 0 <= 0.0001 && n > 0) || (0 - n <= 0.0001 && n < 0))
            result = 0;
        else
            result = 1;
    // degree greater than 0
    } else if (m > 0) {
        for (int i = 0; i < m; i++) {
            result *= n;    
        }
    }
    // defree less than 0
    else {
        m = -m;
        for (int i = 0; i < m; i++) {
            result /= n;
        }
    }
    
    return result;
}
