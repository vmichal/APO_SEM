#ifndef __PERIPHERALS_H__
#define __PERIPHERALS_H__

#include <unistd.h>
#include <stdlib.h>
#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"

#ifdef __cplusplus
extern "C" {
#endif
void peripherals_intit();

void draw_board(int *board, int board_w, int board_h, int cell_s);

void draw_led_strip(int cell_s);

unsigned short rgb_to_565(const unsigned char r, const unsigned char g,const unsigned int b);

void color_square(int col, int row, unsigned short color, unsigned short *window);
	
void draw_window(const unsigned short *window);

#ifdef __cplusplus
}
#endif
#endif
