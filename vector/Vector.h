#ifndef VECTOR_H
#define VECTOR_H

#include <utility>
#include <stdexcept>

// A simplified version of the stl vector
template<class T>
class Vector{
private:

    std::size_t size;
    std::size_t capacity;
    T* arr;


    // Doubles the size of the underlying array when size reaches capacity
    void grow();

public:

    // Iterator for traversing the vector
    struct Iterator{
        T* elt;

        Iterator(T* val) : elt{val} {}
    };


    // Default constructor
    Vector() :
    size{0}, capacity{0}, arr{nullptr} {}


    // Size constructor with default value
    Vector(std::size_t _size) :
    size{_size}, capacity{_size}, arr{new T[_size]} {}


    // Size constructor with given value (assumes copying available)
    Vector(std::size_t _size, T& elt) :
    size{_size}, capacity{_size}, arr{new T[_size]} {
        for(std::size_t i = 0; i < _size; ++i){
            arr[i] = elt;
        }
    }


    // Copy constructor
    Vector(Vector& other) :
    size{other.size}, capacity{other.capacity}, arr{new T[other.capacity]} {
        for(std::size_t i = 0; i < other.size; ++i){
            push_back(other.at(i));
        }
    }


    // Adds the given element in place in memory
    template<class... Args>
    void emplace_back(Args args);


    // Adds the given element to the back of the vector
    void push_back(const T& elt);


    // Returns the size of the vector
    std::size_t size() const {
        return size;
    }


    // Returns the capacity of the vector
    std::size_t capacity() const {
        return capacity;
    }


    // Returns true if the vector is empty
    bool empty() const {
        return size == 0;
    }


    // Returns a reference to the indexed element
    T& at(const std::size_t i){
        if(i >= size) throw std::out_of_range("Indexed out of range");
        return &arr[i];
    }


    // Returns a const reference to the indexed element
    const T& at(const std::size_t i) const {
        if(i >= size) throw std::out_of_range("Indexed out of range");
        return &arr[i];
    }


    // Operator overload to allow direct indexing
    T& operator[](const std::size_t i){
        return &arr[i];
    }


    // Operator overload to allow direct const indexing
    const T& operator[](const std::size_t i) const {
        return &arr[i];
    }
};


#endif