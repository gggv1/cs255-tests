/* Symbol Table test program
 *
 * File: testsymtab.c
 *
 * @author : Alexandros Antonakakis <csd4802>
 *
 * This file contains functions that test the symboltable, run along test.sh
 *
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "alloc_list.h"
#include "simpletest.h"
#include "symtable.h"

extern alloc_list *allocs;

void pfApply(__attribute__((unused)) const char *pcKey, void *pvValue,
             void *pvExtra) {
  *(char *)pvValue += *(char *)pvExtra;
}

void check_leaks(void) {
  if (allocs == NULL) {
    return;
  }

  alloc_list *leaks = allocs;
  void *MEMCHECK = NULL;

  TEST_ASSERT(MEMCHECK);

  fprintf(stderr, "\n\n");

  while (leaks != NULL) {
    fprintf(stderr, RED"Memory leak"RESET" of %zu bytes at %p\n", leaks->size,
            (void *)leaks->ptr);

    fprintf(stderr, "Individual bytes:\n");
    char *ptr = (char *)leaks->ptr;
    for (size_t i = 0; i < leaks->size; i++) {
      fprintf(stderr, "%02x ", (unsigned char)ptr[i]);
    }

    fprintf(stderr, "\nASCII:\n");
    for (size_t i = 0; i < leaks->size; i++) {
      if (isprint(ptr[i])) {
        fprintf(stderr, "%c ", ptr[i]);
      }
    }
    fprintf(stderr, "\n\n");

    fprintf(stderr, "stack trace:\n");
    print_stack_trace(leaks);
    fprintf(stderr, "\n\n");

    leaks = leaks->next;
  }
}

void test_new(void) {
  SymTable_T oSymTable = SymTable_new();

  fprintf(stderr, "test_new():\n");

  TEST_ASSERT(oSymTable != NULL);

  SymTable_free(oSymTable);
  check_leaks();
  alloc_list_destroy(allocs);
  allocs = NULL;
}

void test_put(void) {
  char *word     = "abcdfghijklmnopqrstuvwxyz";
  char  buff[10] = "";

  SymTable_T oSymTable = SymTable_new();

  fprintf(stderr, "test_put():\n");

  TEST_ASSERT(SymTable_put(oSymTable, "Hello", "world") == 1);
  TEST_ASSERT(SymTable_put(oSymTable, "Hello", "world") == 0);

  for (size_t i = 0; word[i]; i++) {
    buff[0] = word[i];
    TEST_ASSERT(SymTable_put(oSymTable, buff, buff) == 1);
  }

  SymTable_free(oSymTable);
  check_leaks();
  alloc_list_destroy(allocs);
  allocs = NULL;
}

void test_contains(void) {
  SymTable_T oSymTable = SymTable_new();

  fprintf(stderr, "test_contains():\n");

  (void)SymTable_put(oSymTable, "Hello", "world");
  (void)SymTable_put(oSymTable, "lalala", "fafafa");

  TEST_ASSERT(SymTable_contains(oSymTable, "Hello") == 1);
  TEST_ASSERT(SymTable_contains(oSymTable, "lalala") == 1);

  TEST_ASSERT(SymTable_contains(oSymTable, "world") == 0);
  TEST_ASSERT(SymTable_contains(oSymTable, "fafafa") == 0);

  SymTable_free(oSymTable);
  check_leaks();
  alloc_list_destroy(allocs);
  allocs = NULL;
      
}

void test_length(void) {
  SymTable_T oSymTable = SymTable_new();

  fprintf(stderr, "test_length():\n");

  TEST_ASSERT(SymTable_getLength(oSymTable) == 0);

  (void)SymTable_put(oSymTable, "Hello", "world");
  (void)SymTable_put(oSymTable, "lololo", "hahaha");
  (void)SymTable_put(oSymTable, "why am i doing this?", "idk");

  TEST_ASSERT(SymTable_getLength(oSymTable) == 3);

  SymTable_free(oSymTable);
  check_leaks();
  alloc_list_destroy(allocs);
  allocs = NULL;
}

void test_remove(void) {
  SymTable_T oSymTable = SymTable_new();

  fprintf(stderr, "test_remove():\n");

  TEST_ASSERT(SymTable_remove(oSymTable, "empty") == 0); /* empty */

  (void)SymTable_put(oSymTable, "Hello", "world");
  (void)SymTable_put(oSymTable, "lololo", "hahaha");
  (void)SymTable_put(oSymTable, "why am i doing this?", "idk");

  TEST_ASSERT(SymTable_remove(oSymTable, "Hello") == 1);
  TEST_ASSERT(SymTable_remove(oSymTable, "lololo") == 1);

  TEST_ASSERT(SymTable_getLength(oSymTable) == 1); /* length after remove */

  TEST_ASSERT(SymTable_remove(oSymTable, "why am i doing this?") == 1);

  TEST_ASSERT(SymTable_remove(oSymTable, "Hello") == 0); /* already removed */

  SymTable_free(oSymTable);
  check_leaks();
  alloc_list_destroy(allocs);
  allocs = NULL;
}

void test_get(void) {
  SymTable_T oSymTable = SymTable_new();

  fprintf(stderr, "test_get():\n");

  TEST_ASSERT(SymTable_get(oSymTable, "empty") == NULL); /* empty */

  (void)SymTable_put(oSymTable, "Hello", "world");
  (void)SymTable_put(oSymTable, "lololo", "hahaha");
  (void)SymTable_put(oSymTable, "why am i doing this?", "idk");

  TEST_ASSERT_EQUAL_STRING(SymTable_get(oSymTable, "Hello"), "world");
  TEST_ASSERT_EQUAL_STRING(SymTable_get(oSymTable, "lololo"), "hahaha");
  TEST_ASSERT_EQUAL_STRING(SymTable_get(oSymTable, "why am i doing this?"),
                           "idk");

  SymTable_free(oSymTable);
  check_leaks();
  alloc_list_destroy(allocs);
  allocs = NULL;
}

void test_collisions(void) {
  SymTable_T oSymTable = SymTable_new();

  fprintf(stderr, "test_collisions():\n");

  (void)SymTable_put(oSymTable, "AAS", "AAS");
  (void)SymTable_put(oSymTable, "ABAFT", "ABAFT");
  (void)SymTable_put(oSymTable, "SANGUINING", "SANGUINING");
  (void)SymTable_put(oSymTable, "HOMEOPATH", "HOMEOPATH");

  TEST_ASSERT(SymTable_getLength(oSymTable) == 4);

  SymTable_free(oSymTable);
  check_leaks();
  alloc_list_destroy(allocs);
  allocs = NULL;
}

void test_map(void) {
  SymTable_T oSymTable = SymTable_new();

  fprintf(stderr, "test_map():\n");

  char val[] = "AAS";

  (void)SymTable_put(oSymTable, "AAS", val);

  char extra = 1;

  SymTable_map(oSymTable, pfApply, &extra);
  TEST_ASSERT_EQUAL_STRING(SymTable_get(oSymTable, "AAS"), "BAS");

  SymTable_free(oSymTable);
  check_leaks();
  alloc_list_destroy(allocs);
  allocs = NULL;
}

void (*tests[])(void) = {test_new,    test_put, test_contains,   test_length,
                            test_remove, test_get, test_collisions, test_map};

int main(void) {
	  for (size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {
      tests[i]();
      fprintf(stderr, "---------------------------------------------\n");
      TEST_RESET();
	  }

    TEST_PRINT_SUMMARY();

}
