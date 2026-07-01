#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "ptr_vector.h"

PtrVector *ptr_vector_new(int capacity) {
    if (capacity <= 0) {
        capacity = DEFAULT_CAPACITY;
    }

    PtrVector *v = malloc(sizeof(PtrVector));
    if (!v) return NULL;

    v->capacity = capacity;
    v->length = 0;
    v->elements = malloc(v->capacity * sizeof(void *));
    if (!v->elements) { free(v); return NULL; }

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

static int realloc_with_capacity(PtrVector *vec, size_t capacity) {
    void *tmp = realloc(vec->elements, vec->capacity * sizeof(void *));
    if (!tmp) {
        fprintf(stderr, "Cannot expand ptr_vector\n");
        return 1;
    }
    vec->elements = tmp;
    return 0;
}

static void ptr_vector_expand(PtrVector *vec) {
    realloc_with_capacity(vec, vec->capacity * CAPACITY_FACTOR);
}

__attribute__((unused)) static void ptr_vector_shrink(PtrVector *vec) {
    realloc_with_capacity(vec, vec->capacity /=CAPACITY_FACTOR);
}

// Expands vector at the index and inserts an element there
void ptr_vector_insert(PtrVector *vec, size_t index, void *element, size_t element_size) {
    if (ptr_vector_is_full(vec)) {
        ptr_vector_expand(vec);
    }
    if (index > vec->length) return;

    for (size_t i = vec->length; i > index; --i) {
        vec->elements[i] = vec->elements[i - 1];
    }

    void *copy = malloc(element_size);
    if (!copy) return;
    memcpy(copy, element, element_size);
    vec->elements[index] = copy;
    vec->length++;
}

// Push element at the end
void ptr_vector_push_back(PtrVector *vec, void *element, size_t element_size) {
    if (ptr_vector_is_full(vec)) {
        ptr_vector_expand(vec);
    }

    void *copy = malloc(element_size);
    if (!copy) return;
    memcpy(copy, element, element_size);
    vec->elements[vec->length] = copy;
    vec->length++;
}


void *ptr_vector_at(PtrVector *vec, size_t index) {
    if (index >= vec->length) {
        return NULL;
    }

    return vec->elements[index];
}

void ptr_vector_free(PtrVector *vec) {
    if (!vec) return;

    if (vec->elements != NULL) {
        for (size_t i = 0; i < vec->length; i++) {
            free(vec->elements[i]);
        }
        free(vec->elements);
    }
    free(vec);
}
