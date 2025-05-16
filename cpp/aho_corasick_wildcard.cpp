/*
 * Find all occurrences of strings from patterns set in text
 */

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <algorithm>
#include <queue>

class FSM {
// PUBLIC FIELDS AND STRUCTURE DEFINITIONS
public:
    struct FSM_vertex {
        std::vector<FSM_vertex*> children;  // Children of vertex
        std::vector<int> pattern_indices;   // Indices of patterns ending here
        int parent_c;                       // Symbol on edge from vertex to it's parent
        FSM_vertex* parent;                 // Pointer to parent of vertex
        FSM_vertex* suff_link;              // Suffix link

        FSM_vertex(size_t alphabet_size)
        : children(alphabet_size, nullptr),
          parent_c(-1),
          parent(nullptr),
          suff_link(nullptr)
        {}
    };
// INTERFACE
public:
    FSM(const std::vector<std::string>& patterns,
        const std::unordered_map<char, size_t>& alphabet)
        : _alphabet(alphabet), _pattern_lengths(patterns.size()) 
    {
        // Build trie DS and store pattern lengths
        for (size_t i = 0; i < patterns.size(); ++i) {
            _add_string(patterns[i], i);
            _pattern_lengths[i] = patterns[i].size();
        }

        // Convert trie to FSM
        this->_build_fsm();
    }

    std::vector<int>
    input(char c)
    {
        std::vector<int> found;

        // Ignore characters not from the alphabet
        if (!_alphabet.count(c)) {
            _carriage = _root;
            return found;
        }

        size_t idx = _alphabet.at(c);
        
        // Traverse until valid transition or root
        while (true) {
            if (_carriage->children[idx]) {
                _carriage = _carriage->children[idx];
                break;
            }
            if (_carriage == _root) break;
            _carriage = _carriage->suff_link;
        }

        // Collect all pattern indices via suffix links
        FSM_vertex* temp = _carriage;
        while (temp != _root) {
            for (int pat_idx : temp->pattern_indices) {
                found.push_back(pat_idx);
            }
            temp = temp->suff_link;
        }

        return found;
    }

    int
    get_pattern_length(int pattern_idx) const
    {
        return _pattern_lengths[pattern_idx];
    }

    ~FSM()
    {
        _delete_vertex(_root, _alphabet.size());
    }

// PRIVATE METHODS
private:
    void
    _add_string(const std::string& s, int pattern_idx) {
        // Skip empty strings
        if (s.empty()) return;

        // Create root vertex firstly (if not exists)
        if (!_root)
            _root = new FSM_vertex(_alphabet.size());

        FSM_vertex* curr = _root;
        for (char c : s) {
            if (!_alphabet.count(c)) {
                throw std::runtime_error("Unsupported character in pattern");
            }

            // Get index of symbol and create new vertex or go through existent
            size_t idx = _alphabet.at(c);
            if (!curr->children[idx]) {
                curr->children[idx] = new FSM_vertex(_alphabet.size());
                curr->children[idx]->parent = curr;
                curr->children[idx]->parent_c = idx;
            }
            curr = curr->children[idx];
        }
        // if (curr->pattern_indices.empty())
        curr->pattern_indices.push_back(pattern_idx);
    }

    void
    _build_fsm()
    {
        if (!_root) return;

        // Initialize suffix links using BFS
        std::queue<FSM_vertex*> q;
        _root->suff_link = _root;

        // Enqueue all children of root
        for (size_t c = 0; c < _alphabet.size(); ++c) {
            if (_root->children[c]) {
                _root->children[c]->suff_link = _root;
                q.push(_root->children[c]);
            }
        }

        // Create suffix link for each vertex
        while (!q.empty()) {
            FSM_vertex* curr = q.front();
            q.pop();

            for (size_t c = 0; c < _alphabet.size(); ++c) {
                FSM_vertex* child = curr->children[c];
                if (!child) continue;

                FSM_vertex* temp = curr->suff_link;
                while (true) {
                    if (temp->children[c]) {
                        child->suff_link = temp->children[c];
                        break;
                    }
                    if (temp == _root) {
                        child->suff_link = _root;
                        break;
                    }
                    temp = temp->suff_link;
                }
                q.push(child);
            }
        }

        _carriage = _root;
    }

    static void
    _delete_vertex(FSM_vertex* v, size_t alphabet_size) {
        if (!v) return;
        for (size_t i = 0; i < alphabet_size; ++i)
            _delete_vertex(v->children[i], alphabet_size);
        delete v;
        v = nullptr;
    }
// PRIVATE FIELDS
private:
    FSM_vertex* _root = nullptr;
    FSM_vertex* _carriage = nullptr;
    std::vector<int> _pattern_lengths;
    const std::unordered_map<char, size_t> _alphabet;
};

struct Subpattern {
    std::string str;
    int pos_in_p;
};

std::vector<Subpattern>
split_into_subpatterns(const std::string& p, char wildcard) {
    std::vector<Subpattern> subpatterns;
    std::string current_str;
    int current_start = -1;

    for (int i = 0; i < p.size(); ++i) {
        char c = p[i];
        if (c != wildcard) {
            if (current_str.empty()) {
                current_start = i;
            }
            current_str += c;
        } else {
            if (!current_str.empty()) {
                subpatterns.push_back({current_str, current_start});
                current_str.clear();
                current_start = -1;
            }
        }
    }

    if (!current_str.empty()) {
        subpatterns.push_back({current_str, current_start});
    }

    return subpatterns;
}
class Solver {
public:
    std::vector<int> solve(const std::string& text,
                           const std::vector<Subpattern>& subpatterns,
                           int len_P,
                           const std::unordered_map<char, size_t>& alphabet) {
        std::vector<int> result;

        if (subpatterns.empty()) {
            return result;
        }

        std::vector<std::string> patterns;
        for (const auto& sub : subpatterns) {
            patterns.push_back(sub.str);
        }

        FSM fsm(patterns, alphabet);

        int len_T = text.size();
        std::vector<int> count(len_T, 0);
        int k = subpatterns.size();

        for (size_t i = 0; i < text.size(); ++i) {
            std::vector<int> found = fsm.input(text[i]);
            for (int pat_idx : found) {
                const auto& sub = subpatterns[pat_idx];
                int len_sub = fsm.get_pattern_length(pat_idx);

                // Start of current substring in text
                int start_in_text = i - len_sub + 1;

                if (start_in_text < 0) continue;

                // Start of whole pattern in text
                int i_candidate = start_in_text - sub.pos_in_p;

                if (i_candidate < 0) continue;
                if (i_candidate + len_P > len_T) continue;

                if (i_candidate >= 0 && i_candidate <= len_T - len_P) {
                    count[i_candidate]++;
                }
            }
        }

        for (int i = 0; i <= len_T - len_P; ++i) {
            if (count[i] == k) {
                result.push_back(i);
            }
        }

        std::sort(result.begin(), result.end());
        return result;
    }
};

int main() {
    std::string text;
    std::cin >> text;

    std::string p;
    std::cin >> p;

    char wildcard;
    std::cin >> wildcard;

    auto subpatterns = split_into_subpatterns(p, wildcard);

    if (subpatterns.empty()) {
        return 0;
    }

    int len_P = p.size();

    const std::unordered_map<char, size_t> alphabet = {
        {'A', 0},
        {'C', 1},
        {'G', 2},
        {'T', 3},
        {'N', 4}
    };

    for (const auto& sub : subpatterns) {
        for (char c : sub.str) {
            if (!alphabet.count(c)) {
                return 0;
            }
        }
    }

    Solver solver;
    auto result = solver.solve(text, subpatterns, len_P, alphabet);

    for (int pos : result) {
        std::cout << pos + 1 << "\n";
    }

    return 0;
}
