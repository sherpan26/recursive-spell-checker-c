//spchk.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include "dictionary.h" //correction

#define MAX_WORD_LEN 100

// int dict_size;

int binary_search(const char* word) {
    int low = 0;
    int high = dict_size - 1;

    while (low <= high) {
        int mid = (low + high) / 2;
        int cmp = strcmp(word, dictionary[mid].word);

        if (cmp == 0) {
            return 1;
        } else if (cmp < 0) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    return 0;
}

void process_file(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return;
    }

    char buffer[MAX_WORD_LEN];
    int line = 1, col = 0, word_start_col = 1;
    int ch, index = 0;

    while ((ch = fgetc(fp)) != EOF) {
        col++; // Increment at the start to correctly position the column for each character

        if (isalpha(ch) || ch == '-') {
            if (index == 0) word_start_col = col; // Set word start column at the beginning of a word
            buffer[index++] = ch;
        } else {
            if (index > 0) {
                buffer[index] = '\0';
                if (!is_correct_word(buffer)) {
                    printf("%s (%d, %d): %s\n", filename, line, word_start_col, buffer);
                }
                index = 0;
            }

            if (ch == '\n') {
                line++;
                col = 0; // Reset col for new line, ensuring the next character starts at column 1
            }
        }
    }

    // Check for the last word in the file
    if (index > 0) {
        buffer[index] = '\0';
        if (!is_correct_word(buffer)) {
            printf("%s (Line: %d, Column: %d): %s\n", filename, line, word_start_col, buffer);
        }
    }

    fclose(fp);
}



void process_directory(const char* dirname) {
    DIR* dir = opendir(dirname);
    if (dir == NULL) {
        fprintf(stderr, "Error opening directory: %s\n", dirname);
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') {
            continue;
        }

        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dirname, entry->d_name);

        struct stat st;
        if (stat(path, &st) == -1) {
            fprintf(stderr, "Error accessing file/directory: %s\n", path);
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            process_directory(path);
        } else if (S_ISREG(st.st_mode) && strstr(entry->d_name, ".txt") != NULL) {
            process_file(path);
        }
    }

    closedir(dir);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <dictionary_file> <file_or_directory> ...\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (!load_dictionary(argv[1])) {
        return EXIT_FAILURE;
    }

    int i;
    for (i = 2; i < argc; i++) {
        struct stat st;
        if (stat(argv[i], &st) == -1) {
            fprintf(stderr, "Error accessing file/directory: %s\n", argv[i]);
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            process_directory(argv[i]);
        } else if (S_ISREG(st.st_mode)) {
            process_file(argv[i]);
        }
    }

    free(dictionary);
    return EXIT_SUCCESS;
}


