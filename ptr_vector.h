#ifndef PTR_VECTOR_H
#define PTR_VECTOR_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define DEFAULT_CAPACITY  2
#define CAPACITY_FACTOR   2

typedef enum {
    PTR_VEC_OK = 0,
    PTR_VEC_ERR__NULLPTR = 1,
    PTR_VEC_ERR__ALLOC = 2,
    PTR_VEC_ERR__INDEX_OUT_OF_RANGE = 3,
    PTR_VEC_ERR__INDEX_NOT_ZERO_ON_EMPTY = 4,
    PTR_VEC_ERR__CAPACITY_EXCEEDED = 5,
} PtrVectorError;

typedef struct PtrVector PtrVector;

/* Create new vector with capacity. If capacity less or equal to zero,
 * then it will use default capacity (2) */
PtrVector *ptr_vec_new(int capacity);

/* Get length of the vector */
size_t ptr_vec_length(PtrVector *vec);

/* Get capacity of the vector */
size_t ptr_vec_capacity(PtrVector *vec);

/* Checks if vector has any elements */
bool ptr_vec_is_empty(PtrVector *vec);

// Replace element by index. Index cannot be greater then capacity
PtrVectorError ptr_vec_set(PtrVector *vec, size_t index, void *elem);

// Expands vector at the index and inserts an element there. Index cannot be greater then capacity
PtrVectorError ptr_vec_insert(PtrVector *vec, size_t index, void *elem);

// Push element at the end (after last element)
PtrVectorError ptr_vec_push_back(PtrVector *vec, void *elem);

// Push element at the front (before first element)
PtrVectorError ptr_vec_push_front(PtrVector *vec, void *elem);

/* Get element at index */
void *ptr_vec_at(PtrVector *vec, size_t index);

/* Free vector */
void ptr_vec_free(PtrVector *vec);

#endif
