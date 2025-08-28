#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios termios_default;

void disableRawMode() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &termios_default); }

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &termios_default);
    atexit(disableRawMode);

    struct termios t = termios_default;

    // ECHO - print pressed keys
    // ICANON - read byte-by-byte instead of line-by-line
    t.c_lflag &= ~(ECHO | ICANON);

    // TCSAFLUSH - wait for pending output, discard unread input
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &t);
}

int main(void) {
    enableRawMode();
    char c = 0;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
        if (iscntrl(c)) {
            printf("%d\n", c);
        } else {
            printf("%d ('%c')\n", c, c);
        }
    }
    return EXIT_SUCCESS;
}
