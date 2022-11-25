#include <sys/time.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <ctype.h>

#define NAME "Typing Test"
#define MAX_X 5
#define MAX_Y 3
#define MAX_STRING 256
#define NUM_MODES 5
#define NUM_MISC 3
#define SUCCESS 1
#define FAILURE 0

char TIMED_MODES_STRING[5][MAX_STRING] = { "5s", "10s", "25s", "30s", "60s" };
int TIMED_MODES[5] = {5, 10, 25, 30, 60};
char WORD_MODES_STRING[5][MAX_STRING] = { "5", "10", "25", "50", "100" };
int WORD_MODES[5] = {5, 10, 25, 50, 100};
char MISC_STRING[3][MAX_STRING] = {"Exit", "Stats", "Settings"};