#ifndef DEQUE_HPP
#define DEQUE_HPP

#include <utility>
#include <stdexcept>
#include <array>
#include <memory>
#include <type_traits>


// A double ended queue based on std::deque from the STL
template<class T>
class Deque{
public:
    typedef std::size_t size_type;
private:

    // The internal blocks for the deque structure
    struct Block{
        static constexpr size_type BLOCKSIZE = 16;

        std::unique_ptr<T[]> data;
        T* overflow = nullptr;
        size_type size;
        size_type first_offset;
        bool is_full;

        // Default constructor
        Block() noexcept
        : data{new T[BLOCKSIZE]{}}
        , size{0}
        , first_offset{0}
        , is_full{false}
        {}

        // Construct the block to be filled with the given value
        Block(const T& val)
        : data{new T[BLOCKSIZE]{}}
        , size{0}
        , first_offset{0}
        , is_full{false} {
            static_assert(std::is_copy_constructible_v<T>);
            for(size_type i = 0; i < BLOCKSIZE; ++i){
                emplace_back(val);
            }
        }

        // Construct the block to be filled with _size of the given value
        Block(const T& val, const size_type _size)
        : data{new T[BLOCKSIZE]{}}
        , size{0}
        , first_offset{0} {
            static_assert(std::is_copy_constructible_v<T>);
            if(_size > BLOCKSIZE) throw std::out_of_range("Too many values for the Block. Max of 16");
            for(size_type i = 0; i < _size; ++i){
                emplace_back(val);
            }
        }

        // Move constructor
        Block(Block&& other)
        : data{nullptr}
        , overflow{std::move(other.overflow)}
        , size{std::move(other.size)}
        , first_offset{std::move(other.first_offset)}
        , is_full{std::move(other.is_full)} {
            data.swap(other);
        }

        // Prevent assignment for internal Blocks
        Block& operator=(const Block&) = delete;
        Block& operator=(Block&&) = delete;

        // Returns true if you can insert into the front of the block
        bool check_front() const noexcept {
            return first_offset > 0 || size == 0;
        }

        // Returns true if you can insert into thee back of the block
        bool check_back() const noexcept {
            return (first_offset + size) < BLOCKSIZE;
        }

        // Create an element in place at the front of the block
        template<class... Args>
        void emplace_front(Args&&... args){
            if(!check_front()) throw std::out_of_range("No space left in the front of Block");
            if(size == 0) first_offset = BLOCKSIZE;
            new(data.get() + (first_offset - 1)) T(std::forward<Args>(args)...);
            ++size;
            --first_offset;
            if(size == BLOCKSIZE) is_full = true;
        }

        // Create an element in place at the back of the block
        template<class... Args>
        void emplace_back(Args&&... args){
            if(!check_back()) throw std::out_of_range("No space left in the back of Block");
            new(data.get() + (size + first_offset)) T(std::forward<Args>(args)...);
            ++size;
            if(size == BLOCKSIZE) is_full = true;
        }

        // Create an element in place at the specified location of the block
        // Returns true if there was an overflow element that needs to be allocated to another block
        // Overflow element can be obtained through the overflow member
        template<class... Args>
        bool emplace(const size_type _pos, Args&&... args){
            if(_pos >= size) throw std::out_of_range("Cannot add element outside of Block");
            T temp(std::forward<Args>(args)...);
            if(first_offset > 0){
                for(size_type i = first_offset + _pos - 1; i >= first_offset; --i){
                    std::swap(temp, data[i]);
                }
                new(data.get() + (first_offset - 1)) T(temp);
                --first_offset;
                ++size;
            }else{
                for(size_type i = _pos; i < size; ++i){
                    std::swap(temp, data[i]);
                }
                if(is_full){
                    overflow = new T(temp);
                    return true;
                }
                new(data.get() + size) T(temp);
                ++size;
            }
            if(size == BLOCKSIZE) is_full = true;

            return false;
        }

        // Clear the overflow
        void clear_overflow() noexcept {
            delete overflow;
            overflow = nullptr;
        }

        // Returns a reference to the specified position
        // Will throw an exception if out of range
        T& at(const size_type _pos){
            if(_pos > size) throw std::out_of_range("Cannot index outside of used elements in the Block");
            return data.at(_pos + first_offset);
        }

        // Returns a const reference to the specified position
        // Will throw an exception if out of range
        const T& at(const size_type _pos) const {
            if(_pos >size) throw std::out_of_range("Cannot index outside of used elements in the Block");
            return data.at(_pos + first_offset);
        }

        // Returns a reference to the specified position
        // Will throw an exception if out of range
        T& at(size_type&& _pos){
            if(_pos > size) throw std::out_of_range("Cannot index outside of used elements in the Block");
            return data.at(_pos + first_offset);
        }

        // Returns a const reference to the specified position
        // Will throw an exception if out of range
        const T& at(size_type&& _pos) const {
            if(_pos >size) throw std::out_of_range("Cannot index outside of used elements in the Block");
            return data.at(_pos + first_offset);
        }

        // Returns a reference to the specified position
        // Will not throw any exceptions
        T& operator[](const size_type _pos) noexcept {
            return data[_pos + first_offset];
        }

        // Returns a const reference to the specified position
        // Will not throw any exceptions
        const T& operator[](const size_type _pos) const noexcept {
            return data[_pos + first_offset];
        }
        
        // Returns a reference to the specified position
        // Will not throw any exceptions
        T& operator[](size_type&& _pos) noexcept {
            return data[_pos + first_offset];
        }

        // Returns a const reference to the specified position
        // Will not throw any exceptions
        const T& operator[](size_type&& _pos) const noexcept {
            return data[_pos + first_offset];
        }

        // Removes the last element from the block, freeing the memory
        // Will not throw any exceptions
        void pop_back() noexcept {
            if(size == 0) return;
            --size;
            data[first_offset + size].~T();
            if(is_full) is_full = false;
        }

        // Removes the first element from the block, freeing the memory
        // Will not throw any exceptions
        void pop_front() noexcept {
            if(size == 0) return;
            --size;
            data[first_offset].~T();
            ++first_offset;
            if(is_full) is_full = false;
        }

        // Frees all elements in the block
        void clear() noexcept {
            while(size > 0) pop_back();
            first_offset = 0;
        }

        ~Block(){
            clear_overflow();
        }
    };

    size_type Size;                       // Number of elements
    size_type blocks_total;               // Total number of allocated Blocks
    size_type blocks_used;                // Number of Blocks currently in use
    size_type first_offset;               // Offset to the first used block
    std::unique_ptr<Block[]> data;            // Array of Blocks

    // Double the number of allocated blocks to the front of the deque
    // REQUIRES first_offset = 0
    void grow_front() noexcept {
        if(blocks_total == 0) blocks_total = 1;
        std::unique_ptr<Block[]> temp(new T[blocks_total * 2]{});
        for(size_type i = 0; i < blocks_used; ++i){
            temp[blocks_total + i] = std::move(data[i]);
        }
        first_offset = blocks_total;
        blocks_total *= 2;
        data.reset(nullptr);
        data.swap(temp);
    }

    // Double the number of allocated blocks to the back of the deque
    void grow_back() noexcept {
        if(blocks_total == 0) blocks_total = 1;
        std::unique_ptr<Block[]> temp(new T[blocks_total * 2]{});
        for(size_type i = 0; i < blocks_used && (first_offset + i) < blocks_total; ++i){
            temp[first_offset + i] = std::move(data[first_offset + i]);
        }
        blocks_total *= 2;
        data.reset(nullptr);
        data.swap(temp);
    }

protected:

    // Base iterator
    template<class Access_Type>
    class IterType{
        friend class Deque;
    public:
        // Iterator traits to make the iterator stl compliant
        using iterator_category = std::random_access_iterator_tag;
        using value_type = Access_Type;
        using difference_type = std::ptrdiff_t;
        using pointer = Access_Type*;
        using reference = Access_Type&;

    protected:
        Block* b;
        size_type offset;

    public:

        // Default constructor
        IterType() noexcept
        : b{nullptr},
        offset{0}
        {}  

        // Iterator to given block and offset (Assumes pointer is safe to use)
        IterType(Block* _b, size_type _offset)
        : b{_b},
        offset{_offset}
        {
            if(offset >= Block::BLOCKSIZE)
                throw std::invalid_argument("Offset for iterator must be less than Block::BLOCKSIZE");
        }

        // Copy constructor
        IterType(const IterType<T>& other) noexcept
        : b{other.b}
        , offset{other.offset}
        {}

        // Move constructor
        IterType(IterType<T>&& other) noexcept
        : b{std::move(other.b)}
        , offset{std::move(other.offset)}
        {
            other.b = nullptr;
            other.offset = 0;
        }

        // Copy assignment
        IterType<T>& operator=(const IterType<T>& other) noexcept {
            // Guard self assignment
            if(b == other.b && offset == other.offset) return *this;

            b = other.b;
            offset = other.offset;
            return *this;
        }

        // Move assignment
        IterType<T>& operator=(IterType<T>&& other) noexcept {
            // Guard self assignment
            if(b == other.b && offset == other.offset) return *this;

            b = std::move(other.b);
            offset = std::move(other.offset);
            other.b = nullptr;
            other.offset = 0;
            return *this;
        }

        // Dereference operator overload
        reference operator*() noexcept {
            return (*b)[offset];
        }

        // Dereference operator overload
        pointer operator->() noexcept {
            return (*b)[offset];
        }

        // Access operator
        reference operator[](const size_type& i) noexcept {
            if((i + offset) >= Block::BLOCKSIZE){
                size_type _i = i - (Block::BLOCKSIZE - offset);
                return (*(b + (_i / Block::BLOCKSIZE) + 1))[(_i % Block::BLOCKSIZE)];
            }
            return (*(b + (i / Block::BLOCKSIZE)))[offset + (i % Block::BLOCKSIZE)];
        }
        reference operator[](size_type&& i) noexcept {
            if((i + offset) >= Block::BLOCKSIZE){
                size_type _i = i - (Block::BLOCKSIZE - offset);
                return (*(b + (_i / Block::BLOCKSIZE) + 1))[(_i % Block::BLOCKSIZE)];
            }
            return (*(b + (i / Block::BLOCKSIZE)))[offset + (i % Block::BLOCKSIZE)];
        }
        const reference operator[](const size_type& i) const noexcept {
            if((i + offset) >= Block::BLOCKSIZE){
                size_type _i = i - (Block::BLOCKSIZE - offset);
                return (*(b + (_i / Block::BLOCKSIZE) + 1))[(_i % Block::BLOCKSIZE)];
            }
            return (*(b + (i / Block::BLOCKSIZE)))[offset + (i % Block::BLOCKSIZE)];
        }
        const reference operator[](size_type&& i) const noexcept {
            if((i + offset) >= Block::BLOCKSIZE){
                size_type _i = i - (Block::BLOCKSIZE - offset);
                return (*(b + (_i / Block::BLOCKSIZE) + 1))[(_i % Block::BLOCKSIZE)];
            }
            return (*(b + (i / Block::BLOCKSIZE)))[offset + (i % Block::BLOCKSIZE)];
        }

        // Increment
        IterType<Access_Type>& operator++() noexcept {
            ++offset;
            if(offset >= Block::BLOCKSIZE){
                ++b;
                offset = 0;
            }
            return *this;
        }
        IterType<Access_Type> operator++(int) noexcept {
            IterType<Access_Type> temp(*this);
            ++offset;
            if(offset >= Block::BLOCKSIZE){
                ++b;
                offset = 0;
            }
            return temp;
        }

        // Decrement
        IterType<Access_Type>& operator--() noexcept {
            if(offset == 0){
                offset = Block::BLOCKSIZE - 1;
                --b;
            }else{
                --offset;
            }
            return *this;
        }
        IterType<Access_Type> operator--(int) noexcept {
            IterType<Access_Type> temp(*this);
            if(offset == 0){
                offset = Block::BLOCKSIZE - 1;
                --b;
            }else{
                --offset;
            }
            return temp;
        }

        // Compound Assignments
        IterType<Access_Type>& operator+=(const size_type& shift) noexcept {
            offset += shift;
            b += offset / Block::BLOCKSIZE;
            offset = offset % Block::BLOCKSIZE;
            return *this;
        }
        IterType<Access_Type>& operator+=(size_type&& shift) noexcept {
            offset += std::move(shift);
            b += offset / Block::BLOCKSIZE;
            offset = offset % Block::BLOCKSIZE;
            return *this;
        }
        IterType<Access_Type>& operator-=(size_type shift) noexcept {
            if(shift > offset){
                if((shift % Block::BLOCKSIZE) > 0){
                    shift -= shift % Block::BLOCKSIZE;
                    if((shift % Block::BLOCKSIZE) > offset){
                        offset = (offset + Block::BLOCKSIZE) - (shift % Block::BLOCKSIZE);
                        --b;
                    }else{
                        offset -= shift % Block::BLOCKSIZE;
                        --b;
                    }
                }
                b -= shift / Block::BLOCKSIZE;
            }else{
                offset -= shift;
            }
            return *this;
        }
        IterType<Access_Type>& operator-=(size_type&& shift) noexcept {
            size_type _shift = std::move(shift);
            if(_shift > offset){
                if((_shift % Block::BLOCKSIZE) > 0){
                    _shift -= _shift % Block::BLOCKSIZE;
                    if((_shift % Block::BLOCKSIZE) > offset){
                        offset = (offset + Block::BLOCKSIZE) - (_shift % Block::BLOCKSIZE);
                        --b;
                    }else{
                        offset -= _shift % Block::BLOCKSIZE;
                        --b;
                    }
                }
                b -= shift / Block::BLOCKSIZE;
            }else{
                offset -= _shift;
            }
            return *this;
        }

        // Addition
        friend IterType<Access_Type> operator+(IterType<Access_Type> it, const size_type& shift) noexcept {
            return std::move(it += shift);
        }
        friend IterType<Access_Type> operator+(const size_type& shift, IterType<Access_Type> it) noexcept {
            return std::move(it += shift);
        }
        friend IterType<Access_Type> operator+(IterType<Access_Type> it, size_type&& shift) noexcept {
            return std::move(it += std::move(shift));
        }
        friend IterType<Access_Type> operator+(size_type&& shift, IterType<Access_Type> it) noexcept {
            return std::move(it += std::move(shift));
        }

        // Subtraction
        friend IterType<Access_Type> operator-(IterType<Access_Type> it, const size_type& shift) noexcept {
            return std::move(it -= shift);
        }
        friend IterType<Access_Type> operator-(const size_type& shift, IterType<Access_Type> it) noexcept {
            return std::move(it -= shift);
        }
        friend IterType<Access_Type> operator-(IterType<Access_Type> it, size_type&& shift) noexcept {
            return std::move(it -= std::move(shift));
        }
        friend IterType<Access_Type> operator-(size_type&& shift, IterType<Access_Type> it) noexcept {
            return std::move(it -= std::move(shift));
        }

        // Difference
        difference_type operator-(const IterType<Access_Type>& other) const noexcept {
            return (static_cast<difference_type>(b - other.b) * Block::BLOCKSIZE)
            + static_cast<difference_type>((b.data.get() + offset) - (other.b.data.get() + other.offset));
        }

        // Equality operator
        bool operator==(const IterType<Access_Type>& other) const noexcept {
            return b == other.b && offset == other.offset;
        }

        // Inequality operator
        bool operator!=(const IterType<Access_Type>& other) const noexcept {
            return b != other.b || offset != other.offset;
        }

        // Comparison operator
        bool operator<(const IterType<Access_Type>& other) const noexcept {
            return b == other.b ? offset < other.offset : b < other.b;
        }
        bool operator<=(const IterType<Access_Type>& other) const noexcept {
            return b == other.b ? offset <= other.offset : b <= other.b;
        }
        bool operator>(const IterType<Access_Type>& other) const noexcept {
            return b == other.b ? offset < other.offset : b < other.b;
        }
        bool operator>=(const IterType<Access_Type>& other) const noexcept {
            return b == other.b ? offset <= other.offset : b <= other.b;
        }

        ~IterType() = default;
    };

public:

    // STL compliant iterator allowing mutable elements
    typedef IterType<T> iterator;

    // STL compliant const iterator ensuring elements cannot be changed
    typedef IterType<const T> const_iterator;

    // Default constructor
    constexpr Deque()
    : Size{0}
    , blocks_total{0}
    , blocks_used{0}
    , first_offset{0}
    , data{nullptr}
    {}

    // Size constructor
    // Creates a Deque of size _size
    Deque(size_type _size)
    : Size{_size}
    , blocks_total{(_size + Block::BLOCKSIZE - 1) / Block::BLOCKSIZE}
    , blocks_used{(_size + Block::BLOCKSIZE - 1) / Block::BLOCKSIZE}
    , first_offset{0}
    , data{new Block[((_size + Block::BLOCKSIZE - 1) / Block::BLOCKSIZE)]{}}
    {}

    // Returns the number of elements in the deque
    constexpr size_type size() const noexcept {
        return Size;
    }

    // Returns true if there are no elements in the deque
    constexpr bool empty() const noexcept {
        return size() == 0;
    }

    // Returns the capacity of the underlying storage currently allocated
    constexpr size_type capacity() const noexcept {
        return blocks_total * Block::BLOCKSIZE;
    }

    // Returns a reference to the specified element
    T& at(const size_type _pos){
        if(_pos > size()) throw std::out_of_range("ERROR: Cannot index outside of range.");
        return data[first_offset + (_pos / Block::BLOCKSIZE)].at(_pos % Block::BLOCKSIZE);
    }
    T& at(size_type&& _pos){
        if(_pos > size()) throw std::out_of_range("ERROR: Cannot index outside of range.");
        return data[first_offset + (_pos / Block::BLOCKSIZE)].at(_pos % Block::BLOCKSIZE);
    }

    // Returns a const reference to the specified element
    const T& at(const size_type _pos) const {
        if(_pos > size()) throw std::out_of_range("ERROR: Cannot index outside of range.");
        return data[first_offset + (_pos / Block::BLOCKSIZE)].at(_pos % Block::BLOCKSIZE);
    }
    const T& at(size_type&& _pos) const {
        if(_pos > size()) throw std::out_of_range("ERROR: Cannot index outside of range.");
        return data[first_offset + (_pos / Block::BLOCKSIZE)].at(_pos % Block::BLOCKSIZE);
    }

    // Returns a reference to the specified element without throwing any exceptions
    T& operator[](const size_type _pos) noexcept {
        return data[first_offset + (_pos / Block::BLOCKSIZE)][_pos % Block::BLOCKSIZE];
    }
    T& operator[](size_type&& _pos) noexcept {
        return data[first_offset + (_pos / Block::BLOCKSIZE)][_pos % Block::BLOCKSIZE];
    }

    // Returns a const reference to the specified element without throwing any exceptions
    const T& operator[](const size_type _pos) const noexcept {
        return data[first_offset + (_pos / Block::BLOCKSIZE)][_pos % Block::BLOCKSIZE];
    }
    const T& operator[](size_type&& _pos) const noexcept {
        return data[first_offset + (_pos / Block::BLOCKSIZE)][_pos % Block::BLOCKSIZE];
    }

    // Returns a reference to the first element in the Deque
    T& front(){
        if(empty()) throw std::out_of_range("ERROR: Cannot index outside of range.");
        return data[first_offset].at(0);
    }

    // Returns a const reference to the first element in the Deque
    const T& front() const {
        if(empty()) throw std::out_of_range("ERROR: Cannot index outside of range.");
        return data[first_offset].at(0);
    }

    // Returns a reference to the last element in the Deque
    T& back(){
        if(empty()) throw std::out_of_range("ERROR: Cannot index outside of range.");
        return data[first_offset + blocks_used - 1].at((Size - 1) % Block::BLOCKSIZE);
    }

    // Returns a const reference to the last element in the Deque
    const T& back() const {
        if(empty()) throw std::out_of_range("ERROR: Cannot index outside of range.");
        return data[first_offset + blocks_used - 1].at((Size - 1) % Block::BLOCKSIZE);
    }

    // Inserts the specified value to the front of the deque
    void push_front(const T& _val){
        emplace_front(_val);
    }
    void push_front(T&& _val){
        emplace_front(std::move(_val));
    }

    // Creates the specified value to the front of the deque in place
    template<class... Args>
    void emplace_front(Args&&... args){
        if(first_offset <= 0){
            if(empty() || !data[0].check_front()) grow_front();
        }

        if(!data[first_offset].check_front()) --first_offset;

        data[first_offset].emplace_front(std::move(args...));
        ++Size;
    }

    // Inserts the specified value to the front of the deque
    void push_back(const T& _val){
        emplace_back(_val);
    }
    void push_back(T&& _val){
        emplace_back(std::move(_val));
    }

    // Creates the specified value to the front of the deque in place
    template<class... Args>
    void emplace_back(Args&&... args){
        if((first_offset + blocks_used) >= blocks_total){
            if(empty() || !data[first_offset + blocks_used].check_back()) grow_back();
        }

        if(!data[first_offset + blocks_used].check_back()) ++blocks_used;

        data[first_offset + blocks_used].emplace_back(std::move(args...));
        ++Size;
    }

    ~Deque() = default;

};

#endif