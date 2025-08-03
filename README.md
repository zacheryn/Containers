# Data Containers

Just a few data containers that I'm making to relearn them.

There is a simple bash script in the main directory that compiles the containers using `g++` named `compile`.

## Makefile

The Makefile can be used to compile all of the containers with their test cases to run the tests. The Makefile has the following commands:

```
make all
make vector
make linked_list
make deque
make bst
make debug
make debug_vector
make debug_linked_list
make debug_deque
make debug_bst
make clean
```

### make all

This compiles all of the containers, outputting their respective executables to the relevant directories.

### make vector

This compiles `Vector` with its test cases and outputs `vector/test.exe`.

### make linked_list

This compiles `Linked_List` with its test cases and outputs `linked_list/test.exe`.

### make deque

This compiles `Deque` with its test cases and outputs `deque/test.exe`.

### make debug_bst

This compiles `BST` with its test cases and outputs `bst/test.exe`.

### make debug

This compiles all of the containers with their debug build, outputting their respective executables to the relevant directories.

### make debug_vector

This compiles the debug build of `Vector` with its test cases and outputs `vector/debug_test.exe`.

### make debug_linked_list

This compiles the debug build of `Linked_List` with its test cases and outputs `linked_list/debug_test.exe`.

### make debug_deque

This compiles the debug build of `Deque` with its test cases and outputs `deque/debug_test.exe`.

### make debug_bst

This compiles the debug build of `BST` with its test cases and outputs `bst/debug_test.exe`.

### make clean

This removes all of the executables created by this script.