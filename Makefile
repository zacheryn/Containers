flags := -Wall -Werror -Wextra -pedantic -Wshadow -Wconversion -O3 -DNDEBUG -lboost_unit_test_framework
debug_flags:= -Wall -Werror -Wextra -pedantic -Wshadow -Wconversion -g -DDEBUG -lboost_unit_test_framework

all:
	g++ vector/Vector.hpp vector/tests.cpp $(flags) -o vector/test.exe;
	g++ linked_list/Linked_List.hpp linked_list/tests.cpp $(flags) -o linked_list/test.exe;
	g++ deque/Deque.hpp deque/tests.cpp $(flags) -o deque/test.exe;

vector:
	g++ vector/Vector.hpp vector/tests.cpp $(flags) -o vector/test.exe;

linked_list:
	g++ linked_list/Linked_List.hpp linked_list/tests.cpp $(flags) -o linked_list/test.exe;

deque:
	g++ deque/Deque.hpp deque/tests.cpp $(flags) -o deque/test.exe;

debug:
	g++ vector/Vector.hpp vector/tests.cpp $(debug_flags) -o vector/test.exe;
	g++ linked_list/Linked_List.hpp linked_list/tests.cpp $(debug_flags) -o linked_list/test.exe;
	g++ deque/Deque.hpp deque/tests.cpp $(debug_flags) -o deque/test.exe;

debug_vector:
	g++ vector/Vector.hpp vector/tests.cpp $(debug_flags) -o vector/test.exe;

debug_linked_list:
	g++ linked_list/Linked_List.hpp linked_list/tests.cpp $(debug_flags) -o linked_list/test.exe;

debug_deque:
	g++ deque/Deque.hpp deque/tests.cpp $(debug_flags) -o deque/test.exe;

clean:
	rm -f */test.exe;