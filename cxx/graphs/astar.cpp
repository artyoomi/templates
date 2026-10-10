// The greedy search method in oriented graph with a..z in nodes

#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>

#include <sys/param.h>

#define ALPH_SIZE 26

struct c_node {
        char c;
        double g;
        double f;
        std::string path;

        bool operator<(const c_node& other) const
        {
                return this->f > other.f;
        }
};

// A* heuristic
double heuristic(char first, char second)
{
        return abs(first - second);
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

        // Resulting path
        std::string path = "";

        std::priority_queue<c_node> open_set;
        open_set.push({start, 0, heuristic(start, finish), {start}});

        std::unordered_map<char, double> closed_set;

        // Find best path from start to finish
        while (!open_set.empty()) {
                c_node current = open_set.top();
                open_set.pop();

                // Best path was found
                if (current.c == finish) {
                        path = current.path;
                        break;
                }

                for (char c = 'a'; c - 'a' < adj_matrix_size; ++c) {
                        if (adj_matrix[current.c - 'a'][c - 'a'] == 0)
                                continue;

                        double g = current.g + adj_matrix[current.c - 'a'][c - 'a'];
                        double f = g + heuristic(c, finish);

                        if (closed_set.find(c) == closed_set.end() || g < closed_set[c]) {
                                closed_set[c] = g;
                                open_set.push({c, g, f, current.path + c});
                        }
                }
        }

        if (!path.empty())
                std::cout << path << '\n';
        else
                throw std::runtime_error("Path was not found!");
        
        return 0;
}
