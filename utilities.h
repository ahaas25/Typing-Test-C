#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define SUCCESS 1
#define FAILURE 0
#define TYPING_PROMPT_START_Y 3
#define TYPING_PROMPT_END_Y 6

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
    int best_wpm, w_10, w_25, w_50, w_100;
    int tests_complete, chars_typed, chars_correct;
    double time_typed; /* In seconds */
} Stat_struct;

void clear_word_array(Word_array *array);
int generate_words(int num_words, Word_array *words, Word_array *to_return);
int parse_words_file(FILE *words_file, Word_array *words);
int create_stats_file(FILE *stats_file);
int load_stats(FILE *stats_file, Stat_struct *stats);
int save_stats(FILE *stats_file, Stat_struct *stats);
void append_line(char *source, char *target);