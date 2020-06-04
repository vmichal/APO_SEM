#ifndef __MENU_H__
#define __MENU_H__

#include "peripherals.hpp"
#include "snake-options.hpp"
#include "text.hpp"
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	const char *text;
} option_t;

typedef struct {
	option_t *options;
	unsigned short selected_color;
	unsigned short unselected_color;
	int selected;
	int num_options;
} menu_t; 

void display_menu(unsigned short *window, int menu_id);

void move_selected(bool up, unsigned short *window, int menu_id);

int get_selected(int menu_id);

#ifdef __cplusplus
}
#endif
#endif
