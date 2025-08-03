# Linked List

A doubly linked list along with a few test cases for it written using Boost's [unit test framework](https://www.boost.org/doc/libs/latest/libs/test/doc/html/index.html).

# Members

## Private Members

### Variables

`Node* first`: A pointer to the first element in the list. Is `nullptr` when the list is empty.

`Node* last`: A pointer to the last element in the list. Is `nullptr` when the list is empty.

`std::size_t Size`: The total number of Nodes in the list.

### Functions

There are no private functions.

### Structs/Classes

`struct Node`: The container for single elements in the list. Stores the next and previous Nodes in the list, and the element itself.

## Public Members

### Variables

There are no public variables.

### Functions

`List() noexcept`: The default constructor. Does not allocate any memory onto the heap.

`List(std::size_t _size)`: Size based contructor. Allocates `_size` Nodes with their default values.

`List(std::size_t _size, const T& _elt)`: Size based contructor with a specified value. Allocates `_size` Nodes with the value `_elt`.  Assumes `_elt` is copyable.

`List(const List<T>& other)`: Copy constructor. Creates of deep copy of `other`.

`std::size_t size() const noexcept`: Returns the number of Nodes in the list.

`bool empty() const noexcept`: Returns true if the list is empty.

`Iterator begin() noexcept`: Returns an Iterator pointing to the first Node in the list (Returns an Iterator pointing to `nullptr` on empty list).

`Iterator end() noexcept`: Returns an Iterator "one past" the final Node in the list.

`void emplace(Iterator& it, Args&&... args)`: Inserts a Node at the Iterator's position, constructing the element in-place.

`void emplace_front(Args&&... args)`: Inserts a Node at the front of the list, constructing the element in-place.

`void emplace_back(Args&&... args)`: Inserts a Node at the back of the list, constructing the element in-place.

`void push_front(const T& elt)`: Inserts a Node at the front of the list from lvalue.

`void push_front(T&& elt)`: Inserts a Node at the front of the list from rvalue.

`void push_back(const T& elt)`: Inserts a Node at the back of the list from lvalue.

`void push_back(T&& elt)`: Inserts a Node at the back of the list from rvalue.

`void insert(Iterator& it, const T& elt)`: Inserts a Node at the Iterator's position from lvalue.

`void insert(Iterator& it, T&& elt)`: Inserts a Node at the Iterator's position from rvalue.

`T& at(std::size_t idx)`: Returns a reference to the element stored in the `idx`th Node. Throws `std::out_of_range` when `idx >= this->size()`.

`const T& at(std::size_t idx) const`: Returns const reference to the element stored in the `idx`th Node. Throws `std::out_of_range` when `idx >= this->size()`.

`T& operator[](std::size_t idx)`: Returns a reference to the element stored in the `idx`th Node.

`const T& operator[](std::size_t idx) const`: Returns const reference to the element stored in the `idx`th Node.

`T& front()`: Returns a reference to the element stored in the first Node. Throws `std::out_of_range` when `this->empty()`.

`const T& front() const`: Returns a const reference to the element stored in the first Node. Throws `std::out_of_range` when `this->empty()`.

`T& back()`: Returns a reference to the element stored in the last Node. Throws `std::out_of_range` when `this->empty()`.

`const T& back() const`: Returns a const reference to the element stored in the last Node. Throws `std::out_of_range` when `this->empty()`.

`void pop_front()`: Frees the first Node in the list. Throws `std::out_of_range` when `this->empty()`.

`void pop_back()`: Frees the last Node in the list. Throws `std::out_of_range` when `this->empty()`.

`void erase(Iterator& it)`: Frees the Node pointed to by `it`. Throws `std::out_of_range` when `this->empty()` or `it->node == nullptr`.

`void clear()`: Frees all Nodes in the list.

`~List()`: Destructor, Frees all Nodes in the list.

### Structs/Classes

`Iterator`: A bidirectional iterator that is stl compliant.