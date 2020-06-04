#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "peripherals.h"
#include "led-line.hpp"
#include "snake-options.hpp"
#include "led-rgb.hpp"

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

	struct timespec sleep_time { 0, 10000 *1000 *5};

	for (int i = 0; i < 1000000; ++i) {
		led::rgbs[0].write(i % 3 == 0 ? 255 : 0, i % 3 == 1 ? 255 : 0, i % 3 == 2 ? 255 : 0);
		nanosleep(&sleep_time, nullptr);
		led::line.write(i);
	}

	printf("Goodbye world\n");

	return 0;
}

