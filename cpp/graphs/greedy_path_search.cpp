// The greedy search method in oriented graph with a..z in nodes

#include <iostream>
#include <vector>
#include <stack>
#include <climits>

#include <sys/param.h>

#define ALPH_SIZE 26

// Print the square matrix to stdout
void print_matrix(double* matrix, size_t n)
{
        for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < n; ++j)
                        std:: cout << matrix[i * ALPH_SIZE + j] << ' ';
                std::cout << '\n';
        }
}

int main(void)
{
        // Get start and finish nodes
        char start, finish;
        std::cin >> start >> finish;
        if (start  - 'a' < 0 || start  - 'a' >= ALPH_SIZE ||
            finish - 'a' < 0 || finish - 'a' >= ALPH_SIZE)
                throw std::runtime_error("Incorrect input, characters must be from a to z");

        // Input adjacent matrix for given graph
        double adj_matrix[ALPH_SIZE][ALPH_SIZE] = {0};
        size_t adj_matrix_size = 0;
        char from = '\0', to = '\0';
        double weight = 0;
        while (std::cin >> from >> to >> weight) {
                if (from - 'a' < 0 || from - 'a' >= ALPH_SIZE ||
                      to - 'a' < 0 ||   to - 'a' >= ALPH_SIZE) {
                        std::cout << "Incorrect input, characters must be from a to z. Try again!\n";
                        continue;
                }
                
                adj_matrix[from - 'a'][to - 'a'] = weight;
                adj_matrix_size = MAX(adj_matrix_size, MAX(from - 'a' + 1, to - 'a' + 1));
        }

        // To store current in-graph path
        std::stack<char> path;
        path.push(start);

        // Mark visited nodes
        std::vector<bool> visited(ALPH_SIZE, false);
        visited[start - 'a'] = true;

        // Find path in graph
        char current = start;
        while (current != finish) {
                double  min_weight = INT_MAX;
                char min_c = '\0';
                for (char c = 'a'; c - 'a' < adj_matrix_size; ++c) {
                        if (adj_matrix[current - 'a'][c - 'a'] != 0 &&
                            adj_matrix[current - 'a'][c - 'a'] < min_weight &&
                            !visited[c - 'a']) {
                                min_c = c;
                                min_weight = adj_matrix[current - 'a'][c - 'a'];
                        }
                }

                if (min_weight == INT_MAX) {
                        if (path.empty())
                                throw std::runtime_error("No path found!");

                        path.pop();

                        if (path.empty())
                                throw std::runtime_error("No path found!");

                        // Get previous node from stack
                        current = path.top();
                } else {
                        current = min_c;
                        path.push(current);
                        visited[current - 'a'] = true;
                }
        }

        // Form output string & print it
        std::string path_str(path.size(), '\0');
        int position = path_str.size() - 1;
        while (!path.empty()) {
                path_str[position--] = path.top();
                path.pop();
        }

        std::cout << path_str << '\n';
        
        return 0;
}
