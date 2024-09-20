#include <iostream>

#include "../stack.h"

bool
is_pair(const char left, const char right)
{
    if ('(' == left && ')' == right)
        return true;
    else if ('{' == left && '}' == right)
        return true;
    else if ('[' == left && ']' == right)
        return true;
    return false;
}

size_t
brackets_check(std::string code)
{
    Stack<char> *stack = new Stack<char>();
    size_t last_position {};

    size_t i;
    for (i = 0; i < code.size(); ++i) {
        if ('(' == code[i] || '[' == code[i] || '{' == code[i]) {
            if (stack->is_empty())
                last_position = i;
            
            stack->push(code[i]);
        }
        else if (')' == code[i] || ']' == code[i] || '}' == code[i]) {
            if (stack->is_empty() || !is_pair(stack->get_top(), code[i]))
                return i + 1;
            else if (is_pair(stack->get_top(), code[i]))
                stack->pop();
        }
    }

    if (stack->is_empty())
        return 0;
    else {
        delete stack;
        stack = nullptr;
        
        return last_position;
    }
}

int
main(int argc, char *argv[])
{
    std::string code;
    std::cin >> code;

    std::size_t ret = brackets_check(code);
    if (0 == ret)
        std::cout << "Success" << std::endl;
    else
        std::cout << ret << std::endl;

    return 0;
}
