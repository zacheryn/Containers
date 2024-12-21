#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <utility>
#include <stdexcept>


// A doubly-linked list
template<class T>
class List{
private:

    // One node in the list
    struct Node{
        T elt;
        Node* next;
        Node* prev;


        // Default constructor
        Node() :
        elt{T()}, next{nullptr}, prev{nullptr} {}


        // Constructor with variable parameters
        template<class... Args>
        Node(Node* _next, Node* _prev, Args&&... args) :
        next{_next}, prev{_prev}, elt{T(std::forward<Args>(args)...)} {}
    };


    Node* first;        // First Node in the list
    Node* last;         // Last Node in the list
    std::size_t Size;   // Number of Nodes in the list

public:

    // Default constructor
    List() :
    first{nullptr}, last{nullptr}, Size{0} {}


    // 

};

#endif