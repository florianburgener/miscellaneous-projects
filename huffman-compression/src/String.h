#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct String {
    size_t capacity;
    size_t len;
    char *data;
} String;

String *String_init(size_t capacity);

void String_destroy(String **self);

void String_concat(String *self, char *str);

char String_at(String *self, size_t index);

#endif
