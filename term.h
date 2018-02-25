#ifndef TERM_H
#define TERM_H

#include <termios.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <error.h>
#include "escape.h"

/*Terminal Mode Function*/
struct termios get_term();
struct termios raw_term();
void set_term(struct termios term);

/*Input functions*/
void set_cursor(int x, int y);
void get_cursor(int *x, int *y);
void get_winsize(int *w, int *y);

/*Standard in/out wrappers*/
void write_str(const char *str);
void writef(const char *fmt, ...);
void write_buffer(char *buf, int len);
void read_until(char *buf, char delim, int max);
void read_buffer(char *buf, int len);
char read_char();

/*Escape Code Wrappers*/
void clear_line();
void clear_screen();

void die(const char *err);

#endif
