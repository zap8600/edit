#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#define CTRL_KEY(k) ((k) & 0x1f)

struct termios ot; // Not familiar with termios

void dr() {
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &ot) == -1) {
        perror("tcsetattr");
        exit(1);
    }
}

int main() {
    if(tcgetattr(STDIN_FILENO, &ot) == -1) {
        perror("tcgetattr");
        exit(1);
    }
    atexit(dr);
    struct termios r = ot;
    r.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP); // No XON/XOFF (CTRL+S and CTRL+Q), no CR to LF (won't translate \r to \n), no SIGINT, no parity checks, no 8th input bit
    r.c_oflag &= ~(OPOST); // No output processing
    r.c_cflag |= (CS8); // Character size is 8 bits
    r.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN); // No echo (character won't get automatically printed), no canonical (no need to press enter to send input), no signals (CTRL+C and CTRL+Z), no CTRL+V
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &r) == -1) {
        perror("tcsetattr");
    }
    r.c_cc[VMIN] = 0; // Set minimum number of bytes needed before read can return
    r.c_cc[VTIME] = 1; // Set timeout on read. 1/10th of a second
    while(1) {
        write(STDOUT_FILENO, "\x1b[2J", 4); // Clear screen
        write(STDOUT_FILENO, "\x1b[H", 3); // Set cursor to top left
        char c = 0;
        if((read(STDIN_FILENO, &c, 1) == -1) && (errno != EAGAIN)) {
            perror("read");
            exit(1);
        }
        if(c == CTRL_KEY('q')) break;
    }

    return 0;
}