#include "escape.h"
#include <stdlib.h>

int escape_code(const char * input, const int len)
{
	if (!input || !len) return 0;
	if (*input != '\x1b') return 0;
	
	int esc_len = strlen(SMOOTH_SCROLLING);
	if (!strncmp(input, SMOOTH_SCROLLING, esc_len))
		return esc_len;

	esc_len = strlen(CLEAR_LINE);
	if (!strncmp(input, CLEAR_LINE, esc_len))
		return esc_len;
	
	esc_len = strlen(CLEAR_SCRN);
	if (!strncmp(input, CLEAR_SCRN, esc_len))
		return esc_len;
	
	esc_len = strlen(CURSOR_REPORT);
	if (!strncmp(input, CURSOR_REPORT, esc_len))
		return esc_len;
	
	esc_len = strlen(CURSOR_DOWN);
	if (!strncmp(input, CURSOR_DOWN, esc_len))
		return esc_len;
	
	esc_len = strlen(CURSOR_FORWARD);
	if (!strncmp(input, CURSOR_FORWARD, esc_len))
		return esc_len;

	/*Check for escape codes with variable inputs by using scanf*/
	int dtmp = 0, dtmp2 = 0;
	char ctmp = 0;
	esc_len = 0;
	/*4 bit color: "\x1b[30;0m" for example*/
	if (sscanf(input, "\x1b[%d;0%c", &dtmp, &ctmp) == 2 && ctmp == 'm') {
		while (esc_len < len && input[esc_len] && input[esc_len++] != 'm');
		return esc_len;
	}
	/*Cursor set: "\x1b[30;30f" for example*/
	if (sscanf(input, "\x1b[%d;%d%c", &dtmp, &dtmp2, &ctmp) == 3 && ctmp == 'f') {
		while (esc_len < len && input[esc_len] && input[esc_len++] != 'f');
		return esc_len-1;
	}

	return 0;
}
