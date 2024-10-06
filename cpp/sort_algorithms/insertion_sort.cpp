#include <iostream>
#include <vector>

// void print_vector(std::vector<int> nums)
// {
//     for (size_t i {0}, size = nums.size(); i < size; ++i)
//         std::cout << nums[i] << ((i == size - 1) ? ('\n') :(' '));
// }

void insertion_sort(std::vector<int>& arr)
{
    for (size_t i {1}, size = arr.size(); i < size; ++i) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && key < arr[j]) {
            arr[j + 1] = arr[j];
            --j;
        }

        arr[j + 1] = key;
    }
}

// int main(int argc, char *argv[])
// {
//     std::vector<int> nums;

//     int num;
//     while (std::cin >> num)
//         nums.push_back(num);

//     std::cout << "Nums before insertion sort:\n";
//     print_vector(nums);

//     insertion_sort(nums);
    
//     std::cout << "Nums before insertion sort:\n";
//     print_vector(nums);
    
//     return 0;
// }
