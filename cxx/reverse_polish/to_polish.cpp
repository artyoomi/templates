#include "./to_polish.h"

#include <string>

#include "../stack.h"

bool is_binary(char operation)
{
    if ('+' == operation || '-' == operation ||
        '*' == operation || '/' == operation ||
        '^' == operation)
        return true;
    return false;
    
}

int get_priority(char operation)
{
    const int highest_priority = 100;

    if ('(' == operation || ')' == operation)
        return highest_priority;
    if ('^' == operation)
        return highest_priority - 1;
    if ('*' == operation || '/' == operation)
        return highest_priority - 2;
    if ('+' == operation || '-' == operation)
        return highest_priority - 3;

    return 0;
}

// can proceed only numbers, +, -, *, /, ^, (, )
// else program will work incorrect
std::string to_polish(std::string expr)
{
    std::string polish {};

    Stack<char> stack;

    std::string num = "";
    for (size_t i {0}, size = expr.size(); i < size; ++i) {
        if (expr[i] == ' ') {
            if (num != "") {
                polish += num;
                polish += ' ';
                num = "";
            }
            
            continue;
        }
        
        // if we work with ascii table
        if (expr[i] - '0' >= 0 && expr[i] - '0' <= 9)
            num += expr[i];
            
        else if ('(' == expr[i])
            stack.push(expr[i]);

        else if (')' == expr[i]) {
            if (!stack.is_empty()) {
                while (stack.get_top() != '(') {
                    polish += stack.pop();
                    polish += ' ';

                    // the stack ended before the opening parenthesis was found
                    if (stack.is_empty()) {
                        std::cerr << "Error: incorrect parenthesis!\n";
                        exit(EXIT_FAILURE);
                    }
                }
                
                stack.pop();
            } else {
                std::cerr << "Error: incorrect parenthesis!\n";
                exit(EXIT_FAILURE);
            }

        }

        else if (is_binary(expr[i])) {
            while (!stack.is_empty() &&
                   // is_binary(stack.get_top()) &&
                   get_priority(expr[i]) <= get_priority(stack.get_top())) {
                polish += stack.pop();
                polish += ' ';
            }

            stack.push(expr[i]);
        }
    }

    if (num != "") {
        polish += num;
        polish += ' ';
    }

    while (!stack.is_empty()) {
        polish += stack.pop();

        if (!stack.is_empty())
            polish += ' ';
    }

    size_t size {polish.size()};

    if (size > 0 && polish[size - 1] == ' ')
        polish.erase(size - 1);

    return polish;
}
