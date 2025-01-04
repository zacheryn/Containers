#define BOOST_TEST_MODULE binary_search_tree
#include <boost/test/included/unit_test.hpp>
#include "Binary_Search_Tree.hpp"

BOOST_AUTO_TEST_CASE(insert_values){
    BST<int> tree;

    // Make sure tree is empty
    BOOST_TEST(tree.size() == 0);
    BOOST_TEST(tree.empty());

    // Add some unique values
    tree.insert(5);
    tree.insert(7);
    tree.insert(-2);
    tree.insert(0);
    tree.insert(100);
    tree.insert(99);

    BOOST_TEST(tree.size() == 6);
    BOOST_TEST(!tree.empty());
}


BOOST_AUTO_TEST_CASE(insert_nonUnique_values){
    BST<int> tree;

    // Make sure tree is empty
    BOOST_TEST(tree.size() == 0);
    BOOST_TEST(tree.empty());

    // Add two of the same element and catch error
    try{
        tree.insert(5);
        tree.insert(5);
        BOOST_TEST(false);
    }catch(const std::domain_error& e){
        // Caught the correct error
        BOOST_TEST(true);
    }catch(const std::exception& e){
        // Some unexpected error was thrown
        std::cerr << e.what() << std::endl;
        BOOST_TEST(false);
    }
}


BOOST_AUTO_TEST_CASE(search){
    BST<int> tree;

    // Make sure tree is empty
    BOOST_TEST(tree.size() == 0);
    BOOST_TEST(tree.empty());

    // Add some unique values
    tree.insert(5);
    tree.insert(7);
    tree.insert(-2);
    tree.insert(0);
    tree.insert(100);
    tree.insert(99);

    // Check that the inserts were made
    BOOST_TEST(tree.size() == 6);
    BOOST_TEST(!tree.empty());

    // Search for values in the tree
    BOOST_TEST(tree.search(5));
    BOOST_TEST(tree.search(7));
    BOOST_TEST(tree.search(-2));
    BOOST_TEST(tree.search(0));
    BOOST_TEST(tree.search(100));
    BOOST_TEST(tree.search(99));

    // Search for values not in tree
    BOOST_TEST(!tree.search(1));
    BOOST_TEST(!tree.search(-5));
    BOOST_TEST(!tree.search(200));
}


BOOST_AUTO_TEST_CASE(remove_element){
    BST<int> tree;

    // Make sure tree is empty
    BOOST_TEST(tree.size() == 0);
    BOOST_TEST(tree.empty());

    // Add some unique values
    tree.insert(5);
    tree.insert(7);
    tree.insert(-2);
    tree.insert(-3);
    tree.insert(0);
    tree.insert(100);
    tree.insert(6);

    // Check that the inserts were made
    BOOST_TEST(tree.size() == 7);
    BOOST_TEST(!tree.empty());
    BOOST_TEST(tree.search(5));
    BOOST_TEST(tree.search(7));
    BOOST_TEST(tree.search(-2));
    BOOST_TEST(tree.search(-3));
    BOOST_TEST(tree.search(0));
    BOOST_TEST(tree.search(100));
    BOOST_TEST(tree.search(6));

    // Remove left most leaf node
    BOOST_TEST(tree.remove(-3));
    BOOST_TEST(tree.size() == 6);
    BOOST_TEST(!tree.search(-3));

    // Remove right most leaf node
    BOOST_TEST(tree.remove(100));
    BOOST_TEST(tree.size() == 5);
    BOOST_TEST(!tree.search(100));

    // Remove root node
    BOOST_TEST(tree.remove(5));
    BOOST_TEST(tree.size() == 4);
    BOOST_TEST(!tree.search(5));

    // Make sure all remaining nodes can still be found
    BOOST_TEST(tree.search(7));
    BOOST_TEST(tree.search(-2));
    BOOST_TEST(tree.search(0));
    BOOST_TEST(tree.search(6));
}
