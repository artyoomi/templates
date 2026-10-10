/*
 * Source: https://www.geeksforgeeks.org/kmp-algorithm-for-pattern-searching/
 */

#include <iostream>
#include <string>
#include <vector>

#include <cassert>

#define DISJOINT

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
 * O(n + m) time complexity
 */
std::vector<int> knuth_morris_pratt(const std::string& pattern,
                                    const std::string& text)
{
        assert(pattern.size() <= text.size() &&
               "Pattern size greater than text size");

        if (pattern.size() == 0) return std::vector<int>();
        
        std::vector<int> lps_array = construct_lps(pattern);
        std::vector<int> occ; // Occurences vector

        size_t pattern_i = 0, text_i = 0;
        while (text_i < text.size()) {
                if (pattern_i > 0 && text[text_i] != pattern[pattern_i]) {
                        pattern_i = lps_array[pattern_i - 1];
                        continue;
                }

                if (pattern_i == 0 && text[text_i] != pattern[pattern_i]) {
                        ++text_i;
                        continue;
                }

                if (pattern_i == pattern.size() - 1) {
                        occ.push_back(text_i - pattern.size() + 1);

#ifdef DISJOINT
                        pattern_i = 0;
                        ++text_i;
#else
                        /*
                         * If pattern_i is 0, than either pattern matching
                         * doesn't started or pattern has length 1.
                         * In both cases text_i must be incremented.
                         */
                        if (pattern_i > 0) pattern_i = lps_array[pattern_i - 1];
                        else ++text_i;
#endif
                        
                        continue;
                }
                
                ++pattern_i;
                ++text_i;
        }
        
        return occ;
}

int
main(void)
{
        std::string pattern, text;
        std::cin >> pattern >> text;

        // Just print -1 if pattern size greater than text size
        if (pattern.size() > text.size()) {
                puts("-1");
                return 0;
        }

        std::vector<int> occ_array = knuth_morris_pratt(pattern, text);
        for (size_t i = 0; i < occ_array.size(); ++i) {
                std::cout << occ_array[i];
                if (i < occ_array.size() - 1) putchar(',');
                else putchar('\n');
        }

        // No match found, so we print -1
        if (occ_array.size() == 0) puts("-1");

        return 0;
}
