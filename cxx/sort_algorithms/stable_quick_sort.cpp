#include <iostream>
#include <vector>

std::vector<int> stable_qst(std::vector<int>& arr)
{
    if (arr.size() <= 1)
        return arr;

    int mid = arr.size() / 2;
    int pivot = arr[mid];

    std::vector<int> smaller;
    std::vector<int> greater;

    for (int i = 0; i < arr.size(); ++i) {
        int val = arr[i];
        if (i != mid) {
            if (val < pivot)
                smaller.push_back(val);
            else if (val > pivot)
                greater.push_back(val);
            else {
                if (i < mid)
                    smaller.push_back(val);
                else
                    greater.push_back(val);
            }
        }
    }

    std::vector<int> sorted_arr;

    std::vector<int> sorted_left_side = stable_qst(smaller);
    std::vector<int> sorted_right_side = stable_qst(greater);

    for (const int& val1 : sorted_left_side)
        sorted_arr.push_back(val1);

    sorted_arr.push_back(pivot);
    
    for (const int& val2 : sorted_right_side)
        sorted_arr.push_back(val2);

    return sorted_arr;
}

inline void print(const std::vector<int>& arr)
{
    for (size_t i {0}, size = arr.size(); i < size; ++i)
        std::cout << arr[i] << ((i == size - 1) ? ('\n') : (' '));
}

int main()
{
    std::vector<int> nums;

    int num;
    while (std::cin >> num)
        nums.push_back(num);

    print(stable_qst(nums));

    return 0;
}
