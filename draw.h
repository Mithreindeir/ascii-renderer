#ifndef DRAW_H
#define DRAW_H

#include <stdarg.h>
#include "term.h"

/*Attempts to provide a simple API for rendering primitives
 * in ASCII using escape codes
 * */

struct text_buffer {
    char * buf;
    int len;
    int buf_size;
};

struct text_buffer *text_buffer_init();
void text_buffer_destroy(struct text_buffer * buf);

void text_buffer_print(struct text_buffer * buf, const char * fmt, ...);
void text_buffer_allocate(struct text_buffer * buf, int len);

void text_buffer_render(struct text_buffer * buf);
void text_buffer_clear(struct text_buffer * buf);

/*TODO Primitive Interface*/
struct text_rect {
    char ** fill;
    int x, y;
    int w, h;
};

struct text_rect * text_rect_init(int x, int y, int w, int h);
void text_wrap(char ** line_buf, int w, int h, const char * input);
void draw_rect(struct text_buffer * buf, struct text_rect * rect);

#endif
