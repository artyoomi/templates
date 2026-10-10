#ifndef _TO_POLISH_H_
#define _TO_POLISH_H_

#include <string>

bool is_binary(char operation);
int get_priority(char operation);
std::string to_polish(std::string expr);

#endif
