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
void print_typing_prompt(WINDOW *win, Word_array *prompt, char *prompt_string,
    char *user_input) {
    int i, k, window_width = win->_maxx;

    for (i = TYPING_PROMPT_START_Y; i < TYPING_PROMPT_END_Y; i++) {
        move(i, 0);
        clrtoeol();
    }

    /* Iterate through each character to make lines that fit in console width */

    move(TYPING_PROMPT_START_Y, 0);
    printw("%s", prompt_string); /* Temporary, prints entire prompt on one line (is auto wrapped) */
    move(TYPING_PROMPT_START_Y, 0);


    /* Goes through each character to print if user typed it correctly or not */
    for (i = 0; i < strlen(user_input); i++) {
        /* If user character matches prompt character */
        if (prompt_string[i] == user_input[i]) {
            attron(COLOR_PAIR(3));
            printw("%c", user_input[i]);
            attroff(COLOR_PAIR(3));
        } else {
            attron(COLOR_PAIR(4));
            printw("%c", user_input[i]);
            attroff(COLOR_PAIR(4));
        }
    }
}

void typing_ui(WINDOW *win, int level, int mode, Word_array *word_array) {
    int run = 1, ch, i, new_test = 1, user_input_length;
    char str[1024], *user_input = NULL;
    char *prompt_string = NULL; /* Full prompt string */
    Word_array *prompt = NULL;
    Word *prompt_lines; /* Array of lines for displaying */


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

            if (prompt == NULL) {
                prompt = malloc(sizeof(Word_array));
            } else {
                clear_word_array(prompt);
            }

            /* Only works for words mode for now, no timed */

            generate_words(WORD_MODES[level], word_array, prompt);

            if (user_input != NULL) {
                free(user_input);
            }

            user_input = malloc(prompt->num_characters + 1);
            user_input[0] = '\0';
            user_input_length = 0;

            new_test = 0;

            prompt_string = malloc(prompt->num_characters); /* For easier parsing of prompt */
            prompt_string[0] = '\0';

            /* This goes out of bounds */
            for (i = 0; i < prompt->number_of_words; i++) {
                append_line(prompt->words[i].text, prompt_string);
                if (i < prompt->number_of_words - 1) {
                    append_line(" ", prompt_string); /* Add spaces */
                }
            }

            printf("%s", user_input);

            print_typing_prompt(win, prompt, prompt_string, user_input);
        }

        ch = getch();
        if (isalnum(ch) || ch == ' ') {
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
            new_test = 1;
        }

        user_input_length = strlen(user_input);
        if (user_input_length == strlen(prompt_string) && user_input[user_input_length - 1] == prompt_string[user_input_length - 1]) {
            clear();
            move(0, 0);
            print_centered_text(win, 0, "Test Complete!");
            print_centered_text(win, 2, "WPM: ");
            print_centered_text(win, 3, "Accuracy: ");
            print_centered_text(win, 4, "Press tab to start a new test, or any key to return to main menu");
            ch = getch();
            if (ch != '	') {
                run = 0;
                clear();
            } else {
                new_test = 1;
            }
        } else {
            print_typing_prompt(win, prompt, prompt_string, user_input);
        }

        /* Prints typing prompt after all input is done processing */
    }

    free(prompt);

}

/* Main function. Creates main menu */
int main() {
    FILE *words_file;
    Word_array *word_array;
    int cursor_x = 0, cursor_y = 0, run = 1;
    int ch;

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
        init_pair(1, COLOR_RED, COLOR_WHITE);
        init_pair(2, COLOR_WHITE, COLOR_BLACK);
        init_pair(3, COLOR_GREEN, COLOR_BLACK);   /* Correct Text Color */
        init_pair(4, COLOR_RED, COLOR_BLACK);   /* Incorrect Text Color */
    }

    if (words_file == NULL) {
        run = 0;

        init_color(COLOR_WHITE, 255, 255, 0);
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