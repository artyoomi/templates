#include <iostream>

template <typename T>
inline void print(const T *arr, std::size_t size)
{
    for (std::size_t i = 0; i < size; ++i)
        std::cout << arr[i] << ((i == size - 1) ? ('\n') : (' '));
}
