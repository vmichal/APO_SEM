#include "text.hpp"

void clear_line(int line, unsigned short bg_color)
{
	int pos_y = line_to_pos_y(line);
	for(int y = 0; y < LINE_HEIGHT; ++y) {
		for(int i = 0; i < LCD_WIDTH; ++i) {
			assert((pos_y + y) * LCD_WIDTH  + i >= 0 && (pos_y + y) * LCD_WIDTH  + i < LCD_WIDTH *LCD_HEIGHT);
			fill_pixel_lcd(i, pos_y + y, bg_color);
		}
	}
}

void write_line_to_fb(int line, const char *text, unsigned short color)
{
	if (possible_line_number(line)) {
		int pos_y = line_to_pos_y(line);

		font_descriptor_t* fdes = &font_winFreeSystem14x16;
		clear_line(line, BLACK);

		// prepares so that the line is printed aligned to center
		int pos_x = align_center(text, fdes);
		int i = 0;
		while (text[i] != '\0') {
			draw_char(pos_x, pos_y, fdes, text[i], color);
			pos_x += char_width(fdes, text[i]);
			++i;
		}
	} else {
		printf("Line number out of range.\n");
	}
	// draw the frame buffer
}

void draw_char(int x, int y, font_descriptor_t* fdes, char ch, unsigned short color) {
	int pos = ch - 0x20;
	int width = fdes->width[pos];

	font_bits_t bitmap[fdes->height];

	for(unsigned int i = 0; i < fdes->height; ++i) {
		bitmap[i] = fdes->bits[pos * 16 + i];
	}

	int msb = 1 << (BITS - 1);
	for(unsigned int i = 0; i < fdes->height; ++i) {
		for(int increment_y = 0; increment_y < width; ++increment_y) {
			if (bitmap[i] & msb) {
				fill_pixel_lcd(x + increment_y, i + y, color);
			}
			bitmap[i] = bitmap[i] << 1;
		}
	}
}

// asdfasdf


void clear_line_b(int line, unsigned short *window, unsigned short bg_color)
{
	int pos_y = line_to_pos_y(line);
	for(int y = 0; y < LINE_HEIGHT; ++y) {
		for(int i = 0; i < LCD_WIDTH; ++i) {
			assert((pos_y + y) * LCD_WIDTH  + i >= 0 && (pos_y + y) * LCD_WIDTH  + i < LCD_WIDTH *LCD_HEIGHT);
			window[(pos_y + y) * LCD_WIDTH  + i] = bg_color;
		}
	}
}

void write_line_to_fb_b(int line, const char *text, unsigned short *fb, unsigned short color)
{
	if (possible_line_number(line)) {
		int pos_y = line_to_pos_y(line);

		font_descriptor_t* fdes = &font_winFreeSystem14x16;
		// clear_line(line, fb, BLACK);

		// prepares so that the line is printed aligned to center
		int pos_x = align_center(text, fdes);
		int i = 0;
		while (text[i] != '\0') {
			// draw_char(pos_x, pos_y, fdes, text[i], fb, color);
			pos_x += char_width(fdes, text[i]);
			++i;
		}
	} else {
		printf("Line number out of range.\n");
	}
	// draw the frame buffer
}

int char_width(font_descriptor_t* fdes, int ch) {
	int width = 0;
	if ((ch >= fdes->firstchar) && (ch-fdes->firstchar < fdes->size)) {
		ch -= fdes->firstchar;
		if (!fdes->width) {
			width = fdes->maxwidth;
		} else {
			width = fdes->width[ch];
		}
	}
	return width;
}

int line_to_pos_y(int line)
{
	return STARTING_OFFSET + line * LINE_HEIGHT;
}

void draw_char_b(int x, int y, font_descriptor_t* fdes, char ch, unsigned short *fb, unsigned short color) {
	int pos = ch - 0x20;
	int width = fdes->width[pos];

	font_bits_t bitmap[fdes->height];

	for(unsigned int i = 0; i < fdes->height; ++i) {
		bitmap[i] = fdes->bits[pos * 16 + i];
	}

	int msb = 1 << (BITS - 1);
	for(unsigned int i = 0; i < fdes->height; ++i) {
		for(int increment_y = 0; increment_y < width; ++increment_y) {
			if (bitmap[i] & msb) {
				fb[(i + y) * LCD_WIDTH + x + increment_y] = color;
			}
			bitmap[i] = bitmap[i] << 1;
		}
	}
}

int line_length(const char *text, font_descriptor_t* fdes)
{
	int length = 0;
	int i = 0;
	while (text[i] != '\0') {
		length += char_width(fdes, text[i]);
		++i;
	}
	return length;
}

int align_center(const char *text, font_descriptor_t* fdes)
{
	return (LCD_WIDTH - line_length(text, fdes)) / 2;
}

bool possible_line_number(int line)
{
	if (line >= 0 && line < MAX_LINE_NUMBER) {
		return true;
	} else {
		return false;
	}
}
