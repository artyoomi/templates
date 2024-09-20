#include <iostream>
#include <cstddef>

// list elements class
template <typename T>
class Node {
private:
    T val;
    Node *next;
public:
    Node(const T val = 0)
    {
        this->val = val;
        this->next = nullptr;
    }

    void set_val(const T val)
        {this->val = val;}
    T get_val()
    {
        try {
            return this->val;
        }
        catch (const std::exception& ex) {
            std::cout << ex.what() << std::endl;
        }
    }

    void set_next(Node *next)
        {this->next = next;}
    Node *get_next()
        {return this->next;}
};

// list based stack
template <typename T>
class Stack {
private:
    Node<T> *top;
    std::size_t len;
public:
    Stack()
    {
        this->top = nullptr;
        this->len = 0;
    }
    ~Stack()
    {
        while (!is_empty())
            pop();
    }
    
    void push(const char brt)
    {
        if (nullptr == top)
            top = new Node<T>(brt);
        else {
            Node<T> *new_node = new Node<T>(brt);
            new_node->set_next(top);

            top = new_node;
        }
        ++len;
    }

    char pop()
    {
        if (this->is_empty()) {
            std::cerr << "Error: stack is empty!\n" << std::endl;
            exit(EXIT_FAILURE);
        }
        
        const char top_brt = top->get_val();

        Node<T> *tmp_node = top;
        top = top->get_next();

        delete tmp_node;
        tmp_node = nullptr;

        --len;

        return top_brt;
    }

    char get_size()
        {return this->len;}

    char get_top()
    {
        if (nullptr != top)
            return top->get_val();

        std::cerr << "Error: trying to read nullptr!\n";
        exit(EXIT_FAILURE);
    }
    
    bool is_empty()
    {
        return (0 == len);
    }
};
