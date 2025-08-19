# Vector

This is a partially simplified version of `std::vector` in the stl along with a few test cases for it written using Boost's [unit test framework](https://www.boost.org/doc/libs/latest/libs/test/doc/html/index.html).

# Members

## Private Members

### Variables

`std::size_t Size`: Stores the number of elements actively stored in the vector.

`std::size_t Capacity`: Stores the size of the underlying array.

`std::unique_ptr<T[]> uarr`: The pointer to the underlying array.

### Functions

`void grow()`: Allocates a new array of double the previous capacity and moves all elements over.

### Structs/Classes

There are no private custom structs or classes.

## Public Members

### Variables

There are no public variables

### Functions

`Vector() noexcept`: The defualt constuctor. Does not allocate any memory.

`Vector(const std::size_t _size) noexcept`: Creates a vector of `_size` with the default value.

`Vector(const std::size_t _size, const T& elt)`: Creates a vector of `_size` with all elements being a copy of elt.

`Vector(const Vector<T>& other) noexcept`: A copy constructor that makes a deep copy of `other`. Runs in O(n) where n = `other.size()`.

`Vector(Vector<T>&& other) noexcept`: A move constructor which moves the internals of `other` to `this` via `std::move`. Runs in O(1) time.

`Vector<T>& operator=(const Vector<T>& other) noexcept`: Creates a deep copy of `other` into `this`. Runs in O(n) where n = `other.size()`.

`Vector<T>& operator=(Vector<T>&& other) noexcept`: Move assignment operator overload. Moves `other` into `this`. Runs in O(1) time.

`void emplace_back(Args&&... args)`: Creates an element in-place at the end of the vector.

`void push_back(const T& elt)`: Copies `elt` to the end of the vector.

`void push_back(T&& elt)`: Moves `elt` to the end of the vector.

`std::size_t size() const noexcept`: Returns the number of elements stored in the vector.

`std::size_t capacity() const noexcept`: Returns the total capacity of the underlying array.

`bool empty() const noexcept`: Returns true when there are no elements stored in the vector.

`T& at(const std::size_t i)`: Returns a reference to the element at `i`. Throws `std::out_of_range` exception when `i >= this->size()`.

`const T& at(const std::size_t i) const`: Returns a const reference to the element at `i`. Throws `std::out_of_range` exception when `i >= this->size()`.

`T& front()`: Returns a reference to the first element in the vector. Throws `std::out_of_range` exception when the vector is empty.

`const T& front() const`: Returns a const reference to the first element in the vector. Throws `std::out_of_range` exception when the vector is empty.

`T& back()`: Returns a reference to the last element in the vector. Throws `std::out_of_range` exception when the vector is empty.

`const T& back() const`: Returns a const reference to the last element in the vector. Throws `std::out_of_range` exception when the vector is empty.

`T& operator[](const std::size_t i) noexcept`: Returns a reference to the element at `i` in the vector. Relies of the user to not index outside of bounds.

`const T& operator[](const std::size_t i) const noexcept`: Returns a const reference to the element at `i` in the vector. Relies of the user to not index outside of bounds.

`Iterator begin() const`: Returns an iterator to the first element of the vector. Throws `std::out_of_range` exception when the vector is empty.

`Iterator end() const`: Returns an iterator to one past the last element of the vector. Throws `std::out_of_range` exception when the vector is empty.

`void pop_back()`: Removes the last element from the vector by calling its destructor and decrimenting `this->Size`. Throws `std::out_of_range` exception when the vector is empty.

`void clear()`: Removes every element of the vector while keeping the underlying array allocated.

`void shrink_to_fit() noexcept`: Shrinks the internal buffer capacity down to the number of elements. If `capacity() == size()`, then this function does nothing. If `empty()`, it frees the memory, and sets the array pointer to `nullptr`. Otherwise, allocates a new array of size `size()` and moves over all elements.

`void reserve(const std::size_t _size) noexcept`: Allocates an array of at least `_size` elements. Only affects `Capacity`. If `_size <= capacity()`, the function doesn't do anything. Otherwise, allocates an array of size `_size`, moves over all elements to the new array, and maintains the current `size()` of the Vector.

`void resize(const std::size_t _size) noexcept`: Changes the size of the Vector to match `_size`. If `_size == size()`, this function does nothing. Otherwise, allocates a new array of size `_size`, copying over the minimum of `_size` and `size()` elements into the new array. If `_size < size()`, it truncates the Vector.  If `_size > size()`, fills thee extra space with the default value. Sets both `Size` and `Capacity` to `_size`.

`T* data() noexcept`: Returns a pointer to the underlying array. Assumes the class invariants stay valid. If they are broken, it is undefined behavior.

`~Vector()`: Destructor, frees `this->arr`.

### Structs/Classes

`Iterator`: A random access iterator that is stl compliant.