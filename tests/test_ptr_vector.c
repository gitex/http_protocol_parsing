#include "../ptr_vector.h"

#include "utils.h"


static void test_init_with_zero_capacity() {
    PtrVector *vec = ptr_vector_new(0);
    ASSERT_NOT_NULL(vec);
    ASSERT_EQ_SIZE((size_t) DEFAULT_CAPACITY, vec->capacity);
    ASSERT_EQ_SIZE((size_t) 0, vec->length);
    ptr_vector_free(vec);
}

static void test_init_with_some_capacity() {
    PtrVector *vec = ptr_vector_new(4);
    ASSERT_NOT_NULL(vec);
    ASSERT_EQ_SIZE((size_t) 4, vec->capacity);
    ASSERT_EQ_SIZE((size_t) 0, vec->length);
    ptr_vector_free(vec);
}

static void test_length() {
    PtrVector *vec = ptr_vector_new(0);
    ASSERT_EQ_SIZE((size_t) 0, ptr_vector_length(vec));
}

static void test_capacity() {
    PtrVector *vec = ptr_vector_new(4);
    ASSERT_EQ_SIZE((size_t) 4, ptr_vector_capacity(vec));
}

static void test_push() {
    PtrVector *vec = ptr_vector_new(4);
    size_t expected = 20;

    for (size_t i = 0; i < expected; i++) {
        ptr_vector_push_back(vec, &i, sizeof(int));
    }
    ASSERT_EQ_SIZE(expected, ptr_vector_length(vec));
    ASSERT_EQ_SIZE(expected, vec->length);
}

static void test_insert_at_the_end() {
    PtrVector *vec = ptr_vector_new(2);
    size_t expected = 5;

    for (size_t i = 0; i < expected; i++) {
        ptr_vector_insert(vec, i, &i, sizeof(int));
    }
    ASSERT_EQ_SIZE(expected, ptr_vector_length(vec));
    ASSERT_EQ_SIZE(expected, vec->length);
    ASSERT_EQ_INT(0, *(int*)ptr_vector_at(vec, 0));
    ASSERT_EQ_INT(1, *(int*)ptr_vector_at(vec, 1));
    ASSERT_EQ_INT(2, *(int*)ptr_vector_at(vec, 2));
    ASSERT_EQ_INT(3, *(int*)ptr_vector_at(vec, 3));
    ASSERT_EQ_INT(4, *(int*)ptr_vector_at(vec, 4));
}

static void test_insert_in_the_middle() {
    PtrVector *vec = ptr_vector_new(2);

    for (size_t i = 0; i < 5; i++) {
        ptr_vector_insert(vec, i, &i, sizeof(int));
    }

    int new_value = 10;
    ptr_vector_insert(vec, 2, &new_value , sizeof(int));

    ASSERT_EQ_INT(0, *(int*)ptr_vector_at(vec, 0));
    ASSERT_EQ_INT(1, *(int*)ptr_vector_at(vec, 1));
    ASSERT_EQ_INT(10,*(int*)ptr_vector_at(vec, 2));
    ASSERT_EQ_INT(2, *(int*)ptr_vector_at(vec, 3));
    ASSERT_EQ_INT(3, *(int*)ptr_vector_at(vec, 4));
    ASSERT_EQ_INT(4, *(int*)ptr_vector_at(vec, 5));
}

static void test_is_empty() {
    PtrVector *vec = ptr_vector_new(4);
    ASSERT_TRUE(ptr_vector_is_empty(vec));

    char *tmp = "test";
    ptr_vector_push_back(vec, tmp, strlen(tmp));
    ASSERT_FALSE(ptr_vector_is_empty(vec));
}


int main() {
    RUN_TEST(test_init_with_zero_capacity);
    RUN_TEST(test_init_with_some_capacity);
    RUN_TEST(test_length);
    RUN_TEST(test_capacity);
    RUN_TEST(test_push);
    RUN_TEST(test_insert_at_the_end);
    RUN_TEST(test_insert_in_the_middle);
    RUN_TEST(test_is_empty);
    TEST_SUMMARY();

    return tests_failed;
}
