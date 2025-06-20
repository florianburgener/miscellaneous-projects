#ifndef VECU8_H
#define VECU8_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct VecU8 {
    uint8_t *items;
    size_t len;
} VecU8;

VecU8 *VecU8_init(size_t capacity);

void VecU8_destroy(VecU8 **self);

void VecU8_push(VecU8 *self, uint8_t item);

uint8_t VecU8_at(VecU8 *self, size_t index);

void VecU8_copy(VecU8 *src, VecU8 *dest);

#endif
