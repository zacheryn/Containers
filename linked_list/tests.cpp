#define BOOST_TEST_MODULE linked_list
#include <boost/test/included/unit_test.hpp>
#include "Linked_List.hpp"


BOOST_AUTO_TEST_CASE(add_elements){
    // Initialize list
    List<int> l;
    for(int i = 0; i < 10; ++i){
        l.push_back(i);
    }

    // Check for the correct size
    BOOST_TEST(l.size() == 10);

    // Check elements are in the correct order
    for(int i = 0; i < 10; ++i){
        BOOST_TEST(l.at(i) == i);
    }
}


BOOST_AUTO_TEST_CASE(size_constructor){
    // Initialize List
    List<int> l(10);


    // Ensure the correct size
    BOOST_TEST(l.size() == 10);
}


BOOST_AUTO_TEST_CASE(default_value_constructor){
    // Initialize List
    List<std::size_t> l(50, 51);

    // Check that size is correct
    BOOST_TEST(l.size() == 50);

    // Check that all nodes have the correct value (also checks some iterator functionality)
    for(auto it = l.begin(); it != l.end(); ++it){
        BOOST_TEST(*it == 51);
    }
}


BOOST_AUTO_TEST_CASE(push_front){
    // Initialize List
    List<std::size_t> l;
    for(std::size_t i = 0; i < 10; ++i) l.push_front(i);

    // Check that size is correct
    BOOST_TEST(l.size() == 10);

    // Check that all nodes have the correct value (also checks some iterator functionality)
    std::size_t i = 0;
    for(auto it = l.begin(); it != l.end(); ++it){
        BOOST_TEST(*it == (9 - i));
        ++i;
    }
}


BOOST_AUTO_TEST_CASE(pop_elements){
    // Initialize List
    List<std::size_t> l;
    for(std::size_t i = 0; i < 10; ++i) l.push_back(i);

    // Check that size is correct
    BOOST_TEST(l.size() == 10);

    // Pop the back two elements
    l.pop_back();
    l.pop_back();

    // Check that size is correct
    BOOST_TEST(l.size() == 8);

    // Check that it was actually the last two nodes that are gone
    for(std::size_t i = 0; i < l.size(); ++i){
        BOOST_TEST(l[i] == i);
    }

    // Pop the front two elements
    l.pop_front();
    l.pop_front();

    // Check that size is correct
    BOOST_TEST(l.size() == 6);

    // Check that it was actually the first two nodes that are gone
    for(std::size_t i = 0; i < l.size(); ++i){
        BOOST_TEST(l[i] == i + 2);
    }
}


BOOST_AUTO_TEST_CASE(insert){
    // Initialize List
    List<std::size_t> l;
    for(std::size_t i = 0; i < 10; ++i) l.push_back(i);

    // Check that size is correct
    BOOST_TEST(l.size() == 10);

    // Insert an element in the middle of the list
    auto it = l.begin();    // 0
    ++it;                   // 1
    ++it;                   // 2
    ++it;                   // 3
    ++it;                   // 4
    l.insert(it, 11);

    // Check that size is correct
    BOOST_TEST(l.size() == 11);

    // Check that the inserted element is in the correct position
    BOOST_TEST(l.at(4) == 11);
}
