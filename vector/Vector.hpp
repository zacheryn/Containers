#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <utility>
#include <memory>
#include <stdexcept>


// A simplified version of the stl vector
template<class T>
class Vector{
public:
    typedef std::size_t size_type;
private:

    size_type Size;           // The Current number of elements in the vector
    size_type Capacity;       // The total space allocated for the array
    std::unique_ptr<T[]> uarr;  // The pointer for the array


    // Doubles the size of the underlying array when size reaches capacity
    void grow(){
        if(capacity() == 0) Capacity = 1;
        std::unique_ptr<T[]> utemp(new T[capacity() * 2]{});
        Capacity *= 2;
        for(size_type i = 0; i < size(); ++i){
            new(utemp.get() + i) T(std::move(uarr[i]));
        }

        uarr = std::move(utemp);
    }

protected:

    // Bidirectional iterator for traversing the vector
    template<class Access_Type>
    class IteratorType{
        friend class Vector;
    public:
        // Iterator traits to make the iterator stl compliant
        using iterator_category = std::random_access_iterator_tag;
        using value_type = Access_Type;
        using difference_type = std::ptrdiff_t;
        using pointer = Access_Type*;
        using reference = Access_Type&;


    protected:
        Access_Type* elt; // A pointer to a given element in a vector

    public:
        // Simple contructor
        IteratorType(Access_Type* val) noexcept : elt{val} {};


        // Dereference operator overload
        Access_Type& operator*() noexcept {
            return *elt;
        }


        // Dereference operator overload
        Access_Type* operator->() noexcept {
            return *elt;
        }


        // Access operator
        Access_Type& operator[](const size_type& i) noexcept {
            return *(elt + i);
        }
        Access_Type& operator[](size_type&& i) noexcept {
            return *(elt + std::move(i));
        }
        const Access_Type& operator[](const size_type& i) const noexcept {
            return *(elt + i);
        }
        const Access_Type& operator[](size_type&& i) const noexcept {
            return *(elt + std::move(i));
        }


        // Prefix increment
        IteratorType<Access_Type>& operator++() noexcept {
            ++elt;
            return *this;
        }


        // Postfix increment
        IteratorType<Access_Type> operator++(int) noexcept {
            IteratorType<Access_Type> temp(elt);
            ++elt;
            return temp;
        }


        // Prefix decrement
        IteratorType<Access_Type>& operator--() noexcept {
            --elt;
            return *this;
        }


        // Postfix decrement
        IteratorType<Access_Type> operator--(int) noexcept {
            IteratorType<Access_Type> temp(elt);
            --elt;
            return temp;
        }


        // Compound Assignments
        IteratorType<Access_Type>& operator+=(const size_type& offset) noexcept {
            elt += offset;
            return *this;
        }
        IteratorType<Access_Type>& operator+=(size_type&& offset) noexcept {
            elt += std::move(offset);
            return *this;
        }
        IteratorType<Access_Type>& operator-=(const size_type& offset) noexcept {
            elt -= offset;
            return *this;
        }
        IteratorType<Access_Type>& operator-=(size_type&& offset) noexcept {
            elt -= std::move(offset);
            return *this;
        }


        // Addition
        friend IteratorType<Access_Type> operator+(IteratorType<Access_Type> it, const size_type& offset) noexcept {
            return std::move(it += offset);
        }
        friend IteratorType<Access_Type> operator+(IteratorType<Access_Type> it, size_type&& offset) noexcept {
            return std::move(it += std::move(offset));
        }
        friend IteratorType<Access_Type> operator+(const size_type& offset, IteratorType<Access_Type> it) noexcept {
            return std::move(it += offset);
        }
        friend IteratorType<Access_Type> operator+(size_type&& offset, IteratorType<Access_Type> it) noexcept {
            return std::move(it += std::move(offset));
        }


        // Subtraction
        friend IteratorType<Access_Type> operator-(IteratorType<Access_Type> it, const size_type& offset) noexcept {
            return std::move(it -= offset);
        }
        friend IteratorType<Access_Type> operator-(IteratorType<Access_Type> it, size_type&& offset) noexcept {
            return std::move(it -= std::move(offset));
        }
        friend IteratorType<Access_Type> operator-(const size_type& offset, IteratorType<Access_Type> it) noexcept {
            return std::move(it -= offset);
        }
        friend IteratorType<Access_Type> operator-(size_type&& offset, IteratorType<Access_Type> it) noexcept {
            return std::move(it -= std::move(offset));
        }


        difference_type operator-(const IteratorType<Access_Type>& other) noexcept {
            return static_cast<difference_type>(elt - other.elt);
        }


        // Equality operator overload
        // Checks that the two iterators point to the same object
        friend bool operator==(const IteratorType<Access_Type>& left, const IteratorType<Access_Type>& right) noexcept {
            return left.elt == right.elt;
        }


        // Inequality operator overload
        // Checks that the two iterators point to different objects
        friend bool operator!=(const IteratorType<Access_Type>& left, const IteratorType<Access_Type>& right) noexcept {
            return left.elt != right.elt;
        }


        // Comparison operators
        bool operator<(const IteratorType<Access_Type>& other) const noexcept {
            return elt < other.elt;
        }
        bool operator<=(const IteratorType<Access_Type>& other) const noexcept {
            return elt <= other.elt;
        }bool operator>(const IteratorType<Access_Type>& other) const noexcept {
            return elt > other.elt;
        }
        bool operator>=(const IteratorType<Access_Type>& other) const noexcept {
            return elt >= other.elt;
        }
    };


public:

    // Typedefs of the base iterator class for reduced code duplication for const and regular iterators

    // STL compliant iterator allowing mutable elements
    typedef IteratorType<T> iterator;

    // STL compliant const iterator ensuring elements cannot be changed
    typedef IteratorType<const T> const_iterator;


    // Default constructor
    constexpr Vector() noexcept :
    Size{0}, Capacity{0}, uarr{nullptr} {}


    // Size constructor with default value
    Vector(const size_type _size) noexcept :
    Size{_size}, Capacity{_size}, uarr{new T[_size]{}} {}


    // Size constructor with given value
    Vector(const size_type _size, const T& elt) :
    Size{0}, Capacity{_size}, uarr{new T[_size]} {
        for(size_type _ = 0; _ < _size; ++_){
            push_back(elt);
        }
    }


    // Copy constructor
    Vector(const Vector<T>& other) noexcept :
    Size{other.size()}, Capacity{other.capacity()}, uarr{new T[other.capacity()]} {
        std::copy(other.begin(), other.end(), begin());
    }


    // Move constructor
    Vector(Vector<T>&& other) noexcept :
    Size{0}, Capacity{0}, uarr{std::move(other.uarr)} {
        std::swap(Size, other.Size);
        std::swap(Capacity, other.Capacity);
    }


    // Copy assignment
    Vector<T>& operator=(const Vector<T>& other) noexcept {
        // Guard self assignment
        if(this->uarr == other.uarr) return *this;

        uarr.reset(new T[other.capacity()]);
        Size = other.size();
        Capacity = other.capacity();
        std::copy(other.begin(), other.end(), begin());

        return *this;
    }


    // Move assignment
    Vector<T>& operator=(Vector<T>&& other) noexcept {
        // Guard self assignment
        if(this->uarr == other.uarr) return *this;

        if(uarr != nullptr){
            uarr = nullptr;
            Size = 0;
            Capacity = 0;
        }
        std::swap(Size, other.Size);
        std::swap(Capacity, other.Capacity);
        uarr.swap(other.uarr);
        return *this;
    }


    // Adds the given element in place in memory
    template<class... Args>
    void emplace_back(Args&&... args){
        if(size() == capacity()) grow();
        new(uarr.get() + size()) T(std::forward<Args>(args)...);
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
    constexpr size_type size() const noexcept {
        return Size;
    }


    // Returns the capacity of the vector
    constexpr size_type capacity() const noexcept {
        return Capacity;
    }


    // Returns true if the vector is empty
    constexpr bool empty() const noexcept {
        return Size == 0;
    }


    // Returns a reference to the indexed element
    T& at(const size_type i){
        if(i >= size()) throw std::out_of_range("Indexed out of range");
        return uarr[i];
    }


    // Returns a const reference to the indexed element
    const T& at(const size_type i) const {
        if(i >= size()) throw std::out_of_range("Indexed out of range");
        return uarr[i];
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
    T& operator[](const size_type i) noexcept {
        return uarr[i];
    }


    // Operator overload to allow direct const indexing
    const T& operator[](const size_type i) const noexcept {
        return uarr[i];
    }


    // Returns an iterator to the first element in the vector
    iterator begin() const {
        if(empty()) throw std::out_of_range("Cannot create an Iterator of an empty Vector");
        return iterator(uarr.get());
    }


    // Returns an iterator one element past the last element in the vector
    iterator end() const {
        if(empty()) throw std::out_of_range("Cannot create an Iterator of an empty Vector");
        return iterator(uarr.get() + size());
    }


    // Returns a const iterator to the first element in the vector
    const_iterator cbegin() const {
        if(empty()) throw std::out_of_range("Cannot create an Iterator of an empty Vector");
        return const_iterator(uarr.get());
    }


    // Returns a const iterator one element past the last element in the vector
    const_iterator cend() const {
        if(empty()) throw std::out_of_range("Cannot create an Iterator of an empty Vector");
        return const_iterator(uarr.get() + size());
    }


    // Removes the last element in the vector
    void pop_back(){
        if(empty()) throw std::out_of_range("Cannot remove element from empty vector");
        --Size;
        std::destroy_at(uarr.get() + size());
    }


    // Clear the vector
    void clear(){
        while(!empty()) pop_back();
    }


    // Shrinks the internal array to the number of elements in the vector
    void shrink_to_fit() noexcept {
        if(capacity() == size()) return;

        if(empty()){
            uarr = nullptr;
            Capacity = 0;
            return;
        }

        std::unique_ptr<T[]> utemp(new T[size()]);
        for(size_type i = 0; i < size(); ++i){
            new(utemp.get() + i) T(std::move(uarr[i]));
        }
        Capacity = size();

        uarr = std::move(utemp);
    }


    // Allocates at least _size elements in of space
    // Only affects capacity
    void reserve(const size_type _size) noexcept {
        if(_size <= capacity()) return;

        std::unique_ptr<T[]> utemp(new T[_size]);
        for(size_type i = 0; i < size(); ++i){
            new(utemp.get() + i) T(std::move(uarr[i]));
        }
        Capacity = _size;

        uarr = std::move(utemp);
    }


    // Resizes the underlying array to _size
    // Fills empty space with default values
    void resize(const size_type _size) noexcept {
        if(_size == size()) return;

        std::unique_ptr<T[]> utemp(new T[_size]{});
        for(size_type i = 0; i < (_size < size() ? _size : size()); ++i){
            new(utemp.get() + i) T(std::move(uarr[i]));
        }
        uarr = std::move(utemp);
        Size = _size;
        Capacity = _size;
    }


    // Returns a pointer to the underlying array
    // Assumes class invariants will not be invalidated.
    T* data() noexcept {
        return uarr.get();
    }


    // Destructor
    ~Vector() = default;
};

#endif