# Assignment 3 Test Cases

## Requirements
* gcc
* make
* ar

## How to run
0. Place your files (symtablelist.c, symtablehash.c, symtable.h) in the tests directory
1. Run `make list` to compile the list test
2. Run `make hash` to compile the hash test
3. Execute the test with `./list` or `./hash`
4. Run `make clean` to remove all the mess

## Details
Instead of using `valgrind` or similar tools, I have implemented a simple memory leak detector (a malloc wrapper). 
It is not perfect, but it is good enough for this assignment. It (will?) print out the memory leaks at the end of each test.