#ifndef __PERIPHERALS_H__
#define __PERIPHERALS_H__

#include <unistd.h>
#include <stdlib.h>
#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"

void peripherals_intit();

void draw_board(int *board, int board_w, int board_h, int cell_s);

void draw_led_strip(int cell_s);

unsigned int rgb_to_565(const unsigned int r, const unsigned int g,const unsigned int b);

#endif
