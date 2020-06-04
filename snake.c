#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "peripherals.h"


int main(int argc, char *argv[]) {
	int board[48 * 32 * 2 *2];

	bool som = false;
	for(int y = 0; y < 32 * 2; ++y) {
		for(int i = 0; i < 48 * 2; ++i) {
			board[y * 48 * 2+ i] = som;
			som = !som;
		}
		som = !som;
	}
	
	peripherals_intit();
	draw_board(board, 48 * 2, 32 * 2, 5);

	printf("Goodbye world\n");

	return 0;
}

