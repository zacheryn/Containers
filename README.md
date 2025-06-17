# Data Containers

Just a few data containers that I'm making to relearn them.

There is a simple bash script in the main directory that compiles the containers using `g++` named `compile`.

## Bash script

There is a help message if `compile` is called without an argument, and it only takes one argument. It accepts the follwing commands:

```
./compile vector
./compile linked_list
./compile deque
./compile all
./compile clean
```

### ./compile vector

This compiles `Vector` with its test casesand outputs `vector/test.exe`.

### ./compile linked_list

This compiles `Linked_Listr` with its test casesand outputs `linked_list/test.exe`.

### ./compile deque

This compiles `Deque` with its test casesand outputs `deque/test.exe`.

### ./compile all

This compiles all of the containers, outputting their respective executables to the relevant directories.

### ./compile clean

This removes all of the executables created by this script.