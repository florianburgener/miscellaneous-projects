#include "String.h"

String *String_init(size_t capacity) {
    String *self = (String *)malloc(sizeof(String));
    if (self == NULL) {
        perror("String_init");
        exit(EXIT_FAILURE);
    }

    self->capacity = capacity;
    self->len = 0;
    self->data = (char *)malloc(sizeof(char) * self->capacity);
    if (self->data == NULL) {
        perror("String_init");
        exit(EXIT_FAILURE);
    }

    self->data[0] = '\0';
    return self;
}

void String_destroy(String **self) {
    free((*self)->data);
    free(*self);
    *self = NULL;
}

void String_concat(String *self, char *str) {
    while (self->len + strlen(str) + 1 > self->capacity) {
        self->capacity *= 2;
        self->data = realloc(self->data, self->capacity);
    }

    for (size_t i = 0; i < strlen(str); i++) {
        self->data[self->len++] = str[i];
    }

    self->data[self->len] = '\0';
}

char String_at(String *self, size_t index) {
    return self->data[index];
}
