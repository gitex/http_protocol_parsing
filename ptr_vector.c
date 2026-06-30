#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "ptr_vector.h"

PtrVector *ptr_vector_new(int capacity) {
    if (capacity <= 0) {
        capacity = DEFAULT_CAPACITY;
    }

    PtrVector *v = malloc(sizeof(PtrVector));
    v->capacity = capacity;
    v->length = 0;
    v->elements = malloc(v->capacity * sizeof(void *));
    return v;
}

size_t ptr_vector_length(PtrVector *vec) {
    return vec->length;
}

size_t ptr_vector_capacity(PtrVector *vec) {
    return vec->capacity;
}

bool ptr_vector_is_empty(PtrVector *vec) {
    return vec->length == 0;
}

static bool ptr_vector_is_full(PtrVector *vec) {
    return vec->length == vec->capacity;
}

static void ptr_vector_expand(PtrVector *vec) {
    vec->capacity *= CAPACITY_FACTOR;
    vec->elements = realloc(vec->elements, vec->capacity * sizeof(void *));
}

static void ptr_vector_shrink(PtrVector *vec) {
    vec->capacity /= CAPACITY_FACTOR;
    vec->elements = realloc(vec->elements, vec->capacity * sizeof(void *));
}

void ptr_vector_set(PtrVector *vec, int index, void *element, size_t element_size) {
    if (index < 0 || index >= vec->length) {
        return;
    }

    free(vec->elements[index]);
    void *copy = malloc(element_size);
    memcpy(copy, element, element_size);
    vec->elements[index] = copy;
}

void ptr_vector_insert(PtrVector *vec, int index, void *element, size_t element_size) {
    if (index < 0 || index > vec->length) return;
    if (ptr_vector_is_full(vec)) {
        ptr_vector_expand(vec);
    }

    for (int i = vec->length; i > index; --i) {
        vec->elements[i] = vec->elements[i - 1];
    }

    free(vec->elements[index]);
    void *copy = malloc(element_size);
    memcpy(copy, element, element_size);
    vec->elements[index] = copy;
}

// Push element at the end
void ptr_vector_push(PtrVector *vec, void *element, size_t element_size) {
    if (ptr_vector_is_full(vec)) {
        ptr_vector_expand(vec);
    }

    ptr_vector_insert(vec, vec->length, element, element_size);
    vec->length++;
}

void *ptr_vector_at(PtrVector *vec, int index) {
    if (index < 0 || index >= vec->length) {
        return NULL;
    }

    return vec->elements[index];
}

void ptr_vector_free(PtrVector *vec) {
    free(vec->elements);
    free(vec);
}
