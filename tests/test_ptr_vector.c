#include "../ptr_vector.h"

#include "test.h"

#define COUNT_OF(arr) sizeof((arr)) / sizeof((arr[0]))
#define CAST_INT_AT_IDX(vec, idx) *(int*)ptr_vec_at((vec), (idx))


void fill_vector(int xs[], size_t n, PtrVector* vec) {
    PtrVectorError result = -1;
    for (size_t i = 0; i < n; i++) {
        result = ptr_vec_push_back(vec, &xs[i]);
        ASSERT_EQ_INT(PTR_VEC_OK, result);
    }
}

void assert_int_array_eq(int xs[], size_t n, PtrVector* vec) {
    ASSERT_EQ_SIZE(n, ptr_vec_length(vec));

    for (size_t i = 0; i < n; i++) {
        ASSERT_EQ_INT(xs[i], CAST_INT_AT_IDX((vec), i));
    }
}


// ptr_vec_new SHOULD allow to set capacity at initialization
TEST(test_init_with_some_capacity) {
    PtrVector *vec = ptr_vec_new(4);
    ASSERT_NOT_NULL(vec);
    ASSERT_EQ_SIZE((size_t)4, ptr_vec_capacity(vec));
    ASSERT_EQ_SIZE((size_t)0, ptr_vec_length(vec));
    ptr_vec_free(vec);
}

// ptr_vec_new with zero capacity SHOULD set capacity to DEFAULT_CAPACITY
TEST(test_init_with_zero_capacity) {
    PtrVector *vec = ptr_vec_new(0);
    ASSERT_NOT_NULL(vec);
    ASSERT_EQ_SIZE((size_t)DEFAULT_CAPACITY, ptr_vec_capacity(vec));
    ASSERT_EQ_SIZE((size_t)0, ptr_vec_length(vec));
    ptr_vec_free(vec);
}

// ptr_vec_length SHOULD return current length of the vector
TEST(test_length) {
    PtrVector *vec = ptr_vec_new(0);
    ASSERT_EQ_SIZE((size_t) 0, ptr_vec_length(vec));
    ptr_vec_free(vec);
}

// ptr_vec_capacity SHOULD return current capacity of the vector
TEST(test_capacity) {
    PtrVector *vec = ptr_vec_new(4);
    ASSERT_EQ_SIZE((size_t) 4, ptr_vec_capacity(vec));
    ptr_vec_free(vec);
}

/* --- set --- */

// ptr_vec_set SHOULD be able to place element at index zero
TEST(test_set_at_begining) {
    PtrVector *vec = ptr_vec_new(2);
    int x = 2;
    size_t exp_len = 1;

    ptr_vec_set(vec, 0, &x);
    ASSERT_EQ_SIZE(exp_len, ptr_vec_length(vec));
    ASSERT_EQ_INT(x, CAST_INT_AT_IDX(vec, 0));

    ptr_vec_free(vec);
}

// ptr_vec_set SHOULD be able to replace element with different element
TEST(test_set_as_replace) {
    PtrVector *vec = ptr_vec_new(2);

    int x = 2;
    ptr_vec_set(vec, 0, &x);

    int y = 3;
    ptr_vec_set(vec, 0, &y);

    ASSERT_EQ_PTR(&y, ptr_vec_at(vec, 0));
    ASSERT_EQ_INT(y, CAST_INT_AT_IDX(vec, 0));
    ASSERT_NOT_EQ_PTR(&x, ptr_vec_at(vec, 0));
    ASSERT_EQ_SIZE((size_t)1, ptr_vec_length(vec));

    ptr_vec_free(vec);
}

// ptr_vec_set SHOULD NOT emplace elements beyond length of vector
TEST(test_set_beyond_bounds) {
    PtrVector *vec = ptr_vec_new(4);

    int xs[] = {0, 1};
    size_t n = COUNT_OF(xs);
    fill_vector(xs, n, vec);
    ASSERT_EQ_SIZE(n, ptr_vec_length(vec));

    int x = 20;
    PtrVectorError result = ptr_vec_set(vec, 2, &x);
    ASSERT_EQ_INT(PTR_VEC_ERR__INDEX_OUT_OF_RANGE, result);

    ptr_vec_free(vec);
}

// ptr_vec_set SHOULD NOT emplace elements at empty vector in any position, except 0
TEST(test_set_beyond_bounds_on_empty_vec) {
    PtrVector *vec = ptr_vec_new(4);
    ASSERT_EQ_SIZE((size_t)0, ptr_vec_length(vec));

    int x = 20;
    PtrVectorError result = ptr_vec_set(vec, 2, &x);
    ASSERT_EQ_INT(PTR_VEC_ERR__INDEX_NOT_ZERO_ON_EMPTY, result);

    ptr_vec_free(vec);
}

/* --- push_back --- */

// ptr_vec_push_back SHOULD add elements at last index
TEST(test_push_back) {
    PtrVector *vec = ptr_vec_new(2);
    ASSERT_EQ_SIZE((size_t)2, ptr_vec_capacity(vec));

    int xs[] = {0, 1, 2, 3};
    size_t n = COUNT_OF(xs);

    for (size_t i = 0; i < n; i++) {
        ASSERT_EQ_INT(PTR_VEC_OK, ptr_vec_push_back(vec, &xs[i]));
    }
    ASSERT_EQ_SIZE(n, ptr_vec_length(vec));
    ASSERT_EQ_SIZE(n, ptr_vec_capacity(vec));

    ptr_vec_free(vec);
}


/* --- insert --- */

// ptr_vec_insert SHOULD be able to place element at zero index
// ptr_vec_insert SHOULD be able to append element at vec->length index (push_back)
TEST(test_insert_as_push_back) {
    PtrVector *vec = ptr_vec_new(8);
    ASSERT_EQ_SIZE((size_t)0, ptr_vec_length(vec));

    int xs[] = {0, 1, 2, 3};
    size_t n = COUNT_OF(xs);

    for (size_t i = 0; i < n; i++) {
        ASSERT_EQ_INT(PTR_VEC_OK, ptr_vec_insert(vec, i, &xs[i]));
    }

    int exp[4] = {0, 1, 2, 3};
    assert_int_array_eq(exp, COUNT_OF(exp), vec);

    ptr_vec_free(vec);
}

// ptr_vec_insert SHOULD expand vector in the middle and place value there.
//      Elements to the right of index SHOULD move one position to the right.
TEST(test_insert_in_the_middle) {
    PtrVector *vec = ptr_vec_new(4);

    int xs[4] = {0, 1, 2, 3};
    fill_vector(xs, COUNT_OF(xs), vec);

    int x = 10;
    ASSERT_EQ_INT(PTR_VEC_OK, ptr_vec_insert(vec, 2, &x));

    int y = 20;
    ASSERT_EQ_INT(PTR_VEC_OK, ptr_vec_insert(vec, 2, &y));

    int exp[6] = {0, 1, 20, 10, 2, 3};
    assert_int_array_eq(exp, COUNT_OF(exp), vec);

    ptr_vec_free(vec);
}

// ptr_vec_insert SHOULD NOT emplace elements out of the range of vector
TEST(test_insert_beyond_length) {
    PtrVector *vec = ptr_vec_new(8);

    int xs[4] = {0, 1, 2, 3};
    fill_vector(xs, COUNT_OF(xs), vec);

    int x = 6;
    size_t idx = 6;
    ASSERT_EQ_INT(PTR_VEC_ERR__INDEX_OUT_OF_RANGE, ptr_vec_insert(vec, idx, &x));

    assert_int_array_eq(xs, COUNT_OF(xs), vec);
}

// ptr_vec_is_empty SHOULD return true if vector is empty, otherwise - false
TEST(test_is_empty) {
    PtrVector *vec = ptr_vec_new(4);
    ASSERT_TRUE(ptr_vec_is_empty(vec));

    char *tmp = "test";
    ptr_vec_push_back(vec, tmp);
    ASSERT_FALSE(ptr_vec_is_empty(vec));

    ptr_vec_free(vec);
}


int main() {
    RUN_TEST(test_init_with_zero_capacity);
    RUN_TEST(test_init_with_some_capacity);
    RUN_TEST(test_length);
    RUN_TEST(test_capacity);
    RUN_TEST(test_set_at_begining);
    RUN_TEST(test_set_as_replace);
    RUN_TEST(test_set_beyond_bounds);
    RUN_TEST(test_set_beyond_bounds_on_empty_vec);
    RUN_TEST(test_push_back);
    RUN_TEST(test_insert_as_push_back);
    RUN_TEST(test_insert_in_the_middle);
    RUN_TEST(test_insert_beyond_length);
    RUN_TEST(test_is_empty);
    TEST_SUMMARY();

    return tests_failed;
}
