#include <iostream>
#include <vector>
#include <queue>
#include <fstream>

template <typename T>
class AVL {
private:
    struct Node {
        T key;
        Node *left;
        Node *right;
        std::size_t height;
        Node(int key, Node *left = nullptr, Node *right = nullptr)
            : key{key}, left{left}, right{right}, height{1} {}

        ~Node()
        {
            key = 0;
            height = 0;
            left = nullptr;
            right = nullptr;
        }
    } *_root;

public:
    AVL() : _root{nullptr} {}
    ~AVL() { clear(); }
    
    std::size_t height() { return _get_height(_root); }
    void insert(T key)   { _root = _run_insert(_root, key); }
    void remove(T key)   { _root = _run_remove(_root, key); }
    void remove_min()    { _root = _run_remove_min(_root); }
    void remove_max()    { _root = _run_remove_max(_root); }

    T min()
    {
        Node *min_node = _min(_root);

        if (min_node == nullptr)
            throw std::runtime_error("AVL tree is empty!");

        return min_node->key;
    }

    T max()
    {
        Node *max_node = _max(_root);

        if (max_node == nullptr)
            throw std::runtime_error("AVL tree is empty!");

        return max_node->key;
    }
    
    std::vector<T> get_keys(std::string order)
    {
        std::vector<T> keys;
        if      (order == "in")    _get_in_order(_root, keys);
        else if (order == "pre")   _get_pre_order(_root, keys);
        else if (order == "post")  _get_post_order(_root, keys);
        else if (order == "width") _get_width_traversal(_root, keys);
        else throw std::invalid_argument("Unknown traversal order!");

        return keys;
    }

    void print(std::string order = "in")
    {
        std::vector<T> keys = get_keys(order);
           
        for (std::size_t i = 0, size = keys.size(); i < size; ++i)
            std::cout << keys[i] << ((i == size - 1) ? ('\n') : (' '));
    }

    void generate_DOT_file() { _generate_DOT_file(_root); }

    void clear() { _run_clear(_root); }

private:
    // find height of tree
    std::size_t _get_height(Node *node)
    {
        if (node == nullptr)
            return 0;
        return node->height;
            
    }
    
    // recalculates height of node
    void _recalc_height(Node *&node)
    {
        node->height = 1 + std::max(_get_height(node->left),
                                    _get_height(node->right));
    }

    ssize_t _calc_bfactor(Node *node)
    {
        // balance factor
        // difference between heights of right and left subtrees
        ssize_t bfactor = 0;
        bfactor = static_cast<ssize_t>(_get_height(node->right)) -
                  static_cast<ssize_t>(_get_height(node->left));
        return bfactor;
    }
    
    Node *_run_left_rotation(Node *&rotate_root)
    {
        /* We have these tree (balance factor in brackets):
         * A(-2)               B(-1)
         *   \                /    \
         *   B(-1)     ->   A(-1)  C(0) 
         *   /  \             \
         * D(0)  C(0)        D(0)
         * Logic of rotation:
         * 1. B - new root 
         * 2. A - new left of B
         *    If B have children, then this children become new A right subtree
         */
        
        Node *new_root = rotate_root->right;
        Node *old_new_root_left = new_root->left;
        
        new_root->left = rotate_root;
        new_root->left->right = old_new_root_left;

        _recalc_height(rotate_root);
        _recalc_height(new_root);
        
        return new_root;
    }
    
    Node *_run_right_rotation(Node *&rotate_root)
    {
        // Like a left turn, only everything is mirrored
        
        Node *new_root = rotate_root->left;
        Node *old_new_root_right = new_root->right;

        new_root->right = rotate_root;
        new_root->right->left = old_new_root_right;

        _recalc_height(rotate_root);
        _recalc_height(new_root);

        return new_root;
    }

    Node *_run_balancing(Node *&rotate_node)
    {
        // balance factor
        // difference between heights of right and left subtrees
        ssize_t bfactor = _calc_bfactor(rotate_node);

        if (bfactor < 2 && bfactor > -2)
            return rotate_node;

        else if (bfactor >= 2) {
            // small left rotate
            if (_calc_bfactor(rotate_node->right) >= 0)
                return _run_left_rotation(rotate_node);
            // big left rotate
            else {
                rotate_node->right = _run_right_rotation(rotate_node->right);
                return _run_left_rotation(rotate_node);
            }
                
        } else {
            // left right rotate
            if (_calc_bfactor(rotate_node->left) <= 0)
                return _run_right_rotation(rotate_node);
            // big right rotate
            else {
                rotate_node->left = _run_left_rotation(rotate_node->left);
                return _run_right_rotation(rotate_node);
            }
        }
    }

    Node *_run_insert(Node *&node, T key)
    {
        // if tree is empty
        if (node == nullptr) {
            node = new Node(key);
            return node;
        }

        // choose subtree to insert: left or right
        if (key < node->key)
            node->left = _run_insert(node->left, key);
        else
            node->right = _run_insert(node->right, key);

        _recalc_height(node);
        return _run_balancing(node);
    }

    Node *_min(Node *root)
    {
        if (root == nullptr) return nullptr;

        Node *current = root;
        while (current->left != nullptr)
            current = current->left;

        return current;
    }

    Node *_max(Node *root)
    {
        if (root == nullptr) return nullptr;

        Node *current = root;
        while (current->right != nullptr)
            current = current->right;

        return current;
    }

    Node *_run_remove(Node *&node, T key)
    {
        // returns minimal value from right subtree of node
        auto right_subtree_min_key = [](Node *node) -> T
        {
            Node *current = node;
            while (current->left != nullptr)
                current = current->left;
            return current->key;
        };
        
        if (node == nullptr)
            return nullptr;
               
        if (key < node->key)
            node->left = _run_remove(node->left, key);
        else if (key > node->key)
            node->right = _run_remove(node->right, key);
        else {
            if (node->left == nullptr) {
                Node *tmp = node->right;
                
                delete node;
                node = nullptr;
                
                return tmp;
            } else if (node->right == nullptr) {
                Node *tmp = node->left;
                
                delete node;
                node = nullptr;
                
                return tmp;
            }

            // NOTE: node->right is guaranteed non nullptr (due to upper conditions)
            T tmp_key = _min(node->right)->key;
            node->key = tmp_key;
            node->right = _run_remove(node->right, tmp_key);
        }
        
        _recalc_height(node);
        return _run_balancing(node);
    }

    Node *_run_remove_min(Node *&node)
    {
        Node *min_node = _min(node);
        if (min_node != nullptr)
            return _run_remove(node, min_node->key);
        return node;
    }

    Node *_run_remove_max(Node *&node)
    {
        Node *max_node = _max(node);
        if (max_node != nullptr)
            return _run_remove(node, max_node->key);
        return node;
    }

    void _run_clear(Node *&node)
    {
        if (node == nullptr) return;
        _run_clear(node->left);
        _run_clear(node->right);
        delete node;
        node = nullptr;
    }
   
    // left subtree -> root -> right subtree
    void _get_in_order(Node *node, std::vector<T> &keys)
    {
        if (node == nullptr) return;
        _get_in_order(node->left, keys);
        keys.emplace_back(node->key);
        _get_in_order(node->right, keys);
    }

    // root -> left subtree -> right subtree
    void _get_pre_order(Node *node, std::vector<T> &keys)
    {
        if (node == nullptr) return;
        keys.emplace_back(node->key);
        _get_pre_order(node->left, keys);
        _get_pre_order(node->right, keys);
    }

    // left subtree -> right subtree -> root
    void _get_post_order(Node *node, std::vector<T> &keys)
    {
        if (node == nullptr) return;
        _get_post_order(node->left, keys);
        _get_post_order(node->right, keys);
        keys.emplace_back(node->key);
    }

    void _get_width_traversal(Node *root, std::vector<T> &keys)
    {
        if (root == nullptr)
            return;

        Node *current = root;
        
        std::queue<Node*> q;
        q.push(current);

        while (!q.empty()) {
            keys.emplace_back((q.front())->key);
            q.pop();
            
            if (current->left != nullptr)
                q.push(current->left);
            if (current->right != nullptr)
                q.push(current->right);

            current = q.front();
        }
    }

    void _generate_DOT(Node *root, std::ofstream &out)
    {
        if (root == nullptr) return;

        auto form_node = [this](Node *node, std::ofstream &out)
        {
            out << "\"" << node->key << "\\n"
                        << "bfactor: " << this->_calc_bfactor(node) << "\\n"
                        << "height: " << node->height << "\"";
        };

        if (root->left) {
            out << "    ";
            form_node(root, out);
            out << " -> ";
            form_node(root->left, out);
            out << ";\n";

            _generate_DOT(root->left, out);
        }
        if (root->right) {
            out << "    ";
            form_node(root, out);;
            out << " -> ";
            form_node(root->right, out);
            out << ";\n";

            _generate_DOT(root->right, out);
        }
    }

    void _generate_DOT_file(Node *root)
    {
        std::ofstream out("avl_tree.dot");
        out << "digraph AVLTree {\n";
        out << "    node [fontname=\"Liberation Mono\"];\n";

        // if tree is empty
        if (root == nullptr)
            out << "    null;\n";
        // if tree have only root element
        else if (root->left == nullptr && root->right == nullptr)
            out << "    " << root->key << ";\n";
        // if tree have at least 2 elements
        else
            _generate_DOT(root, out);

        out << "}\n";
        out.close();
    }
};
