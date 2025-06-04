//dictionary.h
#ifndef DICTIONARY_H
#define DICTIONARY_H

#define MAX_WORD_LEN 100

typedef struct {
    char word[MAX_WORD_LEN];
} DictEntry;

extern DictEntry* dictionary;
extern int dict_size;

int load_dictionary(const char* filename);
int is_correct_word(const char* word);
int binary_search(const char* word); // Declared binary_search

#endif // DICTIONARY_H

