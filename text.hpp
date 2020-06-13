/*
	Computer architectures semestral assignment - Zelinka Josef & Michal Vojtìch

	This module allows user to display text on the LCD screen. User has to provide frame buffer,
	and wanted text is renderd using fonts specified in font_types.h.
*/

#ifndef __TEXT_H__
#define __TEXT_H__

#include "font_types.h"
#include "display.hpp"
#include "snake-options.hpp"
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	char character;
	int pos_x;
	int pos_y;
	font_descriptor_t *font;
	unsigned short color;
} char_t;

void get_help();

void display_score(int *scores, int count);

void closing_screen();

void welcome_screen();

void write_big_centerd_text(const char *text);

void write_char(const char_t *ch);

int get_offset(const char_t *ch);

void clear_line(int line, unsigned short bg_color);

void write_line_to_display(int line, const char* text, unsigned short color, unsigned short bg_color);

int char_width(font_descriptor_t* fdes, int ch);

int line_to_pos_y(int line);

void draw_char(int x, int y, font_descriptor_t* fdes, char ch, unsigned short color);

int line_length(const char *text, font_descriptor_t* fdes);

int align_center(const char *text, font_descriptor_t* fdes);

bool possible_line_number(int line);

#ifdef __cplusplus
}
#endif
#endif
