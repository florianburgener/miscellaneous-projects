/**
 * @file Directory.h
 * @author Florian Burgener (florian.burgener@etu.hesge.ch)
 * @version 1.0
 * @date 2022-06-17
 */
#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <stdbool.h>

#include "BPTree.h"
#include "DirectoryRecord.h"

#define DEFAULT_ORDER 2
#define FILENAME_MAXLEN 100

/**
 * @brief Data structure that represents a directory database.
 *
 */
typedef struct Directory {
    char database_filename[FILENAME_MAXLEN];
    BPTreeNode *index;
} Directory;

/**
 * @brief Initializes the "Directory" data structure.
 *
 * @param database_filename The name of the database file.
 * @return Directory* The initialized directory.
 */
Directory *Directory_init(char database_filename[FILENAME_MAXLEN]);

/**
 * @brief Destroys the directory and free its memory.
 *
 * @param directory The directory to be destroyed.
 */
void Directory_destroy(Directory **directory);

/**
 * @brief Displays the directory on the console.
 *
 * @param directory The directory to be displayed.
 */
void Directory_print(Directory *directory);

/**
 * @brief Appends a record to the directory.
 *
 * @param directory The directory in which the record must be added.
 * @param record The record to be added.
 * @return true The record could be added.
 * @return false The record could not be added.
 */
bool Directory_append(Directory *directory, DirectoryRecord *record);

/**
 * @brief Searches for a record in the directory via the phone number.
 *
 * @param directory The directory in which to search.
 * @param phone_number The telephone number of the record.
 * @return DirectoryRecord* The corresponding record, NULL if not found.
 */
DirectoryRecord *Directory_search(Directory *directory, char phone_number[PHONE_NUMBER_MAXLEN]);

/**
 * @brief Deletes a record from the directory.
 *
 * @param directory The directory in which the record must be deleted.
 * @param phone_number The telephone number of the record.
 * @return true The record could be deleted.
 * @return false The record could not be deleted.
 */
bool Directory_delete(Directory *directory, char phone_number[PHONE_NUMBER_MAXLEN]);

#endif
