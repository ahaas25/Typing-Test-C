#include "utilities.h"

/* This will go through the word_array structure and find the requested
    amount of words */
int generate_words(int num_words, Word_array *words, char *to_return) {
    int i, k, number_words_in_txt;

    to_return = malloc(num_words * 64); /* Each word will be less than 64 chars */

    for (i = 0; i < words->number_of_words; i++) {

    }

    return SUCCESS;
}


/* This will dynamically allocate memory to a 2d char array of all the words
    in words.txt */
int parse_words_file(FILE *words_file, Word_array *words) {
    int i, number_of_words = 0;
    char buf[256];

    /* Counts number of lines in words file (Each line contains a unique word) */
    while (fgets(buf, sizeof(buf), words_file) != NULL) {
        number_of_words++;
    }

    printf("Number of words: %d\n", number_of_words);

    return SUCCESS;
}