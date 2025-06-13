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

        std::array<T, BLOCKSIZE> data;
        T* overflow = nullptr;
        std::size_t size;
        std::size_t first_offset;
        bool is_full;
        bool back_overflow = false;

        // Default constructor
        Block() noexcept
        : size{0}
        , first_offset{0}
        , is_full{false}
        {}

        // Construct the block to be filled with the given value
        Block(const T& val)
        : size{0}
        , first_offset{0} {
            static_assert(std::is_copy_constructible_v<T>);
            for(std::size_t i = 0; i < BLOCKSIZE; ++i){
                emplace_back(val);
            }
        }

        // Construct the block to be filled with _size of the given value
        Block(const T& val, const std::size_t _size)
        : size{0}
        , first_offset{0} {
            static_assert(std::is_copy_constructible_v<T>);
            if(_size > BLOCKSIZE) throw std::out_of_range("Too many values for the Block.  Max of 16");
            for(std::size_t i = 0; i < _size; ++i){
                emplace_back(val);
            }
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
            new(data.data + (first_offset - 1)) T(std::forward<Args>(args)...);
            ++size;
            --first_offset;
            if(size == BLOCKSIZE) is_full = true;
        }

        // Create an element in place at the back of the block
        template<class... Args>
        void emplace_back(Args&&... args){
            if(!check_back()) throw std::out_of_range("No space left in the back of Block");
            new(data.data + (size + first_offset)) T(std::forward<Args>(args)...);
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
                new(data.data + (first_offset - 1)) T(temp);
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
                new(data.data + size) T(temp);
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

        ~Block(){
            clear_overflow();
        }
    };

    std::size_t Size;               // Number of elements
    std::size_t blocks;             // Number of Blocks
    std::unique_ptr<Block> data;    // Array of Blocks

public:

};

#endif
