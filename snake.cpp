#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "peripherals.h"

#include "snake-options.hpp"


int main(int argc, char *argv[]) {
	int board[COLUMNS * ROWS];

	bool som = false;
	for(int y = 0; y < ROWS; ++y) {
		for(int i = 0; i < COLUMNS; ++i) {
			board[y * COLUMNS + i] = som;
			som = !som;
		}
		som = !som;
	}
	
	peripherals_intit();
	draw_led_strip(10);
	draw_board(board, COLUMNS, ROWS, SIDE);

	printf("Goodbye world\n");

	return 0;
}

