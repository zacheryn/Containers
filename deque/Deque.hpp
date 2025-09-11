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
private:

    // The internal blocks for the deque structure
    struct Block{
        static constexpr std::size_t BLOCKSIZE = 16;

        std::unique_ptr<T[]> data;
        T* overflow = nullptr;
        std::size_t size;
        std::size_t first_offset;
        bool is_full;

        // Default constructor
        Block() noexcept
        : data{new T[BLOCKSIZE]}
        , size{0}
        , first_offset{0}
        , is_full{false}
        {}

        // Construct the block to be filled with the given value
        Block(const T& val)
        : data{new T[BLOCKSIZE]}
        , size{0}
        , first_offset{0}
        , is_full{false} {
            static_assert(std::is_copy_constructible_v<T>);
            for(std::size_t i = 0; i < BLOCKSIZE; ++i){
                emplace_back(val);
            }
        }

        // Construct the block to be filled with _size of the given value
        Block(const T& val, const std::size_t _size)
        : data{new T[BLOCKSIZE]}
        , size{0}
        , first_offset{0} {
            static_assert(std::is_copy_constructible_v<T>);
            if(_size > BLOCKSIZE) throw std::out_of_range("Too many values for the Block. Max of 16");
            for(std::size_t i = 0; i < _size; ++i){
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

        // Returns true if you can insert into the front of the block
        bool check_front() const noexcept {
            return first_offset > 0;
        }

        // Returns true if you can insert into thee back of the block
        bool check_back() const noexcept {
            return (first_offset + size) < BLOCKSIZE;
        }

        // Create an element in place at the front of the block
        template<class... Args>
        void emplace_front(Args&&... args){
            if(!check_front()) throw std::out_of_range("No space left in the front of Block");
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
        bool emplace(const std::size_t _pos, Args&&... args){
            if(_pos >= size) throw std::out_of_range("Cannot add element outside of Block");
            T temp(std::forward<Args>(args)...);
            if(first_offset > 0){
                for(std::size_t i = first_offset + _pos - 1; i >= first_offset; --i){
                    std::swap(temp, data[i]);
                }
                new(data.get() + (first_offset - 1)) T(temp);
                --first_offset;
                ++size;
            }else{
                for(std::size_t i = _pos; i < size; ++i){
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
        T& at(const std::size_t _pos){
            if(_pos > size) throw std::out_of_range("Cannot index outside of used elements in the Block");
            return data.at(_pos + first_offset);
        }

        // Returns a const reference to the specified position
        // Will throw an exception if out of range
        const T& at(const std::size_t _pos) const {
            if(_pos >size) throw std::out_of_range("Cannot index outside of used elements in the Block");
            return data.at(_pos + first_offset);
        }

        // Returns a reference to the specified position
        // Will not throw any exceptions
        T& operator[](const std::size_t _pos) noexcept {
            return data[_pos + first_offset];
        }

        // Returns a const reference to the specified position
        // Will not throw any exceptions
        const T& operator[](const std::size_t _pos) const noexcept {
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

        // Move assignment
        Block& operator=(Block&& other) noexcept {
            data = std::move(other.data);
            clear_overflow();
            overflow = std::move(other.overflow);
            size = std::move(other.size);
            first_offset = std::move(other.first_offset);
            is_full = std::move(other.is_full);
        }

        ~Block(){
            clear_overflow();
        }
    };

    std::size_t Size;                       // Number of elements
    std::size_t blocks_total;               // Total number of allocated Blocks
    std::size_t blocks_used;                // Number of Blocks currently in use
    std::size_t first_offset;               // Offset to the first used block
    std::unique_ptr<Block[]> data;            // Array of Blocks

    // Double the number of allocated blocks to the front of the deque
    // REQUIRES first_offset = 0
    void grow_front() noexcept {
        if(blocks_total == 0) blocks_total = 1;
        std::unique_ptr<Block[]> temp(new T[blocks_total * 2]);
        for(std::size_t i = 0; i < blocks_used; ++i){
            temp[blocks_total + i] = std::move(data[i]);
        }
        first_offset = blocks_total;
        blocks_total *= 2;
    }

    // Double the number of allocated blocks to the back of the deque
    void grow_back() noexcept {
        if(blocks_total == 0) blocks_total = 1;
        std::unique_ptr<Block[]> temp(new T[blocks_total * 2]);
        for(std::size_t i = 0; i < blocks_used && (first_offset + i) < blocks_total; ++i){
            temp[first_offset + i] = std::move(data[first_offset + i]);
        }
        blocks_total *= 2;
    }

public:

    // Default constructor
    Deque()
    : data{nullptr}
    , blocks{0}
    , Size{0}
    {}

    // Size constructor
    // Creates a Deque of size _size
    Deque(std::size_t _size)
    : data{new Block[((_size + Block::BLOCKSIZE - 1) / Block::BLOCKSIZE)]}
    , blocks{(_size + Block::BLOCKSIZE - 1) / Block::BLOCKSIZE}
    , Size{_size}
    {}

    // Returns the number of elements in the deque
    std::size_t size() const noexcept {
        return Size;
    }

    // Returns true if there are no elements in the deque
    bool empty() const noexcept {
        return size() == 0;
    }

    // Returns the capacity of the underlying storage currently allocated
    std::size_t capacity() const noexcept {
        return blocks_total * Block::BLOCKSIZE;
    }

    ~Deque() = default;

};

#endif