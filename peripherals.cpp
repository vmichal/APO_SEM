#include "peripherals.hpp"

#include <assert.h>
#include "snake-options.hpp"

unsigned char *mem_base;
unsigned char *parlcd_mem_base;

void peripherals_intit()
{
	/*
	 * Setup memory mapping which provides access to the peripheral
	 * registers region of RGB LEDs, knobs and line of yellow LEDs.
	 */
	mem_base = (unsigned char*)map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);

	/* If mapping fails exit with error code */
	assert(mem_base);

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

void draw_led_strip(int cell_s)
{
	*(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = 0b00000111;	
}

unsigned short rgb_to_565(const unsigned char r, const unsigned char g,const unsigned int b)
{
	return ((r & 0b11111000) << 8) | ((g & 0b11111100) << 3) | (b >> 3);
}

void draw_window(unsigned short *window)
{
	parlcd_write_cmd(parlcd_mem_base, 0x2c);
	for(int i = 0; i < LCD_WIDTH * LCD_HEIGHT; ++i) {
		parlcd_write_data(parlcd_mem_base, window[i]); 
	}
}