#include "utilities.h"

/* Clears word array given in parameters */
void clear_word_array(Word_array *array) {
    int i;
    for (i = 0; i < array->number_of_words; i++) {
        free(array->words[i].text);
    }
    free(array->words);
    array->number_of_words = 0;
}

/* Generates a pseudo random number using current millisecond + iteration since
    code runs too fast */
int generate_random(int lower, int upper, int c) {
    struct timeval te;
    gettimeofday(&te, NULL);
    long long milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000;
    srand(milliseconds + c);
    return (rand() % (upper - lower + 1)) + lower;
}

/* This will go through the word_array structure and find the requested
    amount of words */
int generate_words(int number_of_words, Word_array *words, Word_array *to_return) {
    int i, k, len = 0;

    /* Remember to free me after the test ends! */

    to_return->number_of_words = number_of_words;
    to_return->words = malloc(sizeof(Word) * number_of_words);

    for (i = 0; i < number_of_words; i++) {
        k = generate_random(0, words->number_of_words - 1, i);
        len += words->words[k].length + 1;
        to_return->words[i].length = words->words[k].length;
        to_return->words[i].text = malloc(words->words[k].length);
        strcpy(to_return->words[i].text, words->words[k].text);
    }

    to_return->num_characters = len;

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

/* Creates empty stats file */
int create_stats_file(FILE *stats_file) {
    int i;

    for (i = 0; i < NUM_STATS; i++) {
        fputs("0\n", stats_file);
    }

    fclose(stats_file);
    return SUCCESS;
}

/* Loads user data from stats file located in same directory as program */
int load_stats(FILE *stats_file, Stat_struct *stats) {
    int i = 0;
    int temp_stat;
    char buf[256];

    while (fgets(buf, sizeof(buf), stats_file) != NULL) {
        sscanf(buf, "%d", &temp_stat);
        stats->data[i] = temp_stat;
        i++;
    }

    fclose(stats_file);

    return SUCCESS;
}

/* Saves user data to stats file located in same directory as program */
int save_stats(FILE *stats_file, Stat_struct *stats) {
    int i;

    for (i = 0; i < NUM_STATS; i++) {
        fprintf(stats_file, "%d\n", stats->data[i]);
    }

    fclose(stats_file);

    return SUCCESS;
}

/* Finds highest WPM score */
int update_max_wpm(Stat_struct *stats) {
    int i, max = stats->data[W_5];
    for (i = W_5; i <= W_100; i++) {
        if (max < stats->data[i]) {
            max = stats->data[i];
        }
    }
    stats->data[BEST_WPM] = max;
    return SUCCESS;
}


/* Appends first parameter to the end of end of the second parameter
    (Does not change size of the target, assumes there is space) */
void append_line(char *source, char *target) {
    int length = strlen(target) + strlen(source);
    int i, cursor = 0;

    for (i = strlen(target); i < length; i++) {
        target[i] = source[cursor];
        cursor++;
    }
    target[i] = '\0'; /* End string */

}