#ifndef VECTOR_H
#define VECTOR_H

#include <utility>
#include <stdexcept>

// A simplified version of the stl vector
template<class T>
class Vector{
private:

    std::size_t size;       // The Current number of elements in the vector
    std::size_t capacity;   // The total space allocated for the array
    T* arr;                 // Pointer to the start of the array


    // Doubles the size of the underlying array when size reaches capacity
    void grow(){
        T* temp = new T[capacity() * 2];
        capacity *= 2;
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
        Iterator(T* val) : elt{val} {}


        // Dereference operator overload
        T& operator*(){
            return *elt;
        }


        // Dereference operator overload
        T* operator->(){
            return elt;
        }


        // Prefix increment
        Iterator& operator++(){
            ++elt;
            return *this;
        }


        // Postfix increment
        Iterator operator++(int){
            Iterator temp(elt);
            ++elt;
            return temp;
        }


        // Prefix decrement
        Iterator& operator--(){
            --elt;
            return *this;
        }


        // Postfix decrement
        Iterator operator--(int){
            Iterator temp(elt);
            --elt;
            return temp;
        }


        // Equality operator overload
        friend bool operator==(const Iterator& left, const Iterator& right) const {
            return left.elt == right.elt;
        }


        // Inequality operator overload
        friend bool operator!=(const Iterator& left, const Iterator& right) const {
            return left.elt != right.elt;
        }
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
    void emplace_back(Args&&... args){
        if(size() == capacity()) grow();
        new(arr + size()) T(std::forward<Args>(args)...);
        ++size;
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
        if(i >= size()) throw std::out_of_range("Indexed out of range");
        return &arr[i];
    }


    // Returns a const reference to the indexed element
    const T& at(const std::size_t i) const {
        if(i >= size()) throw std::out_of_range("Indexed out of range");
        return &arr[i];
    }


    // Returns a reference to the first element in the vector
    T& front(){
        return at(0);
    }


    // Operator overload to allow direct indexing
    T& operator[](const std::size_t i){
        return &arr[i];
    }


    // Operator overload to allow direct const indexing
    const T& operator[](const std::size_t i) const {
        return &arr[i];
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


    // Destructor
    ~Vector(){
        delete[] arr;
    }
};


#endif