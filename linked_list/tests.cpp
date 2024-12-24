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
