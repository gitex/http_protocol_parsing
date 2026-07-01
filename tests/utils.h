#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

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


#define _TEST_RUN(test)                                                                   \
    tests_run++;                                                                          \
    printf("%s %s\n", RUN_PREFIX, #test)                                                  \

#define _TEST_FAIL(msg)                                                                   \
    tests_failed++;                                                                       \
    asserts_failed++;                                                                     \
    fprintf(stderr, "%s %s:%d: %s\n", FAIL_PREFIX, __FILE__, __LINE__, msg);                                      \
    return;                                                                               \


#define _TEST_FAILF(msg, ...)                                                             \
    tests_failed++;                                                                       \
    asserts_failed++;                                                                     \
    fprintf(stderr, "%s %s:%d: ", FAIL_PREFIX, __FILE__, __LINE__);                       \
    fprintf(stderr, msg "\n", __VA_ARGS__);                                               \
    return;                                                                               \

#define _TEST_PASS(test)                                                                     \
    tests_passed++;                                                                       \
    fprintf(stdout, "%s %s\n", PASS_PREFIX, (test));                                                           \

#define ASSERT_EQ_STR(expected, actual)                                                      \
    do {                                                                                  \
        if ((expected) != (actual)) {                                                        \
            _TEST_FAILF("expected=%s actual=%s", (expected), (actual));                   \
        }                                                                                 \
    } while (0)                                                                           \

#define ASSERT_EQ_INT(expected, actual)                                                   \
    do {                                                                                  \
        if ((expected) != (actual)) {                                                     \
            _TEST_FAILF("expected=%d actual=%d", (expected), (actual));                   \
        }                                                                                 \
    } while (0)                                                                           \

#define ASSERT_NULL(ptr)                                                                  \
    do {                                                                                  \
        if ((ptr) != NULL) {                                                              \
            _TEST_FAILF("expected='%s' actual='%s'", (expected), (actual));               \
        }                                                                                 \
    } while (0)

#define ASSERT_EQ_SIZE(expected, actual)                                                  \
    do {                                                                                  \
        if ((expected) != (actual)) {                                                     \
            _TEST_FAILF("expected=%ld actual=%ld", (expected), (actual));                 \
        }                                                                                 \
    } while (0)


#define ASSERT_NOT_NULL(ptr)                                                              \
    do {                                                                                  \
        if ((ptr) == NULL) {                                                              \
            _TEST_FAIL("unexpected NULL");                                                \
        }                                                                                 \
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

#define TEST(name) static void name(void)

#define RUN_TEST(test)                                                                    \
    do {                                                                                  \
        asserts_failed = 0;                                                               \
        _TEST_RUN(test);                                                                  \
        test();                                                                           \
        if (asserts_failed == 0) {                                                        \
            _TEST_PASS(#test);                                                                \
        }                                                                                 \
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

#endif
