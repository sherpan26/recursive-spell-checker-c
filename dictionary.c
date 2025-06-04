//dictionary.c
#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Assumes MAX_WORD_LEN is defined in dictionary.h

DictEntry* dictionary = NULL;
int dict_size = 0;

// Function to load dictionary from a file
int load_dictionary(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening dictionary file: %s\n", filename);
        return 0;
    }

    // First, count the number of words
    char line[MAX_WORD_LEN];
    int count = 0;
    while (fgets(line, sizeof(line), fp)) {
        count++;
    }
    dict_size = count;

    // Allocate memory for the dictionary
    dictionary = (DictEntry*)malloc(dict_size * sizeof(DictEntry));
    if (dictionary == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(fp);
        return 0;
    }

    // Reset file pointer to beginning
    rewind(fp);

    // Populate the dictionary
    int index = 0;
    while (fgets(line, sizeof(line), fp) && index < dict_size) {
        // Remove newline character
        line[strcspn(line, "\n")] = '\0';
        strcpy(dictionary[index++].word, line);
    }

    fclose(fp);
    return 1;
}


// Function to check if a word is spelled correctly
// int is_correct_word(const char* word) {
//     // First, attempt an exact match.
//     if (binary_search(word)) return 1;

//     // Convert the word to lowercase for further comparisons
//     char lower[MAX_WORD_LEN];
//     strncpy(lower, word, MAX_WORD_LEN);
//     lower[MAX_WORD_LEN - 1] = '\0'; // Ensure null termination
//     for (int i = 0; lower[i]; i++) {
//         lower[i] = tolower(lower[i]);
//     }

//     // If the original word had at least one uppercase letter (excluding the first character),
//     // and it's not an all-uppercase word, don't match against lowercase or capitalized versions.
//     int hasInternalCaps = 0;
//     for (int i = 1; word[i]; i++) {
//         if (isupper(word[i])) {
//             hasInternalCaps = 1;
//             break;
//         }
//     }
//     if (hasInternalCaps && strcmp(word, lower) != 0) {
//         // The word has internal capitalization and is not all uppercase; 
//         // skip lowercase and initial capital checks since they wouldn't match the spec.
//         return 0;
//     }

//     // Attempt to match the lowercase version against the dictionary.
//     if (binary_search(lower)) return 1;

//     // For the initial capital, capitalize the first letter if it's not already.
//     if (!isupper(lower[0])) {
//         lower[0] = toupper(lower[0]);
//         if (binary_search(lower)) return 1;
//     }

//     // Check the all-uppercase version only if the word isn't already uppercase.
//     if (strcmp(word, lower) != 0) {
//         for (int i = 0; lower[i]; i++) {
//             lower[i] = toupper(lower[i]);
//         }
//         if (binary_search(lower)) return 1;
//     }

//     return 0; // Word is not found
// }
// Function to check if a word is spelled correctly

// Function to check if a word is spelled correctly
int is_correct_word(const char* word) {
    int isAllCaps = 1, hasInternalCaps = 0;
    char lower[MAX_WORD_LEN];
    size_t len = strlen(word);

    // Convert the word to lowercase and check for internal caps
    for (size_t i = 0; i < len; i++) {
        if (i > 0 && isupper(word[i])) {
            hasInternalCaps = 1; // Flag for internal uppercase letters
        }
        if (!isupper(word[i])) {
            isAllCaps = 0; // Not all characters are uppercase
        }
        lower[i] = tolower(word[i]);
    }
    lower[len] = '\0'; // Ensure null-termination

    // If the word is all uppercase or has no internal caps, check the lowercase version
    if (isAllCaps || !hasInternalCaps) {
        if (binary_search(lower)) return 1;
    }

    // Check for initial capital (after confirming it's not all-uppercase or internally capitalized)
    if (!isAllCaps && !hasInternalCaps) {
        lower[0] = toupper(lower[0]);
        if (binary_search(lower)) return 1;
    }

    // At this point, if the word was all uppercase, it's already been checked.
    // Words with internal capitalization not matching any allowed pattern are considered incorrect.
    return 0; // Word is not found
}


