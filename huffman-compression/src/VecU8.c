#include "VecU8.h"

VecU8 *VecU8_init(size_t capacity) {
    VecU8 *self = (VecU8 *)malloc(sizeof(VecU8));
    if (self == NULL) {
        perror("VecNode_init");
        exit(EXIT_FAILURE);
    }

    self->items = (uint8_t *)malloc(sizeof(uint8_t) * capacity);
    if (self->items == NULL) {
        perror("VecNode_init");
        exit(EXIT_FAILURE);
    }

    self->len = 0;
    return self;
}

void VecU8_destroy(VecU8 **self) {
    free((*self)->items);
    free(*self);
    *self = NULL;
}

void VecU8_push(VecU8 *self, uint8_t item) {
    self->items[self->len] = item;
    self->len++;
}

uint8_t VecU8_at(VecU8 *self, size_t index) {
    return self->items[index];
}

void VecU8_delete_at_index(VecU8 *self, size_t index) {
    for (size_t i = index; i + 1 < self->len; i++) {
        self->items[i] = VecU8_at(self, i + 1);
    }

    self->len--;
}

uint8_t VecU8_pop(VecU8 *self, size_t index) {
    uint8_t item = VecU8_at(self, index);
    VecU8_delete_at_index(self, index);
    return item;
}

void VecU8_copy(VecU8 *src, VecU8 *dest) {
    for (size_t i = 0; i < src->len; i++) {
        dest->items[i] = VecU8_at(src, i);
    }

    dest->len = src->len;
}
