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


BOOST_AUTO_TEST_CASE(erase){
    // Initialize List
    List<std::size_t> l;
    for(std::size_t i = 0; i < 10; ++i) l.push_back(i);

    // Check that size is correct
    BOOST_TEST(l.size() == 10);

    // Erase an element in the middle of the list
    auto it = l.begin();    // 0
    ++it;                   // 1
    ++it;                   // 2
    ++it;                   // 3
    ++it;                   // 4
    l.erase(it);

    // Check that size is correct
    BOOST_TEST(l.size() == 9);

    // Check that elements are correct
    std::size_t i = 0;
    for(auto it2 = l.begin(); it2 != l.end(); ++it2){
        BOOST_TEST(*it2 == i);
        ++i;
        if(i == 4) ++i;
    }
}


BOOST_AUTO_TEST_CASE(emplace){
    // Create a simple struct that has a unique constructor
    struct A{
        std::size_t one;
        std::size_t two;

        // Default constructor (emplace should NOT call this)
        A() :
        one{0}, two{0} {
            // Ensure the correct constructor is called
            BOOST_TEST(false);
        }

        // Custom constructor (what emplace should call)
        A(const std::size_t _one, const std::size_t _two) :
        one{_one}, two{_two} {
            // Ensure the correct constructor is called
            BOOST_TEST(true);
        }
    };

    // Initialize list
    List<A> l;

    // Check that size is correct
    BOOST_TEST(l.size() == 0);
    BOOST_TEST(l.empty());

    // Emplace two elements on the front
    l.emplace_front(3, 4);
    l.emplace_front(1, 2);

    // Check size
    BOOST_TEST(l.size() == 2);

    // Emplace two elements onto the back of the list
    l.emplace_back(7, 8);
    l.emplace_back(9, 10);

    // Check size
    BOOST_TEST(l.size() == 4);

    // Emplace an element in the middle of the list (index = 2)
    auto it = l.begin();
    ++it;
    ++it;
    l.emplace(it, 5, 6);

    // Check size
    BOOST_TEST(l.size() == 5);

    // Check element ordering
    std::size_t i = 1;
    for(const A& a : l){
        BOOST_TEST(a.one == i);
        BOOST_TEST(a.two == (i + 1));
        i += 2;
    }
}


BOOST_AUTO_TEST_CASE(front_back){
    // Initialize list
    List<std::size_t> l;
    for(std::size_t i = 0; i < 10; ++i) l.push_back(i);

    // Check size
    BOOST_TEST(l.size() == 10);
    BOOST_TEST(!l.empty());

    // Check front
    BOOST_TEST(l.front() == 0);

    // Check back
    BOOST_TEST(l.back() == 9);

    // Update front and back
    l.front() = 10;
    l.back() = 11;

    // Check front
    BOOST_TEST(l.front() == 10);

    // Check back
    BOOST_TEST(l.back() == 11);
}


BOOST_AUTO_TEST_CASE(clear){
    // Initialize list
    List<std::size_t> l(10, 0);

    // Check size
    BOOST_TEST(l.size() == 10);
    BOOST_TEST(!l.empty());

    // Clear the list
    l.clear();

    // Check size
    BOOST_TEST(l.size() == 0);
    BOOST_TEST(l.empty());

    // Add an element to make sure it is correctly cleared
    l.push_front(1);

    // Check size
    BOOST_TEST(l.size() == 1);
    BOOST_TEST(!l.empty());

    // Check the value
    BOOST_TEST(l.front() == l.back());
    BOOST_TEST(l.front() == 1);
    BOOST_TEST(l.back() == 1);      // Redundancy for clarity in case of error
}
