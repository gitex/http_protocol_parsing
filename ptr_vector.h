#ifndef PTR_VECTOR_H
#define PTR_VECTOR_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define DEFAULT_CAPACITY  2
#define CAPACITY_FACTOR   2

typedef struct {
    void **elements;
    size_t length;
    size_t capacity;
} PtrVector;

/* Create new vector */
PtrVector *ptr_vector_new(int capacity);

/* Get length of the vector */
size_t ptr_vector_length(PtrVector *vec);

/* Get capacity of the vector */
size_t ptr_vector_capacity(PtrVector *vec);

/* Checks if vector has any elements */
bool ptr_vector_is_empty(PtrVector *vec);

/* Split vector at index and insert element in between */
void ptr_vector_insert(PtrVector *vec, size_t index, void *element, size_t element_size);

/* Push element in the end of vector */
void ptr_vector_push_back(PtrVector *vec, void *element, size_t element_size);

/* Get element at index */
void *ptr_vector_at(PtrVector *vec, size_t index);

/* Free vector */
void ptr_vector_free(PtrVector *vec);

#endif  // !PTR_VECTOR_H
