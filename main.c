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

    // VMIN   - minimum necessary characters for read()
    // VTIME  - 0.1 * n seconds of max wait before read() returns
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    // TCSAFLUSH - wait for pending output, discard unread input
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main(void) {
    enableRawMode();
    while (1) {
        char c = 0;
        read(STDIN_FILENO, &c, 1);
        if (iscntrl(c)) {
            printf("%d\r\n", c);
        } else {
            printf("%d ('%c')\r\n", c, c);
        }
        if (c == 'q')
            break;
    }
    return EXIT_SUCCESS;
}
