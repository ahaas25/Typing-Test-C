#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <sys/time.h>
#include <time.h>
#include "utilities.h"
#include "typing_test.h"

void debug_words() {
    FILE *words_file;
    words_file = fopen("words.txt", "r");
    Word_array word_array, return_words;
    int i, number_of_words;
    /* Test parse words file and generate words */
    parse_words_file(words_file, &word_array);
    number_of_words = word_array.number_of_words;
    for (i = 0; i < number_of_words; i++) {
        printf("Word List: %s %d\n", word_array.words[i].text, word_array.words[i].length);
    }

    printf("Number of words in list: %d\n", word_array.number_of_words);

    int words_to_generate = 10;
    printf("\nGenerating %d Words\n", words_to_generate);

    generate_words(words_to_generate, &word_array, &return_words);
    for (i = 0; i < words_to_generate; i++) {
        printf("Generated Word: %s %d\n", return_words.words[i].text, return_words.words[i].length);
    }
}

void debug_stats() {
    int i;
    FILE *stats_file;
    Stat_struct x;
    stats_file = fopen("stats", "r");

    load_stats(stats_file, &x);
    
    for (i = 0; i < 9; i++) {
        printf("%d\n", x.data[i]);
    }

}

int main() {
    debug_stats();
    return 0;
}

