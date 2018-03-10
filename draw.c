#include "draw.h"

struct text_buffer *text_buffer_init()
{
	struct text_buffer *buf = malloc(sizeof(struct text_buffer));

	buf->buf = NULL;
	buf->len = 0;
	buf->buf_size = 0;
	buf->pairs = NULL;
	buf->num_pairs = 0;
	buf->color_buf = NULL;
	buf->cur_color = 0;

	return buf;
}

void text_buffer_destroy(struct text_buffer *buf)
{
	if (!buf)
		return;

	free(buf->color_buf);
	free(buf->buf);
	free(buf);
}

void text_buffer_print(struct text_buffer *buf, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	size_t needed = vsnprintf(NULL, 0, fmt, args);
	va_end(args);

	text_buffer_allocate(buf, needed);
	int old_len = buf->len;

	va_start(args, fmt);
	buf->len +=
	    vsnprintf(buf->buf + buf->len, buf->buf_size - buf->len, fmt, args);
	va_end(args);

	memset(buf->color_buf+old_len, buf->cur_color, buf->len - old_len);
}

void text_buffer_allocate(struct text_buffer *buf, int len)
{
	if ((buf->len + len) >= buf->buf_size) {
		buf->buf_size += len * 2;
		if (!buf->buf) {
			buf->buf = malloc(buf->buf_size);
		} else {
			buf->buf = realloc(buf->buf, buf->buf_size);
		}
		if (!buf->color_buf) {
			buf->color_buf = malloc(buf->buf_size);
		} else {
			buf->color_buf = realloc(buf->color_buf, buf->buf_size);
		}
	}
}

void text_buffer_render(struct text_buffer *buf)
{
	if (buf->pairs) {
		color_buffer(buf->buf, buf->color_buf, buf->len, buf->pairs, buf->num_pairs);
		char * cbuf = buf->color_buf;
		int color_start = 0;
		int color = -1;
		char clr_buf[16];
		memset(clr_buf, 0, 16);
		for (int i = 0; i < buf->len; i++) {
			int eob = (i+1) >= buf->len;
			if (color != cbuf[i]) {
				color = cbuf[i];
				if ((i - color_start) > 0 || eob)
					write_buffer(buf->buf+color_start, i - color_start + eob);
				int cl = snprintf(clr_buf, 16, "\x1b[%dm", cbuf[i]);
				write_buffer(clr_buf, cl);
				color_start = i;
			} else if (eob) {
				write_buffer(buf->buf+color_start, i-color_start + 1);
			}
		}
	} else {
		write_buffer(buf->buf, buf->len);
	}
}

void text_buffer_clear(struct text_buffer *buf)
{
	buf->len = 0;
}

struct text_rect *text_rect_init(int x, int y, int w, int h)
{
	struct text_rect *rect = malloc(sizeof(struct text_rect));

	rect->x = x;
	rect->y = y;
	rect->w = w;
	rect->h = h;
	rect->fill = malloc(sizeof(char *) * h);
	for (int i = 0; i < h; i++) {
		rect->fill[i] = malloc(w);
		memset(rect->fill[i], 0, w);
	}

	return rect;
}

void text_rect_destroy(struct text_rect * rect)
{
	if (!rect) return;

	for (int i = 0; i < rect->h; i++) {
		free(rect->fill[i]);
	}
	free(rect->fill);
	free(rect);
}

void text_wrap(char **line_buf, int w, int h, const char *input)
{
	if (!input)
		return;
	//Clear line buffer first
	for (int i = 0; i < h; i++) {
		memset(line_buf[i], 0, w);
	}
	int len = strlen(input);
	int col = 1, ln_last = 0, ln_iter = 0;
	int nl = 0, be = 0, le = 0;

	for (int i = 0; i < len; i++, col++) {
		if ((nl = input[i] == '\n') || (le = col >= w - 1)
		    || (be = i + 1 == len)) {
			strncpy(line_buf[ln_iter], input + ln_last, col);
			ln_last = i + le + nl;
			ln_iter++;
			col = 0;
		}
	}
}

void draw_rect(struct text_buffer *buf, struct text_rect *rect)
{
	int len = rect->w + 2;
	char *line = malloc(len + 1);
	line[len] = 0;

	for (int i = -1; i < rect->h + 1; i++) {
		text_buffer_print(buf, CURSOR_POS, rect->y + i, rect->x);
		if (i == -1 || i == rect->h) {
			memset(line, '-', len);
			line[0] = i == -1 ? ',' : '`';
			line[rect->w + 1] = i == -1 ? '.' : '\'';
		} else {
			memset(line, 0x20, len);
			line[0] = '|';
			line[rect->w + 1] = '|';
			strncpy(line + 1, rect->fill[i], strlen(rect->fill[i]));
		}
		text_buffer_print(buf, "%s", line);
	}
	free(line);
}
