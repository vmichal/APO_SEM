#include "peripherals.hpp"

#include <assert.h>
#include "snake-options.hpp"


unsigned char *parlcd_mem_base;

void peripherals_init()
{

	parlcd_mem_base = (unsigned char*)map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);

	assert(parlcd_mem_base);

	parlcd_hx8357_init(parlcd_mem_base);
}

void draw_board(int *board, int board_w, int board_h, int cell_s)
{
	parlcd_write_cmd(parlcd_mem_base, 0x2c);
	unsigned short c;
	for(int y = 0; y < 320; ++y) {
		for (int i = 0; i < 480; i++) {
			// room to determine the color based on the int on the board
			if (board[y / cell_s * board_w + i / cell_s] == 1) {
				c = rgb_to_565(255, 0, 0);
			} else {
				c = rgb_to_565(0, 255, 0);
			}
			parlcd_write_data(parlcd_mem_base, c);
		}
	}
}


void color_square(int col, int row, unsigned short color, unsigned short *window)
{
	for(int i = 0; i < SIDE; i++) {
		for(int y = 0; y < SIDE; y++) {
			window[(row + i) * LCD_WIDTH + col + y] = color;
		}
	}
}

void draw_window(const unsigned short *window)
{
	parlcd_write_cmd(parlcd_mem_base, 0x2c);
	for(int i = 0; i < LCD_WIDTH * LCD_HEIGHT; ++i) {
		parlcd_write_data(parlcd_mem_base, window[i]); 
	}
}
