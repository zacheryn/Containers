#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <utility>
#include <stdexcept>


// A simplified version of the stl vector
template<class T>
class Vector{
private:

    std::size_t Size;       // The Current number of elements in the vector
    std::size_t Capacity;   // The total space allocated for the array
    T* arr;                 // Pointer to the start of the array


    // Doubles the size of the underlying array when size reaches capacity
    void grow(){
        if(capacity() == 0) Capacity = 1;
        T* temp = new T[capacity() * 2];
        Capacity *= 2;
        for(std::size_t i = 0; i < size(); ++i){
            temp[i] = std::move(arr[i]);
        }

        delete[] arr;
        arr = temp;
    }

public:

    // Bidirectional iterator for traversing the vector
    struct Iterator{
        T* elt; // A pointer to a given element in a vector


        // Simple contructor
        Iterator(T* val) noexcept : elt{val} {};


        // Dereference operator overload
        T& operator*() noexcept {
            return *elt;
        }


        // Dereference operator overload
        T* operator->() noexcept {
            return *elt;
        }


        // Prefix increment
        Iterator& operator++() noexcept {
            ++elt;
            return *this;
        }


        // Postfix increment
        Iterator operator++(int) noexcept {
            Iterator temp(elt);
            ++elt;
            return temp;
        }


        // Prefix decrement
        Iterator& operator--() noexcept {
            --elt;
            return *this;
        }


        // Postfix decrement
        Iterator operator--(int) noexcept {
            Iterator temp(elt);
            --elt;
            return temp;
        }


        // Equality operator overload
        // Checks that the two iterators point to the same object
        friend bool operator==(const Iterator& left, const Iterator& right) noexcept {
            return left.elt == right.elt;
        }


        // Inequality operator overload
        // Checks that the two iterators point to different objects
        friend bool operator!=(const Iterator& left, const Iterator& right) noexcept {
            return left.elt != right.elt;
        }
    };


    // Default constructor
    Vector() noexcept :
    Size{0}, Capacity{0}, arr{nullptr} {}


    // Size constructor with default value
    Vector(const std::size_t _size) :
    Size{_size}, Capacity{_size}, arr{new T[_size]} {}


    // Size constructor with given value
    Vector(const std::size_t _size, const T& elt) :
    Size{0}, Capacity{_size}, arr{new T[_size]} {
        for(std::size_t _ = 0; _ < _size; ++_){
            push_back(elt);
        }
    }


    // Copy constructor
    Vector(const Vector<T>& other) :
    Size{0}, Capacity{other.capacity()}, arr{new T[other.capacity()]} {
        for(std::size_t i = 0; i < other.size(); ++i){
            push_back(other.at(i));
        }
    }


    // Adds the given element in place in memory
    template<class... Args>
    void emplace_back(Args&&... args){
        if(size() == capacity()) grow();
        new(arr + size()) T(std::forward<Args>(args)...);
        ++Size;
    }


    // Adds the given const element to the back of the vector
    void push_back(const T& elt){
        emplace_back(elt);
    }


    // Adds the given element to the back of the vector in place
    void push_back(T&& elt){
        emplace_back(std::move(elt));
    }


    // Returns the size of the vector
    std::size_t size() const noexcept {
        return Size;
    }


    // Returns the capacity of the vector
    std::size_t capacity() const noexcept {
        return Capacity;
    }


    // Returns true if the vector is empty
    bool empty() const noexcept {
        return Size == 0;
    }


    // Returns a reference to the indexed element
    T& at(const std::size_t i){
        if(i >= size()) throw std::out_of_range("Indexed out of range");
        return arr[i];
    }


    // Returns a const reference to the indexed element
    const T& at(const std::size_t i) const {
        if(i >= size()) throw std::out_of_range("Indexed out of range");
        return arr[i];
    }


    // Returns a reference to the first element in the vector
    T& front(){
        return at(0);
    }


    // Returns a const reference to the first element in the vector
    const T& front() const {
        return at(0);
    }


    // Returns a reference to the final element in the vector
    T& back(){
        if(size() == 0) throw std::out_of_range("Indexed out of range");
        return at(size() - 1);
    }


    // Returns a const reference to the final element in the vector
    const T& back() const {
        if(size() == 0) throw std::out_of_range("Indexed out of range");
        return at(size() - 1);
    }


    // Operator overload to allow direct indexing
    T& operator[](const std::size_t i){
        return arr[i];
    }


    // Operator overload to allow direct const indexing
    const T& operator[](const std::size_t i) const {
        return arr[i];
    }


    // Returns an iterator to the first element in the vector
    Iterator begin() const {
        if(size() == 0) throw std::out_of_range("Indexed out of range");
        return Iterator(arr);
    }


    // Returns an iterator one element past the last element in the vector
    Iterator end() const {
        return Iterator(arr + size());
    }


    // Removes the last element in the vector
    void pop_back(){
        if(empty()) throw std::out_of_range("Cannot remove element from empty vector");
        --Size;
        arr[size()].~T();
    }


    // Clear the vector
    void clear(){
        if(empty()) return;
        while(!empty()) pop_back();
    }


    // Destructor
    ~Vector(){
        delete[] arr;
    }
};


#endif