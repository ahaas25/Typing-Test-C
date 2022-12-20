#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define SUCCESS 1
#define FAILURE 0
#define TYPING_PROMPT_START_Y 3
#define TYPING_PROMPT_END_Y 6
#define BEST_WPM 0
#define W_10 1
#define W_25 2
#define W_50 3
#define W_100 4
#define TESTS_COMPLETE 5
#define CHARS_TYPED 6
#define CHARS_CORRECT 7
#define TIME_TYPED 8

/* Word Structure
    length - length of word
    text - word String */
typedef struct {
    int length;
    char *text;
} Word;

/* Word Array Structure
    number_of_words - stores number of words
    num_characters - is total character count of array (including spaces)
    words - Array of 'Word' structures  */
typedef struct {
    int number_of_words, num_characters;
    Word *words;
} Word_array;

typedef struct {
    double data[9];
} Stat_struct;

void clear_word_array(Word_array *array);
int generate_words(int num_words, Word_array *words, Word_array *to_return);
int parse_words_file(FILE *words_file, Word_array *words);
int create_stats_file(FILE *stats_file);
int load_stats(FILE *stats_file, Stat_struct *stats);
int save_stats(FILE *stats_file, Stat_struct *stats);
void append_line(char *source, char *target);