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

    t.c_lflag &= ~(ECHO); // ECHO - print pressed keys

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &t);
    // TCSAFLUSH - wait for pending output, discard unread input
}

int main(void) {
    enableRawMode();
    char c = 0;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q')
        ;

    disableRawMode();
    return EXIT_SUCCESS;
}
