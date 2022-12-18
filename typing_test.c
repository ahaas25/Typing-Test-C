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
    int run = 1, ch, i, new_test = 1, user_input_length, start_timer;
    double test_time, wpm, accuracy;
    char str[1024], temp[24], *user_input = NULL;
    char *prompt_string = NULL; /* Full prompt string */
    Word_array *prompt = NULL;
    Word *prompt_lines; /* Array of lines for displaying */
    struct timeval timer_start, timer_stop;
    curs_set(1);
    while (run) {
        if (new_test == 1) {
            clear();
            accuracy = 0;

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
            start_timer = 1; /* Flag timer as ready to be started */
        }

        ch = getch();
        if (isalnum(ch) || ch == ' ' || ch == '\'') { /* If typing prompt character */
            if (start_timer) {
                gettimeofday(&timer_start, NULL);
                start_timer = 0;
            }
            if (user_input_length < prompt->num_characters) { /* If within prompt length */
                if (ch == prompt_string[user_input_length]) {
                    accuracy++;
                }
                user_input[user_input_length] = ch;
                user_input_length++;
                user_input[user_input_length] = '\0';
                move(0, 0);
            }
        } else if (ch == KEY_BACKSPACE) { /* Backspace */
            if (user_input_length > 0) {
                user_input_length--;
                user_input[user_input_length] = '\0';
                if (accuracy > 0) {
                    accuracy--; /* Account for user deleting line */
                } /* Do it this way or calculate at the end? */
                move(0, 0);
            }
        } else if (ch == '	') {
            new_test = 1;
            start_timer = 1;
        } else if (ch == 27) { /* Esc */
            run = 0;
            clear();
        }

        user_input_length = strlen(user_input);
        if (user_input_length == strlen(prompt_string) && user_input[user_input_length - 1] == prompt_string[user_input_length - 1]) {
            gettimeofday(&timer_stop, NULL);
            /* Convert time stamps to miliseconds */
            long long time_start = timer_start.tv_sec * 1000LL + timer_start.tv_usec / 1000;
            long long time_stop = timer_stop.tv_sec * 1000LL + timer_stop.tv_usec / 1000;

            /* Calculate accuracy */
            accuracy = (accuracy / user_input_length) * 100;

            /* Subtract starting time from ending time to get elapsed time */
            test_time = time_stop - time_start;
            test_time /= 1000; /* Convert time into seconds */

            /* WPM = (Total Chars / 5) /(Total Mins) */
            wpm = ((double)user_input_length / 5) / (test_time / 60);

            clear();

            /* Print end screen */

            print_centered_text(stdscr, 0, str); /* Print test type */
            curs_set(0);
            print_centered_text(win, 2, "Test Complete!");

            /* Note, console resizing support is planned. */

            /* Generate WPM String */
            str[0] = '\0';
            append_line("WPM: ", str);
            gcvt(wpm, 5, temp);
            append_line(temp, str);
            print_centered_text(win, 4, str);

            /* Generate Accuracy String */
            str[0] = '\0';
            append_line("Accuracy: ", str);
            gcvt(accuracy, 5, temp);
            append_line(temp, str);
            append_line("\%%", str);
            print_centered_text(win, 5, str);

            /* Generate Time String */
            str[0] = '\0';
            append_line("Time: ", str);
            gcvt(test_time, 5, temp);
            append_line(temp, str);
            append_line("s", str);
            print_centered_text(win, 6, str);

            /* Generate Characters String */
            str[0] = '\0';
            append_line("Characters: ", str);
            sprintf(temp, "%d", user_input_length);
            append_line(temp, str);
            print_centered_text(win, 7, str);

            print_centered_text(win, 19, "Press tab to start a new test, or any key to return to main menu");

            /* Wait on user input */
            ch = getch();
            if (ch != '	') {
                run = 0;
                clear();
            } else {
                new_test = 1;
            }
        } else {
            /* Prints typing prompt after all input is done processing */
            print_typing_prompt(win, prompt, prompt_string, user_input);
        }
    }

    free(prompt);
    clear();
}

/* Draws settings UI to console */
/* Placeholder, will work on next */
void settings_ui(WINDOW *win) {
    char ch;
    clear();
    print_centered_text(win, 0, "Settings");

    print_centered_text(win, 3, "Controls");
    print_centered_text(win, 4, "Tab - Reset Test");
    print_centered_text(win, 5, "Esc - End Test");

    print_centered_text(win, 7, "Themes");
    print_centered_text(win, 8, "Default");

    print_centered_text(win, 10, "Statistics");
    print_centered_text(win, 11, "Reset Stats");

    print_centered_text(win, 13, "Return to Menu");
    refresh();

    ch = getchar();
    clear();
}

/* Draws statistics UI to console */
/* Placeholder, will work on next */
void stat_ui(WINDOW *win) {
    char ch;
    clear();
    print_centered_text(win, 0, "Statistics");

    print_centered_text(win, 3, "Best WPM:");
    print_centered_text(win, 4, "Average WPM:");
    print_centered_text(win, 5, "Average Accuracy:");

    print_centered_text(win, 7, "Tests Completed:");
    print_centered_text(win, 8, "Time Spent Typing: ");

    print_centered_text(win, 10, "10 Word Test: ");
    print_centered_text(win, 11, "25 Word Test: ");
    print_centered_text(win, 12, "50 Word Test: ");
    print_centered_text(win, 13, "100 Word Test: ");

    print_centered_text(win, 15, "Return to Menu");
    refresh();

    ch = getchar();
    clear();
}

/* Main function. Creates main menu */
int main() {
    FILE *words_file, *stats_file;
    Word_array *word_array;
    Stat_struct stats;
    int cursor_x = 0, cursor_y = 0, run = 1;
    int ch;

    word_array = malloc(sizeof(Word_array));
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();

    print_centered_text(stdscr, 4, "Loading...");
    words_file = fopen("words.txt", "r");
    stats_file = fopen("stats", "r");

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

    /* Creates a new stats file if one is not detected */
    if (stats_file == NULL) {
        stats_file = fopen("stats", "w");
        create_stats_file(stats_file);
        stats_file = fopen("stats", "r");
    }

    /* Load stats from stats file */
    /* These will be modified as the program runs and the stats file will be updated
        upon program exit */
    load_stats(stats_file, &stats);

    clear();

    while (run) {
        attron(COLOR_PAIR(2));
        curs_set(0);
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
        if (ch == '\n') {
            if (cursor_x == 0 && cursor_y == 2) {
                /* Exit */
                run = 0;
            } else if (cursor_x == 1 && cursor_y == 2) {
                /* Stats */
                stat_ui(stdscr);
            } else if (cursor_x == 2 && cursor_y == 2) {
                /* Settings */
                settings_ui(stdscr);
            } else if (cursor_y == 1 || cursor_y == 0) {
                typing_ui(stdscr, cursor_x, cursor_y, word_array);
                clear();
            }
        }

    }

    /* Exiting */
    refresh();
    endwin();
    
    /* Saves stats and exits file */
    save_stats(stats_file, &stats);

    return 0;
}