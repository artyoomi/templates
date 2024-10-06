#include <iostream>
#include <vector>

// first arr1 must have space to add arr2
// -> arr1 - array with length m + n, last n elements equal to zero
// -> arr2 - array with length n
void merge_sorted_arrays(std::vector<int>& arr1, const std::vector<int>& arr2, int m, int n)
{
    int j = 0;    // position in arr2

    for (int i {0}; i < m + n && j < n; ++i) {
        // insert element if true
        if (arr1[i] >= arr2[j] || i >= m + j) {
            // move elements in one position right
            for (int k {m + j - 1}; k >= i; --k)
                arr1[k + 1] = arr1[k];
            
            arr1[i] = arr2[j];
            ++j;
        }
    }
}


void print(std::vector<int> arr)
{
    for (size_t i {0}, size = arr.size(); i < size; ++i)
        std::cout << arr[i] << ((i == size - 1) ? ('\n') : (' '));
}

int main()
{
    int m, n;
    std::cin >> m >> n;

    std::vector<int> arr1(m + n);
    std::vector<int> arr2(n);

    int num {0};
    for (size_t i {0}; i < m + n; ++i) {
        std::cin >> num;
        arr1[i] = num;        
    }
    for (size_t i {0}; i < n; ++i) {
        std::cin >> num;
        arr2[i] = num;        
    }

    merge_sorted_arrays(arr1, arr2, m, n);
    print(arr1);
}
