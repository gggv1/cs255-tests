/*
 * simpletest.h
 * Author: Alexandros Antonakakis <csd4802>
 * Date:   16/03/2023
 * Description: This file contains a simple testing framework for C.
 */

#ifndef SIMPLETEST_H
#define SIMPLETEST_H

#include <stdio.h>
#include <string.h>

/* a struct to keep track of the test results */

struct test {
  unsigned total;
  unsigned total_passed;
  unsigned total_failed;

  unsigned passed;
  unsigned failed;
};

static struct test test;

#define GREEN "\033[32m"
#define RED   "\033[31m"
#define RESET "\033[0m"

/* macros to make the tests easy to execute
   we just have to include this header file
*/

#define TEST_INIT()        \
  do {                     \
    test.total        = 0; \
    test.passed       = 0; \
    test.failed       = 0; \
    test.total_passed = 0; \
    test.total_failed = 0; \
  } while (0)

#define TEST_ASSERT(cond)                                        \
  do {                                                           \
    test.total++;                                                \
    fprintf(stderr, "Test %u: ", test.passed + test.failed + 1); \
    if (cond) {                                                  \
      test.passed++;                                             \
      test.total_passed++;                                       \
      fprintf(stderr, GREEN "\tPASS\n" RESET);                   \
    } else {                                                     \
      test.failed++;                                             \
      test.total_failed++;                                       \
      fprintf(stderr,                                            \
              RED "\tFAIL -> " #cond                             \
                  " in line %d (%s)"                             \
                  "\n" RESET,                                    \
              __LINE__, __FILE__);                               \
    }                                                            \
  } while (0)

#define TEST_ASSERT_EQUAL(a, b)        TEST_ASSERT((a) == (b))
#define TEST_ASSERT_NOT_EQUAL(a, b)    TEST_ASSERT((a) != (b))
#define TEST_ASSERT_NULL(a)            TEST_ASSERT((a) == NULL)
#define TEST_ASSERT_NOT_NULL(a)        TEST_ASSERT((a) != NULL)
#define TEST_ASSERT_EQUAL_STRING(a, b) TEST_ASSERT(strcmp((a), (b)) == 0)

#define TEST_ASSERT_NOT_EQUAL_STRING(a, b) TEST_ASSERT(strcmp((a), (b)) != 0)

#define TEST_PRINT_SUMMARY()                                           \
  do {                                                                 \
    fprintf(stderr, "Total: %u, Passed: %u, Failed: %u\n", test.total, \
            test.total_passed, test.total_failed);                     \
  } while (0)

#define TEST_PRINT_CURRENT_STATS()                                         \
  do {                                                                     \
    fprintf(stderr, "-> %u/%u\n", test.passed, test.passed + test.failed); \
  } while (0)

#define TEST_RESET() \
  do {               \
    test.passed = 0; \
    test.failed = 0; \
  } while (0)
#endif