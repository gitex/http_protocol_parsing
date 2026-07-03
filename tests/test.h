#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_TESTS 1024

static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;
static int asserts_failed = 0;

#define ANSI_COLOR_RED            "\033[31m"
#define ANSI_COLOR_GREEN          "\033[32m"
#define ANSI_COLOR_MAGENTA        "\033[35m"
#define ANSI_COLOR_BLUE           "\033[34m"
#define ANSI_COLOR_RESET          "\033[0m"

#define COLOR_TEXT(text, color) color text ANSI_COLOR_RESET

#define RUN_PREFIX  COLOR_TEXT("  [RUN ]", ANSI_COLOR_BLUE)
#define FAIL_PREFIX COLOR_TEXT("  [FAIL]", ANSI_COLOR_RED)
#define PASS_PREFIX COLOR_TEXT("  [PASS]", ANSI_COLOR_GREEN)


typedef struct {
    const char *name;
    void (*fn)(void);
} TestFn;

static TestFn tests[MAX_TESTS];
static size_t tests_count = 0;


#define _TEST_RUN(test)                                                                   \
    do { \
        tests_run++;                                                                          \
        fprintf(stdout, "%s %s\n", RUN_PREFIX, (test));                                                  \
    } while (0)

#define _TEST_FAIL(msg)                                                                   \
    do { \
        tests_failed++;                                                                       \
        asserts_failed++;                                                                     \
        fprintf(stderr, "%s %s:%d: %s\n", FAIL_PREFIX, __FILE__, __LINE__, msg);                                      \
    } while (0)


#define _TEST_FAILF(msg, ...)                                                             \
    do { \
        tests_failed++;                                                                       \
        asserts_failed++;                                                                     \
        fprintf(stderr, "%s %s:%d: ", FAIL_PREFIX, __FILE__, __LINE__);                       \
        fprintf(stderr, msg "\n", __VA_ARGS__);                                               \
    } while (0)

#define _TEST_PASS(test)                                                                     \
    do { \
        tests_passed++;                                                                       \
        fprintf(stdout, "%s %s\n", PASS_PREFIX, (test));                                                           \
    } while (0)

/* !asserts */

#define ASSERT_EQ_INT(expected, actual)                                                   \
    do {                                                                                  \
        if ((expected) != (actual)) {                                                     \
            _TEST_FAILF("expected=%d actual=%d", (expected), (actual));                   \
        }                                                                                 \
    } while (0)                                                                           \

#define ASSERT_NULL(ptr)                                                                  \
    do {                                                                                  \
        if ((ptr) != NULL) {                                                              \
            _TEST_FAILF("expected=NULL actual='%p'", (ptr));               \
        }                                                                                 \
    } while (0)

#define ASSERT_EQ_SIZE(expected, actual)                                                  \
    do {                                                                                  \
        if ((expected) != (actual)) {                                                     \
            _TEST_FAILF("expected=%zu actual=%zu", (expected), (actual));                 \
        }                                                                                 \
    } while (0)


#define ASSERT_NOT_NULL(actual)                                                              \
    do {                                                                                  \
        if ((actual) == NULL) {                                                              \
            _TEST_FAIL("unexpected NULL");                                                \
        }                                                                                 \
    } while (0)

#define ASSERT_EQ_PTR(expected, actual)                                                  \
    do {                                                                                 \
        if ((actual) != (expected)) {                                                    \
            _TEST_FAILF("expected=%p, actual=%p", (void*)(expected), (void*)(actual));                 \
        }                                                                                \
    } while (0)

#define ASSERT_NOT_EQ_PTR(expected, actual)                                                  \
    do {                                                                                 \
        if ((actual) == (expected)) {                                                    \
            _TEST_FAILF("expected=%p, actual=%p", (void*)(expected), (void*)(actual));                 \
        }                                                                                \
    } while (0)

#define ASSERT_TRUE(cond)                                                                 \
    do {                                                                                  \
        if (!(cond)) {                                                                    \
            _TEST_FAILF("expected=true, actual=%s", #cond);                               \
        }                                                                                 \
    } while (0)

#define ASSERT_FALSE(cond)                                                                \
    do {                                                                                  \
        if ((cond)) {                                                                     \
            _TEST_FAILF("expected=false, actual=%s", #cond);                              \
        }                                                                                 \
    } while (0)



/* test runner */

#define SETUP() void setup(void)
#define TEARDOWN() void teardown(void)

#define TEST(test_name)                                                                  \
    __attribute__((unused)) static void test_name(void);                                 \
    __attribute__((constructor))                                                         \
    static void register_##test_name(void) {                                             \
        if (tests_count >= MAX_TESTS) {                                                  \
            fprintf(stderr, "Exceeded limit of tests (max %d)\n", MAX_TESTS);            \
            abort();                                                                     \
        }                                                                                \
        tests[tests_count++] = (TestFn){.name = #test_name, .fn = test_name};            \
    }                                                                                    \
    static void test_name(void)


#define RUN_TEST(test)                                                                    \
    do {                                                                                  \
        asserts_failed = 0;                                                               \
        _TEST_RUN(#test);                                                                  \
        test();                                                                           \
        if (asserts_failed == 0) {                                                        \
            _TEST_PASS(#test);                                                                \
        }                                                                            \
    } while (0)

#define _RUN_TEST_FN_STRUCT(test)                                                                    \
    do {                                                                                  \
        asserts_failed = 0;                                                               \
        _TEST_RUN(test.name);                                                                  \
        test.fn();                                                                           \
        if (asserts_failed == 0) {                                                        \
            _TEST_PASS(test.name);                                                                \
        }                                                                                 \
    } while (0)

#define RUN_TESTS() \
    do { \
        for (size_t i = 0; i < tests_count; i++) { \
            if (tests_count > 0) { \
                _RUN_TEST_FN_STRUCT(tests[i]); \
            } \
        } \
    } while (0)


#define TEST_SUMMARY()                                                                    \
    do {                                                                                  \
        printf("\n  === RESULT ===\n");                                                     \
        printf("  Run: %d\n", tests_run);                                                   \
        printf("  Passed: %d\n", tests_run - tests_failed);                                 \
        printf("  Failed: %d\n", tests_failed);                                             \
        if (tests_failed == 0) {                                                          \
            printf(COLOR_TEXT("\n  Status: OK\n", ANSI_COLOR_GREEN));                         \
        } else {                                                                          \
            printf(COLOR_TEXT("\n  Status: FAIL\n", ANSI_COLOR_RED));                         \
        }                                                                                 \
    } while (0)

#endif  // !TEST_H
