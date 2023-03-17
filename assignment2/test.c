/*
  * test.c
  * Author: Alexandros Antonakakis <csd4802>
  * Date:   16/03/2023
  * Description: This file contains the test cases for assignment 2, "A String Module".
*/


#include <assert.h>
#include <pthread.h>
#include <stdlib.h>

#include "mystring.h"
#include "simpletest.h"

/* re-implement assert fail to just print a message and exit the thread only */
#if __APPLE__
void __assert_rtn(__attribute__((unused)) const char *assertion,
                  __attribute__((unused)) const char *file,
                  __attribute__((unused)) int         line,
                  __attribute__((unused)) const char *function) {
  TEST_ASSERT(1);
  TEST_PRINT_CURRENT_STATS();
  fprintf(stderr, "%s\n", "----------------------------------------");
  pthread_exit(
      NULL);  /* instead of exiting the whole process we exit the thread */
}
#endif

#if __linux__
void __assert_fail(__attribute__((unused)) const char  *assertion,
                   __attribute__((unused)) const char  *file,
                   __attribute__((unused)) unsigned int line,
                   __attribute__((unused)) const char  *function) {
  TEST_ASSERT(1);
  fprintf(stderr, "%s\n", "----------------------------------------");
  pthread_exit(
      NULL);  /* instead of exiting the whole process we exit the thread */
}
#endif

void *test_length(void *arg) {
    (void)arg;

  fprintf(stderr, "test_length():\n");

  TEST_ASSERT_EQUAL(ms_length("hello"), 5);
  TEST_ASSERT(ms_length("") == 0);

  ms_length(NULL); /* assertion failed */

  return 0;
}

void *test_cmp(void *arg) {
  (void)arg;

  fprintf(stderr, "test_cmp():\n");

  TEST_ASSERT(ms_compare("compare", "compare") == 0); /* 0 */

  TEST_ASSERT(ms_compare("hello", "world") < 0); /* -1 */
  TEST_ASSERT(ms_compare("world", "hello") > 0); /* 1 */

  TEST_ASSERT(ms_compare("", "hello world") < 0); /* -1 */
  TEST_ASSERT(ms_compare("hello world", "") > 0); /* 1 */

  TEST_ASSERT(ms_compare("", "") == 0); /* 0 */

  ms_compare(NULL, NULL); /* assertion failed */

  return 0;
}

void *test_ncmp(void *arg) {
  (void)arg;

  fprintf(stderr, "test_ncmp():\n");

  TEST_ASSERT(ms_ncompare("compare", "compareeee", sizeof("compare") - 1) ==
              0); /* 0 */

  TEST_ASSERT(ms_ncompare("hillo", "hello", sizeof("hello") - 1) > 0); /* 1 */

  TEST_ASSERT(ms_ncompare("hello", "world", sizeof("hello") - 1) < 0); /* -1 */

  TEST_ASSERT(ms_ncompare("", "", 1) == 0); /* 0 */

  TEST_ASSERT(ms_ncompare("dadada", "22", 0) == 0); /* 0 */

  TEST_ASSERT(ms_ncompare("hello", "", 3) > 0); /* 1 */

  ms_ncompare(NULL, NULL, 0); /* assertion failed */

  return 0;
}

#define SIZE 100

void *test_copy(void *arg) {
  (void)arg;

  char src[SIZE] = " world";
  char dest[SIZE];

  fprintf(stderr, "test_copy():\n");

  TEST_ASSERT_EQUAL_STRING(ms_copy(dest, "hello"), "hello");
  ms_copy(&dest[5], src);
  TEST_ASSERT_EQUAL_STRING(dest, "hello world");
  TEST_ASSERT_EQUAL_STRING(ms_copy(dest, ""), "");

  ms_copy(NULL, NULL); /* assertion failed */

  return 0;
}

void *test_ncpy(void *arg) {
  (void)arg;

  fprintf(stderr, "test_ncpy():\n");

  char src[SIZE] = " world";
  char dest[SIZE];

  TEST_ASSERT_EQUAL_STRING(ms_ncopy(dest, "hello", 6), "hello");
  TEST_ASSERT_EQUAL_STRING(ms_ncopy(dest, "fr", 2), "frllo");

  memset(dest, -1, SIZE);  /* reset dest */
  ms_ncopy(dest, "a", 10);
  TEST_ASSERT_EQUAL(dest[5], 0);  /* check if for the remaining of n (9), null bytes are added*/
  ms_ncopy(dest, "hello", 0);
  TEST_ASSERT_EQUAL_STRING(ms_ncopy(dest, "hello", 0), "a");  /* check if n = 0 */

  ms_ncopy(NULL, NULL, 0); /* assertion failed */

  return 0;
}

void *test_cat(void *arg) {
  (void)arg;

  fprintf(stderr, "test_cat():\n");

  char src[SIZE]  = " world";
  char dest[SIZE] = "hello";

  TEST_ASSERT_EQUAL_STRING(ms_concat(dest, src), "hello world");
  TEST_ASSERT_EQUAL_STRING(ms_concat(dest, ""), "hello world");
  ms_concat(&dest[5], src);
  TEST_ASSERT_EQUAL_STRING(dest, "hello world world");

  ms_concat(NULL, NULL); /* assertion failed */

  return 0;
}

void *test_ncat(void *arg) {
  (void)arg;

  fprintf(stderr, "test_ncat():\n");

  char src[SIZE]  = " world";
  char dest[SIZE] = "hello";

  memset(dest + 6, -1, SIZE - 6); /* -1 the rest of dest */

  TEST_ASSERT_EQUAL_STRING(ms_nconcat(dest, src, 6), "hello world");
  TEST_ASSERT_EQUAL_STRING(ms_nconcat(dest, "", 0), "hello world");
  ms_nconcat(&dest[5], src, 5);
  TEST_ASSERT_EQUAL_STRING(dest, "hello world worl");

  ms_nconcat(NULL, NULL, 0); /* assertion failed */

  return 0;
}

void *test_search(void *arg) {
  (void)arg;

  fprintf(stderr, "test_search():\n");

  char *haystack = "hello world";
  char *needle   = "world";

  TEST_ASSERT_EQUAL(ms_search(haystack, needle), haystack + 6);
  TEST_ASSERT_EQUAL(ms_search(haystack, ""), haystack);
  TEST_ASSERT_EQUAL(ms_search("", needle), NULL);

  ms_search(NULL, NULL); /* assertion failed */

  return 0;
}

/* array of function pointers for each test case */
/* we will use this to iterate over all test cases */
/* if you wish to add a test case, append it on the array below */
void *(*tests[])(void *) = {test_length, test_cmp, test_ncmp, test_copy,
                            test_ncpy,   test_cat, test_ncat, test_search};

int main(void) {
  /* Due to the nature of assert (it aborts after failure), we can't test
   * assertions the easy way, since we have to stop normal execution */
  /* We will use one thread for each assertion */

  for (size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {
    pthread_t tid;
    pthread_create(&tid, NULL, tests[i], NULL);
    pthread_join(tid, NULL);
    TEST_RESET();
  }

  TEST_PRINT_SUMMARY();

  return 0;
}
