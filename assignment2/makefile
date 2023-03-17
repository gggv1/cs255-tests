# Author: Alexandros Emmanouil Antonakakis <csd4802>
# Date: 16-03-2023
# Description: Makefile for mystring_ars.c and mystring_ptrs.c, test program

CC := gcc
CFLAGS := -Wall -Wextra -pedantic -ansi

.PHONY: all ars ptrs clean test_ars.exe test_ptrs.exe mystring_ars.o mystring_ptrs.o

check:
ifeq ($shell which $(CC),)
CC := clang
ifeq ($shell which $(CC),)
$(error "No C compiler found")
endif
endif

test_ars: test.c mystring_ars.o
	@echo ' BUILD test_ars.exe'
	@$(CC) -pthread -o $@.exe $^

test_ptrs: test.c mystring_ptrs.o
	@echo ' BUILD test_ptrs.exe'
	@$(CC) -pthread -o $@.exe $^

mystring_ars.o: mystring_ars.c mystring.h
	@echo ' COMPILE mystring_ars.c'
	@$(CC) $(CFLAGS) -c mystring_ars.c

mystring_ptrs.o: mystring_ptrs.c mystring.h
	@echo ' COMPILE mystring_ptrs.c'
	@$(CC) $(CFLAGS) -c mystring_ptrs.c

clean:
	@echo ' CLEAN'
	@rm -f *.o test_ars.exe test_ptrs.exe
