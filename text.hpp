#ifndef __TEXT_H__
#define __TEXT_H__

#include "font_types.h"
#include "peripherals.hpp"
#include "snake-options.hpp"

#define STARTING_OFFSET 10
#define LINE_HEIGHT 16
#define BITS sizeof(uint16_t) * 8

#define RED rgb_to_565(255, 0, 0)
#define GREEN rgb_to_565(0, 255, 0)
#define BLUE rgb_to_565(0, 0, 255)
#define WHITE rgb_to_565(255, 255, 255)
#define BLACK rgb_to_565(0, 0, 0)

#ifdef __cplusplus
extern "C" {
#endif

void clear_line(int line, unsigned short *fb, unsigned short bg_color);

void write_line_to_fb(int line, char* text, unsigned short *fb, unsigned short color);

int char_width(font_descriptor_t* fdes, int ch);

int line_to_pos_y(int line);

void draw_char(int x, int y, font_descriptor_t* fdes, char ch, unsigned short *fb, unsigned short color);

int line_length(char *text, font_descriptor_t* fdes);

int align_center(char *text, font_descriptor_t* fdes);

#ifdef __cplusplus
}
#endif
#endif
