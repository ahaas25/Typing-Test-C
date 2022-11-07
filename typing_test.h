#define NAME "Typing Test"
#define MAX_X 5
#define MAX_Y 3
#define MAX_STRING 256
#define NUM_MODES 5
#define NUM_MISC 3
#define SUCCESS 1
#define FAILURE 0
char TIMED_MODES_STRING[5][MAX_STRING] = { "5s", "10s", "25s", "30s", "60s" };
char WORD_MODES_STRING[5][MAX_STRING] = { "10", "25", "50", "100", "200" };
char MISC_STRING[3][MAX_STRING] = {"Exit", "Stats", "Settings"};

typedef struct {
    int number_of_words;
    char words[];
} Word_array;