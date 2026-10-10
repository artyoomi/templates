#ifndef __UNROLLED_LIST_H__
#define __UNROLLED_LIST_H__

#include <iostream>
#include <cstring>
#include <cmath>
#include <stdexcept>
#include <sstream>

#include "./calculate_node_size.h"

#define OUT_OF_RANGE "Index out of range"

template <typename T>
class UnrolledList {
private:
    struct Node {
        T *array          {nullptr};
        size_t array_size {0};
        Node *next        {nullptr};
        Node *prev        {nullptr};
    
        Node(const size_t capacity)
            : array{new T[std::move(capacity)]} {}
        ~Node()
            {delete[] array;}
    };
    
    Node *head_      {nullptr};
    Node *tail_      {nullptr};
    size_t n_nodes_  {0};         // quantity of nodes in list
    size_t n_elems_  {0};         // quantity of elements in whole list
    size_t capacity_ {2};         // max size of array in node
public:
    UnrolledList() = default;

    UnrolledList(size_t optim_size) : capacity_{std::move(optim_size)} {}

    UnrolledList(T *array, size_t array_size)
    {
        capacity_ = calculate_node_size<int>(array_size);
        
        head_ = new Node(capacity_);
        ++n_nodes_;

        Node *curr {head_};
        
        size_t i {0};
        while (curr) {
            for (size_t k {0}; k < capacity_ && i < array_size; ++k, ++i) {
                curr->array[k] = array[i];
                ++curr->array_size;

                ++n_elems_;
            }

            if (i >= array_size)
                break;

            curr->next = new Node(capacity_);
            n_nodes_++;
            
            curr->next->prev = curr;
            curr = curr->next;
        }
        
        tail_ = curr;
    }
       
    ~UnrolledList()
    {
        this->clear();
    }

    // <SETTERS & GETTERS>
    size_t get_n_nodes() const
        {return n_nodes_;}
    size_t get_n_elems() const
        {return n_elems_;}
    size_t get_capacity() const
        {return capacity_;}
    
    // <MAIN FUNCIONS>
    bool is_empty() const
        {return 0 == n_nodes_;}

    void clear()
    {
        n_nodes_ = 0;
        n_elems_ = 0;
        capacity_ = 2;
        
        Node *tmp {nullptr};
        while (head_) {
            tmp = head_;
            head_ = head_->next;

            delete tmp;
            tmp = nullptr;
        }

        head_ = nullptr;
        tail_ = nullptr;
    }

    void insert(int index, const T& value)
    { 
        if (index < 0 || index > n_elems_) {
            throw std::out_of_range(OUT_OF_RANGE);
        }

        if (nullptr == head_) {
            head_ = new Node(capacity_);
            tail_ = head_;
            ++n_nodes_;
        }

        Node* current {head_};
        int current_index {0};

        // find necessary node
        while (current) {            
            if (current_index + current->array_size >= index)
                break;

            current_index += current->array_size;
            current = current->next;
        }

        // index inside of node
        int node_index {index - current_index};

        // if node in full -> split it 
        if (current->array_size >= capacity_)
            split_node(current);

        if (node_index <= current->array_size && node_index < capacity_) {
            // move elements to provide empty space
            std::memmove(current->array + node_index + 1,
                         current->array + node_index,
                         sizeof(T) * (current->array_size - node_index));

            current->array[node_index] = value;
            ++current->array_size;
        } else {
            node_index -= current->array_size;
            // move elements to provide empty space
            std::memmove(current->next->array + node_index + 1,
                         current->next->array + node_index,
                         sizeof(T) * (current->next->array_size - node_index));

            current->next->array[node_index] = value;
            ++current->next->array_size;
        }
        
        if (current)
            merge_nodes(current);
       
        ++n_elems_;
    }

    void push_front(const T& value)
    {
        this->insert(0, value);
    }

    // add element in end of list
    void push_back(const T& value)
    {
        this->insert(n_elems_, value);     
    }
    
    // delete element by index
    void remove_at(int index) {
        if (index < 0 || index >= n_elems_) {
            throw std::out_of_range(OUT_OF_RANGE);
        }

        Node* current {head_};

        int current_index {0};

        // find necessary node
        while (current) {
            if (current_index + current->array_size > index)
                break;

            current_index += current->array_size;
            current = current->next;
        }

        // index inside of node
        int node_index {index - current_index};

        // remove element from node
        std::memmove(current->array + node_index,
                     current->array + node_index + 1,
                     sizeof(T) * (current->array_size - node_index - 1));
        
        --current->array_size;
        --n_elems_;

        // delete empty node
        if (0 == current->array_size) {
            // | |<->|X|<->| |
            if (current->prev != nullptr && current->next != nullptr) {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            // | |<->|X|
            } else if (current->prev != nullptr) {
                current->prev->next = nullptr;
                tail_ = current->prev;
            // |X|<->| |
            } else if (current->next != nullptr) {
                current->next->prev = nullptr;
                head_ = current->next;
            // |X| is head_
            } else {
                head_ = nullptr;
                tail_ = nullptr;
            }    
           
            delete current;
            current = nullptr;

            --n_nodes_;
        } else if (current->array_size < ceil(static_cast<double>(capacity_) / 2)) {
            // try to merge with prev or next node
            merge_nodes(current);
            redistr_node(current);
        }
    }
    
    // search element by index
    T get(int index) const
    {
        if (index < 0 || index >= n_elems_) {
            throw std::out_of_range(OUT_OF_RANGE);
        }

        Node* current {head_};
        int current_index {0};

        while (current) {
            if (current_index + current->array_size > index) {
                return current->array[index - current_index];
            }
            
            current_index += current->array_size;
            current = current->next;
        }

        throw std::runtime_error("Element not found");
    }

    // get index by element
    int find_index(const T& value)
    {
        Node *current {head_};

        size_t counter {0};
        while (current) {
            // WARNING: T type may not have equation rule
            for (size_t i {0}; i < current->array_size; ++i, ++counter) {
                if (current->array[i] == value)
                    return counter;
            }

            current = current->next;
        }

        return -1;
    }

    T *get_data()
    {
        T *data = new T[n_elems_];
        size_t data_len {0};
        
        Node *current {head_};
        while (current) {
            for (size_t i {0}, size = current->array_size; i < size; ++i, ++data_len)
                data[data_len] = current->array[i];

            current = current->next;
        }

        return data;
    }

    // returns string with elements of array
    std::string get_str() const
    {
        std::ostringstream oss;

        T *data = get_data();
        for (size_t i {0}; i < n_elems_; ++i) {
            oss << data[i];

            if (i < n_elems_ - 1)
                oss << ' ';
        }

        delete data;
        return oss.str();
    }
    
    // prints whole list content
    void print() const
    {
        std::cout << get_str() << '\n';        
    }

    // print list content by nodes
    void print_by_nodes() const
    {
        if (nullptr == head_)
            return;
        
        size_t node_counter {0};
        
        Node *current = head_;
        while (current) {
            std::cout << "Node " << node_counter++ << ":";
            
            for (size_t i {0}; i < current->array_size; ++i)
                std::cout << " " << current->array[i];
            
            std::cout << '\n';
        
            current = current->next;
        }
    }

private:
    void split_node(Node *node)
    {
        Node *new_node {new Node(capacity_)};
        // number of elements to be moved
        int half = ceil(static_cast<double>(node->array_size) / 2);

        // move elements in new node
        memmove(new_node->array, node->array + half,
                sizeof(T) * (node->array_size - half));

        // update sizes
        new_node->array_size = (node->array_size - half);
        node->array_size = half;

        // update pointers
        new_node->next = node->next;
        new_node->prev = node;
        if (node->next)
            node->next->prev = new_node;
        node->next = new_node;

        // update tail
        if (node == tail_)
            tail_ = new_node;
    }

    void merge_nodes(Node *node)
    {
        if (nullptr == node->prev && nullptr == node->next)
            return;

        Node *merge_node {nullptr};

        // swap current and previous node pointers
        if (nullptr != node->prev) {
            merge_node = node->prev;
            if (node->array_size + merge_node->array_size <= capacity_) {
                // swap pointers
                Node *tmp {node};
                node = merge_node;
                merge_node = tmp;
            }
        }
       
        // merge with next node
        if (nullptr != node->next) {
            merge_node = node->next;
            
            if (node->array_size + merge_node->array_size <= capacity_) {
                // move elements
                std::memmove(node->array + node->array_size,
                             merge_node->array,
                             sizeof(T) * merge_node->array_size);
            
                node->array_size += merge_node->array_size;
            
                // update pointers
                node->next = merge_node->next;
                if (merge_node->next)
                    merge_node->next->prev = node;

                if (merge_node == tail_)
                    tail_ = node;
                
                delete merge_node;
                merge_node = nullptr;
            }
        }
    }

    void redistr_node(Node *node)
    {   
        if (!node)
            return;
        
        int prev_diff {0};
        if (node->prev)
            prev_diff = static_cast<ssize_t>(node->prev->array_size) - node->array_size;

        int next_diff {0};
        if (node->next)
            next_diff = static_cast<ssize_t>(node->next->array_size) - node->array_size;

        int offset {0};
        
        if (prev_diff <= 1 && next_diff <= 1)
            return;

        bool prev_is_better {prev_diff > next_diff};

        // if redistr from previous node
        if (prev_is_better && node->prev) {
            offset = ceil(static_cast<double>(prev_diff) / 2);

            // move data inside array
            std::memmove(node->array + offset,
                         node->array,
                         sizeof(T) * node->array_size);

            // move data from prev array
            std::memmove(node->array,
                         node->prev->array + (node->prev->array_size - offset),
                         sizeof(T) * offset);

            node->prev->array_size -= offset;
            node->array_size += offset;
        }
        else if (node->next) {
            offset = ceil(static_cast<double>(next_diff) / 2);

            // move data from next array
            std::memmove(node->array + node->array_size,
                         node->next->array,
                         sizeof(T) * offset);

            // move data in next array to the left side
            std::memmove(node->next->array,
                         node->next->array + offset,
                         sizeof(T) * (node->next->array_size - offset));

            node->next->array_size -= offset;
            node->array_size += offset;
        }
    }
};

#endif
