#include "term.h"

struct termios get_term()
{
	struct termios term;
	if (tcgetattr(STDIN_FILENO, &term) == -1)
		die("tcgetattr");

	return term;
}

struct termios raw_term()
{
	struct termios raw = get_term();

	raw.c_iflag &= ~(ICRNL | INPCK | ISTRIP | IXON | BRKINT);

	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);

	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

	/*Return after 1 Byte or 0.1 seconds */
	raw.c_cc[VMIN] = 1;
	raw.c_cc[VTIME] = 1;

	return raw;
}

void set_term(struct termios term)
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) == -1)
		die("tcsetattr");
}

void set_cursor(int x, int y)
{
	char buf[16];
	snprintf(buf, 16, CURSOR_POS, x, y);
	write(STDOUT_FILENO, buf, strlen(buf));
}

void get_cursor(int *x, int *y)
{
	write(STDOUT_FILENO, CURSOR_REPORT, strlen(CURSOR_REPORT));
	char buf[16];
	memset(buf, 0, 16);
	//Cursor report prints \x1b[y;xR to stdin, so read until R
	read_until(buf, 'R', 16);
	if (sscanf(buf, "\x1b[%d;%d", y, x) != -2)
		x = 0, y = 0;
}

void read_until(char *buf, char delim, int max)
{
	int i = 0;
	while (read(STDIN_FILENO, buf + i++, 1) == 1 && buf[i - 1] != delim
	       && i < max) ;
}

void read_buffer(char *buf, int len)
{
	read(STDIN_FILENO, buf, len);
}

char read_char()
{
	char byte;
	int nbyte = read(STDIN_FILENO, &byte, 1);
	if (nbyte < 0)
		die("read");
	return byte;
}

void write_str(const char *str)
{
	write(STDOUT_FILENO, str, strlen(str));
}

void write_buffer(char *buf, int len)
{
	write(STDOUT_FILENO, buf, len);
}

void die(const char *err)
{
	printf("hEs DeAd JiM!!\n");
	perror(err);
	exit(1);
}
