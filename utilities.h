#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUCCESS 1
#define FAILURE 0

typedef struct {
    int number_of_words;
    char words[];
} Word_array;

int generate_words(int num_words, Word_array *words, char *to_return);
int parse_words_file(FILE *words_file, Word_array *words);