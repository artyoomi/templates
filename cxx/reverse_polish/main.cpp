#include <iostream>

#include "./to_polish.h"

int main()
{
    std::string expr {};
    getline(std::cin, expr);
    
    std::cout << to_polish(expr) << '\n';

    return 0;
}
