CC := gcc
CFLAGS := -Wall -pedantic
LDFLAGS := -ldl
INCLUDE := -I./malloc-allocation-tracker/include
LIB := libmallocwrap.a

list: malloc_wrapper compile_list compile_test
	$(CC) $(CFLAGS) test.o symtablelist.o $(LIB) -o list.exe $(LDFLAGS)

compile_list: tests/symtablelist.c tests/symtable.h
	$(CC) $(CFLAGS) -ansi $(INCLUDE) -c tests/symtablelist.c

hash: malloc_wrapper compile_hash compile_test
	$(CC) $(CFLAGS) $(INCLUDE) test.o symtablehash.o $(LIB) -o hash.exe $(LDFLAGS)

compile_hash:
	$(CC) $(CFLAGS) -ansi $(INCLUDE) -c tests/symtablehash.c

malloc_wrapper:
	$(MAKE) -C malloc-allocation-tracker/
	mv malloc-allocation-tracker/libmallocwrap.a .

compile_test: tests/test.c
	$(CC) $(CFLGAS) $(INCLUDE) -c tests/test.c

clean: 
	rm -f *.exe
	rm -f *.o
	rm -f *.a
	$(MAKE) -C malloc-allocation-tracker/ clean
