#ifndef BINARY_SEARCH_TREE_HPP
#define BINARY_SEARCH_TREE_HPP

#include <utility>
#include <stdexcept>

// A binary search tree
template<class T, class Comparator = std::less<T>>
class BST{
private:

    // A data structure for each node of the tree
    struct Node{
        T elt;
        Node* left;
        Node* right;

        // Default contructor
        Node() :
        left{nullptr},
        right{nullptr},
        elt{T()}
        {}

        // Custom constructor for specifying value
        template<class... Args>
        Node(Node* _left, Node* _right, Args&&... args) :
        left{_left},
        right{_right},
        elt{T(std::forward<Args>(args)...)}
        {}
    };

    
    std::size_t Size;
    Node* root;


    // Recursive insert method
    void insert_private(Node* node, Node* val){
        if(Comparator(node->elt, val->elt)){
            if(node->right == nullptr){
                node->right = val;
                return;
            }else{
                insert_private(node->right, val);
            }
        }else{
            if(node->left == nullptr){
                node->left = val;
                return;
            }else{
                insert_private(node->left, val);
            }
        }
    }


    // Recursive search method
    bool search_private(Node* node, const T& val) const {
        if(node == nullptr){
            return false;
        }else if(node->elt == val){
            return true;
        }else if(Comparator(node->elt, val)){
            return search_private(node->right, val);
        }
        return search_private(node->left, val);
    }

public:

    // Default constructor
    BST() :
    Size{0}, root{nullptr} {}


    // Add a Node to the tree in place
    template<class... Args>
    void emplace(Args&&... args){
        Node* val = new Node(nullptr, nullptr, std::forward<Args>(args)...);
        ++Size;
        if(root == nullptr){
            root = val;
            return;
        }
        insert_private(root, val);
    }


    // Add a Node to the tree
    void insert(T&& val){
        emplace(val);
    }


    // Add a Node to the tree
    void insert(const T& val){
        emplace(val);
    }


    // Returns the numder of Nodes in the tree
    std::size_t size() const {
        return Size;
    }


    // Returns true if there are no Nodes in the tree
    bool empty() const {
        return size() == 0;
    }


    // Return true if the given value is present in the tree
    bool search(const T& val) const {
        return search_private(root, val);
    }

    // Return true if the given value is present in the tree
    bool search(T&& val) const {
        return search_private(root, val);
    }

};

#endif
