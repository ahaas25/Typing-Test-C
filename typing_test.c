#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <ctype.h>
#include "typing_test.h"
#include "utilities.h"

/* Prints Centered Text To Console Screen */
void print_centered_text(WINDOW *win, int row, char *str) {
    int text_spacing = strlen(str);
    int center_col = (win->_maxx - text_spacing) / 2;
    mvwprintw(win, row, center_col, str);
}

/* Prints centered text for menu items */
void print_centered_text_menu(WINDOW *win, int row, int target, char str[][MAX_STRING],
    int highlight, int elements) {
    int text_length = 0, i, k, cursor;

    for (i = 0; i < elements; i++) {
        for (k = 0; k < strlen(str[i]); k++) {
            text_length++;
        }
        if (i < elements - 1) {
            text_length += 2; /* Account for ", " */
        }
    }

    cursor = (win->_maxx - text_length) / 2;
    move(row, cursor);

    for (i = 0; i < elements; i++) {
        if (i == target && highlight) {
            attron(A_STANDOUT);
        }
        printw(str[i]);
        attroff(A_STANDOUT);
        if (i < elements - 1) {
            printw(", ");
        }

    }
}

/* Prints the typing prompt onto the terminal.
    Automatically centers, wraps, and scrolls through text */
void print_typing_prompt(WINDOW *win, Word_array *prompt, char *user_input) {
    int i, window_width = win->_maxx;

    /* Split prompt into Strings that fit terminal width */

    for (i = 0; i < prompt->number_of_words; i++) {
        mvwprintw(win, 3, 0, "%d: %s ", i, prompt->words[i].text);
    }

    printw("User input %s ", strlen(user_input), user_input);
}

void typing_ui(WINDOW *win, int level, int mode, Word_array *word_array) {
    int run = 1, ch, i, new_test = 1, user_input_length;
    char str[1024], *user_input = NULL;
    Word_array *prompt = NULL;

    while (run) {
        if (new_test == 1) {
            clear();

            /* Reset str */
            str[0] = '\0';

            if (mode == 0) {
                strcat(str, "Timed Test - ");
                strcat(str, TIMED_MODES_STRING[level]);
                print_centered_text(stdscr, 0, str);
            } else {
                strcat(str, "Word Test - ");
                strcat(str, WORD_MODES_STRING[level]);
                print_centered_text(stdscr, 0, str);
            }

            printw("\nPrompt:\n"); /* Temp */

            if (prompt == NULL) {
                prompt = malloc(sizeof(Word_array));
            } else {
                clear_word_array(prompt);
            }

            /* 10 is a placeholder */
            generate_words(10, word_array, prompt);

            if (user_input != NULL) {
                free(user_input);
            }

            user_input = malloc(prompt->num_characters + 1);
            user_input[0] = '\0';
            user_input_length = 0;

            new_test = 0;
        }

        ch = getch();
        if (isalpha(ch)) {
            /* Ensures user does not type outside of character limit */
            if (user_input_length < prompt->num_characters) {
                user_input[user_input_length] = ch;
                user_input_length++;
                user_input[user_input_length] = '\0';
                move(0, 0);
                printw("Size: %d", strlen(user_input));
            }
        } else if (ch == KEY_BACKSPACE) {
            if (user_input_length > 0) {
                user_input_length--;
                user_input[user_input_length] = '\0';
                move(0, 0);
                printw("Size: %d", strlen(user_input));
            }
        } else if (ch == '	') {
            printw("RESET TEST");
            new_test = 1;
        }

        /* Prints typing prompt after all input is done processing */
/*         print_typing_prompt(win, prompt, "Hello"); */
    }

    free(prompt);
}

/* Main function. Creates main menu */
int main() {
    FILE *words_file;
    Word_array *word_array;
    int cursor_x = 0, cursor_y = 0, run = 1;
    int ch, key;

    word_array = malloc(sizeof(Word_array));
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();

    words_file = fopen("words.txt", "r");

    print_centered_text(stdscr, 4, "Loading...");

    if (has_colors() == FALSE) {
        print_centered_text(stdscr, 4, "Your terminal does not support color");
        print_centered_text(stdscr, 5, "This may result in unexpected behavior\n");
        print_centered_text(stdscr, 6, "Press any key to continue.\n");
    } else {
        start_color();
    }

    if (words_file == NULL) {
        run = 0;

        init_color(COLOR_WHITE, 255, 255, 0);
        init_pair(1, COLOR_RED, COLOR_WHITE);
        attron(COLOR_PAIR(1));
        print_centered_text(stdscr, 4, "Words list could not be loaded.");
        print_centered_text(stdscr, 5, "Please ensure words.txt is not missing");
        print_centered_text(stdscr, 6, "Press any key to exit.\n");
        attroff(COLOR_PAIR(0));
        ch = getch();
    } else {
        parse_words_file(words_file, word_array);
    }

    clear();

    while (run) {
        init_pair(2, COLOR_WHITE, COLOR_BLACK);
        attron(COLOR_PAIR(2));

        print_centered_text(stdscr, 0, "Typing Test");
        print_centered_text(stdscr, 1, "By Aidan Haas");
        print_centered_text(stdscr, 3, "Modes");
        print_centered_text(stdscr, 5, "Timed");
        print_centered_text_menu(stdscr, 6, cursor_x, TIMED_MODES_STRING, cursor_y == 0, NUM_MODES);
        print_centered_text(stdscr, 8, "Words");
        print_centered_text_menu(stdscr, 9, cursor_x, WORD_MODES_STRING, cursor_y == 1, NUM_MODES);
        print_centered_text_menu(stdscr, 11, cursor_x, MISC_STRING, cursor_y == 2, NUM_MISC);
        printw("\n");

        ch = getch();
        switch (ch) {
        case KEY_UP:
            if (cursor_y > 0) {
                cursor_y--;
            } else {
                cursor_y = MAX_Y - 1;
            }
            break;
        case KEY_DOWN:
            if (cursor_y < MAX_Y - 1) {
                cursor_y++;
            } else {
                cursor_y = 0;
            }
            break;
        case KEY_RIGHT:
            if (cursor_x < MAX_X - 1) {
                cursor_x++;
            } else {
                cursor_x = 0;
            }
            break;
        case KEY_LEFT:
            if (cursor_x > 0) {
                cursor_x--;
            } else {
                cursor_x = MAX_X - 1;
            }
            break;
        default:
            break;
        }
        printw("%d %d", cursor_x, cursor_y);
        if (ch == '\n') {
            if (cursor_x == 0 && cursor_y == 2) {
                run = 0;
            } else if (cursor_y == 1 || cursor_y == 0) {
                typing_ui(stdscr, cursor_x, cursor_y, word_array);
            }
        }

    }

    /* Exiting */

    refresh();
    endwin();

    return 0;
}