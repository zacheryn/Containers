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


        // Copy constructor
        Node(const Node& other) :
        left{other.left},
        right{other.left},
        elt{T(other.elt)}
        {}

    };

    
    std::size_t Size;
    Node* root;


    // Recursive insert method
    void insert_private(Node* node, Node* val){
        if(Comparator(node->elt, val->elt)){    // Look at right child
            if(node->right == nullptr){         // Inserting into right child
                node->right = val;
                return;
            }else{                              // Moving to right child
                insert_private(node->right, val);
            }
        }else{                                  // Looking at left child
            if(node->left == nullptr){          // Inserting into left child
                node->left = val;
                return;
            }else{                              // Moving to left child
                insert_private(node->left, val);
            }
        }
    }


    // Recursive search method
    Node* search_private(const Node* node, const T& val) const {
        if(node == nullptr){                    // Does not exist
            return nullptr;
        }else if(node->elt == val){             // Found the node
            return node;
        }else if(Comparator(node->elt, val)){   // Move to right child
            return search_private(node->right, val);
        }
        return search_private(node->left, val); // Move to left child
    }


    // Find parent of node to be deleted (Assumes node is not nullptr)
    Node* parent_of_removed(const Node* node, const T& val) const {
        if(Comparator(node->elt, val->elt)){                        // Look at right child
            if(node->right == nullptr ||  node->right->elt == val){ // Returning node
                return node;
            }else{                                                  // Moving to right child
                return parent_of_removed(node->right, val);
            }
        }                                                           // Looking at left child
        if(node->left == nullptr ||  node->left->elt == val){       // Returning node
            return node;
        }                                                           // Moving to left child
        return parent_of_removed(node->left, val);
    }


    // Find the in-order succesor of the given node
    Node* in_order_succesor(const Node* node) const {
        if(node->right == nullptr) return nullptr;

        Node* child = node->right;
        while(child->left != nullptr) child = child->left;
        return child;
    }


    // Recursive remove method
    bool remove_private(Node* parent, const T& val){
        Node* child = nullptr;
        bool isLeft = false;
        if(parent->right->elt == val){
            child = parent->right;
        }else{
            child = parent->left;
            isLeft = true;
        }

        if(child->left == nullptr && child->right == nullptr){
            if(isLeft) parent->left = nullptr;
            else parent->right = nullptr;
            delete child;
            return true;
        }else if(child->left == nullptr){
            if(isLeft) parent->left = child->right;
            else parent->right = child->right;
            delete child;
            return true;
        }else if(child->right == nullptr){
            if(isLeft) parent->left = child->left;
            else parent->right = child->left;
            delete child;
            return true;
        }else{
            Node* temp = new Node(in_order_succesor(child));
            remove(temp->elt);
        }
    }

public:

    // Default constructor
    BST() :
    Size{0}, root{nullptr} {}


    // Add a Node to the tree in place
    template<class... Args>
    void emplace(Args&&... args){
        Node* val = new Node(nullptr, nullptr, std::forward<Args>(args)...);
        if(search(val->elt)) throw std::domain_error("Cannot insert a value that already exists");
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
        return search_private(root, val) != nullptr;
    }


    // Return true if the given value is present in the tree
    bool search(T&& val) const {
        return search_private(root, val) != nullptr;
    }


    // Remove the value with the specificed value
    bool remove(const T& val){

    }


    // Remove the value with the specificed value
    bool remove(T&& val){
        
    }

};

#endif
