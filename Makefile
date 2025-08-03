flags := -Wall -Werror -Wextra -pedantic -Wshadow -Wconversion -O3 -DNDEBUG -lboost_unit_test_framework
debug_flags:= -Wall -Werror -Wextra -pedantic -Wshadow -Wconversion -g -DDEBUG -lboost_unit_test_framework

.PHONY: all vector linked_list deque bst debug debug_vector debug_linked_list debug_deque debug_bst clean

all:
	g++ vector/Vector.hpp vector/tests.cpp $(flags) -o vector/test.exe;
	g++ linked_list/Linked_List.hpp linked_list/tests.cpp $(flags) -o linked_list/test.exe;
	g++ deque/Deque.hpp deque/tests.cpp $(flags) -o deque/test.exe;
	g++ bst/Binary_Search_Tree.hpp bst/tests.cpp $(flags) -o bst/test.exe

vector:
	g++ vector/Vector.hpp vector/tests.cpp $(flags) -o vector/test.exe;

linked_list:
	g++ linked_list/Linked_List.hpp linked_list/tests.cpp $(flags) -o linked_list/test.exe;

deque:
	g++ deque/Deque.hpp deque/tests.cpp $(flags) -o deque/test.exe;

bst:
	g++ bst/Binary_Search_Tree.hpp bst/tests.cpp $(flags) -o bst/test.exe

debug:
	g++ vector/Vector.hpp vector/tests.cpp $(debug_flags) -o vector/debug_test.exe;
	g++ linked_list/Linked_List.hpp linked_list/tests.cpp $(debug_flags) -o linked_list/debug_test.exe;
	g++ deque/Deque.hpp deque/tests.cpp $(debug_flags) -o deque/debug_test.exe;
	g++ bst/Binary_Search_Tree.hpp bst/tests.cpp $(debug_flags) -o bst/debug_test.exe

debug_vector:
	g++ vector/Vector.hpp vector/tests.cpp $(debug_flags) -o vector/debug_test.exe;

debug_linked_list:
	g++ linked_list/Linked_List.hpp linked_list/tests.cpp $(debug_flags) -o linked_list/debug_test.exe;

debug_deque:
	g++ deque/Deque.hpp deque/tests.cpp $(debug_flags) -o deque/debug_test.exe;

debug_bst:
	g++ bst/Binary_Search_Tree.hpp bst/tests.cpp $(debug_flags) -o bst/debug_test.exe

clean:
	rm -f */test.exe;