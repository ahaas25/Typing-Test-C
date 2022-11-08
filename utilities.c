#include "utilities.h"
#include <time.h>

int generate_random(int lower, int upper, int c) {
    time_t x;
    srand(x);
    return (rand() % (upper - lower + 1)) + lower;
}

/* This will go through the word_array structure and find the requested
    amount of words */
int generate_words(int number_of_words, Word_array *words, Word_array *to_return) {
    int i, k;

    /* Remember to free me after the test ends! */

    to_return->number_of_words = number_of_words;
    to_return->words = malloc(sizeof(Word) * number_of_words);

    for (i = 0; i < number_of_words; i++) {
        k = generate_random(0, words->number_of_words, i);
        to_return->words[i].length = words->words[k].length;
        to_return->words[i].text = malloc(words->words[k].length);
        strcpy(to_return->words[i].text, words->words[k].text);
    }

    return SUCCESS;
}


/* This will dynamically allocate memory to a 2d char array of all the words
    in words.txt */
int parse_words_file(FILE *words_file, Word_array *words) {
    int i = 0, number_of_words = 0;
    char buf[256], temp[256];

    /* Counts number of lines in words file (Each line contains a unique word) */
    while (fgets(buf, sizeof(buf), words_file) != NULL) {
        number_of_words++;
    }

    words->number_of_words = number_of_words;
    /* Allocates space for 'number_of_words' Words structures */
    words->words = malloc(sizeof(Word) * number_of_words);

    rewind(words_file);

    while (fgets(buf, sizeof(buf), words_file) != NULL) {
        sscanf(buf, "%s", temp);
        words->words[i].length = strlen(temp);
        words->words[i].text = malloc(strlen(temp));
        strcpy(words->words[i].text, temp);
        i++;
    }

    fclose(words_file);

    return SUCCESS;
}