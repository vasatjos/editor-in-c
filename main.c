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

    struct termios raw = termios_default;

    // ICRLN  - don't translate C-m \r into \n
    // IXON   - disable C-s and C-q control flow
    // BRKINT, INPCK, ISTRIP - misc.
    // (https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html)
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    // OPOST  - disable output post processing (\n to \r\n)
    raw.c_oflag &= ~(OPOST);
    // CS8    - set char size to 8
    raw.c_cflag |= CS8;
    // ECHO   - print pressed keys
    // ICANON - read byte-by-byte instead of line-by-line
    // ISIG   - disable C-z and C-c (SIGTSTP, SIGINT)
    // IEXTEN - disable C-v
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

    // TCSAFLUSH - wait for pending output, discard unread input
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main(void) {
    enableRawMode();
    char c = 0;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
        if (iscntrl(c)) {
            printf("%d\r\n", c);
        } else {
            printf("%d ('%c')\r\n", c, c);
        }
    }
    return EXIT_SUCCESS;
}
