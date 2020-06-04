#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "peripherals.hpp"
#include "led-line.hpp"
#include "snake-options.hpp"
#include "led-rgb.hpp"
#include "text.hpp"

int main(int argc, char* argv[]) {
	int board[COLUMNS * ROWS];

	bool som = false;
	for (int y = 0; y < ROWS; ++y) {
		for (int i = 0; i < COLUMNS; ++i) {
			board[y * COLUMNS + i] = som;
			som = !som;
		}
		som = !som;
	}

	peripherals_intit();

	draw_board(board, COLUMNS, ROWS, SIDE);

	unsigned short *window = (unsigned short *)malloc(sizeof(short) * LCD_WIDTH * LCD_HEIGHT);
	for (int i = 0; i < LCD_WIDTH * LCD_HEIGHT; ++i) {
		window[i] = rgb_to_565(0, 0, 0);
	}

	write_line_to_fb(1, "text", window, BLUE);

	write_line_to_fb(14, "no to snad neni mozny", window, RED);
	write_line_to_fb(14, "problem", window, RED);

	draw_window(window);


	struct timespec sleep_time { 0, 1000 * 1000 *5 };

	for (int i = 0; i < 1000; ++i) {

		led::rgb1.write(0xff7f00); //color orange
		led::rgb2.write(0xff7f00);
		nanosleep(&sleep_time, nullptr);
		led::line.write(i);
	}

	printf("Goodbye world\n");

	return 0;
}

