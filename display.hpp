#ifndef __PERIPHERALS_H__
#define __PERIPHERALS_H__

#include <unistd.h>
#include <stdlib.h>
#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"


void init_lcd();

void fill_pixel_lcd(int pos_x, int pos_y, unsigned short color);

void fill_square_lcd(int col, int row, unsigned short color);

void flood_fill_lcd(unsigned short color);

void display_lcd();

void draw_board(int *board, int board_w, int board_h, int cell_s);

void color_square(int col, int row, unsigned short color, unsigned short *window);
	
void draw_window(const unsigned short *window);



#endif
