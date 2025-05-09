#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

struct termios ot; // Not familiar with termios

void dr() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &ot);
}

int main() {
    tcgetattr(STDIN_FILENO, &ot);
    atexit(dr);
    struct termios r = ot;
    r.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP); // No XON/XOFF (CTRL+S and CTRL+Q), no CR to LF (won't translate \r to \n), no SIGINT, no parity checks, no 8th input bit
    r.c_oflag &= ~(OPOST); // No output processing
    r.c_cflag |= (CS8); // Character size is 8 bits
    r.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN); // No echo (character won't get automatically printed), no canonical (no need to press enter to send input), no signals (CTRL+C and CTRL+Z), no CTRL+V
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &r);
}