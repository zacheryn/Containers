#define BOOST_TEST_MODULE vector
#include <boost/test/included/unit_test.hpp>
#include <iostream>
#include "Vector.h"


BOOST_AUTO_TEST_CASE(add_ints){
    // Default initialization
    Vector<int> vec;

    // Add ten ints
    for(int i = 0; i < 10; ++i){
        vec.push_back(i);
    }

    // Confirm size and capacity
    BOOST_TEST(vec.size() == 10);
    BOOST_TEST(vec.capacity() == 16);

    // Check that the elements are still in the correct order
    for(int i = 0; i < 10; ++i){
        BOOST_TEST(vec.at(i) == i);
        BOOST_TEST(vec[i] == i);
    }
}


BOOST_AUTO_TEST_CASE(edit_elements){
    // Initialize vector
    Vector<int> vec;
    for(int i = 0; i < 10; ++i){
        vec.push_back(i);
    }

    // Edit elements and check that they were actually changed
    for(int i = 0; i < 10; ++i){
        ++vec[i];
        BOOST_TEST(vec[i] == (i + 1));
    }
}


BOOST_AUTO_TEST_CASE(copy_constructor){
    // Create the first vector with some elements
    Vector<std::size_t> vec1;
    for(std::size_t i = 0; i < 10; ++i){
        vec1.push_back(i);
    }

    // Create the second vector using the copy constructor
    Vector<std::size_t> vec2(vec1);

    // Check size and capacity
    BOOST_TEST(vec2.size() == vec1.size());
    BOOST_TEST(vec2.capacity() == vec1.capacity());

    // Check elments
    for(std::size_t i = 0; i < 10; ++i){
        BOOST_TEST(vec2[i] == i);
    }

    // Ensure the elements are independent (not shallow copy)
    for(std::size_t i = 0; i < 10; ++i){
        ++vec1[i];
        BOOST_TEST(vec1[i] != vec2[i]);
    }
}


BOOST_AUTO_TEST_CASE(pop_elements){
    // Initialize vector
    Vector<std::size_t> vec;
    for(std::size_t i = 0; i < 10; ++i){
        vec.push_back(i);
    }

    // Remove five elements
    for(std::size_t _ = 0; _ < 5; ++_){
        vec.pop_back();
    }

    // Check that vector has correct size and capacity
    BOOST_TEST(vec.size() == 5);
    BOOST_TEST(vec.capacity() == 16);

    // Add more elements to ensure elements update properly
    for(std::size_t i = 0; i < 5; ++i){
        vec.push_back(i);
    }
    for(std::size_t i = 0; i < 5; ++i){
        BOOST_TEST(vec[i] == vec[i + 5]);
    }
}


BOOST_AUTO_TEST_CASE(clear_vec){
    // Initialize vector
    Vector<std::size_t> vec;
    for(std::size_t i = 0; i < 10; ++i){
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