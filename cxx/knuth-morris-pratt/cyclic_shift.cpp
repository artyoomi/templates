/*
 * Source: https://www.geeksforgeeks.org/kmp-algorithm-for-pattern-searching/
 */

#include <iostream>
#include <string>
#include <vector>

#include <cassert>

static size_t iter_count = 0;

/*
 * O(n) time complexity and O(n) memory complexity
 */
std::vector<int>
construct_lps(const std::string& pattern)
{
        // lps_array[0] = 0 by definition
        std::vector<int> lps_array(pattern.size(), 0);

        size_t prev_lps_len = 0;

        for (size_t i = 1; i < pattern.size(); ++i) {
                ++iter_count;
                if (pattern[i] == pattern[prev_lps_len]) {
                        ++prev_lps_len;
                        lps_array[i] = prev_lps_len;
                }

                else if (pattern[i] != pattern[prev_lps_len] &&
                         prev_lps_len == 0) {                        
                        lps_array[i] = 0;
                }
                
                else if (pattern[i] != pattern[prev_lps_len] &&
                         prev_lps_len > 0) {

                        // While match was not found
                        while (prev_lps_len > 0 &&
                               pattern[i] != pattern[prev_lps_len]) {
                                ++iter_count;
                                prev_lps_len = lps_array[prev_lps_len - 1];
                        }

                        // If we found match, prev_lps_len must be incremented
                        if (pattern[i] == pattern[prev_lps_len])
                                ++prev_lps_len;
                        lps_array[i] = prev_lps_len;
                }
                
                else {
                        std::cerr << "Check again your conditions, exiting...\n";
                        exit(EXIT_FAILURE);
                }
        }

        return lps_array;
}

/*
 * Modified Knuth-Morris-Pratt algorithm to find if A is cyclic shift of B
 * O(n + m) time complexity
 */
int cyclic_shift(const std::string& A,
                 const std::string& B)
{
        assert(A.size() == B.size() &&
               "Error: A size must be equal B size");

        if (A.size() == 0) return -1;

        std::vector<int> lps_array = construct_lps(B);

        size_t A_i = 0, B_i = 0;

        size_t virt_idx = 0;
        while (virt_idx < 2 * A.size() - 1) {
                ++iter_count;
                A_i = virt_idx % A.size();

                // If characters match -- just go further
                if (A[A_i] == B[B_i]) {
                        ++B_i;
                        ++virt_idx;

                        if (B_i == B.size()) return (virt_idx - B_i) % A.size();
                        else continue;
                }

                /*
                 * If characters does not match and current position in B
                 * is 0 -- go further in A
                 */
                if (B_i == 0 && B[B_i] != A[A_i]) {
                        ++virt_idx;
                        continue;
                }

                /*
                 * If characters does not match, but current position in B
                 * is not 0 -- try to go back in LPS array
                 */
                if (B_i > 0 && B[B_i] != A[A_i]) {
                        B_i = lps_array[B_i - 1];
                        continue;
                }
        }

        return -1;
}

int
main(void)
{
        std::string A, B;
        std::cin >> A >> B;

        // Just print -1 if pattern size greater than text size
        if (A.size() != B.size()) {
                puts("-1");
                return 0;
        }

        std::cout << cyclic_shift(A, B) << '\n';
        printf("Theoretical iter count [%ld]\nExperimental iter count [%ld]\n",
               A.size() + B.size(), iter_count);
       
        return 0;
}
