#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <utility>
#include <stdexcept>


// A doubly-linked list
template<class T>
class List{
private:

    // The data structure for eeach node in the list
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
        elt{T(std::forward<Args>(args)...)}, next{_next}, prev{_prev} {}
    };


    Node* first;        // First Node in the list
    Node* last;         // Last Node in the list
    std::size_t Size;   // Number of Nodes in the list

public:

    // Default constructor
    List() noexcept :
    first{nullptr}, last{nullptr}, Size{0} {}


    // Size based constructor (Fills in with default value)
    List(std::size_t _size) :
    first{nullptr}, last{nullptr}, Size{0} {
        T elt = T();
        for(std::size_t i = 0; i < _size; ++i){
            push_back(elt);
        }
    }


    // Sized based constructor with given value (Assumes copying available)
    List(std::size_t _size, const T& _elt) :
    first{nullptr}, last{nullptr}, Size{0} {
        for(std::size_t i = 0; i < _size; ++i){
            push_back(_elt);
        }
    }


    // Copy constructor
    List(const List<T>& other) :
    first{nullptr}, last{nullptr}, Size{0} {
        for(auto it = other.begin(); it != other.end(); ++it){
            push_back(*it);
        }
    }


    // Bidirectional iterator
    struct Iterator{
    private:

        Node* node; // A pointer to a given element in a linked list
        friend class List;

    public:

        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        // Simple contructor
        Iterator(Node* _node) noexcept : node{_node} {}


        // Dereference operator overload
        T& operator*() noexcept {
            return node->elt;
        }


        // Dereference operator overload
        T* operator->() noexcept {
            return node->elt;
        }


        // Prefix increment
        Iterator& operator++() noexcept {
            node = node->next;
            return *this;
        }


        // Postfix increment
        Iterator operator++(int) noexcept {
            Iterator temp(node);
            node = node->next;
            return temp;
        }


        // Prefix decrement
        Iterator& operator--() noexcept {
            node = node->prev;
            return *this;
        }


        // Postfix decrement
        Iterator operator--(int) noexcept {
            Iterator temp(node);
            node = node->prev;
            return temp;
        }


        // Equality operator overload
        friend bool operator==(const Iterator& left, const Iterator& right) noexcept {
            return left.node == right.node;
        }


        // Inequality operator overload
        friend bool operator!=(const Iterator& left, const Iterator& right) noexcept {
            return left.node != right.node;
        }
    };


    // Returns the number of nodes in the list
    std::size_t size() const noexcept {
        return Size;
    }


    // Returns true if the list is empty
    bool empty() const noexcept {
        return size() == 0;
    }


    // Returns an iterator to the first element
    Iterator begin() noexcept {
        return Iterator(first);
    }


    // Returns an iterator to one past the final element
    Iterator end() noexcept {
        if(empty()) return Iterator(last);
        return Iterator(last->next);
    }


    // Add an element at the given iterator's position
    template<class... Args>
    void emplace(Iterator& it, Args&&... args){
        if(it.node == nullptr) throw std::out_of_range("Cannot insert element at nullptr");

        // Create the new Node
        Node* prev = it.node->prev;
        Node* next = new Node(it.node, prev, std::forward<Args>(args)...);

        // Adjust adjacent Node pointers if they exist
        if(prev == nullptr){
            first = next;
        }else{
            prev->next = next;
        }
        it.node->prev = next;

        // Increment size
        ++Size;
    }


    // Add an element to the front of the list in place
    template<class... Args>
    void emplace_front(Args&&... args){
        // Create the new node
        Node* next = new Node(first, nullptr, std::forward<Args>(args)...);

        // Adjust necessary pointers
        if(first == nullptr){
            last = next;
        }else{
            first->prev = next;
        }
        first = next;

        // Increment Size
        ++Size;
    }


    // Add an element to the back of the list in place
    template<class... Args>
    void emplace_back(Args&&... args){
        // If list is empty, emplace front for simplicity
        if(empty()){
            emplace_front(std::forward<Args>(args)...);
            return;
        }

        // Create a new Node and update the last point accordingly
        Node* next = new Node(nullptr, last, std::forward<Args>(args)...);
        last->next = next;
        last = next;

        // Increment Size
        ++Size;
    }


    // Add a const element to the front of the list
    void push_front(const T& elt){
        emplace_front(elt);
    }


    // Add an element to the front of the list
    void push_front(T&& elt){
        emplace_front(elt);
    }


    // Add a const element to the back of the list
    void push_back(const T& elt){
        emplace_back(elt);
    }


    // Add an element to the back of the list
    void push_back(T&& elt){
        emplace_back(elt);
    }


    // Insert an element at the location of the given iterator
    void insert(Iterator& it, T&& elt){
        emplace(it, elt);
    }


    // Insert a const element at the location of the given iterator
    void insert(Iterator& it, const T& elt){
        emplace(it, elt);
    }


    // Return a reference to the idxth node element in the list
    T& at(std::size_t idx){
        if(idx >= size()) throw std::out_of_range("Cannot index node greater than size");
        Node* node = first;
        for(std::size_t i = 0; i < idx; ++i) node = node->next;
        return node->elt;
    }


    // Return a const reference to the idxth node element in the list
    const T& at(std::size_t idx) const {
        if(idx >= size()) throw std::out_of_range("Cannot index node greater than size");
        Node* node = first;
        for(std::size_t i = 0; i < idx; ++i) node = node->next;
        return node->elt;
    }


    // Return a reference to the idxth node element in the list
    T& operator[](std::size_t idx){
        Node* node = first;
        for(std::size_t i = 0; i < idx; ++i) node = node->next;
        return node->elt;
    }


    // Return a const reference to the idxth node element in the list
    const T& operator[](std::size_t idx) const {
        Node* node = first;
        for(std::size_t i = 0; i < idx; ++i) node = node->next;
        return node->elt;
    }


    // Return a reference to the first element in the list
    T& front(){
        if(empty()) throw std::out_of_range("Cannot index into empty list");
        return first->elt;
    }


    // Return a const reference to the first element in the list
    const T& front() const {
        if(empty()) throw std::out_of_range("Cannot index into empty list");
        return first->elt;
    }


    // Return a reference to the last element in the list
    T& back(){
        if(empty()) throw std::out_of_range("Cannot index into empty list");
        return last->elt;
    }


    // Return a const reference to the last element in the list
    const T& back() const {
        if(empty()) throw std::out_of_range("Cannot index into empty list");
        return last->elt;
    }


    // Remove the first node in the list
    void pop_front(){
        if(empty()) throw std::out_of_range("Cannot delete the a non-existent node");

        // Delete the first node and update first
        Node* temp = first->next;
        delete first;
        first = temp;

        // Decrement Size
        --Size;

        // Update last if list is now empty
        if(empty()) last = nullptr;
    }


    // Remove the final node in the list
    void pop_back(){
        if(empty()) throw std::out_of_range("Cannot delete the a non-existent node");

        // Delete the last node and update last
        Node* temp = last->prev;
        delete last;
        last = temp;

        // Decrement Size
        --Size;

        // Update first if list is now empty
        if(empty()) first = nullptr;
    }


    // Erase the node specified by the given iterator
    void erase(Iterator& it){
        if(empty() || it.node == nullptr) throw std::out_of_range("Cannot delete the a non-existent node");

        // If the iterator is the first of last Node, pop for simplicity
        if(it == begin()){
            pop_front();
            return;
        }else if(it.node->next == nullptr){
            pop_back();
            return;
        }

        // Grab prev and next pointers from the given Node
        Node* prev = it.node->prev;
        Node* next = it.node->next;

        // Delete the given Node
        delete it.node;

        // Update the respective pointers
        prev->next = next;
        next->prev = prev;

        // Decrement Size
        --Size;
    }


    // Remove all elements in the list
    void clear(){
        while(!empty()) pop_front();
    }


    ~List(){
        clear();
    }

};

#endif