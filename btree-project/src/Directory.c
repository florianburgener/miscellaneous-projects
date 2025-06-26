/**
 * @file Directory.c
 * @author Florian Burgener (florian.burgener@etu.hesge.ch)
 * @version 1.0
 * @date 2022-06-17
 */
#include "Directory.h"

#include <openssl/sha.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Array.h"
#include "BPTree.h"
#include "DirectoryRecord.h"

/**
 * @brief Hashes a string with the SHA-256 algorithm and keeps the first 64 bits.
 *
 * @param str The string to be hashed.
 * @return uint64_t The calculated hash.
 */
static uint64_t hash_string(char *str) {
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str, strlen(str));
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_Final(hash, &sha256);

    uint64_t truncated_hash = 0;
    for (int i = 0; i < 8; i++) {
        truncated_hash |= hash[i];

        if (i != 7) {
            // No shift because it is the last byte.
            truncated_hash <<= 8;
        }
    }

    return truncated_hash;
}

/**
 * @brief Gets the size of the file.
 *
 * @param fp The pointer to the file.
 * @return long The size of the file in bytes.
 */
static long get_file_size(FILE *fp) {
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return file_size;
}

/**
 * @brief Rebuilds the database index.
 *
 * @param directory The directory.
 */
static void rebuild_index(Directory *directory) {
    FILE *fp;
    fp = fopen(directory->database_filename, "rb");

    if (fp == NULL) {
        return;
    }

    long file_size = get_file_size(fp);

    while (true) {
        uint64_t data_ptr = (uint64_t)ftell(fp);
        uint8_t is_deleted;
        fread(&is_deleted, 1, 1, fp);

        if ((bool)is_deleted) {
            // This record has been deleted, go to the next one.
            fseek(fp, DirectoryRecord_size_on_disk() - IS_DELETED_SIZE_IN_BYTES, SEEK_CUR);
        } else {
            // The record has not been deleted.
            char phone_number[PHONE_NUMBER_MAXLEN];
            phone_number[PHONE_NUMBER_MAXLEN - 1] = '\0';
            // The phone number is read.
            fread(phone_number, 1, PHONE_NUMBER_MAXLEN_WITHOUT_NULL_CHARACTER, fp);
            // The record is indexed.
            BPTree_insert(directory->index, hash_string(phone_number), data_ptr);
            fseek(fp, DirectoryRecord_size_on_disk() - IS_DELETED_SIZE_IN_BYTES - PHONE_NUMBER_MAXLEN_WITHOUT_NULL_CHARACTER, SEEK_CUR);
        }

        if (ftell(fp) == file_size) {
            // The end of the file has been reached.
            break;
        }
    }

    fclose(fp);
}

Directory *Directory_init(char database_filename[FILENAME_MAXLEN]) {
    Directory *directory = (Directory *)malloc(sizeof(Directory));
    strcpy(directory->database_filename, database_filename);
    directory->index = BPTree_init(DEFAULT_ORDER);
    rebuild_index(directory);
    return directory;
}

void Directory_destroy(Directory **directory) {
    BPTree_destroy(&(*directory)->index);
    free(*directory);
    *directory = NULL;
}

void Directory_print(Directory *directory) {
    FILE *fp;
    fp = fopen(directory->database_filename, "rb");

    if (fp == NULL) {
        printf("===>The directory is empty.\n");
        return;
    }

    long file_size = get_file_size(fp);
    printf("========================\n");
    int i = 0;

    while (true) {
        // Reads and displays all records.
        ByteArray *byte_array = ByteArray_init(DirectoryRecord_size_on_disk());

        fread(byte_array->items, 1, DirectoryRecord_size_on_disk(), fp);
        byte_array->size = DirectoryRecord_size_on_disk();
        bool is_deleted = (bool)byte_array->items[0];

        if (!is_deleted) {
            DirectoryRecord *record = ByteArray_to_DirectoryRecord(byte_array);
            if (i != 0) {
                printf("------------------------\n");
            }
            DirectoryRecord_print(record);
            DirectoryRecord_destroy(&record);
            i++;
        }

        ByteArray_destroy(&byte_array);

        if (ftell(fp) == file_size) {
            break;
        }
    }

    printf("========================\n");
    fclose(fp);
}

bool Directory_append(Directory *directory, DirectoryRecord *record) {
    uint64_t a;
    if (BPTree_search(directory->index, hash_string(record->phone_number), &a)) {
        // The phone number is already used in another record.
        return false;
    }

    // Writes the record to the file.
    FILE *fp;
    fp = fopen(directory->database_filename, "ab");

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    ByteArray *byte_array = DirectoryRecord_to_ByteArray(record);
    fwrite(byte_array->items, 1, byte_array->size, fp);
    ByteArray_destroy(&byte_array);

    uint64_t data_ptr = (uint64_t)ftell(fp) - DirectoryRecord_size_on_disk();
    fclose(fp);

    BPTree_insert(directory->index, hash_string(record->phone_number), data_ptr);
    return true;
}

DirectoryRecord *Directory_search(Directory *directory, char phone_number[PHONE_NUMBER_MAXLEN]) {
    uint64_t data_ptr;
    if (!BPTree_search(directory->index, hash_string(phone_number), &data_ptr)) {
        // The record was not found.
        return NULL;
    }

    FILE *fp;
    fp = fopen(directory->database_filename, "rb");

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fseek(fp, data_ptr, SEEK_SET);
    ByteArray *byte_array = ByteArray_init(DirectoryRecord_size_on_disk());
    fread(byte_array->items, 1, DirectoryRecord_size_on_disk(), fp);
    byte_array->size = DirectoryRecord_size_on_disk();
    fclose(fp);

    DirectoryRecord *record = ByteArray_to_DirectoryRecord(byte_array);
    ByteArray_destroy(&byte_array);
    return record;
}

bool Directory_delete(Directory *directory, char phone_number[PHONE_NUMBER_MAXLEN]) {
    uint64_t data_ptr;
    if (!BPTree_search(directory->index, hash_string(phone_number), &data_ptr)) {
        // The record to be deleted does not exist.
        return false;
    }

    FILE *fp;
    fp = fopen(directory->database_filename, "r+b");

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fseek(fp, data_ptr, SEEK_SET);
    uint8_t is_deleted = (uint8_t) true;
    fwrite(&is_deleted, 1, 1, fp);
    fclose(fp);

    BPTree_delete(directory->index, hash_string(phone_number));
    return true;
}
