#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "ptr_vector.h"


struct PtrVector {
    void **elements;
    size_t length;
    size_t capacity;
};


PtrVector *ptr_vec_new(size_t capacity) {
    if (capacity == 0) { capacity = DEFAULT_CAPACITY; }

    PtrVector *v = calloc(1, sizeof(PtrVector));
    if (!v) return NULL;

    v->capacity = capacity;
    v->length = 0;
    v->elements = calloc(v->capacity, sizeof(void *));
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
static bool _ptr_vector_is_full(PtrVector *vec) {
    return vec->length == vec->capacity;
}

static PtrVectorError _realloc_with_capacity(PtrVector *vec, size_t capacity) {
    void *tmp = realloc(vec->elements, capacity * sizeof(void *));
    if (!tmp) {
        fprintf(stderr, "Cannot expand ptr_vec\n");
        return PTR_VEC_ERR__ALLOC;
    }

    // TODO: do i need to free elements before assigning?
    vec->elements = tmp;
    vec->capacity = capacity;
    return PTR_VEC_OK;
}

static PtrVectorError _ptr_vec_expand_capacity(PtrVector *vec) {
    return _realloc_with_capacity(vec, vec->capacity * CAPACITY_FACTOR);
}

__attribute__((unused)) static PtrVectorError _ptr_vec_shrink_capacity(PtrVector *vec) {
    return _realloc_with_capacity(vec, vec->capacity / CAPACITY_FACTOR);
}

// Set element in the range of capacity
static PtrVectorError _ptr_vec_emplace_at(PtrVector *vec, size_t idx, void *elem) {
    if (!vec || !elem )       { return PTR_VEC_ERR__NULLPTR; }
    if (idx >= vec->capacity) { return PTR_VEC_ERR__CAPACITY_EXCEEDED; }
    if (idx > vec->length)    { return PTR_VEC_ERR__INDEX_OUT_OF_RANGE; }

    vec->elements[idx] = elem;

    if (idx == vec->length) {
        vec->length++;
    }

    return PTR_VEC_OK;

}

PtrVectorError ptr_vec_set(PtrVector *vec, size_t idx, void *elem) {
    if (!vec || !elem ) { return PTR_VEC_ERR__NULLPTR; }


    if (ptr_vec_is_empty(vec) && idx > 0) {
        return PTR_VEC_ERR__INDEX_NOT_ZERO_ON_EMPTY;
    } else if (!ptr_vec_is_empty(vec) && idx >= vec->length)  {
        return PTR_VEC_ERR__INDEX_OUT_OF_RANGE;
    }

    return _ptr_vec_emplace_at(vec, idx, elem);
}

PtrVectorError ptr_vec_insert(PtrVector *vec, size_t index, void *elem) {
    if (!vec || !elem ) { return PTR_VEC_ERR__NULLPTR; }

    if (!ptr_vec_is_empty(vec) && index > vec->length) {
        return PTR_VEC_ERR__INDEX_OUT_OF_RANGE;
    }

    if (_ptr_vector_is_full(vec)) {
        PtrVectorError err = _ptr_vec_expand_capacity(vec);
        if (err != PTR_VEC_OK) { return err; }
    }

    if (index < vec->length) {
        for (size_t i = vec->length; i > index; i--) {
            vec->elements[i] = vec->elements[i - 1];
        }
        vec->elements[index] = NULL;
        vec->length++;
    }

    // TODO: what are we doing if vector expanded but result is not OK?
    return _ptr_vec_emplace_at(vec, index, elem);
}

PtrVectorError ptr_vec_push_back(PtrVector *vec, void *elem) {
    if (!vec || !elem ) return PTR_VEC_ERR__NULLPTR;

    if (_ptr_vector_is_full(vec)) {
        PtrVectorError err = _ptr_vec_expand_capacity(vec);
        if (err != PTR_VEC_OK) { return err; }
    }

    // TODO: what are we doing if vector expanded but result is not OK?
    return _ptr_vec_emplace_at(vec, vec->length, elem);
}

PtrVectorError ptr_vec_push_front(PtrVector *vec, void *elem) {
    return ptr_vec_insert(vec, 0, elem);
}

void *ptr_vec_last(PtrVector *vec) {
    if (!vec || vec->length == 0) { return NULL; }

    return vec->elements[vec->length - 1];
}

void *ptr_vec_pop_back(PtrVector *vec) {
    if (!vec)             { return NULL; }
    if (vec->length == 0) { return NULL; }

    void *last = ptr_vec_last(vec);

    // TODO: Do we even want to shrink it?
    // It does not seem optimal to shrink it to exact length
    //     and then potentially expand it back later.
    // Maybe shrink it in 1/2 when it's 1/3 full?
    // Or shrink it 1/3? Seems useless. Seems like realloc just for realloc

    vec->elements[vec->length - 1] = NULL;  // Useless operation???
    vec->length--;

    return last;
}

void *ptr_vec_at(PtrVector *vec, size_t index) {
    if (!vec)                 { return NULL; }
    if (index >= vec->length) { return NULL; }

    return vec->elements[index];
}

void ptr_vec_free(PtrVector *vec) {
    if (!vec) { return; }

    if (vec->elements != NULL) {
        free(vec->elements);
        // NOTE(Evgeniy) We don't own memory, that why we don't clean elements itself.
    }
    free(vec);
}
