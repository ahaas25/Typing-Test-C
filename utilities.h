#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUCCESS 1
#define FAILURE 0

typedef struct {
    int length;
    char *text;
} Word;

typedef struct {
    int number_of_words;
    Word *words;
} Word_array;

void clear_word_array(Word_array *array);
int generate_words(int num_words, Word_array *words, Word_array *to_return);
int parse_words_file(FILE *words_file, Word_array *words);