/**
 * @file DirectoryRecord.h
 * @author Florian Burgener (florian.burgener@etu.hesge.ch)
 * @version 1.0
 * @date 2022-06-17
 */
#ifndef DIRECTORY_RECORD_H
#define DIRECTORY_RECORD_H

#include <stdbool.h>

#include "Array.h"

#define IS_DELETED_SIZE_IN_BYTES 1

#define PHONE_NUMBER_MAXLEN_WITHOUT_NULL_CHARACTER 10
#define PHONE_NUMBER_MAXLEN 1 + PHONE_NUMBER_MAXLEN_WITHOUT_NULL_CHARACTER

#define NAME_MAXLEN_WITHOUT_NULL_CHARACTER 20
#define NAME_MAXLEN 1 + NAME_MAXLEN_WITHOUT_NULL_CHARACTER

#define SURNAME_MAXLEN_WITHOUT_NULL_CHARACTER 20
#define SURNAME_MAXLEN 1 + SURNAME_MAXLEN_WITHOUT_NULL_CHARACTER

#define BIRTH_DATE_SIZE_IN_BYTES 4

/**
 * @brief Data structure that represents a record.
 *
 */
typedef struct DirectoryRecord {
    bool is_deleted;
    char phone_number[PHONE_NUMBER_MAXLEN];
    char name[NAME_MAXLEN];
    char surname[SURNAME_MAXLEN];
    int birth_date_year;
    int birth_date_month;
    int birth_date_day;
} DirectoryRecord;

/**
 * @brief Size in bytes that a record takes when written on disk.
 *
 * @return int Size in bytes.
 */
int DirectoryRecord_size_on_disk();

/**
 * @brief Initializes the "DirectoryRecord" data structure.
 *
 * @param is_deleted false = the record is not deleted, true = the record is deleted.
 * @param phone_number The phone number.
 * @param name The name.
 * @param surname The surname.
 * @param birth_date_year The year of birth.
 * @param birth_date_month The month of birth.
 * @param birth_date_day The day of birth.
 * @return DirectoryRecord* The record initialized with the data.
 */
DirectoryRecord *DirectoryRecord_init(bool is_deleted, char phone_number[PHONE_NUMBER_MAXLEN], char name[NAME_MAXLEN], char surname[SURNAME_MAXLEN], int birth_date_year, int birth_date_month, int birth_date_day);

/**
 * @brief Destroys the record and free its memory.
 *
 * @param record The record to be destroyed.
 */
void DirectoryRecord_destroy(DirectoryRecord **record);

/**
 * @brief Displays the record on the console.
 *
 * @param record The record to display.
 */
void DirectoryRecord_print(DirectoryRecord *record);

/**
 * @brief Converts a record into an array of bytes.
 *
 * @param record The record to be converted.
 * @return ByteArray* The resulting byte array.
 */
ByteArray *DirectoryRecord_to_ByteArray(DirectoryRecord *record);

/**
 * @brief Converts an array of bytes into a record.
 *
 * @param byte_array The array of bytes to be converted.
 * @return DirectoryRecord* The resulting record.
 */
DirectoryRecord *ByteArray_to_DirectoryRecord(ByteArray *byte_array);

#endif
