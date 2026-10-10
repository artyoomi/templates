#include <iostream>
#include <vector>

template <typename T>
void merge(std::vector<T>& arr, int low, int mid, int high)
{
    int len_left = mid - low + 1;
    int len_right = high - mid;

    // create tmp vectors
    std::vector<T> L{len_left}, R{len_right};

    // copy data to tmp vectors
    for (int i = 0; i < len_left; ++i)
        L[i] = arr[low + i];
    for (int i = 0; i < len_right; ++i)
        R[i] = arr[mid + 1 + i];

    int i = 0;     // index inside i
    int j = 0;     // index inside j
    int k = low;   // index inside arr

    // merge
    while (i < len_left && j < len_right) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            ++i;
        } else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    // copy remaining elements in L
    while (i < len_left)
        arr[k++] = L[i++];

    while (j < len_right)
        arr[k++] = R[j++];
}

template <typename T>
void merge_sort(std::vector<int>& arr, int low, int high)
{
    if (low >= high)
        return;

    int mid = low + (high - low) / 2;
    merge_sort<T>(arr, low, mid);
    merge_sort<T>(arr, mid + 1, high);
    merge<T>(arr, low, mid, high);
}

template <typename T>
inline void print(const std::vector<int>& arr)
{
    for (size_t i {0}, size = arr.size(); i < size; ++i)
        std::cout << arr[i] << ((i == size - 1) ? ('\n') : (' '));
}

int main()
{
    int n; 
    std::cout << "Input count of elements: ";
    std::cin >> n;
    
    std::vector<int> arr;

    std::cout << "Input elements of array: ";
    int num;
    for (size_t i {0}; i < n; ++i) {
        std::cin >> num;
        arr.push_back(num);
    }

    std::cout << "Array before merge_sort:\n";
    print<int>(arr);

    merge_sort<int>(arr, 0, n - 1);
    
    std::cout << "Array after merge_sort:\n";
    print<int>(arr);

    return 0;
}

