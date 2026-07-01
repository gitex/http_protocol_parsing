#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "ptr_vector.h"


typedef struct PtrVector {
    void **elements;
    size_t length;
    size_t capacity;
} PtrVector;


PtrVector *ptr_vec_new(int capacity) {
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

size_t ptr_vec_length(PtrVector *vec) {
    return vec->length;
}

size_t ptr_vec_capacity(PtrVector *vec) {
    return vec->capacity;
}

bool ptr_vec_is_empty(PtrVector *vec) {
    return vec->length == 0;
}

// Only for internal use??? Does it help with something outside?
static bool ptr_vec_is_full(PtrVector *vec) {
    return vec->length == vec->capacity;
}

// Next 3 functions reallocate vector with capacity. Do we need this control outside?
// What if outside we decide to shrink, but there values in other side???
// Maybe it's enough to create vector with capacity?
static int realloc_with_capacity(PtrVector *vec, size_t capacity) {
    void *tmp = realloc(vec->elements, capacity);
    if (!tmp) {
        fprintf(stderr, "Cannot expand ptr_vec\n");
        return 1;
    }
    vec->elements = tmp;
    return 0;
}

static void ptr_vec_expand(PtrVector *vec) {
    realloc_with_capacity(vec, vec->capacity * CAPACITY_FACTOR);
}

__attribute__((unused)) static void ptr_vec_shrink(PtrVector *vec) {
    realloc_with_capacity(vec, vec->capacity /=CAPACITY_FACTOR);
}

void ptr_vec_set(PtrVector *vec, size_t index, void *elem) {
    if (!vec || !elem || index >= vec->capacity) return;

    vec->elements[index] = elem;

    if (index > vec->length) {
        vec->length = index;
    } else if (index == vec->length) {
        vec->length++;
    }
}

void ptr_vec_insert(PtrVector *vec, size_t index, void *elem) {
    if (!vec || !elem || index >= vec->capacity) return;

    if (ptr_vec_is_full(vec)) {
        ptr_vec_expand(vec);
    }
    if (index > vec->length) return;

    for (size_t i = vec->length; i > index; --i) {
        vec->elements[i] = vec->elements[i - 1];
    }

    ptr_vec_set(vec, index, elem);
}

void ptr_vec_push_back(PtrVector *vec, void *elem) {
    if (!vec || !elem) return;
    if (ptr_vec_is_full(vec)) {
        ptr_vec_expand(vec);
    }

    ptr_vec_set(vec, vec->length, elem);
}

void ptr_vec_push_front(PtrVector *vec, void *element) {
    ptr_vec_insert(vec, 0, element);
}

void *ptr_vec_at(PtrVector *vec, size_t index) {
    if (index >= vec->length) {
        return NULL;
    }

    return vec->elements[index];
}

void ptr_vec_free(PtrVector *vec) {
    if (!vec) return;

    if (vec->elements != NULL) {
        free(vec->elements);
    }
    free(vec);
}
