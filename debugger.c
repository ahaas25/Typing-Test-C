#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "typing_test.h"

int main() {
    FILE *words;

    words = fopen("words.txt", "r");
    Word_array word_array;
    parse_words_file(words, word_array);
}