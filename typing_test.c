#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct winsize w;

int draw_ui() {
    system("clear");
    printf ("lines %d\n", w.ws_row);
    printf ("columns %d\n", w.ws_col);
}

int print_center_text(char str[], int width) {
    int i, x = strlen(str);

    width - x;
    width /= 2; /* Padding Area */

    for (i = 0; i < width; i++) {
        
    }
}

int main (int argc, char **argv) {
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    draw_ui();
    return 0;
}