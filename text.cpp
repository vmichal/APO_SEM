#include "text.hpp"

// function to display help
void get_help()
{
	flood_fill_lcd(WHITE);
	FILE *help_file = fopen("menus/help.txt", "r");
	assert(help_file != NULL);
	char *line = (char *)malloc(sizeof(char));
	assert(line != NULL);
	size_t size = 1;
	size_t chars_read;

	int i = 0;
	while (!feof(help_file) && i < MAX_LINE_NUMBER) {
		chars_read = getline(&line, &size, help_file);
		if (chars_read < MAX_LINE_LENGTH) {
			line[chars_read - 1] = '\0';
		} else {
			line[MAX_LINE_LENGTH] = '\0';
		}
		write_line_to_display(i, line, RED, WHITE);
	}

	fclose(help_file);
	free(line);
}

void display_score(int *scores, int count)
{
	// flood_fill_lcd(WHITE);
	// get the length of both numbers
	// display : in the middle
	// get biger font
	//
	// prepare char to print
}

void closing_screen()
{
	flood_fill_lcd(WHITE);
	write_big_centerd_text("BYE!");
}

void welcome_screen()
{
	flood_fill_lcd(WHITE);
	write_big_centerd_text("SNAKE!");
	write_line_to_display(11, "Press any key to continue.", RED, WHITE);
}

void write_big_centerd_text(const char *text)
{
	char_t ch;
	ch.color = BLUE;
	ch.font = &font_wArial_88;
	ch.pos_y = MIDDLE_FOR_HEADER;
	ch.pos_x = align_center(text, ch.font);

	int i = 0;
	while (text[i] != '\0') {
		ch.character = text[i];
		write_char(&ch);
		ch.pos_x += char_width(ch.font, ch.character);
		++i;
	}
}

void write_char(const char_t *ch)
{
	int offset = get_offset(ch);
	int width = char_width(ch->font, ch->character);

	// for me to be able to mask bits
	int masking_bit; 
	// font_bits_t curent_part_of_bitmap;
	// i want to iterate over the bits in
	for (unsigned int  row = 0; row < ch->font->height; row++) {
		// iterate over one line from the bitmap
		masking_bit = 1 << (BITS - 1);
		// curent_part_of_bitmap = ch->font->bits;
		for (int col = 0; col < width; col++) {
			// reset the msb, and get to other part of the bitmap
			if (col != 0 && col % 16 == 0) {
				// printf("%x", ch->font->bits[offset]);
				++offset;
				masking_bit = 1 << (BITS - 1);
			}
			
			// color pixel if stated so in bitmap
			if (masking_bit & ch->font->bits[offset]) {
				fill_pixel_lcd(ch->pos_x + col, ch->pos_y + row, ch->color);
			}
			// move masking bit to mask the next bit
			masking_bit = masking_bit >> 1;
		}
		// move to next row of the bitmap
		++offset;
	}
}

int get_offset(const char_t *ch)
{
	return ch->font->offset[ch->character - ch->font->firstchar];
}

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

void write_line_to_display(int line, const char *text, unsigned short color, unsigned short bg_color)
{
	if (possible_line_number(line)) {
		int pos_y = line_to_pos_y(line);

		font_descriptor_t* fdes = &font_winFreeSystem14x16;
		clear_line(line, bg_color);

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
	for(unsigned int increment_y = 0; increment_y < fdes->height; ++increment_y) {
		for(int increment_x = 0; increment_x < width; ++increment_x) {
			if (bitmap[increment_y] & msb) {
				fill_pixel_lcd(x + increment_x, increment_y + y, color);
			}
			bitmap[increment_y] = bitmap[increment_y] << 1;
		}
	}
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
