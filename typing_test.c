#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "typing_test.h"

/* Prints Centered Text To Console Screen */
int print_centered_text(WINDOW *win, int row, char str[]) {
    int text_spacing = strlen(str);
    int center_col = (win->_maxx - text_spacing) / 2;

    mvwprintw(win, row, center_col, str);
}

/* Prints centered text for menu items */
int print_centered_text_menu(WINDOW *win, int row, int target, char str[][MAX_STRING],
    int highlight, int elements) {
    int text_length = 0, center_col, i, k, cursor;

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

/* Main function. Creates main menu */
int main() {
    int cursor_x = 0, cursor_y = 0, run = 1;
    int ch, key;

    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();

    while (run) {
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
            }
        }

    }

    refresh();
    endwin();

    return 0;
}