#define BOOST_TEST_MODULE vector
#include <boost/test/included/unit_test.hpp>
#include "Vector.hpp"


BOOST_AUTO_TEST_CASE(add_ints){
    // Default initialization
    Vector<int> vec;

    // Add ten ints
    constexpr int expected_size = 10;
    for(int i = 0; i < expected_size; ++i){
        vec.push_back(i);
    }

    // Confirm size and capacity
    BOOST_TEST(vec.size() == 10);
    BOOST_TEST(vec.capacity() == 16);

    // Check that the elements are still in the correct order
    std::size_t idx = 0;
    for(int i = 0; i < expected_size; ++i, ++idx){
        BOOST_TEST(vec.at(idx) == i);
        BOOST_TEST(vec[idx] == i);
    }
}


BOOST_AUTO_TEST_CASE(edit_elements){
    // Initialize vector
    Vector<std::size_t> vec;
    constexpr std::size_t size = 10;
    for(std::size_t i = 0; i < size; ++i){
        vec.push_back(i);
    }

    // Edit elements and check that they were actually changed
    for(std::size_t i = 0; i < size; ++i){
        ++vec[i];
        BOOST_TEST(vec[i] == (i + 1));
    }
}


BOOST_AUTO_TEST_CASE(copy_constructor){
    // Create the first vector with some elements
    Vector<std::size_t> vec1;
    constexpr std::size_t size = 10;
    for(std::size_t i = 0; i < size; ++i){
        vec1.push_back(i);
    }

    // Create the second vector using the copy constructor
    Vector<std::size_t> vec2(vec1);

    // Check size and capacity
    BOOST_TEST(vec2.size() == vec1.size());
    BOOST_TEST(vec2.capacity() == vec1.capacity());

    // Check elments
    for(std::size_t i = 0; i < size; ++i){
        BOOST_TEST(vec2[i] == i);
    }

    // Ensure the elements are independent (not shallow copy)
    for(std::size_t i = 0; i < size; ++i){
        ++vec1[i];
        BOOST_TEST(vec1[i] != vec2[i]);
    }
}


BOOST_AUTO_TEST_CASE(size_constructor){
    // Initialize vector with siz, but no default value
    constexpr std::size_t expected_size = 10;
    Vector<int> vec(expected_size);

    // Check size and capacity
    BOOST_TEST(vec.size() == 10);
    BOOST_TEST(vec.capacity() == 10);

    // Loop through the vector
    std::size_t size = 0;
    for( [[maybe_unused]] const int i : vec){
        ++size;
    }

    // Ensure that the vector has 10 traversable elements
    BOOST_TEST(size == expected_size);
}


BOOST_AUTO_TEST_CASE(size_with_value_constructor){
    // Initialize vector
    constexpr std::size_t expected_size = 10;
    constexpr std::size_t expected_value = 1;
    Vector<std::size_t> vec(expected_size, expected_value);

    // Check size and capacity
    BOOST_TEST(vec.size() == 10);
    BOOST_TEST(vec.capacity() == 10);

    // Make sure all elements are the correct value
    for(const std::size_t i : vec){
        BOOST_TEST(i == 1);
    }
}


BOOST_AUTO_TEST_CASE(pop_elements){
    // Initialize vector
    Vector<std::size_t> vec;
    constexpr std::size_t expected_size = 16;
    constexpr std::size_t expected_removed = 5;
    for(std::size_t i = 0; i < expected_size; ++i){
        vec.push_back(i);
    }

    // Remove five elements
    for(std::size_t _ = 0; _ < expected_removed; ++_){
        vec.pop_back();
    }

    // Check that vector has correct size and capacity
    BOOST_TEST(vec.size() == (expected_size - expected_removed));
    BOOST_TEST(vec.capacity() == expected_size);

    // Add more elements to ensure elements update properly
    for(std::size_t i = 0; i < expected_removed; ++i){
        vec.push_back(i);
    }
    for(std::size_t i = 0; i < expected_removed; ++i){
        BOOST_TEST(vec[i] == vec[i + 11]);
    }
}


BOOST_AUTO_TEST_CASE(clear_vec){
    // Initialize vector
    Vector<std::size_t> vec;
    constexpr std::size_t expected_size = 10;
    for(std::size_t i = 0; i < expected_size; ++i){
        vec.push_back(i);
    }

    // Ensure vector has correct size and capacity
    BOOST_TEST(vec.size() == 10);
    BOOST_TEST(vec.capacity() == 16);

    // Clear vector
    vec.clear();

    // Check size and capacity
    BOOST_TEST(vec.size() == 0);
    BOOST_TEST(vec.empty());
    BOOST_TEST(vec.capacity() == 16);
}


BOOST_AUTO_TEST_CASE(emplace_back_construct){
    // Create a simple struct for testing
    struct A{
        int a;
        int b;
        char c;

        A() :
        a{-1}, b{-1}, c{'A'} {}

        A(const int _a, const int _b, const char _c) :
        a{_a}, b{_b}, c{_c} {}
    };

    // Initialize vector using emplace_back()
    Vector<A> vec;
    constexpr int expected_size = 10;
    char c = 0;
    for(int i = 0; i < expected_size; ++i, ++c){
        vec.emplace_back(i, i * 2, static_cast<char>('a' + c));
    }

    // Check size and capacity
    BOOST_TEST(vec.size() == 10);
    BOOST_TEST(vec.capacity() == 16);

    // Check the 10 elements
    std::size_t idx = 0;
    for(int i = 0; i < expected_size; ++i, ++idx){
        BOOST_TEST(vec[idx].a == i);
        BOOST_TEST(vec[idx].b == (i * 2));
        BOOST_TEST(vec[idx].c == ('a' + i));
    }
}


BOOST_AUTO_TEST_CASE(iterator_basics){
    // Initialize vector with the alphabet (lowercase)
    Vector<char> vec;
    constexpr char expected_size = 26;
    for(char i = 0; i < expected_size; ++i){
        vec.push_back(static_cast<char>('a' + i));
    }

    // Make sure size and capacity are correct
    BOOST_TEST(vec.size() == 26);
    BOOST_TEST(vec.capacity() == 32);

    // Check that the begin is correct
    BOOST_TEST(*vec.begin() == 'a');

    // Check that iterators can be used to loop through vector
    std::size_t i = 0;
    for(auto it = vec.begin(); it != vec.end(); ++it){
        BOOST_TEST(*it == ('a' + i));
        BOOST_TEST(*it == vec[i]);
        ++i;
    }

    // Ensure iterators can be decremented
    auto it1 = vec.begin();
    auto it2 = vec.begin();
    bool val = false; // For simplicity in compilation

    val = it1 == it2;
    BOOST_TEST(val);
    ++it2;
    val = it1 != it2;
    BOOST_TEST(val);
    --it2;
    val = it1 == it2;
    BOOST_TEST(val);

    // // Check that iterators check internal pointers rather than value
    ++it2;
    *it2 = *it1;
    val = it1 != it2;
    BOOST_TEST(val);
    BOOST_TEST(*it1 == *it2);
}


BOOST_AUTO_TEST_CASE(range_based_for){
    // Initialize vector
    Vector<std::size_t> vec;
    for(std::size_t i = 0; i < 10; ++i){
        vec.push_back(i);
    }

    // Confirm correct size and capacity
    BOOST_TEST(vec.size() == 10);
    BOOST_TEST(vec.capacity() == 16);

    // Use a range based for loop to traverse the vector
    std::size_t i = 0; // For testing
    for(const std::size_t j : vec){
        BOOST_TEST(j == i);
        BOOST_TEST(j == vec[i]);
        ++i;
    }
}


BOOST_AUTO_TEST_CASE(iterator_assignments){
    // Initialize vector with the alphabet (lowercase)
    Vector<char> vec;
    for(char i = 0; i < 26; ++i){
        vec.push_back(static_cast<char>('a' + i));
    }

    // Confirm correct size and capacity
    BOOST_TEST(vec.size() == 26);
    BOOST_TEST(vec.capacity() == 32);

    // Update all elements using iterators (uppercase alphabet)
    char i = 0;
    for(auto it = vec.begin(); it != vec.end(); ++it){
        *it = static_cast<char>('A' + i);
        ++i;
    }

    // Check two things:
    //  1. Capable of range based for loops
    //  2. Letters were properly updated
    i = 0;
    for(const char c : vec){
        BOOST_TEST(c == ('A' + i));
        ++i;
    }
}


BOOST_AUTO_TEST_CASE(front_back){
    // Initialize vector
    Vector<std::size_t> vec;
    for(std::size_t i = 0; i < 100; ++i){
        vec.push_back(i);
    }

    // Check size and capacity
    BOOST_TEST(vec.size() == 100);
    BOOST_TEST(vec.capacity() == 128);

    // Check front and back values
    BOOST_TEST(vec.front() == 0);
    BOOST_TEST(vec.front() == vec[0]);
    BOOST_TEST(vec.back() == 99);
    BOOST_TEST(vec.back() == vec[99]);

    // Check that they can be reassigned
    vec.front() = 100;
    vec.back() = 101;

    BOOST_TEST(vec.front() == 100);
    BOOST_TEST(vec.front() == vec[0]);
    BOOST_TEST(vec.back() == 101);
    BOOST_TEST(vec.back() == vec[99]);
}


BOOST_AUTO_TEST_CASE(copy){
    // Initialize vector
    Vector<std::size_t> vec;
    for(std::size_t i = 0; i < 100; ++i){
        vec.push_back(i);
    }

    // Check size and capacity
    BOOST_TEST(vec.size() == 100);
    BOOST_TEST(vec.capacity() == 128);

    // Create second Vector via copy constructor
    Vector<std::size_t> vec_copy_con(vec);

    // Check that the copy is correct
    BOOST_TEST(vec_copy_con.size() == 100);
    BOOST_TEST(vec_copy_con.capacity() == 128);
    for(std::size_t i = 0; i < 100; ++i){
        BOOST_TEST(vec_copy_con[i] == vec[i]);
        BOOST_TEST(vec_copy_con[i] == i);
    }

    // Create third vector through copy assignment
    Vector<std::size_t> vec_copy_assign;
    vec_copy_assign = vec;

    // Check that the copy is correct
    BOOST_TEST(vec_copy_assign.size() == 100);
    BOOST_TEST(vec_copy_assign.capacity() == 128);
    for(std::size_t i = 0; i < 100; ++i){
        BOOST_TEST(vec_copy_assign[i] == vec[i]);
        BOOST_TEST(vec_copy_assign[i] == i);
    }

    // Change original to make sure they are both deep copies
    std::reverse(vec.begin(), vec.end());

    // Check values
    for(std::size_t i = 0; i < 100; ++i){
        BOOST_TEST(vec_copy_con[i] == i);
        BOOST_TEST(vec_copy_assign[i] == i);
        BOOST_TEST(vec[99 - i] == i);
    }
}


BOOST_AUTO_TEST_CASE(move){
    // Initialize vector
    Vector<std::size_t> vec;
    for(std::size_t i = 0; i < 100; ++i){
        vec.push_back(i);
    }

    // Check size and capacity
    BOOST_TEST(vec.size() == 100);
    BOOST_TEST(vec.capacity() == 128);

    // Track the underlying array address to check if it was moved
    std::size_t* p = vec.data();

    // Create second Vector via move constructor
    Vector<std::size_t> vec_move_con(std::move(vec));

    BOOST_TEST(vec_move_con.size() == 100);
    BOOST_TEST(vec_move_con.capacity() == 128);
    BOOST_TEST(vec_move_con.data() == p);

    for(std::size_t i = 0; i < 100; ++i){
        BOOST_TEST(vec_move_con[i] == i);
    }

    // Create third Vector via move assignment
    Vector<std::size_t> vec_move_assign;
    vec_move_assign = std::move(vec_move_con);

    BOOST_TEST(vec_move_assign.size() == 100);
    BOOST_TEST(vec_move_assign.capacity() == 128);
    BOOST_TEST(vec_move_assign.data() == p);

    for(std::size_t i = 0; i < 100; ++i){
        BOOST_TEST(vec_move_assign[i] == i);
    }
}

BOOST_AUTO_TEST_CASE(shrink_to_fit){
    // Initialize vector
    Vector<std::size_t> vec;
    for(std::size_t i = 0; i < 100; ++i){
        vec.push_back(i);
    }

    // Check size and capacity
    BOOST_TEST(vec.size() == 100);
    BOOST_TEST(vec.capacity() == 128);

    vec.shrink_to_fit();

    BOOST_TEST(vec.size() == 100);
    BOOST_TEST(vec.capacity() == 100);

    for(std::size_t i = 0; i < 100; ++i){
        BOOST_TEST(vec[i] == i);
    }
}


BOOST_AUTO_TEST_CASE(resize){
    // Initialize vector
    Vector<std::size_t> vec;
    for(std::size_t i = 0; i < 10; ++i){
        vec.push_back(i);
    }

    // Check size and capacity
    BOOST_TEST(vec.size() == 10);
    BOOST_TEST(vec.capacity() == 16);

    vec.resize(20);

    BOOST_TEST(vec.size() == 20);
    BOOST_TEST(vec.capacity() == 20);

    for(std::size_t i = 10; i < 20; ++i){
        BOOST_TEST(vec[i] == 0);
    }

    vec.resize(5);

    BOOST_TEST(vec.size() == 5);
    BOOST_TEST(vec.capacity() == 5);

    for(std::size_t i = 0; i < 5; ++i){
        BOOST_TEST(vec[i] == i);
    }
}


BOOST_AUTO_TEST_CASE(reserve){
    // Initialize vector
    Vector<std::size_t> vec;
    for(std::size_t i = 0; i < 10; ++i){
        vec.push_back(i);
    }

    // Check size and capacity
    BOOST_TEST(vec.size() == 10);
    BOOST_TEST(vec.capacity() == 16);

    vec.reserve(100);

    // Check size and capacity
    BOOST_TEST(vec.size() == 10);
    BOOST_TEST(vec.capacity() == 100);

    for(std::size_t i = 0; i < 10; ++i){
        BOOST_TEST(vec[i] == i);
    }
}