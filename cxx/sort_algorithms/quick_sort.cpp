#include <iostream>
#include <vector>
#include <string>

int lomuto_partition(std::vector<std::string>& arr, int low, int high)
{
    std::string pivot = arr[high];   // choose last element as pivot
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (arr[j].size() <= pivot.size()) {
            ++i;

            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);

    return i + 1;
}

int hoare_partition(std::vector<std::string>& arr, int low, int high)
{
    std::string pivot = arr[low];
    int i = low - 1, j = high + 1;

    while (true) {
        do {
            ++i;
        } while (arr[i].size() < pivot.size());
        
        do {
            --j;
        } while (arr[j].size() > pivot.size());

        if (i >= j)
            return j;

        std::swap(arr[i], arr[j]);
    }
}

void quick_sort(std::vector<std::string>& arr, int low, int high)
{
    if (low < high) {
        // int pi = lomuto_partition(arr, low, high);
        int pi = hoare_partition(arr, low, high);

        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}

inline void print(const std::vector<std::string>& arr)
{
    for (size_t i {0}, size = arr.size(); i < size; ++i)
        std::cout << arr[i] << ((i == size - 1) ? ('\n') : (' '));
}

int main()
{
    std::vector<std::string> words;

    std::string word {};
    while (std::cin >> word)
        words.push_back(word);

    std::cout << "Sentence before qsort:\n";
    print(words);

    quick_sort(words, 0, words.size() - 1);
    
    std::cout << "Sentence after qsort:\n";
    print(words);
    return 0;
}
