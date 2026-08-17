#include "../include/terminal.h"
#include <termios.h>
#include <unistd.h>

static struct termios orig_termios;

int
enable_raw_mode()
{
	tcgetattr(STDIN_FILENO, &orig_termios);

	struct termios raw;
	tcgetattr(STDIN_FILENO, &raw);

	raw.c_lflag &= ~(ECHO | ICANON | ISIG);

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) != 0)
		return -1;

	return 0;
}

int
disable_raw_mode()
{
	return tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}
