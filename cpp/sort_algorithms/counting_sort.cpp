// only for positive numbers

#include <iostream>
#include <vector>

int find_max(const std::vector<int>& nums)
{
    if (nums.size() == 0)
        return -1;
    
    int max_element {nums[0]};
    
    for (size_t i {0}, size = nums.size(); i < size; ++i)
        max_element = std::max(max_element, nums[i]);

    return max_element;       
}

std::vector<int> counting_sort(const std::vector<int>& nums)
{
    // create array of element counts
    std::vector<int> counts(find_max(nums) + 1, 0);
    for (size_t i {0}, size = nums.size(); i < size; ++i)
        counts[nums[i]]++;

    // variant with cumulative array
    // // create cumulative array
    // std::vector<int> cumulative_counts(counts.size(), 0);
    // cumulative_counts[0] = counts[0];
    // for (size_t i {1}, size = counts.size(); i < size; ++i)
    //     cumulative_counts[i] = cumulative_counts[i - 1] + counts[i];

    // // fill output array
    // std::vector<int> sorted_nums(nums.size(), 0);
    // for (int i = nums.size() - 1; i >= 0; --i)
    //     sorted_nums[--cumulative_counts[nums[i]]] = nums[i];

    // variant without cumulative array
    std::vector<int> sorted_nums(nums.size(), 0);
    int index {0};
    for (int i {0}, size = counts.size(); i < size; ++i) {
        while (counts[i] != 0) {
            sorted_nums[index++] = i;
            --counts[i];
        }
    }

    return sorted_nums;
}

// int main(int argc, char *argv[])
// {
//     std::vector<int> nums {};

//     int num {};
//     while(std::cin >> num)
//         nums.push_back(num);

//     std::vector<int> sorted_nums = counting_sort(nums);
//     for (size_t i {0}, size = sorted_nums.size(); i < size; ++i)
//         std::cout << sorted_nums[i] << ' ';
//     std::cout << '\n';
        
//     return 0;
// }
