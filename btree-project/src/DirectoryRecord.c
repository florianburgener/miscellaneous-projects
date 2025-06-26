/**
 * @file DirectoryRecord.c
 * @author Florian Burgener (florian.burgener@etu.hesge.ch)
 * @version 1.0
 * @date 2022-06-17
 */
#include "DirectoryRecord.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Array.h"

int DirectoryRecord_size_on_disk() {
    int size = 0;
    size += IS_DELETED_SIZE_IN_BYTES;
    size += PHONE_NUMBER_MAXLEN_WITHOUT_NULL_CHARACTER;
    size += NAME_MAXLEN_WITHOUT_NULL_CHARACTER;
    size += SURNAME_MAXLEN_WITHOUT_NULL_CHARACTER;
    size += BIRTH_DATE_SIZE_IN_BYTES;
    return size;
}

DirectoryRecord *DirectoryRecord_init(bool is_deleted, char phone_number[PHONE_NUMBER_MAXLEN], char name[NAME_MAXLEN], char surname[SURNAME_MAXLEN], int birth_date_year, int birth_date_month, int birth_date_day) {
    DirectoryRecord *record = (DirectoryRecord *)malloc(sizeof(DirectoryRecord));
    record->is_deleted = is_deleted;
    memset(record->phone_number, 0, PHONE_NUMBER_MAXLEN);
    strcpy(record->phone_number, phone_number);
    memset(record->name, 0, NAME_MAXLEN);
    strcpy(record->name, name);
    memset(record->surname, 0, SURNAME_MAXLEN);
    strcpy(record->surname, surname);
    record->birth_date_year = birth_date_year;
    record->birth_date_month = birth_date_month;
    record->birth_date_day = birth_date_day;
    return record;
}

void DirectoryRecord_destroy(DirectoryRecord **record) {
    free(*record);
    *record = NULL;
}

void DirectoryRecord_print(DirectoryRecord *record) {
    printf("Phone Number: %s\n", record->phone_number);
    printf("Name: %s\n", record->name);
    printf("Surname: %s\n", record->surname);
    printf("Birth Date: %d-%.2d-%.2d\n", record->birth_date_year, record->birth_date_month, record->birth_date_day);
}

ByteArray *DirectoryRecord_to_ByteArray(DirectoryRecord *record) {
    int capacity = DirectoryRecord_size_on_disk();
    ByteArray *array = ByteArray_init(capacity);

    // Convert the "is_deleted" field to byte.
    ByteArray_append(array, (uint8_t)record->is_deleted);

    // Converts each character of the phone number to byte.
    for (int i = 0; i < PHONE_NUMBER_MAXLEN_WITHOUT_NULL_CHARACTER; i++) {
        ByteArray_append(array, (uint8_t)record->phone_number[i]);
    }

    // Converts each character of the name to byte.
    for (int i = 0; i < NAME_MAXLEN_WITHOUT_NULL_CHARACTER; i++) {
        ByteArray_append(array, (uint8_t)record->name[i]);
    }

    // Converts each character of the surname to byte.
    for (int i = 0; i < SURNAME_MAXLEN_WITHOUT_NULL_CHARACTER; i++) {
        ByteArray_append(array, (uint8_t)record->surname[i]);
    }

    // Converts the year of birth to bytes.
    ByteArray_append(array, (uint8_t)((record->birth_date_year >> 8) & 255));
    ByteArray_append(array, (uint8_t)(record->birth_date_year & 255));
    // Converts the month of birth to byte.
    ByteArray_append(array, (uint8_t)record->birth_date_month);
    // Converts the day of birth to byte.
    ByteArray_append(array, (uint8_t)record->birth_date_day);

    return array;
}

DirectoryRecord *ByteArray_to_DirectoryRecord(ByteArray *byte_array) {
    int i = 0;

    // Initliases the "is_deleted" variable with the first byte.
    bool is_deleted = (bool)byte_array->items[i];
    i++;

    char phone_number[PHONE_NUMBER_MAXLEN];
    phone_number[PHONE_NUMBER_MAXLEN - 1] = '\0';

    // Reconstruct the phone number string with the next 10 bytes.
    for (int j = 0; j < PHONE_NUMBER_MAXLEN_WITHOUT_NULL_CHARACTER; j++) {
        phone_number[j] = (char)byte_array->items[i];
        i++;
    }

    char name[NAME_MAXLEN];
    name[NAME_MAXLEN - 1] = '\0';

    // Reconstruct the name string with the next 20 bytes.
    for (int j = 0; j < NAME_MAXLEN_WITHOUT_NULL_CHARACTER; j++) {
        name[j] = (char)byte_array->items[i];
        i++;
    }

    char surname[SURNAME_MAXLEN];
    surname[SURNAME_MAXLEN - 1] = '\0';

    // Reconstruct the name string with the next 20 bytes.
    for (int j = 0; j < SURNAME_MAXLEN_WITHOUT_NULL_CHARACTER; j++) {
        surname[j] = (char)byte_array->items[i];
        i++;
    }

    // Reconstitutes the date of birth from the last bytes.
    int birth_date_year = (int)byte_array->items[i] << 8 | (int)byte_array->items[i + 1];
    int birth_date_month = (int)byte_array->items[i + 2];
    int birth_date_day = (int)byte_array->items[i + 3];

    return DirectoryRecord_init(is_deleted, phone_number, name, surname, birth_date_year, birth_date_month, birth_date_day);
}
