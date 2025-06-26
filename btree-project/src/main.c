/**
 * @file main.c
 * @author Florian Burgener (florian.burgener@etu.hesge.ch)
 * @version 1.0
 * @date 2022-06-17
 */
#include <stdio.h>
#include <stdlib.h>

#include "Directory.h"
#include "DirectoryRecord.h"

/**
 * @brief Empties the buffer.
 *
 */
void clear_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

/**
 * @brief Procedure to append a record to the database.
 *
 * @param directory The directory.
 */
void append_record(Directory *directory) {
    // Reads the user entry for the phone number.
    char phone_number[PHONE_NUMBER_MAXLEN];
    printf("Enter the phone number: ");
    scanf("%10s", phone_number);
    clear_buffer();

    // Reads the user entry for the name.
    char name[NAME_MAXLEN];
    printf("Enter the name: ");
    scanf("%20s", name);
    clear_buffer();

    // Reads the user entry for the surname.
    char surname[SURNAME_MAXLEN];
    printf("Enter the surname: ");
    scanf("%20s", surname);
    clear_buffer();

    // Loop until three integers are given.
    int birth_date_year, birth_date_month, birth_date_day;
    do {
        printf("Enter the birth date (Y-m-d): ");
    } while (scanf("%d-%d-%d", &birth_date_year, &birth_date_month, &birth_date_day) != 3);

    clear_buffer();
    printf("\nIs the information entered correct? (Y/n) ");

    char choice = getchar();
    if (choice != '\n') {
        getchar();
    }

    if (choice == 'n') {
        // The user has cancelled the procedure.
        printf("\n===>The procedure has been cancelled.\n");
        return;
    }

    DirectoryRecord *record = DirectoryRecord_init(false, phone_number, name, surname, birth_date_year, birth_date_month, birth_date_day);

    if (Directory_append(directory, record)) {
        printf("\n===>The record has been saved in the directory.\n");
    } else {
        printf("\n===>This phone number is already associated with a record. The record has not been saved.\n");
    }

    DirectoryRecord_destroy(&record);
}

/**
 * @brief Procedure to search for a record in the database.
 *
 * @param directory The directory.
 */
void search_record(Directory *directory) {
    printf("Enter the phone number that corresponds to the record you are looking for: ");
    char phone_number[PHONE_NUMBER_MAXLEN];
    scanf("%10s", phone_number);
    clear_buffer();
    DirectoryRecord *record = Directory_search(directory, phone_number);
    printf("\n");

    if (record == NULL) {
        printf("===>No records were found for this phone number.\n");
    } else {
        printf("========================\n");
        DirectoryRecord_print(record);
        DirectoryRecord_destroy(&record);
        printf("========================\n");
    }
}

/**
 * @brief Procedure for deleting a record from the database.
 *
 * @param directory The directory.
 */
void delete_record(Directory *directory) {
    printf("Enter the phone number that corresponds to the record you wish to delete: ");

    char phone_number[PHONE_NUMBER_MAXLEN];
    scanf("%10s", phone_number);
    clear_buffer();
    DirectoryRecord *record = Directory_search(directory, phone_number);
    printf("\n");

    if (record == NULL) {
        printf("===>No records were found for this phone number.\n");
        return;
    }

    printf("========================\n");
    DirectoryRecord_print(record);
    DirectoryRecord_destroy(&record);
    printf("========================\n");

    printf("\nAre you sure you want to delete this record? (Y/n) ");

    char choice = getchar();
    if (choice != '\n') {
        getchar();
    }

    if (choice == 'n') {
        printf("\n===>The procedure has been cancelled.\n");
    } else {
        Directory_delete(directory, phone_number);
        printf("\nThe record has been deleted from the directory.\n");
    }
}

int main() {
    Directory *directory = Directory_init("directory_database");

    while (true) {
        BPTree_print(directory->index, 0);

        printf("Enter 1 to add a member.\n");
        printf("Enter 2 to search for a member via their phone number.\n");
        printf("Enter 3 to delete a member.\n");
        printf("Enter 4 to display all members.\n");
        printf("Enter 5 to exit the program.\n");
        printf("What do you want to do? ");
        int action;
        scanf("%d", &action);
        clear_buffer();

        if (action < 1 || action > 5) {
            // The action is invalid.
            system("clear");
            continue;
        }

        if (action == 5) {
            // The program is exited.
            break;
        }

        printf("\n");

        switch (action) {
            case 1:
                append_record(directory);
                break;
            case 2:
                search_record(directory);
                break;
            case 3:
                delete_record(directory);
                break;
            case 4:
                Directory_print(directory);
                break;
        }

        printf("\nPress enter to continue...");
        getchar();
        system("clear");
    }

    Directory_destroy(&directory);
    return EXIT_SUCCESS;
}
