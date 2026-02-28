#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdint.h>

#define MAX_TESTS 256
#define MAX_NAME_LEN 40

typedef enum {
  TEST_PASS = 0,
  TEST_FAIL = 1,
  TEST_SKIP = 2,
} test_status_t;

typedef struct {
  char name[MAX_NAME_LEN];
  test_status_t status;
  uint32_t expected;
  uint32_t actual;
} test_result_t;

/* GDB-inspectable results struct in SRAM */
typedef struct {
  uint32_t magic;    /* 0xDEADBEEF when complete */
  uint32_t total;
  uint32_t passed;
  uint32_t failed;
  uint32_t skipped;
  test_result_t results[MAX_TESTS];
} test_results_t;

extern volatile test_results_t test_results;

void test_init(void);
void test_pass(const char *name, uint32_t expected, uint32_t actual);
void test_fail(const char *name, uint32_t expected, uint32_t actual);
void test_skip(const char *name, const char *reason);
void test_finalize(void);

#endif /* TEST_FRAMEWORK_H */
