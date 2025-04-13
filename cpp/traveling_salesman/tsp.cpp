#include <iostream>
#include <vector>
#include <exception>
#include <algorithm>
#include <iomanip>
#include <unordered_set>
#include <queue>
#include <sstream>

#include <climits>

// To add some strange staff
#define MOEVM_STYLE

#define INF INT_MAX

/*
 * Square sized table
 */
class STable {
public:
    // Constructors
    STable() = default;
    STable(const std::vector<uint32_t>& data, size_t n)
        : _matrix(data), _row_labels(n), _col_labels(n)
    {
        if (data.size() != n * n)
            throw std::invalid_argument("data has incorrect size");
        
        for (int i = 0; i < n; ++i) { this->_row_labels[i] = i; }
        for (int i = 0; i < n; ++i) { this->_col_labels[i] = i; }
    }

    // Getters
    size_t order() const
    {
        if (this->_rows_count() != this->_cols_count())
            throw std::runtime_error("order(): rows count != cols_count");

        return this->_rows_count();
    }

    // Operators
    uint32_t& operator[](size_t index)
    {
        if (index >= this->_row_labels.size() * this->_col_labels.size())
            throw std::out_of_range("operator[]: Index out of range");

        return this->_matrix[index];
    }

    uint32_t operator[](size_t index) const
    {
        if (index >= this->_row_labels.size() * this->_col_labels.size())
            throw std::out_of_range("operator[]: Index out of range");

        return this->_matrix[index];
    }

    // void delete_rc(int row_label, int col_label)
    // {
    //     _delete_row(row_label);
    //     _delete_col(col_label);
    // }

    // Print matrix with row-column labels
    friend std::ostream& operator<<(std::ostream& os, const STable& matrix)
    {
        if (matrix._matrix.size() == 0) return os;
        
        const size_t field = 8;
            
        size_t n = matrix._row_labels.size();
        size_t m = matrix._col_labels.size();

        // Print column labels
        os << std::setw(field) << "";
        for (size_t i = 0; i < m; ++i)
            os << std::setw(field) << matrix._col_labels[i];
        os <<'\n';

        // For each row print row label and row values
        for (size_t i = 0; i < n; ++i) {
            os << std::setw(field) << matrix._row_labels[i];
            for (size_t j = 0; j < m; ++j) {
                if (matrix._matrix[i * m + j] == INF) os << std::setw(field) << "INF";
                else os << std::setw(field) << matrix._matrix[i * m + j];
            }
            os << '\n';
        }
        
        return os;
    }

// PRIVATE METHODS
private:
    // void _delete_row(int label)
    // {
    //     // Initial size check
    //     if (this->_row_labels.size() == 0)
    //         throw std::invalid_argument("Row labels is empty");

    //     // Find row iterator
    //     auto it = std::find(this->_row_labels.begin(), this->_row_labels.end(), label);
    //     if (it == this->_row_labels.end())
    //         throw std::invalid_argument("Label was not found");

    //     // Delete row from adjacency matrix
    //     size_t row_begin = (it - this->_row_labels.begin()) * this->_row_labels.size();
    //     size_t row_end = row_begin + this->_col_labels.size();
    //     _matrix.erase(this->_matrix.begin() + row_begin, this->_matrix.begin() + row_end);

    //     // Delete label from row labels
    //     this->_row_labels.erase(it);
    // }

    // void _delete_col(int label)
    // {
    //     // Initial size check
    //     if (this->_col_labels.size() == 0)
    //       throw std::invalid_argument("Column labels is empty");

    //     // Find col iterator
    //     auto it = std::find(this->_col_labels.begin(), this->_col_labels.end(), label);
    //     if (it == this->_col_labels.end())
    //         throw std::invalid_argument("Label was not found");

    //     // Delete col from adjacency matrix
    //     size_t col_begin = it - this->_col_labels.begin();
    //     size_t col_end = this->_row_labels.size() * this->_col_labels.size() - 1 - (this->_col_labels.size() - col_begin - 1) + 1;
    //     for (size_t i = 0; i < this->_row_labels.size(); ++i)
    //         this->_matrix.erase(this->_matrix.begin() + col_begin + i * (this->_col_labels.size() - 1));

    //     // Delete label from col labels
    //     this->_col_labels.erase(it);
    // }
    
    size_t _rows_count() const { return this->_row_labels.size(); }
    size_t _cols_count() const { return this->_col_labels.size(); }
    
// PRIVATE FIELDS
private:
    std::vector<int> _row_labels;
    std::vector<int> _col_labels;

    std::vector<uint32_t> _matrix;
};

class TSP_solver {
public:
    struct Node {
        uint64_t lower_bound;
        STable table;
        std::vector<int> path;
        std::unordered_set<int> visited;

        Node(uint64_t lb, const STable& tbl, const std::vector<int>& p, const std::unordered_set<int>& v)
            : lower_bound(lb), table(tbl), path(p), visited(v) {}
    };

    /*
     * Source of all explanations: https://galyautdinov.ru/post/zadacha-kommivoyazhera
     */
    void solve(STable stable)
    {
        size_t loop_count = 0;
    
        STable table = stable;
        
        // Clear previous solution
        this->_best_cost = INF;
        this->_best_path.clear();

        // Root fields
        uint64_t root_bound = this->_row_reduction(table) +
                              this->_col_reduction(table);
        std::vector<int> root_path = {0};
        std::unordered_set<int> root_visited = {0};

        if (table.order() < 2) {
            throw std::invalid_argument("Can solve only n>=2 adjacency matrix");
        // Hardcode, heh :p
        } else if (table.order() == 2) {
            this->_best_cost = root_bound;
            this->_best_path = {0, 1, 0};
            return;
        }

        // Create priority queue to store Node's
        auto cmp = [](const Node* a, const Node* b) { return a->lower_bound > b->lower_bound; };
        std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> pq(cmp);

        pq.push(new Node(root_bound, table, root_path, root_visited));

        while (!pq.empty()) {
            ++loop_count;
        
            // Get element with minimal lower_bound
            Node* current = pq.top();
            pq.pop();

            // Pruning irrelevant branches
#ifdef MOEVM_STYLE
            if (current->lower_bound > this->_best_cost) {
#else
            if (current->lower_bound >= this->_best_cost) {
#endif
                delete current;
                continue;
            }

            // Save solution if it is found
            if (current->path.size() == current->table.order()) {
                uint32_t last = current->path.back();
                uint32_t cost_back = current->table[last * current->table.order() + 0];
                if (cost_back != INF) {
                    uint64_t total = current->lower_bound + cost_back;
                    if (total <= this->_best_cost) {
                        this->_best_cost = total;
                        this->_best_path = current->path;
                        this->_best_path.push_back(0);

#ifdef MOEVM_STYLE
                        this->_results.push_back(current->path);
#endif
                    }
                }
                delete current;
                continue;
            }

            int current_city = current->path.back();
            auto [i, j] = this->_max_zero_score(current->table,
                                                current_city,
                                                current->visited);

            // No zero values
            if (i == -1) {
                delete current;
                continue;
            }

            // Branching include (i, j)
            STable include_table = current->table;
            // Mark i-row and j-col with INF
            for (size_t k = 0; k < include_table.order(); ++k) {
                if (k != j) include_table[current_city * include_table.order() + k] = INF;
                if (k != current_city) include_table[k * include_table.order() + j] = INF;
            }
            // Forbid the return journey
            include_table[j * include_table.order() + current_city] = INF;

            uint64_t include_bound = current->lower_bound 
                + this->_row_reduction(include_table) 
                + this->_col_reduction(include_table);
            std::vector<int> include_path = current->path;
            include_path.push_back(j);
            std::unordered_set<int> include_visited = current->visited;
            include_visited.insert(j);

#ifdef MOEVM_STYLE            
            if (include_bound <= this->_best_cost)
#else
            if (include_bound < this->_best_cost)
#endif
                pq.push(new Node(include_bound, include_table, include_path, include_visited));

            // Branching exclude (i, j)
            STable exclude_table = current->table;
            exclude_table[current_city * exclude_table.order() + j] = INF;
            uint64_t exclude_bound = current->lower_bound 
                + this->_row_reduction(exclude_table) 
                + this->_col_reduction(exclude_table);

#ifdef MOEVM_STYLE
            if (exclude_bound <= this->_best_cost) {
#else
            if (exclude_bound < this->_best_cost) {
#endif
                pq.push(new Node(exclude_bound, exclude_table, current->path, current->visited));
            }

            delete current;
        }

#ifdef MOEVM_STYLE
        auto vec_to_str = [](const std::vector<int>& vec)
        {
            std::stringstream ss;
            for (size_t i = 0; i < vec.size(); ++i) {
                ss << vec[i] << ((i == vec.size() - 1) ? ('\n') : (' '));
            }
            return ss.str();
        };
   
        std::sort(_results.begin(), _results.end(), [vec_to_str](const std::vector<int>& a, const std::vector<int>& b) { return vec_to_str(a) < vec_to_str(b); });

        this->_best_path = _results[0];
#endif
        std::cout << loop_count << '\n';
    }

    // Getters
    std::vector<int> best_path() { return this->_best_path; }
    uint64_t best_cost() { return this->_best_cost; }
    
private: // PRIVATE METHODS
    uint64_t _row_reduction(STable& table)
    {
        uint64_t sum = 0;
        size_t n = table.order();
        for (size_t i = 0; i < n; ++i) {
            uint32_t min_val = INF;
            for (size_t j = 0; j < n; ++j) {
                if (i != j) min_val = std::min(min_val, table[i * n + j]);
            }
            if (min_val != INF) {
                for (size_t j = 0; j < n; ++j) {
                    if (i != j && table[i*n + j] != INF) 
                        table[i*n + j] -= min_val;
                }
                sum += min_val;
            }
        }
        return sum;
    }

    uint64_t _col_reduction(STable& table)
    {
        uint64_t sum = 0;
        size_t n = table.order();
        for (size_t j = 0; j < n; ++j) {
            uint32_t min_val = INF;
            for (size_t i = 0; i < n; ++i) {
                if (i != j) min_val = std::min(min_val, table[i*n + j]);
            }
            if (min_val != INF) {
                for (size_t i = 0; i < n; ++i) {
                    if (i != j && table[i*n + j] != INF) 
                        table[i*n + j] -= min_val;
                }
                sum += min_val;
            }
        }
        return sum;
    }

    std::pair<int32_t, int32_t> _max_zero_score(const STable& table, int current_city, 
                                                const std::unordered_set<int>& visited)
    {
        int64_t max_score = -1;
        std::pair<int, int> target = {-1, -1};
        size_t n = table.order();

        // Search in rows
        for (size_t j = 0; j < n; ++j) {
            if (j == current_city || visited.count(j) || table[current_city*n + j] != 0)
                continue;

            uint32_t row_min = INF;
            for (size_t k = 0; k < n; ++k) {
                if (k != j && table[current_city*n + k] != INF)
                    row_min = std::min(row_min, table[current_city*n + k]);
            }

            uint32_t col_min = INF;
            for (size_t k = 0; k < n; ++k) {
                if (k != current_city && table[k*n + j] != INF)
                    col_min = std::min(col_min, table[k*n + j]);
            }

            int64_t score = (row_min == INF ? 0 : row_min) + (col_min == INF ? 0 : col_min);
            if (score > max_score) {
                max_score = score;
                target = {current_city, j};
            }
        }

        return target;
    }

private: // PRIVATE FIELDS
    uint64_t _best_cost;
    std::vector<int> _best_path;

#ifdef MOEVM_STYLE
    std::vector<std::vector<int>> _results;
#endif
};


int main(void)
{
        int n;
        std::cin >> n;

        // Fill adjacency matrix
        int64_t buf;
        std::vector<uint32_t> graph(n * n);
        for (size_t i = 0; i < n * n; ++i) {
                std::cin >> buf;
                // Replace -1 with INF
                if (buf == -1) graph[i] = INF;
                else graph[i] = buf;
        }

        // Solve and print result
        STable table(graph, n);
        TSP_solver solver;
        solver.solve(table);

        std::vector<int> best_path = solver.best_path();
        for (size_t i = 0; i < table.order(); ++i)
                std::cout << best_path[i] << ((i == table.order() - 1) ? ('\n') : (' '));
        printf("%.1lf\n", (double)solver.best_cost());

        return 0;
}
